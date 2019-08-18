//
// Created by NCY on 2019-08-16.
//

#ifndef STLCONTAINER_NODE_BASE_HPP
#define STLCONTAINER_NODE_BASE_HPP

#include <algorithm>

namespace sc::regular{
    template <class> class list;
}

namespace sc::utils{

    // single direction node base
    template <class T, class Node>
    class node_base{
    public:

        node_base(): val_(0), next_(nullptr){}

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

        node_base(T val): val_(val), next_(nullptr){}

    private:
        template <class> friend class sc::regular::list;
        T val_;

        Node* next_;

    };


}
#endif //STLCONTAINER_NODE_BASE_HPP
