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

        rbtreenode(T val, rbcolor color=RED): treenode<T>(val), color_(color) {}
        rbtreenode(const rbtreenode& other): treenode<T>(other), color_(other.color_){}
        rbtreenode(rbtreenode&& other): treenode<T>(std::move(other)), color_(std::move(other.color_)){}

        rbtreenode& operator=(rbtreenode other){
            swap(other, *this);
            return *this;
        }

        void setColor(rbcolor color){
            color_ = color;
        }

        bool isRed(){
            return color_ == RED;
        }

        bool isBlack(){
            return color_ == BLACK;
        }

        friend void swapColor(rbtreenode& tn1, rbtreenode& tn2){
            std::swap(tn1.color_, tn2.color_);
        }

        friend void swap(rbtreenode& tn1, rbtreenode tn2){
            using std::swap;

            swap(tn1.parent_, tn2.parent_);
            swap(tn1.right_, tn2.right_);
            swap(tn1.left_, tn2.left_);
            swap(tn1.color_, tn2.color_);
            swap(tn1.val_, tn2.val_);
        }

    };

}


#endif //STLCONTAINER_RBTREENODE_HPP
