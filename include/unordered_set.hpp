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
        using key_type = Key;

        using value_type = Key;

        using size_type = std::size_t ;

        using bucket_type = sc::utils::bucket<Key>;

        using diffrence_type = std::ptrdiff_t ;

        using key_equal = KeyEqual;

        using reference = value_type &;

        using const_refrence = const value_type &;

        using pointer = value_type *;

        using const_pointer = const value_type *;

        using iterator = typename list<Key>::iterator ;

        using const_iterator = typename list<Key>::iterator;

        //An iterator type whose category, value, difference, pointer and
        //reference types are the same as iterator. This iterator
        //can be used to iterate through a single bucket but not across buckets
        using local_iterator = iterator;

        using const_local_iterator = const_iterator;

        using node_type = list_node<Key>;

        template <class Iter, class NodeTye>
        struct insert_return_type{
            Iter posisiton;
            bool inserted;
            NodeTye node;
        };


    private:
        // the start position of the bucket array
        bucket_type* start_;
        // the end position of the bucket array
        bucket_type* end_;
        // the number of keys in the set
        size_type size_;
    };

}

#endif //STLCONTAINER_UNORDERED_SET_HPP
