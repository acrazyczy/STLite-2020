#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"
#include "utility.hpp"

#include <cstddef>

namespace sjtu
{
template <class T>
class deque
{
private:
	size_t total_size , total_space , block_size;

	struct Node
	{
		T *value;
		Node *prec , *succ;

		explicit Node (T * const &value_ = nullptr) : value(value_) , prec(nullptr) , succ(nullptr) {}
		Node (const Node &other) : value(other.value == nullptr ? nullptr : new T (*other.value)) , prec(nullptr) , succ(nullptr) {}
		Node &operator=(const Node &other) = delete;
		~Node () {delete value;}
	};

	static Node *find_tail(Node *node)
	{
		for (;node -> succ;node = node -> succ);
		return node;
	}

	static Node *list_copy(Node * const &other)
	{
		if (other == nullptr) return nullptr;
		Node *node = new Node (*other);
		if (node -> succ = list_copy(other -> succ)) node -> succ -> prec = node;
		return node;
	}

	static void list_clear(Node *&node)
	{
		if (node == nullptr) return;
		list_clear(node -> succ) , delete node , node = nullptr;
	}

	struct Block
	{
		Node *head , *tail;
		Block *prec , *succ;
		size_t tot;

		Block() : head(nullptr) , tail(nullptr) , prec(nullptr) , succ(nullptr) , tot(0) {}
		Block(Node * const &node) : head(node) , tail(node) , prec(nullptr) , succ(nullptr) , tot(1) {}
		Block(Node * const &head_, Node * const &tail_ , const size_t &tot_) : head(head_) , tail(tail_) , prec(nullptr) , succ(nullptr) , tot(tot_) {}
		Block (const Block &other) : head(list_copy(other.head)) , tail(find_tail(head)) , tot(other.tot) {}
		Block &operator=(const Block &other) = delete;
		~Block() {list_clear(head);}
	}*Blk;

	pair<Block * , Node *> nodeend;

	/*void merge(Block *lhs , Block *rhs)//lhs and rhs are not empty
	{
		(lhs -> tail -> succ = rhs -> head) -> prec = lhs -> tail , lhs -> tot += rhs -> tot , lhs -> tail = rhs -> tail;
		rhs -> head = nullptr , delete rhs;
		if ((lhs -> succ = rhs -> succ) == nullptr) nodeend.first = lhs , nodeend.second = lhs -> tail;
	}*/

	void split(Block *blk)//blk.size() >= 2
	{
		Node *mid = blk -> head;
		for (int i = 1;i + 1 <= (blk -> tot + 1 >> 1);++ i) mid = mid -> succ;
		mid -> succ -> prec = nullptr;
		Block *nblk = new Block (mid -> succ , blk -> tail , blk -> tot >> 1);
		mid -> succ = nullptr , blk -> tot -= nblk -> tot;
		if (nblk -> succ = blk -> succ) nblk -> succ -> prec = nblk;
		((blk -> succ = nblk) -> prec = blk) -> tail = mid;
		if (nblk -> succ == nullptr) nodeend.first = nblk , nodeend.second = nblk -> tail;
	}

	pair<Block * , Node *> maintain(Block *blk , pair<Block * , Node *> pos , bool opt)//to do : return value
	{
		/*if (!opt && blk -> tot < block_size)
		{
			Block *blk_ = nullptr;
			if (blk -> prec)
			{
				if (pos.first == blk) pos.first = blk_;
				merge(blk_ = blk -> prec , blk);
			}
			else if (blk -> succ)
			{
				if (pos.first == blk -> succ) pos.first = blk;
				merge(blk_ = blk , blk -> succ);
			}
			if (blk_) blk = blk_ , opt = 1;
		}*/
		if (opt && blk -> tot >= (block_size << 1))
		{
			split(blk);bool found = 0;
			if (pos.first == blk)
			{
				for (Node *node = blk -> head;node != nullptr && !found;node = node -> succ) found |= node == pos.second;
				if (!found) pos.first = pos.first -> succ;
			}
		}
		return pos;
	}

