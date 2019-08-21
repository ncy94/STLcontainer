//
// Created by NCY on 2019-08-18.
//

#ifndef STLCONTAINER_DEQUE_ITERATOR_HPP
#define STLCONTAINER_DEQUE_ITERATOR_HPP

#include "iterator_base.hpp"

#define BLOCK_SIZE 8

// forward declaration
namespace sc::regular{
    template <class> class deque;
}

namespace sc::utils{

    template <class T>
    class deque_iterator: public iterator_base<T, deque_iterator<T>>{

    public:
        using iterator_base<T, deque_iterator<T>>::ptr_;
        using typename iterator_base<T,deque_iterator<T>>::value_type ;
        using typename iterator_base<T, deque_iterator<T>>::difference_type ;
        using typename iterator_base<T, deque_iterator<T>>::pointer;
        using typename iterator_base<T, deque_iterator<T>>::reference;

        deque_iterator():iterator_base<T,deque_iterator>(), first_(nullptr), last_(nullptr), block_(nullptr){}


        template <class OtherT, class = std::enable_if_t<std::is_convertible_v<OtherT*, T*>>>
        deque_iterator(const deque_iterator<OtherT>& other):iterator_base<T,deque_iterator<T>>(other),first_(other.first_), last_(other.last_){
            block_ = other.block_;
        }

        //deque_iterator(const deque_iterator& other): iterator_base<T,deque_iterator>(other), first_(other.first_), last_(other.last_), block_(other.block_){}
//        deque_iterator(deque_iterator&& other) noexcept {
//            ptr_ = other.ptr_;
//            other.ptr_ = nullptr;
//            first_ = other.first_;
//            other.first_ = nullptr;
//            last_ = other.last_;
//            other.last_ = nullptr;
//            block_ = other.block_;
//            other.block_ = nullptr;
//        }

        // copy and move assignment
        deque_iterator operator=(deque_iterator other){
            swap(*this, other);
            return *this;
        }

        // initialize by a map pointer and a current pointer
        void set(T** block, difference_type offset=0){
            block_ = block;
            first_ = *block;
            last_ = first_ + BLOCK_SIZE;
            ptr_ = first_ + offset;
        }

        deque_iterator&operator++(){
            // if it's at the end of current block, move to the next block
            if(ptr_ == last_-1){
                nextblock();
                ptr_ = *block_;
            }else{
                ++ptr_;
            }
            return *this;
        }

        deque_iterator operator++(int){
            deque_iterator old(*this);
            // if it's at the end of current block, move to the next block
            if(ptr_ == last_-1){
                nextblock();
                ptr_ = *block_;
            }else{
                ++ptr_;
            }
            return old;
        }

        deque_iterator&operator--(){
            // if it's at the beginning of current block, move to the previous block
            if(ptr_ == first_){
                previousblock();
                ptr_ = last_ - 1;
            } else{
                --ptr_;
            }

            return *this;
        }

        deque_iterator operator--(int){
            deque_iterator old(*this);
            if(ptr_ == first_){
                previousblock();
                ptr_ = last_ - 1;
            } else{
                --ptr_;
            }
            return old;

        }

        deque_iterator&operator+=(difference_type n){
            // the remaining space to end of block
            difference_type remain = last_ - ptr_ - 1;

            // the number of blocks to be jumped
            difference_type blocknum = n>remain? ceil((float)(n-remain)/BLOCK_SIZE) : 0;
            block_ += blocknum;
            first_ = *block_;
            last_ = first_+BLOCK_SIZE;
            ptr_ = first_ + ((n-remain)%BLOCK_SIZE);
            return *this;
        }

        deque_iterator&operator-=(difference_type n){
            // the remaining space to the beginning of block
            difference_type remain = ptr_ - first_;

            // the number of blocks to be jumped
            difference_type blocknum = n>remain? ceil((float)(n-remain)/BLOCK_SIZE) : 0;

            //decrement out of bound is undefined behaviour
            if(blocknum == 0)
                ptr_ -= n;
            else {
                block_ -= blocknum;
                first_ = *block_;
                last_ = first_+BLOCK_SIZE;
                ptr_ = last_ - ((n - remain) % BLOCK_SIZE);
            }

            return *this;
        }

        deque_iterator&operator[](difference_type n) const{
            if(n>=0){
                difference_type remain = last_ - ptr_ - 1;

                difference_type blocknum = n>remain? ceil((float)(n-remain)/BLOCK_SIZE) : 0;

                return *(*(block_+blocknum) + ((n-remain)%BLOCK_SIZE));
            } else{
                difference_type remain = ptr_ - first_;

                if(n <= remain)
                    return ptr_[n];
                else{
                    difference_type blocknum = ceil((-n-remain)/BLOCK_SIZE);

                    return *(block_+blocknum)+ BLOCK_SIZE - (-n-remain)%BLOCK_SIZE;
                }

            }
        }

        deque_iterator operator+(difference_type n) const{
            deque_iterator tmp(*this);
            tmp += n;
            return tmp;
        }

        difference_type operator-(const deque_iterator& other) const{
            return (ptr_-first_+1) + (other.last_-other.ptr_) + (block_-other.block_-1)*BLOCK_SIZE;
        }

        deque_iterator operator-(difference_type n) const{
            deque_iterator tmp(*this);
            tmp -= n;
            return tmp;
        }

        /*
         * Swap
         */

        // by swapping the pointers, two iterators are effectively swapped
        friend void swap(deque_iterator& i1, deque_iterator& i2){
            std::swap(i1.ptr_, i2.ptr_);
            std::swap(i1.first_, i2.first_);
            std::swap(i1.last_, i2.last_);
            std::swap(i1.block_, i2.block_);
        }

        /*
         * Comparison
         */

        bool operator>(const deque_iterator& other) const{
            if(block_ == other.block_){
                return ptr_>other.ptr_;
            }
            return block_>other.block_;
        }

        bool operator<(const deque_iterator& other) const{
            return other > *this;
        }

        bool operator>=(const deque_iterator& other) const{
            return !(*this < other);
        }

        bool operator<=(const deque_iterator& other) const{
            return !(*this > other);
        }

        bool operator==(const deque_iterator& other) const{
            return (block_==other.block_ && ptr_==other.ptr_);
        }

        bool operator!=(const deque_iterator& other) const{
            return !(*this == other);
        }


    private:
        template <class> friend class sc::regular::deque;
        template <class> friend class deque_iterator;
        // move to the next block
        void nextblock();
        // move to the previous block
        void previousblock();
        T* first_; // pointer to first element in block
        T* last_; // pointer to end element in block
        // cannot assign from int** to const int**
        value_type ** block_; //pointer to current block
    };

    template <class T>
    deque_iterator<T> operator+(const deque_iterator<T>& i1, ptrdiff_t n){
        deque_iterator tmp(i1);
        tmp += n;
        return tmp;
    }

    template<class T>
    void deque_iterator<T>::nextblock() {
        assert(ptr_ == last_-1);

        // Incrementing the end block is undefined behaviour
        ++(*block_);
        first_ = *block_;
        last_ = first_ + BLOCK_SIZE;
    }

    template<class T>
    void deque_iterator<T>::previousblock() {
        assert(ptr_ == first_);

        //decrementing the first block is undefined behaviour
        --(*block_);
        first_ = *block_;
        last_ = first_ + BLOCK_SIZE;
    }

}

#endif //STLCONTAINER_DEQUE_ITERATOR_HPP
