//
// Created by NCY on 2019-08-22.
//

#ifndef STLCONTAINER_UNORDERED_SET_HPP
#define STLCONTAINER_UNORDERED_SET_HPP

/*
 * The implementation corresponds to Microsoft
 * Visual Studio standard library of Dinkumware.
 *
 * This implementation has problem that if
 * user-defined hash function throws, erasure
 * would be undefined-behaviour.
 * references: http://bannalia.blogspot.com/2013/10/implementation-of-c-unordered.html
 *
 * This implementation uses the sc::regular::list
 * which is implemented in this library
 */

#include "bucket.hpp"
#include "list.hpp"

namespace sc::regular{

    template <
            class Key,
            class Hash = std::hash<Key>,
            class KeyEqual = std::equal_to<Key>
    >class unordered_set{

    public:
    private:

    };

}

#endif //STLCONTAINER_UNORDERED_SET_HPP
