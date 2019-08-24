//
// Created by NCY on 2019-08-16.
//

#ifndef STLCONTAINER_LIST_ITERATOR_HPP
#define STLCONTAINER_LIST_ITERATOR_HPP

#include "iterator_base.hpp"

namespace sc::utils{

    // list iterator is implicitly bi-directional
    template <class T>
    class list_iterator : public iterator_base<T, list_iterator<T>>{
    public:

        // C++ doesn’t consider superclass templates for name resolution
        using iterator_base<T, list_iterator<T>>::ptr_;
        using typename iterator_base<T, list_iterator<T>>::difference_type ;
        using typename iterator_base<T, list_iterator<T>>::pointer;
        using typename iterator_base<T, list_iterator<T>>::reference;

        list_iterator(pointer ptr): iterator_base<T, list_iterator<T>>(ptr){}

        list_iterator& operator++(){
            ptr_ = ptr_->next_;
            return *this;
        }

        list_iterator operator++(int){
            list_iterator old(*this);
            ptr_ = ptr_->next_;
            return old;
        }

        list_iterator&operator--(){
            ptr_ = ptr_->prev_;
            return *this;
        }

        list_iterator operator--(int){
            list_iterator old(*this);
            ptr_ = ptr_->prev_;
            return old;
        }

    };
}

#endif //STLCONTAINER_LIST_ITERATOR_HPP