	pair<Block * , Node *> rebuild(pair<Block * , Node *> pos)
	{
		if (total_size < total_space) return pos;
		pair<Block * , Node *> ret;
		for (total_space <<= 1;(block_size + 1) * (block_size + 1) <= total_space;++ block_size);
		Block *Blk_ = new Block , *curblk = Blk_;
		for (Block *nxt;Blk;Blk = nxt)
		{
			for (Node *nxt_;Blk -> head;Blk -> head = nxt_)
			{
				if (curblk -> tot + 1 == (block_size << 1)) (curblk -> succ = new Block) -> prec = curblk , curblk = curblk -> succ;
				if (curblk -> tail == nullptr) curblk -> tail = curblk -> head = new Node (*(Blk -> head));
				else (curblk -> tail -> succ = new Node (*(Blk -> head))) -> prec = curblk -> tail , curblk -> tail = curblk -> tail -> succ;
				if (Blk -> head == pos.second) ret.first = curblk , ret.second = curblk -> tail;
				++ curblk -> tot , nxt_ = Blk -> head -> succ , delete Blk -> head;
			}
			nxt = Blk -> succ , delete Blk;
		}
		Blk = Blk_ , nodeend.first = curblk , nodeend.second = curblk -> tail;
		return ret;
	}

	Block *copy(Block * const &blk)//total_size remains unchanged
	{
		if (blk == nullptr) return nullptr;
		Block *blk_ = new Block (*blk);
		if (blk_ -> succ = copy(blk -> succ)) blk_ -> succ -> prec = blk_;
		return blk_;
	}

	void clear(Block *&blk)//total_size remains unchanged
	{
		if (blk == nullptr) return;
		clear(blk -> succ) , delete blk , blk = nullptr;
	}

	pair<Block * , Node *> insert(pair<Block * , Node *> node , const T &value)
	{
		Node *node_ = new Node (new T (value));
		if (node.second -> prec == nullptr) node.second -> prec = node_ , (node.first -> head = node_) -> succ = node.second;
		else (node_ -> prec = node.second -> prec) -> succ = node_ , (node_ -> succ = node.second) -> prec = node_;
		++ node.first -> tot , ++ total_size;
		return rebuild(maintain(node.first , pair<Block * , Node *>(node.first , node_) , 1));
	}

	pair<Block * , Node *> erase(pair<Block * , Node *> node)
	{
		pair<Block * , Node *> ret;
		if (node.first -> tot == 1) node.first -> head = nullptr , ret.first = node.first -> succ , ret.second = node.first -> succ -> head;
		else if (node.second -> prec == nullptr) (node.first -> head = node.second -> succ) -> prec = nullptr , ret.first = node.first , ret.second = node.first -> head;
		else if (node.second -> prec -> succ = node.second -> succ) node.second -> succ -> prec = node.second -> prec , ret.first = node.first , ret.second = node.second -> succ;
		else (node.first -> tail = node.first -> tail -> prec) -> succ = nullptr , ret.first = node.first -> succ , ret.second = node.first -> succ -> head;
		delete node.second , -- node.first -> tot , -- total_size;
		if (!node.first -> tot)
		{
			if (node.first -> prec) node.first -> prec -> succ = node.first -> succ;
			else Blk = node.first -> succ;
			node.first -> succ -> prec = node.first -> prec;
			delete node.first;
		}
		return ret;
	}

	void findend()
	{
		Block *blk = Blk;
		for (;blk -> succ != nullptr;blk = blk -> succ);
		nodeend.first = blk , nodeend.second = blk -> tail;
	}
public:
	class const_iterator;
	class iterator {
		friend class deque;
		friend class const_iterator;
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		const deque *cor;
		pair<Block * , Node *> loc;
	public:
		iterator() {}
		explicit iterator(const deque * const &cor_ , const pair<Block * , Node *> &loc_) : cor(cor_) , loc(loc_) {}
		iterator(const iterator &other) : cor(other.cor) , loc(other.loc) {}
		iterator &operator=(const iterator &rhs) {cor = rhs.cor , loc.first = rhs.loc.first , loc.second = rhs.loc.second;return *this;}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			if (n < 0) return operator-(-n);
			int n_ = n;
			pair<Block * , Node *> loc_(loc);
			for (;n_ && loc_.second -> succ;-- n_ , loc_.second = loc_.second -> succ);
			if (n_)
			{
				-- n_;
				if (loc_.first = loc_.first -> succ) loc_.second = loc_.first -> head;
				else throw(invalid_iterator());
			}
			for (;n_ && n_ >= loc_.first -> tot;)
			{
				n_ -= loc_.first -> tot;
				if (loc_.first = loc_.first -> succ) loc_.second = loc_.first -> head;
				else throw(invalid_iterator());
			}
			for (;n_ && loc_.second -> succ;-- n_ , loc_.second = loc_.second -> succ);
			return iterator(cor , loc_);
		}

