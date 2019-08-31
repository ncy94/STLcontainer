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

        treenode(const treenode& other):node_base<T, treenode<T>>(other), right_(other.right_), parent_(other.parent_){}
        treenode(treenode&& other):node_base<T, treenode<T>>(std::move(other)), right_(std::move(other.right)), parent_(std::move(other.parent_)) {}
        treenode& operator=(treenode other){
            swap(*this, other);
            return *this;
        }

        friend void swap(treenode& tn1, treenode& tn2){
            using std::swap;
            swap(tn1.parent_, tn2.parent_);
            swap(tn1.left_, tn2.left_);
            swap(tn1.right_, tn2.right_);
            swap(tn1.val_, tn2.val_);
        }


    };
}

#endif //STLCONTAINER_TREENODE_HPP
