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
        deque_iterator(const T** block, const T* current){
            block_ = block;
            first_ = *block;
            last_ = first_ + BLOCK_SIZE;
            ptr_ = current;
        }

        deque_iterator&operator++(){
            // if it's at the end of current block, move to the next block
            if(ptr_ == last_){
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
            if(ptr_ == last_){
                nextblock();
                ptr_ = *block_;
            }else{
                ++ptr_;
            }
            return old;
        }


    private:
        template <class> friend class sc::regular::deque;
        // move to the next block
        void nextblock();
        T* first_; // pointer to first element in block
        T* last_; // pointer to end element in block
        T** block_; //pointer to current block
    };

}

#endif //STLCONTAINER_DEQUE_ITERATOR_HPP
