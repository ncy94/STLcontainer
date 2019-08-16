//
// Created by NCY on 2019-08-14.
//

#ifndef STLCONTAINER_ITERATOR_BASE_HPP
#define STLCONTAINER_ITERATOR_BASE_HPP

namespace sc::utils{

    template <class T>
    class iterator_base{

    public:

        using value_type = std::remove_const_t<T>;

        using reference = T&;

        using pointer = T*;

        using difference_type = std::ptrdiff_t;

        //default constructor
        iterator_base(): ptr_(nullptr) {}

        //forbids to copy a const iterator to a non-const iterator
        template <class OtherT, class =
                std::enable_if_t<std::is_convertible_v<OtherT*, T*>>>
                iterator_base(const iterator_base<OtherT>& other): ptr_(other.ptr_) {}

        reference operator*() {return *ptr_;}

        pointer operator->() {return ptr_;}

        virtual iterator_base&operator++();

        virtual iterator_base operator++(int);

        template <class OtherT>
        bool operator==(const iterator_base<OtherT> other) const {
            return ptr_ == other.ptr_;
        }

        template <class OtherT>
        bool operator!=(const iterator_base<OtherT> other) const {
            return ptr_ != other.ptr_;
        }


    protected:
        template <class>friend class iterator_base;
        pointer ptr_;

    };


}

#endif //STLCONTAINER_ITERATOR_BASE_HPP
