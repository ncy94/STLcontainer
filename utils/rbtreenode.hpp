//
// Created by NCY on 2019-08-30.
//

#ifndef STLCONTAINER_RBTREENODE_HPP
#define STLCONTAINER_RBTREENODE_HPP

#include "treenode.hpp"

namespace sc::utils{

    enum rbcolor{
        RED = 0;
        BLACK = 1;
    };

    template <class T>
    final class rbtreenode: public class treenode<T>{
    public:
        rbcolor color_;

        using treenode::left_;
        using treenode::right_;
        using treenode::parent_;
        using treenode::val_;

        rbtreenode(T val): node_base(val), color_(RED) {}

        friend bool isRed(const rbtreenode& node){
            return node.color_ == RED;
        }

        friend bool isBlack(const rbtreenode& node){
            return node.color_ == BLACK;
        }

    };

}

#endif //STLCONTAINER_RBTREENODE_HPP
