//
// Created by NCY on 2019-08-16.
//

#ifndef STLCONTAINER_NODE_BASE_HPP
#define STLCONTAINER_NODE_BASE_HPP

#include <algorithm>

namespace sc::regular{
    template <class> class list;
    template <class,class,class> class unordered_set;
    template <class, class, class, class> class unordered_map;
    template <class> class rbtree;
}

namespace sc::utils{

    // single direction node base
    template <class T, class Node>
    class node_base{
    public:

        node_base(T val): val_(val), next_(nullptr){}

        node_base(const node_base& other): val_(other.val_), next_(other.next_) {}
        node_base(node_base&& other): val_(std::move(other.val_)), next_(std::move(other.next_)){ }

        node_base&operator=(const node_base& other){
            val_ = other.val_;
            next_ = other.next_;
            return *this;
        }

        node_base&operator=(node_base&& other) noexcept {
            val_ = other.val_;
            other.val_ = 0;
            next_ = other.next_;
            other.next_ = nullptr;
            return *this;
        }

        T getValue() const {return val_;}

    private:
        template <class,class,class> friend class sc::regular::unordered_set;
        template <class, class, class, class> friend class sc::regular::unordered_map;
        template <class> friend class sc::regular::rbtree;
        template <class> friend class sc::regular::list;
        template <class> friend class list_iterator;

    protected:
        T val_;

        Node* next_;

    };


}
#endif //STLCONTAINER_NODE_BASE_HPP
