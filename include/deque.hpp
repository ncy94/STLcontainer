//
// Created by NCY on 2019-08-18.
//

#ifndef STLCONTAINER_DEQUE_HPP
#define STLCONTAINER_DEQUE_HPP

#include <functional>
#include <cmath>
#include "deque_iterator.hpp"


namespace sc::regular{

    // STL deque use array of array implementation, this deque has local contigous memory
    // and has stable references.
    template <class T>
    class deque{

    public:

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

        explicit deque(size_type count);

        template <class InputIt>
        deque( InputIt first, InputIt last);

        //copy/move constructor
        deque(const deque&);
        deque(deque&&) noexcept ;

        //copy/move assignment operator
        //use copy-and-swap idiom and copy elision for better efficiency
        deque&operator=(deque other);

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

        friend void swap(deque& q1, deque& q2);

    private:
        T** map_; // array of block pointers
        size_type size_; // the size of map array
        iterator start_; // iterator for first element in queue
        iterator finish_; // iterator for last element in queue

    };

    template <class T>
    deque<T>::deque(deque::size_type count) {
        size_ = ceil((float)count / BLOCK_SIZE);
        size_type block_offset = count % BLOCK_SIZE;
        map_ = static_cast<T**>(::operator new(size_ * sizeof(T*)));
        for(int i=0; i<size_; ++i){
            *(map_+i) = static_cast<T*>(::operator new(BLOCK_SIZE * sizeof(T)));
        }

        // initialize start iterator
        start_.set(map_, *map_);

        //initialize finish iterator
        //start_ and finish_ points to the same element
        finish_.set(map_, *map_);

    }

    template<class T>
    template<class InputIt>
    deque<T>::deque(InputIt first, InputIt last) {
        size_type element_count = last - first;

        // call default construction
        this->deque(element_count); //size_ uninitialized? remember to test

        // provide strong exception guarantee
        try {
            for (int i=0, iter=first; i<size_; ++i) {
                if(i == size_-1){
                    // copy the last block
                    finish_.ptr_ = std::uninitialized_copy(iter, last, finish_.first_);
                    assert(last - iter == element_count % BLOCK_SIZE);
                } else {
                    // copy the entire block
                    std::uninitialized_copy(iter, iter + BLOCK_SIZE, finish_.first_);
                    finish_ += BLOCK_SIZE;
                }
            }
        }catch (...){
            // if throws, deallocates all allocated memory
            for(int i=0; i<size_; ++i){
                ::operator delete(*(map_+i));
            }
            ::operator delete(map_);
        }

    }

    template<class T>
    deque<T>::deque(const deque &other) {
        // first call default constructor
        deque(other.size_);

        // provide strong exception guarantee
        try {
            for (int i=0; i<size_; ++i) {
                if(i == size_-1){
                    // copy the last block
                    finish_.ptr_ = std::uninitialized_copy(*(other.map_)+i, other.finish_.ptr_, finish_.first_);
                    assert(other.finish_.ptr_ - *(other.map_)-i == other.size_ % BLOCK_SIZE);
                } else {
                    // copy the entire block
                    std::uninitialized_copy_n((*other.map_+i), BLOCK_SIZE, finish_.first_);
                    finish_ += BLOCK_SIZE;
                }
            }
        }catch (...){
            // if throws, deallocates all allocated memory
            for(int i=0; i<size_; ++i){
                ::operator delete(*(map_+i));
            }
            ::operator delete(map_);
        }

    }

    template<class T>
    deque<T>::deque(deque &&other) noexcept {
        map_ = other.map_;
        other.map_ = nullptr;
        start_ = other.start_;
        other.start_ = nullptr;
        finish_ = other.finish_;
        other.finish_ = nullptr;
        size_ = other.size_;
        other.size_ = nullptr;
    }

    // this function has no-throw guarantee because std::swap does not throw
    template <class T>
    void swap(deque<T> &q1, deque<T> &q2) {
        // by swapping the pointers, the two containers are effectively swapped
        std::swap(q1.map_, q2.map_);
        std::swap(q1.size_, q2.size_);
        std::swap(q1.start_, q2.start_);
        std::swap(q1.finish_, q2.finish_);
    }

    template<class T>
    deque<T> &deque<T>::operator=(deque other) {
        // use copy-and-swap idiom here
        swap(*this, other);

        return *this;
    }

    template<class T>
    deque<T>::~deque() {
        // first clear the data
        clear();
        // second deallocate the memory
        for(int i=0; i<size_; ++i){
            ::operator delete(*(map_+i));
        }
        ::operator delete(map_);
    }


}

#endif //STLCONTAINER_DEQUE_HPP
