//
// Created by NCY on 2019-08-30.
//

#ifndef STLCONTAINER_RBTREE_HPP
#define STLCONTAINER_RBTREE_HPP

#include "rbtreenode.hpp"

namespace sc::regular{

    template <class T, class Compare=std::less<T>>
    class rbtree{

    public:

        using value_type = T;

        using size_type = std::size_t ;

        using node_type = sc::utils::rbtreenode<T>;

        using reference = T&;

        using pointer = T*;

        rbtree():root_(nullptr), size_(0), comp_(Compare()){}
        rbtree(const rbtree& other):root_(other.root_), size_(other.size_), comp_(other.comp_){}
        rbtree(rbtree&& other) noexcept {
            root_ = other.root_;
            other.root_ = nullptr;
            size_ = other.size_;
            other.size_ = 0;
            comp_ = std::move(other.comp_);
        }
        rbtree& operator=(rbtree other){
            swap(*this, other);
            return *this;
        }

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

        friend void swap(rbtree& rbt1, rbtree& rbt2);

    private:
        void preOrder(node_type* node) const;

        void inOrder(node_type* node) const;

        void postOrder(node_type* node) const;

        pointer search(node_type* node, value_type key) const;

        pointer iterativeSearch(node_type* node, value_type key) const;

        void leftRotate(node_type* x);

        void rightRotate(node_type* x);

        void insert(node_type* node);

        void insertFixup(node_type* node);

        void remove(node_type* node);

        void removeFixup(node_type* node);

        node_type * root_;

        size_type size_;

        Compare comp_;

    };

    template<class T, class Compare>
    void rbtree<T,Compare>::leftRotate(rbtree::node_type *x) {
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

    template<class T, class Compare>
    void rbtree<T,Compare>::rightRotate(rbtree::node_type *x) {
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

    template <class T, class Compare>
    void swap(rbtree<T,Compare> &rbt1, rbtree<T,Compare> &rbt2) {
        std::swap(rbt1.root_, rbt2.root_);
        std::swap(rbt1.size_, rbt2.size_);
        std::swap(rbt1.comp_, rbt2.comp_);
    }

    template<class T, class Compare>
    void rbtree<T, Compare>::insert(rbtree::node_type *node) {
        if(root_ == nullptr){
            root_  = node;
            return;
        }
        // step1: search the node
        node_type* current = root_;
        node_type* pre = nullptr; // pre is set the record the parent of root
        while(current != nullptr){
            pre = current;
            if(comp_(current->getValue(), node->getValue()) > 0)
                current = current->right_;
            else if(comp_(current->getValue(), node->getValue()) < 0)
                current = current->left_;
            else
                return; // the node is already there
        }

        // step2: insert the node
        assert(current == nullptr);
        node->parent_ = pre;
        if (comp_(pre->getValue(), node->getValue()) > 0) {
            pre->left_ = node;
        } else if (comp_(pre->getValue(), node->getValue()) > 0) {
            pre->right_ = node;
        } else throw;

        // step3: set the color to red
        node->setColor(utils::RED);

        // step3: fix the tree
        insertFixup(node);

    }

    template<class T, class Compare>
    void rbtree<T, Compare>::insert(value_type key) {
        node_type* tmp = new node_type(key);

        insert(tmp);
    }

    template<class T, class Compare>
    void rbtree<T, Compare>::insertFixup(rbtree::node_type *node) {


    }

}

#endif //STLCONTAINER_RBTREE_HPP