		iterator operator-(const int &n) const
		{
			if (n < 0) return operator+(-n);
			int n_ = n;
			pair<Block * , Node *> loc_(loc);
			for (;n_ && loc_.second -> prec;-- n_ , loc_.second = loc_.second -> prec);
			if (n_)
			{
				-- n_;
				if (loc_.first = loc_.first -> prec) loc_.second = loc_.first -> tail;
				else throw(invalid_iterator());
			}
			for (;n_ && n_ >= loc_.first -> tot;)
			{
				n_ -= loc_.first -> tot;
				if (loc_.first = loc_.first -> prec) loc_.second = loc_.first -> tail;
				else throw(invalid_iterator());
			}
			for (;n_ && loc_.second -> prec;-- n_ , loc_.second = loc_.second -> prec);
			return iterator(cor , loc_);
		}
		// return the distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if (cor != rhs.cor) throw(invalid_iterator());
			int ret = 0;
			pair<Block * , Node *> loc_(rhs.loc);
			for (;loc_.second != loc.second && loc_.second -> succ;++ ret , loc_.second = loc_.second -> succ);
			if (loc_.second != loc.second && loc_.first -> succ) ++ ret , loc_.first = loc_.first -> succ , loc_.second = loc_.first -> head;
			for (;loc_.first != loc.first && loc_.first -> succ;ret += loc_.first -> tot , loc_.first = loc_.first -> succ , loc_.second = loc_.first -> head);
			for (;loc_.second != loc.second && loc_.second -> succ;++ ret , loc_.second = loc_.second -> succ);
			if (loc_.second == loc.second) return ret;
			ret = 0 , loc_.first = rhs.loc.first , loc_.second = rhs.loc.second;
			for (;loc_.second != loc.second && loc_.second -> prec;-- ret , loc_.second = loc_.second -> prec);
			if (loc_.second != loc.second && loc_.first -> prec) -- ret , loc_.first = loc_.first -> prec , loc_.second = loc_.first -> tail;
			for (;loc_.first != loc.first && loc_.first -> prec;ret -= loc_.first -> tot , loc_.first = loc_.first -> prec , loc_.second = loc_.first -> tail);
			for (;loc_.second != loc.second && loc_.second -> prec;-- ret , loc_.second = loc_.second -> prec);
			return ret;
		}

		iterator& operator+=(const int &n) {return (*this) = (*this) + n;}

