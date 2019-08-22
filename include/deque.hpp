//
// Created by NCY on 2019-08-18.
//

#ifndef STLCONTAINER_DEQUE_HPP
#define STLCONTAINER_DEQUE_HPP

#include <functional>
#include <cmath>
#include <iostream>
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
        iterator begin() {return start_;}
        const_iterator begin() const {return const_iterator(start_);}

        //returns an iterator to the end of the elements
        iterator end() {return finish_;}
        const_iterator end() const { return const_iterator(finish_);}

        /*
         * Capacity
         */

        // checks whether this function is empty
        bool empty() const { return start_.ptr_ == finish_.ptr_;}

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
        // this function invalidates iterators and references
        iterator insert(iterator iter, const value_type& value);
        const_iterator insert(const_iterator citer, const value_type& value);

        // construct the element directly at position before iter
        template <class... Args>
        reference emplace(const_iterator citer, Args&&... args);

        // erase the element at the position of iter
        // this function invalidates iterators and references
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
        void growfront(size_type n);
        void growrear(size_type n);
        T** map_; // array of block pointers
        size_type size_; // the size of map array
        iterator start_; // iterator for first element in queue
        iterator finish_; // iterator for last element in queue

    };

    template <class T>
    deque<T>::deque(deque::size_type count) {
        size_ = ceil((float)count / BLOCK_SIZE);
        map_ = static_cast<T**>(::operator new(size_ * sizeof(T*)));
        for(int i=0; i<size_; ++i){
            *(map_+i) = static_cast<T*>(::operator new(BLOCK_SIZE * sizeof(T)));
        }

        // initialize start iterator
        start_.set(map_);

        //initialize finish iterator
        //start_ and finish_ points to the same element
        finish_.set(map_);

    }

    template<class T>
    template<class InputIt>
    deque<T>::deque(InputIt first, InputIt last) {
        size_type element_count = last - first;

        size_ = ceil((float)element_count / BLOCK_SIZE);
        size_type block_offset = element_count % BLOCK_SIZE;
        map_ = static_cast<T**>(::operator new(size_ * sizeof(T*)));
        for(int i=0; i<size_; ++i){
            *(map_+i) = static_cast<T*>(::operator new(BLOCK_SIZE * sizeof(T)));
        }

        // inititialize start,finish iterator, at the beginning of map
        start_.set(map_);
        finish_.set(map_);

        // provide strong exception guarantee
        try {
            for(auto iter=first; iter!=last; ++iter){
                std::uninitialized_copy_n(iter,1,finish_.ptr_);
                ++finish_;
            }
//            for (int i=0, iter=first; i<size_; ++i) {
//                if(i == size_-1){
//                    // copy the last block
//                    finish_.ptr_ = std::uninitialized_copy(iter, last, finish_.first_);
//                    assert(last - iter == element_count % BLOCK_SIZE);
//                } else {
//                    // copy the entire block
//                    std::uninitialized_copy(iter, iter + BLOCK_SIZE, finish_.first_);
//                    finish_ += BLOCK_SIZE;
//                }
//            }
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

        size_ = other.size_;
        map_ = static_cast<T**>(::operator new(size_ * sizeof(T*)));
        for(int i=0; i<size_; ++i){
            *(map_+i) = static_cast<T*>(::operator new(BLOCK_SIZE * sizeof(T)));
        }

        // provide strong exception guarantee
        try {
            for (int i=0; i<size_; ++i) {
                std::uninitialized_copy(*(other.map_+i), *(other.map_+i)+BLOCK_SIZE, *(map_+i));
            }
        }catch (...){
            // if throws, deallocates all allocated memory
            for(int i=0; i<size_; ++i){
                ::operator delete(*(map_+i));
            }
            ::operator delete(map_);
        }
        start_.set(map_ + (other.start_.block_-other.map_), other.start_.ptr_ - other.start_.first_);
        finish_.set(map_ + (other.finish_.block_-other.map_), other.finish_.ptr_ - other.finish_.first_);

    }

    template<class T>
    deque<T>::deque(deque &&other) noexcept {
        map_ = other.map_;
        other.map_ = nullptr;
        start_ = std::move(other.start_);
        finish_ = std::move(other.finish_);
        size_ = other.size_;
        other.size_ = 0;
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

    template<class T>
    void deque<T>::assign(deque::size_type count, const value_type &value) {
        //clear the deque first
        clear();

        //adjust size
        if(count > size())
            growrear(count);

        size_type block_offset = count % BLOCK_SIZE;

        // provide strong exception guarantee
        try {
            for(int i=0; i<size_; ++i){
                if(i != size_ -1)
                    std::uninitialized_fill_n(*(map_+i), BLOCK_SIZE, value);
                else
                    std::uninitialized_fill_n(*(map_+i), block_offset, value);
            }
            finish_.set(*(map_+size_-1), block_offset);
        }catch (...){
            for(int i=0; i<size_; ++i)
                ::operator delete(*(map_+i));
            ::operator delete(map_);
        }
    }

    template<class T>
    template<class InputIterator>
    void deque<T>::assign(InputIterator first, InputIterator last) {
        //clear the deque first
        clear();

        size_type element_count = last-first;
        size_ = ceil(element_count/BLOCK_SIZE);

        //adjust size
        if(last-first > size())
            growrear(last-first);

        // provide strong exception guarantee
        try {
            for(auto iter=first; iter!=last; ++iter){
                std::uninitialized_copy_n(iter,1,finish_.ptr_);
                ++finish_;
            }
//            auto iter = first;
//            for (int i=0; i<size_; ++i) {
//                if(i == size_-1){
//                    // copy the last block
//                    std::uninitialized_copy(iter, last, finish_.first_);
//                    finish_ += (last - iter);
//                    //assert((last - iter)%BLOCK_SIZE == element_count % BLOCK_SIZE);
//                } else {
//                    // copy the entire block
//                    std::uninitialized_copy(iter, iter + BLOCK_SIZE, finish_.first_);
//                    finish_ += BLOCK_SIZE;
//                    iter += BLOCK_SIZE;
//                }
//            }
        }catch (...){
            // if throws, deallocates all allocated memory
            for(int i=0; i<size_; ++i){
                ::operator delete(*(map_+i));
            }
            ::operator delete(map_);
        }
    }

    template<class T>
    typename deque<T>::reference deque<T>::at(deque::size_type pos) {
        return start_[pos];
    }

    template<class T>
    typename deque<T>::const_ref deque<T>::at(deque::size_type pos) const {
        return start_[pos];
    }

    template<class T>
    typename deque<T>::reference deque<T>::operator[](deque::size_type pos) {
        return start_[pos];
    }

    template<class T>
    typename deque<T>::const_ref deque<T>::operator[](deque::size_type pos) const {
        return start_[pos];
    }

    template<class T>
    typename deque<T>::reference deque<T>::front() {
        return *(start_.first_);
    }

    template<class T>
    typename deque<T>::const_ref deque<T>::front() const {
        return *(start_.first_);
    }

    template<class T>
    typename deque<T>::reference deque<T>::back() {
        return *(finish_.last_-1);
    }

    template<class T>
    typename deque<T>::const_ref deque<T>::back() const {
        return *(finish_.last_-1);
    }

    template<class T>
    typename deque<T>::size_type deque<T>::size() const {
        return finish_-start_;
    }

    template<class T>
    typename deque<T>::size_type deque<T>::max_size() const {
        return BLOCK_SIZE*size_;
    }

    template<class T>
    void deque<T>::shrink_to_fit() {
        // allocates memory for new map
        difference_type new_size = finish_.block_-start_.block_+1;
        T** new_map = static_cast<T**>(::operator new(new_size * sizeof(T*)));
        for(int i=0; i<new_size; ++i)
            *(new_map+i) = static_cast<T*>(::operator new(BLOCK_SIZE * sizeof(T)));

        difference_type start_offset = start_.ptr_ - start_.first_;
        difference_type finish_offset = finish_.ptr_ - finish_.first_;

        // move the elements to the new container
        try {
            for(int i=0; i<new_size; ++i){
                std::uninitialized_move(*(start_.block_+i), *(start_.block_+i)+BLOCK_SIZE, *(new_map+i));
            }
        }catch (...){
            // if throws, deallocate new memory
            for(int i=0; i<new_size; ++i)
                ::operator delete(*(new_map+i));
            ::operator delete(new_map);
        }

        // deallocate the old container
        for(int i=0; i<size_; ++i)
            ::operator delete(*(map_+i));
        ::operator delete(map_);

        size_ = new_size;
        map_ = new_map;
        start_.set(map_, start_offset);
        finish_.set((map_+size_-1), finish_offset);

    }

    template<class T>
    void deque<T>::clear() {
        for(int i=0; i<size_; ++i)
            std::destroy_n(*(map_+i), BLOCK_SIZE);
        std::destroy_n(map_, size_);
        finish_ = start_;
    }

    template<class T>
    void deque<T>::growrear(deque::size_type n) {
        assert(size_ > 0);
        size_type new_size = ceil(n/BLOCK_SIZE);
        if(new_size == size_)
            return;

        // allocate new memory
        T** new_map = static_cast<T**>(::operator new(new_size* sizeof(T*)));

        for(int i=0; i<new_size; ++i)
            *(new_map+i) = static_cast<T*>(::operator new(BLOCK_SIZE* sizeof(T)));

        difference_type finish_offset = finish_.ptr_ - finish_.first_;
        difference_type start_offset = start_.ptr_ - start_.first_;

        // move the elements
        try {
            if(new_size > size_) {
                for (int i = 0; i < size_; ++i) {
                    if (i == size_ - 1)
                        std::uninitialized_move(finish_.first_, finish_.ptr_, *(new_map + i));
                    else
                        std::uninitialized_move(*(map_ + i), *(map_ + i) + BLOCK_SIZE, *(new_map + i));
                }
            }
            else{ // if n is less than current size, destroy the extra elements
                for(int i=0; i<new_size; ++i){
                    std::uninitialized_move(*(map_+i), *(map_+i)+BLOCK_SIZE, *(new_map+i));
                }
                //destroy the extra elements
                for(iterator i = *(map_+new_size); i!=end(); ++i)
                    std::destroy_at(i.ptr_);
            }

        }catch (...){
            // if move throws, deallocate these memory
            for(int i=0; i<new_size; ++i)
                ::operator delete(*(new_map+i));
            ::operator delete(new_map);
        }

        //deallocates the old container
        for(int i=0; i<size_; ++i)
            ::operator delete(*(map_+i));
        ::operator delete(map_);

        //change the pointer to the new map;

        map_ = new_map;
        start_.set(new_map, start_offset);
        finish_.set(new_map+size_-1, finish_offset);
        size_ = new_size;

    }

    template<class T>
    void deque<T>::growfront(deque::size_type n) {
        assert(size_ > 0);
        size_type new_size = ceil(n/BLOCK_SIZE);

        if(new_size == size_)
            return;

        // allocate new memory
        T** new_map = static_cast<T**>(::operator new(new_size* sizeof(T*)));

        for(int i=0; i<new_size; ++i)
            *(new_map+i) = static_cast<T*>(::operator new(BLOCK_SIZE* sizeof(T)));

        difference_type finish_offset = finish_.ptr_ - finish_.first_;
        difference_type start_offset = start_.ptr_ - start_.first_;

        // move the elements
        try {
            if(new_size > size_) {
                for (int i = 1; i <= size_; ++i) {
                    std::uninitialized_move(*(map_+size_-i), *(map_+size_-i) + BLOCK_SIZE, *(new_map+new_size-i));
                }
            }
            else{ // if n is less than current size, destroy the extra elements
                for(int i=0; i<new_size; ++i){
                    std::uninitialized_move(*(map_+i), *(map_+i)+BLOCK_SIZE, *(new_map+new_size-1-i));
                }
                //destroy the extra elements
                for(iterator i= begin(); i != *(map_+(size_-new_size)-1)+BLOCK_SIZE; ++i)
                    std::destroy_at(i.ptr_);
            }

        }catch (...){
            // if move throws, deallocate these memory
            for(int i=0; i<new_size; ++i)
                ::operator delete(*(new_map+i));
            ::operator delete(new_map);
        }

        //deallocates the old container
        for(int i=0; i<size_; ++i)
            ::operator delete(*(map_+i));
        ::operator delete(map_);

        //change the pointer to the new map;
        map_ = new_map;
        finish_.set(new_map+new_size-1, finish_offset);
        start_.set(finish_.block_-size_+1, start_offset);
        size_ = new_size;

    }

    // insert invalidates iterators and references
    template<class T>
    typename deque<T>::iterator deque<T>::insert(deque::iterator iter, const value_type &value) {
        // if the container is full, double the size
        if(finish_.block_ == map_+size_-1 && finish_.ptr_ == finish_.last_-1)
            growrear(2 * BLOCK_SIZE * size_);

        //shift from the end by 1
        for(auto i = end(); i>iter; --i){
            *i = *(i-1);
        }
        *iter = value;

        ++finish_;
        return iter;
    }

    template<class T>
    typename deque<T>::iterator deque<T>::erase(deque::iterator iter) {
        difference_type n = iter - start_;

        std::destroy_at((start_+n).ptr_);
        for(iterator i = start_+n; i!=end(); ++i)
            *i = *(i+1);

        --finish_;
        std::destroy_at(finish_.ptr_);

        // if the unused space is more than half, shrink to fit
        if(finish_-start_+1 <= size_*BLOCK_SIZE/2)
            shrink_to_fit();

        return start_+n;

    }

    template<class T>
    void deque<T>::push_back(const value_type &value) {
        if(finish_.block_ == map_+size_-1 && finish_.ptr_ == finish_.last_-1)
            growrear(2 * BLOCK_SIZE * size_);

        *(finish_.ptr_) = value;
        ++finish_;
    }

    template<class T>
    void deque<T>::push_back(value_type &&value) {
        if(finish_.block_ == map_+size_-1 && finish_.ptr_ == finish_.last_-1)
            growrear(2 * BLOCK_SIZE * size_);

        *(finish_.ptr_) = std::move(value);
        ++finish_;
    }

    template<class T>
    void deque<T>::pop_back() {
        --finish_;
        std::destroy_at(finish_.ptr_);

        if(size() < size_*BLOCK_SIZE/2)
            shrink_to_fit();
    }

    template<class T>
    void deque<T>::push_front(const T &value) {
        if(start_.block_ == map_ && start_.ptr_ == start_.first_)
            growfront(2 * BLOCK_SIZE * size_);

        --start_;
        *(start_.ptr_) = value;
    }

    template<class T>
    void deque<T>::push_front(T &&value) {
        if(start_.block_ == map_ && start_.ptr_ == start_.first_)
            growfront(2 * BLOCK_SIZE * size_);

        --start_;
        *(start_.ptr_) = std::move(value);
    }

    template<class T>
    void deque<T>::pop_front() {
        std::destroy_at(start_.ptr_);
        ++start_;

        if(size() < size_*BLOCK_SIZE/2)
            shrink_to_fit();

    }

    template<class T>
    void deque<T>::resize(deque::size_type size) {
        growrear(size);

        if(size > this->size_){
            std::uninitialized_default_construct(finish_.ptr_, finish_.last_);
            for(int i=finish_.block_-start_.block_+1; i< size_; ++i){
                std::uninitialized_default_construct_n(*(map_+i), BLOCK_SIZE);
            }
        }
    }

    template<class T>
    void deque<T>::resize(deque::size_type size, const value_type &value) {
        growrear(size);

        if(size > this->size_){
            std::uninitialized_fill(finish_.ptr_, finish_.last_, value);
            for(int i=finish_.block_-start_.block_+1; i< size_; ++i){
                std::uninitialized_fill_n(*(map_+i), BLOCK_SIZE, value);
            }
        }

    }

    template<class T>
    void deque<T>::swap(deque &other) {
        swap(*this, other);
    }

    //lexically compare two deque
    template <class T>
    bool operator==(const deque<T> &q1, const deque<T> &q2) {
        if(q1.size() == q2.size()) {
            for (auto iter1 = q1.begin(), iter2 = q2.begin(); q1 != q1.end(); ++q1) {
                if(*q1 != *q2)
                    return false;
            }
            return true;
        }
        return false;

    }

    template <class T>
    bool operator!=(const deque<T> &q1, const deque<T> &q2) {
        return !(q1 == q2);
    }

    template <class T>
    bool operator>(const deque<T> &q1, const deque<T> &q2){
        if(q1.size() == q2.size()){
            for(auto iter1 = q1.begin(), iter2 = q2.begin(); q1 != q1.end(); ++q1){
                if(*q1 == *q2)
                    continue;
                return *q1 > *q2;
            }
            return false; //q1 == q2
        }

        return q1.size() > q2.size();
    }

    template <class T>
    bool operator<(const deque<T> &q1, const deque<T> &q2){
        return q2 > q1;
    }

    template <class T>
    bool operator>=(const deque<T> &q1, const deque<T> &q2){
        return !(q1 < q2);
    }

    template <class T>
    bool operator<=(const deque<T> &q1, const deque<T> &q2){
        return !(q1 > q2);
    }


}

#endif //STLCONTAINER_DEQUE_HPP
