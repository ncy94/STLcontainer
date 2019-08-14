#ifndef STLCONTAINER_VECTOR_HPP
#define STLCONTAINER_VECTOR_HPP

#include <cstddef>
#include "iterator.hpp"

namespace sc::regular{

    template <class T>
    class vector{
    public:

        //declare member types
        using value_type = T;

        using size_type = std::size_t;

        using difference_type = std::ptrdiff_t;

        using reference = T&;

        using const_ref = const T&;

        using pointer = T*;

        using const_ptr = const T*;

        using iterator = sc::utils::iterator<T>;

        using const_iterator = sc::utils::iterator<T const>;

        /*
         * consturctors
         */

        //declare member functions
        //default constructor
        vector();

        //copy constructor/assignment
        vector(const vector&);
        vector& operator=(const vector&);

        //move constructor/assignment
        vector(vector&&);
        vector operator=(vector&&);

        //destructor
        ~vector();

        //assign the value of the size in the container
        void assign(size_type count, const value_type& value);

        //assign the contents in the range [first, last)
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last);

        /*
         * element access
         */

        //access the element at the pos location, returns its reference
        reference at(size_type pos);
        const_ref at(size_type pos) const;

        //subscription operator
        reference operator[](size_type pos);
        const_ref operator[](size_type pos) const;

        //access the front element, returns its reference
        reference front();
        const_ref front() const;

        //access the back element, returns its reference
        reference back();
        const_ref back() const;

        /*
         * Iterators
         */

        //returns an iterator to the begin of the elements
        iterator being();
        const_iterator begin() const;

        //returns an iterator to the end of the elements
        iterator end();
        const_iterator end() const;

        /*
         * Capacity
         */

        // checks whether this function is empty
        bool empty() const;

        // returns the number of elements
        size_type size() const;

        // returns the maximum number of elements that can be held
        size_type max_size() const;

        // reserve this container to the specified size.
        // if the size parameter doesn't exceed the current
        // max size, this function has no efffect
        void reserve(size_type size);

        // returns the number of elements that can be held
        size_type capacity() const;

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
        void emplace(const_iterator citer, Args&&... args);

        // erase the element at the position of iter
        iterator erase(iterator iter);
        iterator erase(const_iterator citer);

        // append the element at the back of the container
        void push_back(const value_type& value);
        void push_back(value_type&& value);

        // construct the element directly at the back of the container
        template <class... Args>
        void emplace_back(Args&&... args);

        // removes the element at the back of the container
        void pop_back();

        // resize the container to the specified size
        // if size is less than the number of elements, the exceeding elements are trimmed
        // if size is greater than the number of elements, the new places are filled with 0 or specified values
        void resize(size_type size);
        void resize(size_type size, const value_type& value);

        // exchange the contents with another vector
        void swap(vector& other);

        /*
         * Non-member functions
         */

        friend bool operator==(const vector&, const vector&);

        friend bool operator!=(const vector&, const vector&);

        friend bool operator<(const vector&, const vector&);

        friend bool operator<=(const vector&, const vector&);

        friend bool operator>(const vector&, const vector&);

        friend bool operator>=(const vector&, const vector&);

        //friend std::swap(vector&, vector&);


    private:
        pointer start_; // points to the start of container
        pointer finish_; // points to the last element
        pointer end_; // points to the end of container



    };

}

#endif //STLCONTAINER_VECTOR_HPP