		iterator& operator-=(const int &n) {return (*this) = (*this) - n;}
		/**
		 * TODO iter++
		 */
		iterator operator++(int)
		{
			iterator ret = *this;
			(*this) = (*this) + 1;
			return ret;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {return (*this) = (*this) + 1;}
		/**
		 * TODO iter--
		 */
		iterator operator--(int)
		{
			iterator ret = *this;
			(*this) = (*this) - 1;
			return ret;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {return (*this) = (*this) - 1;}
		/**
		 * TODO *it
		 */
		T& operator*() const
		{
			if (loc.second -> value == nullptr) throw(invalid_iterator());
			return *(loc.second -> value);
		}
		/**
		 * TODO it->field
		 */
		T* operator->() const noexcept {return loc.second -> value;}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {return loc.second == rhs.loc.second;}
		bool operator==(const const_iterator &rhs) const {return loc.second == rhs.loc.second;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return loc.second != rhs.loc.second;}
		bool operator!=(const const_iterator &rhs) const {return loc.second != rhs.loc.second;}
	};

	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		friend class deque;
		friend class iterator;
	private:
		// data members.
		const deque *cor;
		pair<const Block * , const Node *> loc;
	public:
		const_iterator() {}
		explicit const_iterator(const deque * const &cor_ , const pair<const Block * , const Node *> &loc_) : cor(cor_) , loc(loc_) {}
		const_iterator(const const_iterator &other) : cor(other.cor) , loc(other.loc) {}
		const_iterator(const iterator &other) : cor() , loc() {}
		const_iterator &operator=(const const_iterator &rhs) {cor = rhs.cor , loc.first = rhs.loc.first , loc.second = rhs.loc.second;return *this;}
		// And other methods in iterator.
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		const_iterator operator+(const int &n) const
		{
			if (n < 0) return operator-(-n);
			int n_ = n;
			pair<const Block * , const Node *> loc_(loc);
			for (;n_ && loc_.second -> succ;-- n_ , loc_.second = loc_.second -> succ);
			if (n_)
			{
				-- n_;
				if (loc_.first = loc_.first -> succ) loc_.second = loc_.first -> head;
				else throw(invalid_iterator());
			}
			for (;n_ && n_ >= loc_.first -> tot;)
			{
				n_ -= loc_.first -> tot;
				if (loc_.first = loc_.first -> succ) loc_.second = loc_.first -> head;
				else throw(invalid_iterator());
			}
			for (;n_ && loc_.second -> succ;-- n_ , loc_.second = loc_.second -> succ);
			return const_iterator(cor , loc_);
		}

		const_iterator operator-(const int &n) const
		{
			if (n < 0) return operator+(-n);
			int n_ = n;
			pair<const Block * , const Node *> loc_(loc);
			for (;n_ && loc_.second -> prec;-- n_ , loc_.second = loc_.second -> prec);
			if (n_)
			{
				-- n_;
				if (loc_.first = loc_.first -> prec) loc_.second = loc_.first -> tail;
				else throw(invalid_iterator());
			}
			for (;n_ && n_ >= loc_.first -> tot;)
			{
				n_ -= loc_.first -> tot;
				if (loc_.first = loc_.first -> prec) loc_.second = loc_.first -> tail;
				else throw(invalid_iterator());
			}
			for (;n_ && loc_.second -> prec;-- n_ , loc_.second = loc_.second -> prec);
			return const_iterator(cor , loc_);
		}
		// return the distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const
		{
			if (cor != rhs.cor) throw (invalid_iterator());
			int ret = 0;
			pair<const Block * , const Node *> loc_(rhs.loc);
			for (;loc_.second != loc.second && loc_.second -> succ;++ ret , loc_.second = loc_.second -> succ);
			if (loc_.second != loc.second && loc_.first -> succ) ++ ret , loc_.first = loc_.first -> succ , loc_.second = loc_.first -> head;
			for (;loc_.first != loc.first && loc_.first -> succ;ret += loc_.first -> tot , loc_.first = loc_.first -> succ , loc_.second = loc_.first -> head);
			for (;loc_.second != loc.second && loc_.second -> succ;++ ret , loc_.second = loc_.second -> succ);
			if (loc_.second == loc.second) return ret;
			ret = 0 , loc_.first = rhs.loc.first , loc_.second = rhs.loc.second;
			for (;loc_.second != loc.second && loc_.second -> prec;-- ret , loc_.second = loc_.second -> prec);
			if (loc_.second != loc.second && loc_.first -> prec) -- ret , loc_.first = loc_.first -> prec , loc_.second = loc_.first -> tail;
			for (;loc_.first != loc.first && loc_.first -> prec;ret -= loc_.first -> tot , loc_.first = loc_.first -> prec , loc_.second = loc_.first -> tail);
			for (;loc_.second != loc.second && loc_.second -> prec;-- ret , loc_.second = loc_.second -> prec);
			return ret;
		}

		const_iterator& operator+=(const int &n) {return (*this) = (*this) + n;}

		const_iterator& operator-=(const int &n) {return (*this) = (*this) - n;}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int)
		{
			const_iterator ret = *this;
			(*this) = (*this) + 1;
			return ret;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() {return (*this) = (*this) + 1;}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int)
		{
			const_iterator ret = *this;
			(*this) = (*this) - 1;
			return ret;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() {return (*this) = (*this) - 1;}
		/**
		 * TODO *it
		 */
		const T& operator*() const
		{
			if (loc.second -> value == nullptr) throw(invalid_iterator());
			return *(loc.second -> value);
		}
		/**
		 * TODO it->field
		 */
		const T* operator->() const noexcept {return loc.second -> value;}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const const_iterator &rhs) const {return loc.second == rhs.loc.second;}
		bool operator==(const iterator &rhs) const {return loc.second == rhs.loc.second;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const const_iterator &rhs) const {return loc.second != rhs.loc.second;}
		bool operator!=(const iterator &rhs) const {return loc.second != rhs.loc.second;}
	};
	/**
	 * TODO Constructors
	 */
	deque() : total_size(1) , total_space(4) , block_size(2) , Blk(new Block(new Node)) , nodeend(Blk , Blk -> head) {}

	deque(const deque &other) : total_size(other.total_size) , total_space(other.total_space) , block_size(other.block_size) , Blk(copy(other.Blk)) {findend();}
	/**
	 * TODO Deconstructor
	 */
	~deque() {clear(Blk);}
	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other)
	{
		if (&other == this) return *this;
		clear(Blk) , Blk = copy(other.Blk) , total_size = other.total_size , total_space = other.total_space , block_size = other.block_size , findend();
		return *this;
	}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	const T & at(const size_t &pos) const
	{
		if (pos >= total_size - 1) throw(index_out_of_bound());
		size_t cur = 0;
		Block *blk;
		if ((pos + 1 << 1) <= total_size) for (blk = Blk;cur + blk -> tot <= pos;cur += blk -> tot , blk = blk -> succ);
		else
		{
			for (blk = nodeend.first , cur = total_size;cur - blk -> tot > pos;cur -= blk -> tot , blk = blk -> prec);
			cur -= blk -> tot;
		}
		Node *node;
		if ((pos - cur << 1) <= blk -> tot) for (node = blk -> head , ++ cur;cur <= pos;++ cur , node = node -> succ);
		else for (node = blk -> tail , cur += blk -> tot - 2;cur >= pos;-- cur , node = node -> prec);
		return *(node -> value);
	}

	T & at(const size_t &pos) {return const_cast<T &>(static_cast<const deque &>(*this).at(pos));}

	T & operator[](const size_t &pos) {return at(pos);}
	const T & operator[](const size_t &pos) const {return at(pos);}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const
	{
		if (total_size == 1) throw(container_is_empty());
		return *(Blk -> head -> value);
	}

	T & front() {return const_cast<T &>(static_cast<const deque &>(*this).front());}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const
	{
		if (total_size == 1) throw(container_is_empty());
		return *((nodeend.second -> prec ? nodeend.second -> prec : nodeend.first -> prec -> tail) -> value);
	}

	T & back() {return const_cast<T &>(static_cast<const deque &>(*this).back());}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {return iterator(this , pair<Block * , Node *>(Blk , Blk -> head));}
	const_iterator cbegin() const {return const_iterator(this , pair<Block * , Node *>(Blk , Blk -> head));}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {return iterator(this , nodeend);}
	const_iterator cend() const {return const_iterator(this , nodeend);}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const {return cbegin() == cend();}
	/**
	 * returns the number of elements
	 */
	size_t size() const {return total_size - 1;}
	/**
	 * clears the contents
	 */
	void clear() {clear(Blk) , total_size = 1 , total_space = 4 , block_size = 2 , Blk = new Block(new Node) , findend();}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value)
	{
		if (pos.cor != this) throw(runtime_error());
		return iterator(this , insert(pos.loc , value));
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos)
	{
		if (pos.cor != this || pos == end()) throw(runtime_error());
		return iterator(this , erase(pos.loc));
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {insert(end() , value);}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {erase(end() - 1);}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {insert(begin() , value);}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {erase(begin());}
};

}

#endif