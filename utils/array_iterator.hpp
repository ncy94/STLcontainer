//
// Created by NCY on 2019-08-15.
//

#ifndef STLCONTAINER_ARRAY_ITERATOR_HPP
#define STLCONTAINER_ARRAY_ITERATOR_HPP

#include "iterator_base.hpp"

namespace sc::utils{

    //array iterator is implicity random access
    template <class T>
    class array_iterator : public iterator_base<T> {

        using iterator_base<T>::ptr_;
        using typename iterator_base<T>::difference_type ;
        using typename iterator_base<T>::pointer;
        using typename iterator_base<T>::reference;

        //forward
        array_iterator&operator++() {
            ++ptr_;
            return *this;
        }

        array_iterator operator++(int){
            array_iterator old(*this);
            ++ptr_;
            return old;
        }

        //bidirectional
        array_iterator&operator--() {
            --ptr_;
            return *this;
        }

        array_iterator operator--(int) {
            array_iterator old(*this);
            --ptr_;
            return old;
        }

        //random access
        array_iterator&operator+=(difference_type n){
            ptr_ += n;
            return *this;
        }

        array_iterator&operator-=(difference_type n){
            ptr_ -= n;
            return *this;
        }

        reference operator[](difference_type n) const{
            return ptr_[n];
        }

        array_iterator operator+(difference_type n) const{
            return array_iterator(ptr_+n);
        }

        difference_type operator-(const array_iterator& other) const{
            return ptr_ - other.ptr_;
        }

        array_iterator operator-(difference_type n) const{
            return array_iterator(ptr_ - n);
        }

        //comparison
        template <class OtherT>
        bool operator> (const array_iterator<OtherT>& other) const{
            return ptr_ > other.ptr_;
        }

        template <class OtherT>
        bool operator< (const array_iterator<OtherT>& other) const{
            return ptr_ < other.ptr_;
        }

        template <class OtherT>
        bool operator>= (const array_iterator<OtherT>& other) const{
            return ptr_ >= other.ptr_;
        }

        template <class OtherT>
        bool operator<= (const array_iterator<OtherT>& other) const{
            return ptr_ <= other.ptr_;
        }

    };

    template <class T>
    array_iterator<T> operator+(std::ptrdiff_t n, const array_iterator<T>& iter) {
        return array_iterator<T>(iter) += n ;
    }

}


#endif //STLCONTAINER_ARRAY_ITERATOR_HPP
