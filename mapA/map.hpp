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

template <class T>
void swap(T &x , T &y)
{
	T z(x);
	x = y , y = z;
}

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
private:
	enum color_type {RED , BLACK};

	struct Node
	{
		Key key;
		T value;
		color_type color;
		Node *left , *right , *parent;

		Node (const Key &key_ , const T &value_) : key(key_) , value(value_) , color(RED) , left(nullptr) , right(nullptr) , parent(nullptr) {}
	}*nil;

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
				else if (y -> left == BLACK && y -> right == BLACK)
				{
					x -> color = y -> color = RED , x -> parent -> color = BLACK;
					x = x -> parent;
				}
				else
				{
					if (y -> right == BLACK)
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
				else if (y -> left == BLACK && y -> right == BLACK)
				{
					x -> color = y -> color = RED , x -> parent -> color = BLACK;
					x = x -> parent;
				}
				else
				{
					if (y -> left == BLACK)
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
		Node *x = node;
		for (;x -> left != nil;x = x -> left);
		return x;
	}

	Node *findmax(const Node * const node) const
	{
		Node *x = node;
		for (;x -> right != nil;x = x -> right);
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
		for (;x != nil && x -> key != key;x = compare(key , x -> key) ? x -> left : x -> right);
		return x;
	}

	Node *insert(Node * const node)
	{
		Node *x = nil -> left , *y = nil;
		for (;x != nil;y = x , x = compare(key , x -> key) ? x -> left : x -> right);
		if (compare(key , y -> key)) y -> left = node;
		else y -> right = node;
		node -> color = RED , node -> left = node -> right = nil , node -> parent = y;
		insert_fixup(x);
	}

	void erase(Node *x)
	{
		color_type col = x -> color;Node *y;
		if (x -> left == nil) transplant(x , x -> right) , y = x -> right;
		else if (x -> right == nil) transplant(x , x -> left) , y = x -> left;
		else
		{
			Node *z = findmin(x -> right);col = z -> color , y = z -> right;
			if (z -> parent == x) z -> right -> parent = z;
			else
			{
				transplant(z , z -> right);
				z -> right = x -> right , z -> right -> parent = z;
			}
			transplant(x , z);
			z -> color = x -> color , z -> left = x -> left , z -> left -> parent = z;
		}
		delete x;
		if (col == BLACK) erase_fixup(y);
	}
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		iterator() {
			// TODO
		}
		iterator(const iterator &other) {
			// TODO
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {}
		bool operator==(const iterator &rhs) const {}
		bool operator==(const const_iterator &rhs) const {}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {}
		bool operator!=(const const_iterator &rhs) const {}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			// data members.
		public:
			const_iterator() {
				// TODO
			}
			const_iterator(const const_iterator &other) {
				// TODO
			}
			const_iterator(const iterator &other) {
				// TODO
			}
			// And other methods in iterator.
			// And other methods in iterator.
			// And other methods in iterator.
	};
	/**
	 * TODO two constructors
	 */
	map() {}
	map(const map &other) {}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {}
	/**
	 * TODO Destructors
	 */
	~map() {}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {}
	const T & at(const Key &key) const {}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {}
	const_iterator cbegin() const {}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {}
	const_iterator cend() const {}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {}
	/**
	 * clears the contents
	 */
	void clear() {}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {}
	const_iterator find(const Key &key) const {}
};

}

#endif
