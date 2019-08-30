//
// Created by NCY on 2019-08-30.
//

#ifndef STLCONTAINER_TREENODE_HPP
#define STLCONTAINER_TREENODE_HPP

#include "node_base.hpp"

namespace sc::utils{

    template <class T>
    class treenode: public class node_base<T, treenode<T>>{

    protected:
        using left_ = node_base<T, treenode<T>>::next_; //left child

    public:
        treenode* right_; //right child
        treenode* parent_; //parent node

    };
}

#endif //STLCONTAINER_TREENODE_HPP
