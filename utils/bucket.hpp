//
// Created by NCY on 2019-08-22.
//

#ifndef STLCONTAINER_BUCKET_HPP
#define STLCONTAINER_BUCKET_HPP

/*
 * This class represents the bucket of unordered set
 *
 * The implementation corresponds to Microsoft Visual Studio
 * C++ standard library
 *
 * references: http://bannalia.blogspot.com/2013/10/implementation-of-c-unordered.html
 */

#include "list_node.hpp"

// forward declare friend classes
namespace sc::regular{
    template <class> class list;
}


namespace sc::utils{

    template <class T>
    class bucket{
    public:
        bucket(unsigned long hash): hash_(hash), first_(nullptr), last_(nullptr){}
    private:
        template <class> friend class sc::regular::list;

    protected:
        sc::utils::list_node<T>* first_;
        sc::utils::list_node<T>* last_;
        unsigned long hash_;
    };

}

#endif //STLCONTAINER_SET_NODE_HPP
