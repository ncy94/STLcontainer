//
// Created by NCY on 2019-08-14.
//

#ifndef STLCONTAINER_ITERATOR_BASE_HPP
#define STLCONTAINER_ITERATOR_BASE_HPP

#include <type_traits>

//forward declaration
namespace sc::regular{
    template <class> class deque;
}

namespace sc::utils{

    // use CRTP idiom to prevent post-increment returns abstract class
    // https://stackoverflow.com/questions/18172858/c-abstract-base-class-postfix-operator)
    template <class T, class Iterator>
    class iterator_base{

    public:

        using value_type = std::remove_const_t<T>;

        using reference = T&;

        using pointer = T*;

        using difference_type = std::ptrdiff_t;

        //default constructor
        iterator_base(): ptr_(nullptr) {}

        //iterator_base(const iterator_base& other):ptr_(other.ptr_){}

        //forbids to copy a const iterator to a non-const iterator
        template <class OtherT, class OtherIter,
                class = std::enable_if_t<std::is_convertible_v<OtherT*, T*>>>
                iterator_base(const iterator_base<OtherT,OtherIter>& other): ptr_(other.ptr_) {}

        reference operator*() {return *ptr_;}

        pointer operator->() {return ptr_;}

        virtual Iterator&operator++() = 0;

        virtual Iterator operator++(int) = 0;

        template <class OtherT, class OtherIter>
        bool operator==(const iterator_base<OtherT, OtherIter> other) const {
            return ptr_ == other.ptr_;
        }

        template <class OtherT, class OtherIter>
        bool operator!=(const iterator_base<OtherT, OtherIter> other) const {
            return ptr_ != other.ptr_;
        }


    protected:
        template <class> friend class sc::regular::deque;
        template <class> friend class list_iterator;
        template <class> friend class array_iterator;
        template <class,class> friend class iterator_base;
        template <class> friend class deque_iterator;
        pointer ptr_;

    };


}

#endif //STLCONTAINER_ITERATOR_BASE_HPP
