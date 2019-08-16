//
// Created by NCY on 2019-08-16.
//

#ifndef STLCONTAINER_LIST_HPP
#define STLCONTAINER_LIST_HPP

#include "list_node.hpp"
#include "list_iterator.hpp"
#include <functional>

namespace sc::utils{

    //doubly-linked list with sentinel node
    template <class T>
    class list{
    public:

        using value_type = T;

        using size_type = std::size_t;

        using reference = T&;

        using const_refernce = const T&;

        using pointer = T*;

        using const_pointer = const T*;

        using iterator = sc::utils::list_iterator<T>;

        using const_iterator = sc::utils::list_iterator<T const>;

        /*
         * constructors, assignments, destructor
         */

        //default constructor
        list(): node_(), size_(0) {}

        //construct list of length count, default value if not specified
        explicit list(size_type count, const value_type& value = value_type());

        template <class InputIterator >
                list(InputIterator first, InputIterator last);

        list(const list& other);
        list(list&& other);
        list&operator=(const list& other);
        list&operator=(list&& other);

        ~list(){
            clear();
        }

        template <class InputIterator>
                void assign(InputIterator first, InputIterator last);

        void assign( size_type count, const T& value);

        /*
         * Element acess
         */

        reference front() { return *(node_.next());}

        const_refernce front() const{ return *(node_.next());}

        reference back() { return *(node_.prev());}

        const_refernce back() const{ return *(node_.prev());}

        /*
         * Iterators
         */

        iterator begin() { return iterator(node_.next());}

        const_iterator begin() const{ return const_iterator(node_.next());}

        const_iterator cbegin() const{ return const_iterator(node_.next());}

        //end returns the iterator points to the one-past-the-end element

        iterator end() { return iterator(node_);}

        const_iterator end() const{ return const_iterator(node_);}

        const_iterator cend() const{ return const_iterator(node_);}

        /*
         * Capacity
         */

        bool empty() const{ return size_ == 0;}

        size_type size() const{ return size_;}

        size_type max_size() const{ return std::numeric_limits<std::ptrdiff_t >::max();}

        /*
         * Modifiers
         */

        void clear();

        // insert one value before pos
        // returns the iterator points to the inserted element
        iterator insert( const_iterator pos, const T& value);

        iterator insert( const_iterator pos, T&& value);

        // insert copies of values before pos
        // returns the iterator points to the first inserted value
        iterator insert( const_iterator pos, size_type count, const T& value);

        // insert elements from range [first, last) before pos
        template <class InputIt>
                iterator insert( const_iterator pos, InputIt first, InputIt last);

        template <class... Args>
                iterator emplace(const_iterator pos, Args&&... args);

        // erase the element at pos, return the iterator following the last element
        // if pos refers to the last element, end() iterator is returned.
        iterator erase( const_iterator pos);

        // erase elements in range [first, last)
        iterator erase( const_iterator first, const_iterator last);

        void push_back( const T& value);
        void push_back( T&& value);

        // return reference to the inserted value: since C++ 17
        template <class... Args>
                reference emplace_back(Args&&... args);

        void pop_back();

        void push_front( const T& value);
        void push_front( T&& value);

        // resize the container to contain count elements.
        // if the current size is greater than count, the container is trimmed
        // if the current size is less than count, additional elements of either
        // default value or copies of value are appended.
        void resize( size_type count);
        void resize( size_type count, const value_type& value);

        void swap( list& other);

        /*
         * Operations
         */

        // merge two sorted lists
        void merge( list& other);
        void merge( list&& other);

        template <class Compare>
                void merge( list& other, Compare comp);

        template <class Compare>
                void merge( list&& other, Compare comp);

        // splice: transfers elements from one list to another
        // No elements are copied or moved, only the
        // internal pointers of the list nodes are re-pointed
        void splice( const_iterator pos, list& other);
        void splice( const iterator pos, list&& other);

        // remove all elements of value, return the removed number
        size_type remove( const T& value);

        // reverse the order of the elements
        void reverse();

        // remove all the duplicates
        void unique();

        // this sort uses operator> to compare elements
        void sort();

        template <class Compare>
                void sort( Compare comp);

        /*
         * Non-member functions
         */

        friend bool operator==(const list& l1, const list& l2);

        friend bool operator!=(const list& l1, const list& l2);

        friend bool operator<(const list& l1, const list& l2);

        friend bool operator>(const list& l1, const list& l2);

        friend bool operator<=(const list& l1, const list& l2);

        friend bool operator>=(const list& l1, const list& l2);

        //friend std::swap(list& l1, list& l2);

    private:
        list_node<T> node_; // sentinel node
        size_type size_;
    };

}

#endif //STLCONTAINER_LIST_HPP
