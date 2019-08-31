//
// Created by NCY on 2019-08-30.
//

#ifndef STLCONTAINER_RBTREE_HPP
#define STLCONTAINER_RBTREE_HPP

#include "rbtreenode.hpp"

namespace sc::regular{

    template <class T>
    class rbtree{

    public:

        using value_type = T;

        using size_type = std::size_t ;

        using node_type = sc::utils::rbtreenode<T>;

        using reference = T&;

        using pointer = T*;

        rbtree();
        rbtree(const rbtree&);
        rbtree(rbtree&&);
        rbtree& operator=(rbtree);

        void preOrder();

        void inOrder();

        void postOrder();

        pointer search(value_type key);

        pointer iterativeSearch(value_type key);

        reference minimum() const;

        reference maximum() const;

        // successor: the min node that is bigger than node
        pointer successor(node_type* node);

        // predecessor: the max node that is smaller than node
        pointer predecessor(node_type* node);

        void insert(value_type key);

        void remove(value_type key);

        void destroy();

        void print();

    private:
        void preOrder(node_type* node) const;

        void inOrder(node_type* node) const;

        void postOrder(node_type* node) const;

        pointer search(node_type* node, value_type key) const;

        pointer iterativeSearch(node_type* node, value_type key) const;

        void leftRotate(node_type* x);

        void rightRotate(node_type* node);

        void insert(node_type* node);

        void insertFixup(node_type* node);

        void remove(node_type* node);

        void removeFixup(node_type* node);

        node_type * root_;

    };

    template<class T>
    void rbtree<T>::leftRotate(rbtree::node_type *x) {
        /*
         *      |                        |
         *      X           =>           Y
         *    /   \                    /   \
         *   a     Y                  X    c
         *       /  \                / \
         *      b   c               a  b
         */
        node_type* y = x->right_;

        //give y's left child to x as right child
        x->right_ = y->left_;
        if(y->left_ != nullptr) {
            y->left_->parent_ = x;
        }

        y->parent_ = x->parent_;
        // change x's parent to y
        if(x == root_) {
            assert(x->parent_ == nullptr);
            root_ = y;
        } else{
            if(x == x->parent_->left_)
                x->parent_->left_ = y;
            else if(x == x->parent_->right_)
                x->parent_->right_ = y;
            else
                throw;

        }
        y->left_ = x;
        x->parent_ = y;

    }

    template<class T>
    void rbtree<T>::rightRotate(rbtree::node_type *x) {
       /*
        *        |                  |
        *        X        =>        Y
        *      /   \              /   \
        *     Y     c            a    X
        *    /  \                   /  \
        *   a    b                 b    c
        */
        node_type* y = x->left_;

        // give y's right child to x
        x->left_ = y->right_;
        if(y->right_ != nullptr)
            y->right_->parent_ = x;

        if(x == root_){
            assert(x->parent_ == nullptr);
            root_ = y;
        } else {
            if (x == x->parent_->left_)
                x->parent_->left_ = y;
            else if (x == x->parent_->right_)
                x->parent_->right_ = y;
            else
                throw;
        }
        y->right_ = x;
        x->parent_ = y;

    }

}

#endif //STLCONTAINER_RBTREE_HPP
