#ifndef STLCONTAINER_VECTOR_HPP
#define STLCONTAINER_VECTOR_HPP

#include <cstddef>
#include <memory>
#include <cassert>
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
        vector()noexcept : start_(nullptr), finish_(nullptr), end_(nullptr) {};

        explicit vector(size_type size);

        //copy constructor/assignment
        vector(const vector&);
        vector& operator=(const vector&);

        //move constructor/assignment
        vector(vector&&) noexcept ;
        vector& operator=(vector&&) noexcept ;

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
        reference at(size_type pos) {
            return start_[pos];
        }

        const_ref at(size_type pos) const {
            return start_[pos];
        }

        //subscription operator
        reference operator[](size_type pos) {
            return start_[pos];
        }

        const_ref operator[](size_type pos) const{
            return start_[pos];
        }

        //access the front element, returns its reference
        reference front() { return *start_; }
        const_ref front() const {return *start_;} // cannot return front() here, infinite invoation

        //access the back element, returns its reference
        reference back() {return finish_[-1];}
        const_ref back() const {return finish_[-1];}

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
        size_type size() const {return finish_ - start_; }

        // returns the maximum number of elements that can be held
        size_type max_size() const {return end_ - start_;}

        // reserve this container to the specified size.
        // if the size parameter doesn't exceed the current
        // max size, this function has no effect
        void reserve(size_type n);

        // returns the number of elements that can be held
        size_type capacity() const {return end_ - start_;}

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

        //all the relations can be derived by operator<
        friend bool operator==(const vector&, const vector&);

        friend bool operator!=(const vector&, const vector&);

        friend bool operator<(const vector&, const vector&);

        friend bool operator<=(const vector&, const vector&);

        friend bool operator>(const vector&, const vector&);

        friend bool operator>=(const vector&, const vector&);

        //friend std::swap(vector&, vector&);


    private:
        void changeCapacity(size_type n); //change the memory allocation for the container
        pointer start_; // points to the start of container
        pointer finish_; // points to one-past-the-last element
        pointer end_; // points to the end of container

    };


    // construct the container with given size
    template <class T>
    vector<T>::vector(size_type size) {
        start_ = static_cast<pointer >(::operator new(size * sizeof(size_type)));
        finish_ = start_;
        end_ = start_ + size;

    }

    // copy constructor
    template <class T>
    vector<T>::vector(const vector& other) {
        // only the memory that stores elements need to be copied, the allocated memory
        // of two vectors doesn't need to be the same
        start_ = static_cast<pointer >(::operator new(other.size() * sizeof(size_type)));
        end_ = start_ + other.size();
        try {
            finish_ = std::uninitialized_copy(other.start_, other.finish_, finish_);
            // finish and end must points to the same memory in this context.
            assert(finish_ == end_);
        }catch (...){
            ::operator delete(start_);
            throw;
        }

    }

    // move constructor
    template <class T>
    vector<T>::vector(sc::regular::vector<T> && other) noexcept {
        start_ = other.start_;
        other.start_ = nullptr;
        end_ = other.end_;
        other.end_ = nullptr;
        finish_ = other.finish_;
        other.finish_ = nullptr;
    }

    // copy assignment
    template <class T>
    vector<T>& vector<T>::operator=(const vector<T> & other) {
        start_ = static_cast<pointer>(::operator new(other.size() * sizeof(size_type)));
        end_ = start_ + other.size();
        try {
            finish_ = std::uninitialized_copy(other.start_, other.finish_, finish_);
        }catch (...){
            ::operator delete(start_);
            throw;
        }

    }

    // move assignment
    template <class T>
    vector<T>& vector<T>::operator=(sc::regular::vector<T> &&other) noexcept {
        start_ = other.start_;
        other.start_ = nullptr;
        end_ = other.end_;
        other.end_ = nullptr;
        finish_ = other.finish_;
        other.finish_ = nullptr;
    }

    //destructor
    template <class T>
    vector<T>::~vector() {
        clear();
        ::operator delete(start_);
    }

    template <class T>
    void vector<T>::clear() {
        // if the container is not empty, destroy the elements
        if(size())
            std::destroy(start_, finish_);
    }

    template<class T>
    void vector<T>::changeCapacity(vector::size_type n) {
        // allocates a new chunk of memory of this size
        auto new_start = static_cast<pointer >(::operator new(n * sizeof(value_type)));
        size_type old_size = size();
        try {
            // if size is smaller than old size, the extra part will be trimmed
            std::uninitialized_move(start_, start_+n, new_start);
        }catch (...){
            ::operator delete(new_start);
            throw;
        }

        //deallocate the chunk of memory of the old vector
        ::operator delete(start_);
        start_ = new_start;
        finish_ = new_start + old_size;

        // if the new size is smaller, trim the extra part
        if(old_size > n)
            end_ = finish_;
        else
            end_ = new_start + n;

    }

    template<class T>
    void vector<T>::reserve(vector::size_type n) {
        // if size doesn't exceed current size, this
        // function has no effect
        if(n <= size())
            return;

        changeCapacity(n);

    }

    template<class T>
    void vector<T>::shrink_to_fit() {
        // if current size doesn't exceed capacity, function has no effect
        if(capacity() == size())
            return;
        changeCapacity(size());

    }

    template<class T>
    typename vector<T>::iterator vector<T>::insert(vector::iterator iter, const value_type &value) {
        //record the relative position of iter, because changeCapacity invalidates iterator
        difference_type offset = iter - begin();

        //if it's at max capacity, double the size
        if(capacity() == size())
            changeCapacity(2 * capacity());

        //insert the element in place

        std::uninitialized_copy(start_ + offset, finish_, start_ + offset + 1);
        start_[offset] = value;
        return vector::iterator(start_+offset);

    }

    template<class T>
    typename vector<T>::const_iterator vector<T>::insert(vector::const_iterator citer, const value_type &value){
        //record the relative position of iter, because changeCapacity invalidates iterator
        difference_type offset = citer - begin();

        //if it's at max capacity, double the size
        if(capacity() == size())
            changeCapacity(2 * capacity());

        //insert the element in place

        std::uninitialized_copy(start_+offset, finish_, start_+offset+1);
        start_[offset] = value;
        return const_iter(start_+offset);
    }

    template<class T>
    typename vector<T>::iterator vector<T>::erase(vector::iterator iter) {
        // record the offset, because const_iterator-pointed element cannot be modified
        difference_type offset = iter - begin();

        // copy the elements in place
        std::uninitialized_copy(start_+offset+1, finish_, start_+offset);

        //destroy the last element
        std::destroy_at(finish_);

        //decrement the finish pointer
        --finish_;

        return start_ + offset;
    }

    template<class T>
    void vector<T>::push_back(const value_type &value) {
        if(capacity() == size())
            //double the capacity to achieve asymptotic logarithmic time complexity
            changeCapacity(2 * capacity());

        *finish_ = value;
        ++finish_;

    }

    template<class T>
    void vector<T>::push_back(value_type &&value) {
        if(capacity() == size())
            //double the capacity to achieve asymptotic logarithmic time complexity
            changeCapacity(2 * capacity());

        *finish_ = std::move(value);
        ++finish_;
    }

    template<class T>
    void vector<T>::pop_back() {
        assert(size() != 0);
        //destruct the element at finish-1
        std::destroy_at(finish_--);

    }

    template<class T>
    void vector<T>::resize(vector::size_type size) {
        changeCapacity(size);

    }

    template<class T>
    void vector<T>::resize(vector::size_type size, const value_type &value) {
        changeCapacity(size);

        //if changeCapacity is the result of expansion, fill the empty spaces
        if(capacity() > this->size()){
            std::uninitialized_fill_n(finish_, end_-1 ,value);
        }

    }

    template<class T>
    void vector<T>::swap(vector &other) {
        // this swap only swap pointers
        // however, this function invalidates iterators and references
        std::swap(start_, other.start_);
        std::swap(end_, other.end_);
        std::swap(finish_, other.finish_);

    }

    template <class T>
    bool operator==(const vector<T> &v1, const vector<T> &v2) {
        if(v1.size == v2.size()){
            for(auto iter1= v1.begin(), iter2= v2.begin(); v1!=v1.end(); ++v1, ++v2){
                if(*iter1 != *iter2)
                    break;
                return true;
            }
        }

        return false;
    }

    template <class T>
    bool operator!=(const vector<T> &v1, const vector<T> &v2) {
        return !(v1 == v2);
    }

    template <class T>
    bool operator>(const vector<T> &v1, const vector<T> &v2) {
        // lexically compare the elements if the size is equal
        if(v1.size() == v2.size()){
            for(auto iter1= v1.begin(), iter2= v2.begin(); v1!=v1.end(); ++v1, ++v2){
                // if the current element is the same
                if(*iter1 == *iter2)
                    continue;

                return (*iter1 > *iter2);
            }

        }

        return (v1.size() > v2.size());
    }

    template <class T>
    bool operator<(const vector<T> &v1, const vector<T> &v2) {
        return v2 > v1;
    }

    template <class T>
    bool operator>=(const vector<T> &v1, const vector<T> &v2) {
        return !(v2 > v1);
    }

    template <class T>
    bool operator<=(const vector<T> &v1, const vector<T> &v2) {
        return !(v1 > v2);
    }

    template<class T>
    void vector<T>::assign(vector::size_type count, const value_type &value) {
        clear();
        if(count > capacity()){
            changeCapacity(count);
        }
        std::uninitialized_fill_n(start_, count, value);

    }

    template<class T>
    template<class InputIterator>
    void vector<T>::assign(InputIterator first, InputIterator last) {
        clear();
        size_type new_size = last - first;
        if(new_size > capacity()){
            changeCapacity(new_size);
        }
        std::uninitialized_copy(first, last, start_);

    }



}

#endif //STLCONTAINER_VECTOR_HPP