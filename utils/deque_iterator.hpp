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
        using typename iterator_base<T, deque_iterator<T>>::difference_type ;
        using typename iterator_base<T, deque_iterator<T>>::pointer;
        using typename iterator_base<T, deque_iterator<T>>::reference;

        // initialize by a map pointer and a current pointer
        void set(T** block, T* current){
            block_ = block;
            first_ = *block;
            last_ = first_ + BLOCK_SIZE;
            ptr_ = current;
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
            *block_ += blocknum;
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
                *block_ -= blocknum;
                first_ = *block_;
                last_ = first_+BLOCK_SIZE;
                ptr_ = last_ - ((n - remain) % BLOCK_SIZE);
            }

            return *this;
        }


    private:
        template <class> friend class sc::regular::deque;
        // move to the next block
        void nextblock();
        // move to the previous block
        void previousblock();
        T* first_; // pointer to first element in block
        T* last_; // pointer to end element in block
        T** block_; //pointer to current block
    };

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
