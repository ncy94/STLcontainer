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

        using hasher = Hash;

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

        unordered_set() : unordered_set( size_type(0)){}
        explicit unordered_set (size_type bucket_count,
                const Hash& hash = Hash(),
                const key_equal& equal = key_equal());

        template <class InputIt>
        unordered_set( InputIt first, InputIt last, size_type bucket_count, const Hash& hash)
        :unordered_set(first, last, bucket_count, hash, key_equal()){}

        // copy/move constructor
        unordered_set( const unordered_set& other);
        unordered_set( unordered_set&& other);

        // copy/move assignment operator
        unordered_set&operator=(unordered_set other);

        ~unordered_set();

        /*
         * Iterators
         */

        // returns an iterator to the element at the begining
        iterator begin() noexcept ;
        const_iterator begin() const noexcept ;
        const_iterator cbegin() const noexcept ;

        // returns an iterator to the one-past-the-end element
        iterator end() noexcept ;
        const_iterator end() const noexcept ;
        const_iterator cend() const noexcept ;

        /*
         * Capacity
         */
        bool empty() const{ return list_.size() == 0;}

        size_type size() const{ return list_.size();}

        size_type max_size() const { return std::numeric_limits<diffrence_type >::max();}

        /*
         * Modifiers
         */

        void clear() noexcept ;

        /*
         * If rehashing occurs due to the insertion, all iterators are invalidated. Otherwise iterators are not affected.
         * References are not invalidated.
         * Rehashing occurs only if the new number of elements is greater than max_load_factor()*bucket_count()
         *
         * If the insertion is successful, pointers and references to
         * the element obtained while it is held in the node handle are invalidated,
         * and pointers and references obtained to that element before it was extracted become valid. (since C++17)
         */

        // inserts value
        std::pair<iterator,bool> insert( const value_type& value );
        std::pair<iterator,bool> insert( value_type&& value );

        // Inserts value, using hint as a non-binding suggestion to where the search should start.
        iterator insert( const_iterator hint, const value_type& value );
        iterator insert( const_iterator hint, value_type&& value );

        // Inserts elements from range [first, last).
        // If multiple elements in the range have keys that compare equivalent,
        // it is unspecified which element is inserted
        template< class InputIt >
        void insert( InputIt first, InputIt last );

        // If nh is an empty node handle, does nothing.
        // Otherwise, inserts the element owned by nh into the container ,
        // if the container doesn't already contain an element with a key equivalent to nh.key()
        // The behavior is undefined if nh is not empty
        insert_return_type<iterator, node_type > insert(node_type&& nh);

        iterator insert(const_iterator hint, node_type&& nh);

        template <class... Args>
        std::pair<iterator,bool> emplace( Args... args);

        template <class... Args>
        std::pair<iterator,bool> emplace_hint(const_iterator hint, Args... args);

        iterator erase( const_iterator pos);
        iterator erase( const_iterator first, const_iterator last);
        size_type erase( const key_type& key);

        void swap( unordered_set& ohter) noexcept ;

        // unlinks the node that contains the element pointed to by position
        // and returns a node handle that owns it
        // the second function overloaded by finding x
        node_type extract( const_iterator position);
        node_type extract( const key_type& x);

        template <class H2, class P2>
        void merge(unordered_set<Key, H2, P2>& source);

        template <class H2, class P2>
        void merge(unordered_set<Key, H2, P2>&& source);

        /*
         * Look-up
         */
        size_type count( const Key& key) const;

        iterator find(const Key& key);
        const_iterator find(const Key& key) const;

        bool contains( const Key& key) const;

        std::pair<iterator, iterator> equal_range( const Key& key);
        std::pair<const_iterator, const_iterator> equal_range(const Key& key) const;

        /*
         * Bucket interface
         */

        // return the local start/end iterator at the bucket of index n
        local_iterator begin( size_type n);
        const_local_iterator begin( size_type n) const;
        const_local_iterator cbegin( size_type n) const;

        local_iterator end( size_type n);
        const_local_iterator end( size_type n) const;
        const_local_iterator cend( size_type n) const;

        size_type bucket_count() const { return bsize_;}

        size_type max_bucket_count() const{ return end_ - start_;}

        // returns the number of elements in the bucket with index n
        size_type bucket_size( size_type n) const;

        // returns the index of the bucket for key key
        // The behavior is undefined if bucket_count() is zero.
        size_type bucket( const Key& key) const;

        /*
         * Hash policy
         */
        float load_factor() const { return size()/bucket_count();}

        float max_load_factor() const{ return mlf_;}

        float max_load_factor( float ml) { mlf_ = ml;}

        /*
         * Observers
         */

        hasher hash_fucntion() const{ return Hash();}

        key_equal key_eq() const{ return KeyEqual();}

        void rehash( size_type count);

        void reserve( size_type count);

        /*
         * Non-member functions
         */

        bool friend operator==(const unordered_set& lhs, const unordered_set& rhs);

        bool friend operator!=(const unordered_set& lhs, const unordered_set& rhs);


    private:
        // the start position of the bucket array
        bucket_type* start_;
        // the end position of the bucket array
        bucket_type* end_;
        // the bi-directional linked list that hold the Keys
        list<Key> list_;

        // the number of buckets that has elements
        size_type bsize_;
        // the maximum load factor
        float mlf_;

        Hash hash_;
        KeyEqual equal_;
    };

    template<class Key, class Hash, class KeyEqual>
    unordered_set<Key, Hash, KeyEqual>::unordered_set(
            unordered_set::size_type bucket_count,
            const Hash &hash,const key_equal &equal) : hash_(hash), equal_(equal), bsize_(0), mlf_(0.f), list_(bucket_count)
            {
                start_ = static_cast<bucket_type *>(::operator new(bucket_count * sizeof(bucket_type)));
                end_ = start_ + bucket_count* sizeof(bucket_type);

    }

    // copy constructor
    template<class Key, class Hash, class KeyEqual>
    unordered_set<Key, Hash, KeyEqual>::unordered_set(const unordered_set &other):
        hash_(other.hash_),
        equal_(other.equal_),
        bsize_(other.bsize_),
        mlf_(other.mlf_),
        list_(other.list_)
    {
        start_ = static_cast<bucket_type *>(::operator new(other.end_-other.start_));
        end_ = start_ + (other.end_-other.start_);

        try {
            std::uninitialized_copy(other.start_, other.end_, start_);
        }catch (...){
            ::operator delete(start_);
        }

    }

    // move constructor
    template<class Key, class Hash, class KeyEqual>
    unordered_set<Key, Hash, KeyEqual>::unordered_set(unordered_set &&other){
        hash_ = std::move(other.hash_);
        equal_ = std::move(other.equal_);
        bsize_ = other.bsize_;
        other.bsize_ = 0;
        mlf_ = other.mlf_;
        other.mlf_ = 0;
        list_ = other.list_;

        // move the bucket block
        start_ = other.start_;
        other.start_ = nullptr;
        end_ = other.end_;
        other.end_ = nullptr;
    }





}

#endif //STLCONTAINER_UNORDERED_SET_HPP
