//
// Created by NCY on 2019-08-16.
//

#ifndef STLCONTAINER_LIST_NODE_HPP
#define STLCONTAINER_LIST_NODE_HPP

#include "node_base.hpp"

namespace sc::utils{

    // bidirectional list node
    template <class T>
     class list_node: public node_base<T, list_node>{
     public:
         list_node* prev() const{ return prev_;}

    protected:
         list_node* prev_;

    };
}

#endif //STLCONTAINER_LIST_NODE_HPP
