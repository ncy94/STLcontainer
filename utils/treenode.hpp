//
// Created by NCY on 2019-08-30.
//

#ifndef STLCONTAINER_TREENODE_HPP
#define STLCONTAINER_TREENODE_HPP

#include "node_base.hpp"


namespace sc::utils{

    template <class T>
    class treenode: public node_base<T, treenode<T>>{

    protected:
        using left_ = typename node_base<T, treenode<T>>::next_; //left child
        using node_base<T, treenode<T>>::val_;

    public:
        treenode* right_; //right child
        treenode* parent_; //parent node
        treenode(T val): node_base<T,treenode<T>>(val), right_(nullptr), parent_(nullptr){}

    };
}

#endif //STLCONTAINER_TREENODE_HPP
