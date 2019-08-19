//
// Created by NCY on 2019-08-18.
//

#ifndef STLCONTAINER_DEQUE_HPP
#define STLCONTAINER_DEQUE_HPP

#include <functional>
#include "deque_iterator.hpp"


namespace sc::regular{

    // STL deque use array of array implementation, this deque has local contigous memory
    // and has stable references.
    template <class T>
    class deque{

        //declare member types
        using value_type = T;

        using size_type = std::size_t;

        using difference_type = std::ptrdiff_t;

        using reference = T&;

        using const_ref = const T&;

        using pointer = T*;

        using const_ptr = const T*;

        using iterator = sc::utils::deque_iterator<T>;

        using const_iterator = sc::utils::deque_iterator<T const>;

        /*
         * consturctors
         */

        //declare member functions
        //default constructor
        deque()noexcept : map_(nullptr), size_(0), start_(nullptr), finish_(nullptr) {}

        explicit deque(size_type size);

        //copy constructor/assignment
        deque(const deque&);
        deque& operator=(const deque&);

        //move constructor/assignment
        deque(deque&&) noexcept ;
        deque& operator=(deque&&) noexcept ;

        //destructor
        ~deque();

        //assign the value of the size in the container
        void assign(size_type count, const value_type& value);

        //assign the contents in the range [first, last)
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last);

        /*
         * element access
         */

        //access the element at the pos location, returns its reference
        reference at(size_type pos) ;

        const_ref at(size_type pos) const ;

        //subscription operator
        reference operator[](size_type pos) ;

        const_ref operator[](size_type pos) const;

        //access the front element, returns its reference
        reference front() ;
        const_ref front() const ; // cannot return front() here, infinite invoation

        //access the back element, returns its reference
        reference back() ;
        const_ref back() const;

        /*
         * Iterators
         */

        //returns an iterator to the begin of the elements
        iterator being() {return iterator(front());}
        const_iterator begin() const {return const_iterator(front());}

        //returns an iterator to the end of the elements
        iterator end() {return iterator(end());}
        const_iterator end() const { return const_iterator(end());}

        /*
         * Capacity
         */

        // checks whether this function is empty
        bool empty() const { return size() == 0;}

        // returns the number of elements
        size_type size() const ;

        // returns the maximum number of elements that can be held
        size_type max_size() const ;

        // reduces memory usage by freeing unused memory
        void shrink_to_fit();

        /*
         * Modifiers
         */

        void clear();

        // insert the value before iter, return the iterator points to the inserted element
        iterator insert(iterator iter, const value_type& value);
        const_iterator insert(const_iterator citer, const value_type& value);

        // construct the element directly at position before iter
        template <class... Args>
        reference emplace(const_iterator citer, Args&&... args);

        // erase the element at the position of iter
        iterator erase(iterator iter);
        iterator erase(const_iterator citer);

        // append the element at the back of the container
        void push_back(const value_type& value);
        void push_back(value_type&& value);

        // construct the element directly at the back of the container
        template <class... Args>
        reference emplace_back(Args&&... args);

        // removes the element at the back of the container
        void pop_back();

        // push a new element to the front of queue
        void push_front(const T& value);
        void push_front(T&& value);

        // construct an element at the front of queue
        template <class... Args>
        reference emplace_front(Args&&... args);

        // pop an element at the front of queue
        void pop_front();

        // resize the container to the specified size
        // if size is less than the number of elements, the exceeding elements are trimmed
        // if size is greater than the number of elements, the new places are filled with 0 or specified values
        void resize(size_type size);
        void resize(size_type size, const value_type& value);

        // exchange the contents with another vector
        void swap(deque& other);

        /*
         * Non-member functions
         */

        //all the relations can be derived by operator<
        friend bool operator==(const deque&, const deque&);

        friend bool operator!=(const deque&, const deque&);

        friend bool operator<(const deque&, const deque&);

        friend bool operator<=(const deque&, const deque&);

        friend bool operator>(const deque&, const deque&);

        friend bool operator>=(const deque&, const deque&);

    private:
        T** map_; // array of block pointers
        size_type size_; // the size of map array
        iterator start_; // iterator for first element in queue
        iterator finish_; // iterator for last element in queue

    };
}

#endif //STLCONTAINER_DEQUE_HPP
