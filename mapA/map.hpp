/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {
template <class T>
void swap(T &x , T &y)
{
	T z(x);
	x = y , y = z;
}

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
private:
	enum color_type {RED , BLACK};

	struct Node
	{
		value_type *value;
		color_type color;
		Node *left , *right , *parent;

		Node (value_type * const &value_ = nullptr , const color_type &color_ = BLACK) : value(value_) , color(color_) , left(nullptr) , right(nullptr) , parent(nullptr) {}
		Node (const Node &other) : value(new value_type (*other.value)) , color(other.color) , left(nullptr) , right(nullptr) , parent(nullptr) {}
		Node &operator=(const Node &other) = delete;
		~Node () {delete value;}
	}*nil , *nodebegin;

	Compare compare;
	size_t tot;

	void left_rotate(Node *x)
	{
		Node *y = x -> parent;
		if (x -> left != nil) x -> left -> parent = y;
		y -> right = x -> left;
		if (y -> parent -> left == y) y -> parent -> left = x;
		else y -> parent -> right = x;
		x -> parent = y -> parent , y -> parent = x , x -> left = y;
	}

	void right_rotate(Node *x)
	{
		Node *y = x -> parent;
		if (x -> right != nil) x -> right -> parent = y;
		y -> left = x -> right;
		if (y -> parent -> left == y) y -> parent -> left = x;
		else y -> parent -> right = x;
		x -> parent = y -> parent , y -> parent = x , x -> right = y;
	}

	void insert_fixup(Node *x)
	{
		for (Node *y;x -> parent -> color == RED;)
			if (x -> parent -> parent -> left == x -> parent)
			{
				y = x -> parent -> parent -> right;
				if (y -> color == RED)
				{
					x -> parent -> color = y -> color = BLACK , y -> parent -> color = RED;
					x = y -> parent;
				}
				else
				{
					y = x -> parent;
					if (y -> right == x) left_rotate(x) , x = y , y = x -> parent;
					right_rotate(y) , swap(y -> color , y -> right -> color);
				}
			}
			else
			{
				y = x -> parent -> parent -> left;
				if (y -> color == RED)
				{
					x -> parent -> color = y -> color = BLACK , y -> parent -> color = RED;
					x = y -> parent;
				}
				else
				{
					y = x -> parent;
					if (y -> left == x) right_rotate(x) , x = y , y = x -> parent;
					left_rotate(y) , swap(y -> color , y -> left -> color);
				}
			}
		nil -> left -> color = BLACK;
	}

	void erase_fixup(Node *x)
	{
		for (Node *y;x -> parent != nil && x -> color == BLACK;)
			if (x -> parent -> left == x)
			{
				y = x -> parent -> right;
				if (y -> color == RED) left_rotate(y) , swap(y -> color , y -> left -> color);
				else if (y -> left -> color == BLACK && y -> right -> color == BLACK) y -> color = RED , x = x -> parent;
				else
				{
					if (y -> right -> color == BLACK)
					{
						right_rotate(y -> left) , swap(y -> color , y -> parent -> color);
						y = y -> parent;
					}
					left_rotate(y) , swap(y -> color , y -> left -> color) , y -> right -> color = BLACK;
					x = nil -> left;
				}
			}
			else
			{
				y = x -> parent -> left;
				if (y -> color == RED) right_rotate(y) , swap(y -> color , y -> right -> color);
				else if (y -> left -> color == BLACK && y -> right -> color == BLACK) y -> color = RED , x = x -> parent;
				else
				{
					if (y -> left -> color == BLACK)
					{
						left_rotate(y -> right) , swap(y -> color , y -> parent -> color);
						y = y -> parent;
					}
					right_rotate(y) , swap(y -> color , y -> right -> color) , y -> left -> color = BLACK;
					x = nil -> left;
				}
			}
		x -> color = BLACK;
	}

	Node *findmin(const Node * const node) const
	{
		Node *x = const_cast<Node *>(node);
		for (;x -> left != nil;x = x -> left);
		return x;
	}

	Node *findmax(const Node * const node) const
	{
		Node *x = const_cast<Node *>(node);
		for (;x -> right != nil;x = x -> right);
		return x;
	}

	Node *prec(const Node * const node) const
	{
		Node *x = const_cast<Node *>(node);
		if (x -> left == nil)
		{
			for (;x != nil && x -> parent -> left == x;x = x -> parent);
			if (x == nil) throw(invalid_iterator());
			x = x -> parent;
		}else x = findmax(x -> left);
		return x;
	}

	Node *succ(const Node * node) const
	{
		Node *x = const_cast<Node *>(node);
		if (x -> right == nil)
		{
			if (x == nil) throw(invalid_iterator());
			for (;x != nil && x -> parent -> right == x;x = x -> parent);
			x = x -> parent;
		}else x = findmin(x -> right);
		return x;
	}

	void transplant(Node * const x , Node * const y)
	{
		if (x == x -> parent -> left) x -> parent -> left = y;
		else x -> parent -> right = y;
		y -> parent = x -> parent;
	}

	Node *search(const Key &key) const
	{
		Node *x = nil -> left;
		for (;x != nil && (compare(key , x -> value -> first) || compare(x -> value -> first , key));x = compare(key , x -> value -> first) ? x -> left : x -> right);
		return x;
	}

	void insert(Node * const node)//node -> color == RED
	{
		Node *x = nil -> left , *y = nil;
		for (;x != nil;y = x , x = compare(node -> value -> first , x -> value -> first) ? x -> left : x -> right);
		if (y == nil || compare(node -> value -> first , y -> value -> first)) y -> left = node;
		else y -> right = node;
		node -> left = node -> right = nil , node -> parent = y;
		++ tot , insert_fixup(node) , nil -> parent = nil , nodebegin = findmin(nil);
	}

	void erase(Node *x)
	{
		color_type col = x -> color;Node *y;
		if (x -> left == nil) y = x -> right , transplant(x , x -> right);
		else if (x -> right == nil) y = x -> left , transplant(x , x -> left);
		else
		{
			Node *z = findmin(x -> right);col = z -> color , y = z -> right;
			if (z -> parent == x) z -> right -> parent = z;//assure that nil -> parent = z
			else
			{
				transplant(z , z -> right);
				z -> right = x -> right , z -> right -> parent = z;
			}
			transplant(x , z);
			z -> color = x -> color , z -> left = x -> left , z -> left -> parent = z;
		}
		delete x , -- tot;
		if (col == BLACK) erase_fixup(y) , nil -> parent = nil;
		nodebegin = findmin(nil);
	}

	Node *copy(Node *x , Node * const &nilx , Node * const &nily)
	{
		if (x == nilx) return nily;
		Node *y = new Node (*x);++ tot;
		y -> left = copy(x -> left , nilx , nily) , y -> right = copy(x -> right , nilx , nily) , y -> parent = nil;
		if (y -> left != nily) y -> left -> parent = y;
		if (y -> right != nily) y -> right -> parent = y;
		return y;
	}

	void clear(Node *&x)
	{
		if (x == nil) return;
		if (x -> parent == nil) nodebegin = nil;
		clear(x -> left) , clear(x -> right) , delete x , x = nil , -- tot;
	}
public:
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
		friend class map;
		friend class const_iterator;
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		const map *cor;
		Node *ptr;
	public:
		explicit iterator(const map * const &cor_ = nullptr , Node * const &ptr_ = nullptr) : cor(cor_) , ptr(ptr_) {}

		iterator(const iterator &other) : cor(other.cor) , ptr(other.ptr) {}

		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		/**
		 * TODO iter++
		 */
		iterator operator++(int)
		{
			iterator ret(*this);
			ptr = cor -> succ(ptr);
			return ret;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++()
		{
			ptr = cor -> succ(ptr);
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int)
		{
			iterator ret(*this);
			ptr = cor -> prec(ptr);
			return ret;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--()
		{
			ptr = cor -> prec(ptr);
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {return *(ptr -> value);}
		bool operator==(const iterator &rhs) const {return ptr == rhs.ptr;}
		bool operator==(const const_iterator &rhs) const {return ptr == rhs.ptr;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return ptr != rhs.ptr;}
		bool operator!=(const const_iterator &rhs) const {return ptr != rhs.ptr;}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {return ptr -> value;}
	};
	class const_iterator {
		friend class map;
	// it should has similar member method as iterator.
	//  and it should be able to construct from an iterator.
	private:
		// data members.
		const map *cor;
		const Node *ptr;
	public:
		explicit const_iterator(const map * const &cor_ = nullptr , const Node * const &ptr_ = nullptr) : cor(cor_) , ptr(ptr_) {}

		const_iterator(const const_iterator &other) : cor(other.cor) , ptr(other.ptr) {}

		const_iterator(const iterator &other) : cor(other.cor) , ptr(other.ptr) {}

		const_iterator operator++(int)
		{
			const_iterator ret(*this);
			ptr = cor -> succ(ptr);
			return ret;
		}

		const_iterator & operator++()
		{
			ptr = cor -> succ(ptr);
			return *this;
		}

		const_iterator operator--(int)
		{
			const_iterator ret(*this);
			ptr = cor -> prec(ptr);
			return ret;
		}

		const_iterator & operator--()
		{
			ptr = cor -> prec(ptr);
			return *this;
		}

		const value_type & operator*() const {return *(ptr -> value);}

		bool operator==(const const_iterator &rhs) const {return ptr == rhs.ptr;}
		bool operator==(const iterator &rhs) const {return ptr == rhs.ptr;}

		bool operator!=(const const_iterator &rhs) const {return ptr != rhs.ptr;}
		bool operator!=(const iterator &rhs) const {return ptr != rhs.ptr;}

		const value_type* operator->() const noexcept {return ptr -> value;}
	};
	/**
	 * TODO two constructors
	 */
	map() {nodebegin = nil = new Node , nil -> parent = nil -> left = nil -> right = nil , tot = 0;}

	map(const map &other)
	{
		nil = new Node , nil -> parent = nil -> right = nil , tot = 0;
		nil -> left = copy(other.nil -> left , other.nil , nil);
		nodebegin = findmin(nil);
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other)
	{
		if (&other == this) return *this;
		clear(nil -> left) , nil -> left = copy(other.nil -> left , other.nil , nil);
		nodebegin = findmin(nil);
		return *this;
	}
	/**
	 * TODO Destructors
	 */
	~map() {clear(nil -> left) , delete nil;}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	const T & at(const Key &key) const
	{
		Node *node = search(key);
		if (node == nil) throw(index_out_of_bound());
		return node -> value -> second;
	}

	T & at(const Key &key) {return const_cast<T &>(static_cast<const map &>(*this).at(key));}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key)
	{
		Node *node = search(key);
		if (node == nil) node = new Node (new value_type (key , T()) , RED) , insert(node);
		return node -> value -> second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {return at(key);}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {return iterator(this , nodebegin);}
	const_iterator cbegin() const {return const_iterator(this , nodebegin);}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {return iterator(this , nil);}
	const_iterator cend() const {return const_iterator(this , nil);}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {return cbegin() == cend();}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {return tot;}
	/**
	 * clears the contents
	 */
	void clear() {clear(nil -> left);}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator , bool> insert(const value_type &value)
	{
		Node *node = search(value.first);
		if (node == nil)
		{
			node = new Node (new value_type (value) , RED) , insert(node);
			return pair<iterator , bool>(iterator(this , node) , true);
		}
		else return pair<iterator , bool>(iterator(this , node) , false);
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos)
	{
		if (pos.cor != this || pos == end()) throw(runtime_error());
		erase(pos.ptr);
	}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {return search(key) != nil;}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {return iterator(this , search(key));}

	const_iterator find(const Key &key) const {return const_iterator(this , search(key));}
};

}

#endif
