//
// Created by NCY on 2019-08-16.
//

#ifndef STLCONTAINER_NODE_BASE_HPP
#define STLCONTAINER_NODE_BASE_HPP

namespace sc::utils{

    // single direction node base
    template <class T, class Node>
    class node_base{
    public:
        node_base(): val_(0), next_(nullptr){}

        node_base(T val): val_(val), next_(nullptr){}

    private:
        template <class> friend class list;
        T val_;

        Node* next_;

    };


}
#endif //STLCONTAINER_NODE_BASE_HPP
