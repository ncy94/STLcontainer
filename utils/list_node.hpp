//
// Created by NCY on 2019-08-16.
//

#ifndef STLCONTAINER_LIST_NODE_HPP
#define STLCONTAINER_LIST_NODE_HPP

#include "node_base.hpp"

namespace sc::utils{

    // bidirectional list node
    template <class T>
     class list_node: public node_base<T, list_node<T>>{
     public:

         list_node(T val): node_base<T,list_node<T>>(val), prev_(nullptr){}
         list_node(const list_node& other): node_base<T,list_node<T>>(other), prev_(other.prev_) {}
         list_node(list_node&& other) noexcept : node_base<T,list_node<T>>(std::move(other)), prev_(std::move(other.prev_)) {}

         list_node&operator=(const list_node& other) {
             node_base<T,list_node<T>>::operator=(other);
             prev_ = other.prev_;

             return *this;
         }

         list_node&operator=(list_node&& other) noexcept {
             node_base<T,list_node<T>>::operator=(std::move(other));
             prev_ = other.prev_;
             other.prev_ = nullptr;
             return *this;
         }


     private:
         template <class, class, class> friend class sc::regular::unordered_set;
         template <class> friend class sc::regular::list;

         list_node* prev_;

     };
}

#endif //STLCONTAINER_LIST_NODE_HPP
