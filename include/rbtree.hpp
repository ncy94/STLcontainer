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

        void leftRotate(node_type* node);

        void rightRotate(node_type* node);

        void insert(node_type* node);

        void insertFixup(node_type* node);

        void remove(node_type* node);

        void removeFixup(node_type* node);

        node_type * root_;

    };

}

#endif //STLCONTAINER_RBTREE_HPP
