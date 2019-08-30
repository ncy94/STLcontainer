//
// Created by NCY on 2019-08-30.
//

#ifndef STLCONTAINER_RBTREENODE_HPP
#define STLCONTAINER_RBTREENODE_HPP

#include "treenode.hpp"

namespace sc::utils{

    enum rbcolor{
        RED = 0,
        BLACK = 1,
    };

    template <class T>
    class rbtreenode: public treenode<T> {
    public:
        rbcolor color_;

        using treenode<T>::left_;
        using treenode<T>::right_;
        using treenode<T>::parent_;
        using treenode<T>::val_;

        rbtreenode(T val): treenode<T>(val), color_(RED) {}

        friend bool isRed(const rbtreenode& node){
            return node.color_ == RED;
        }

        friend bool isBlack(const rbtreenode& node){
            return node.color_ == BLACK;
        }

    };

}

#endif //STLCONTAINER_RBTREENODE_HPP
