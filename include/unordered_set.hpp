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
        unordered_set( unordered_set&& other) noexcept ;

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
        // Otherwise, inserts the element owned by nh into the container, if the container
        // doesn't already contain an element with a key equivalent to nh.key()
        insert_return_type<iterator, node_type > insert(node_type&& nh);

        iterator insert(const_iterator hint, node_type&& nh);

        template <class... Args>
        std::pair<iterator,bool> emplace( Args... args);

        template <class... Args>
        std::pair<iterator,bool> emplace_hint(const_iterator hint, Args... args);

        iterator erase( const_iterator pos);
        iterator erase( const_iterator first, const_iterator last);
        size_type erase( const key_type& key);

        void swap( unordered_set& other) noexcept ;

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
    unordered_set<Key, Hash, KeyEqual>::unordered_set(unordered_set &&other) noexcept {
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

    template<class Key, class Hash, class KeyEqual>
    void unordered_set<Key, Hash, KeyEqual>::swap(unordered_set &other) noexcept {
        // by swapping the elements, the two containers are effectively swapped
        std::swap(hash_, other.hash_);
        std::swap(equal_, other.equal_);
        std::swap(bsize_, other.bsize_);
        std::swap(mlf_, other.mlf_);
        std::swap(list_, other.swap());
        std::swap(start_, other.start_);
        std::swap(end_, other.end_);
    }

    template<class Key, class Hash, class KeyEqual>
    unordered_set<Key, Hash, KeyEqual>& unordered_set<Key, Hash, KeyEqual>::operator=(unordered_set other) {
        swap(*this, other);
        return *this;
    }

    template<class Key, class Hash, class KeyEqual>
    unordered_set<Key, Hash, KeyEqual>::~unordered_set() {
        clear();
        ::operator delete(start_);
    }

    template<class Key, class Hash, class KeyEqual>
    typename unordered_set<Key, Hash, KeyEqual>::iterator unordered_set<Key, Hash, KeyEqual>::begin() noexcept {
        return list_.begin();
    }

    template<class Key, class Hash, class KeyEqual>
    typename unordered_set<Key, Hash, KeyEqual>::const_iterator unordered_set<Key, Hash, KeyEqual>::begin()const noexcept{
        return list_.begin();
    }

    template<class Key, class Hash, class KeyEqual>
    typename unordered_set<Key, Hash, KeyEqual>::const_iterator unordered_set<Key, Hash, KeyEqual>::cbegin()const noexcept{
        return list_.begin();
    }

    template<class Key, class Hash, class KeyEqual>
    typename unordered_set<Key, Hash, KeyEqual>::iterator unordered_set<Key, Hash, KeyEqual>::end() noexcept {
        return list_.end();
    }

    template<class Key, class Hash, class KeyEqual>
    typename unordered_set<Key, Hash, KeyEqual>::const_iterator unordered_set<Key, Hash, KeyEqual>::end()const noexcept{
        return list_.end();
    }

    template<class Key, class Hash, class KeyEqual>
    typename unordered_set<Key, Hash, KeyEqual>::const_iterator unordered_set<Key, Hash, KeyEqual>::cend()const noexcept{
        return list_.end();
    }

    template<class Key, class Hash, class KeyEqual>
    void unordered_set<Key, Hash, KeyEqual>::clear() noexcept {
        list_.clear();
        std::destroy(start_, end_);
        bsize_ = 0;
    }

    template<class Key, class Hash, class KeyEqual>
    std::pair<typename unordered_set<Key, Hash, KeyEqual>::iterator, bool>
            unordered_set<Key, Hash, KeyEqual>::insert(const value_type &value) {
        if(size() == max_load_factor()* bucket_count()-1)
            rehash(2 * max_bucket_count());

        auto hs = hash_(value);
        auto bindex = bucket(hs); //index of the bucket
        if(start_[bindex] != hs){
            list_.push_back(value);
            start_[bindex].hash_ = hs;
            ++bsize_;
            start_[bindex].first_ = list_.node_.prev_;
            start_[bindex].last_ = list_.node_.prev_;
        }else{
            // handle collsion
            // go throw the elements in the bucket, use local iterator
            for(local_iterator li = begin(index); li != end(index); ++li){
                if(equal_(*li, value))
                    return std::pair<unordered_set::iterator, bool>(li, false);
                // element is not found in the bucket, insert the element at the end of list
                list_.push_back(value);

                // update the last pointer of the bucket
                start_[bindex].last_ = list_.node_.prev_;
            }
        }

        return std::pair<unordered_set::iterator, bool>(list_.end(), true);
    }

    template<class Key, class Hash, class KeyEqual>
    std::pair<typename unordered_set<Key, Hash, KeyEqual>::iterator, bool>
            unordered_set<Key, Hash, KeyEqual>::insert(value_type &&value) {
        if(size() == max_load_factor()* bucket_count()-1)
            rehash(2 * max_bucket_count());

        auto hs = hash_(value);
        auto bindex = bucket(hs); //index of the bucket
        if(start_[bindex] != hs){
            list_.push_back(std::move(value));
            start_[bindex].hash_ = hs;
            ++bsize_;
            start_[bindex].first_ = list_.node_.prev_;
            start_[bindex].last_ = list_.node_.prev_;
        }else{
            // handle collsion
            // go throw the elements in the bucket, use local iterator
            for(local_iterator li = begin(index); li != end(index); ++li){
                if(equal_(*li, value))
                    return std::pair<unordered_set::iterator, bool>(li, false);
                // element is not found in the bucket, insert the element at the end of list
                list_.push_back(std::move(value));

                // update the last pointer of the bucket
                start_[bindex].last_ = list_.node_.prev_;
            }
        }

        return std::pair<unordered_set::iterator, bool>(list_.end(), true);

    }

    template<class Key, class Hash, class KeyEqual>
    typename unordered_set<Key, Hash, KeyEqual>::iterator
    unordered_set<Key, Hash, KeyEqual>::insert(unordered_set::const_iterator hint, const value_type &value) {
        if(size() == max_load_factor()* bucket_count()-1)
            rehash(2 * max_bucket_count());

        auto hs = hash_(value);
        auto bindex = bucket(hs); //index of the bucket
        if(start_[bindex] != hs){
            list_.push_back(value);
            start_[bindex].hash_ = hs;
            ++bsize_;
            start_[bindex].first_ = list_.node_.prev_;
            start_[bindex].last_ = list_.node_.prev_;
        }else{
            // handle collsion
            // go throw the elements in the bucket, use local iterator
            for(auto iter = hint; iter != hint-1; ++iter){
                if(equal_(*iter, value))
                    return std::pair<unordered_set::iterator, bool>(iter, false);
                // element is not found in the bucket, insert the element at the end of list
                list_.push_back(value);

                // update the last pointer of the bucket
                start_[bindex].last_ = list_.node_.prev_;
            }
        }

        return std::pair<unordered_set::iterator, bool>(list_.end(), true);
    }

    template<class Key, class Hash, class KeyEqual>
    typename unordered_set<Key, Hash, KeyEqual>::iterator
    unordered_set<Key, Hash, KeyEqual>::insert(unordered_set::const_iterator hint, value_type &&value) {
        if(size() == max_load_factor()* bucket_count()-1)
            rehash(2 * max_bucket_count());

        auto hs = hash_(value);
        auto bindex = bucket(hs); //index of the bucket
        if(start_[bindex] != hs){
            list_.push_back(std::move(value));
            start_[bindex].hash_ = hs;
            ++bsize_;
            start_[bindex].first_ = list_.node_.prev_;
            start_[bindex].last_ = list_.node_.prev_;
        }else{
            // handle collsion
            // go throw the elements in the bucket, use local iterator
            for(auto iter = hint; iter != hint-1; ++iter){
                if(equal_(*iter, value))
                    return std::pair<unordered_set::iterator, bool>(iter, false);
                // element is not found in the bucket, insert the element at the end of list
                list_.push_back(std::move(value));

                // update the last pointer of the bucket
                start_[bindex].last_ = list_.node_.prev_;
            }
        }

        return std::pair<unordered_set::iterator, bool>(list_.end(), true);
    }

    template<class Key, class Hash, class KeyEqual>
    template<class InputIt>
    void unordered_set<Key, Hash, KeyEqual>::insert(InputIt first, InputIt last) {
        for(auto iter = first; iter != last; ++iter)
            insert(*iter);
    }

    template<class Key, class Hash, class KeyEqual>
    typename unordered_set<Key, Hash, KeyEqual>::template insert_return_type<
            typename unordered_set<Key, Hash, KeyEqual>::iterator,
            typename unordered_set<Key, Hash, KeyEqual>::node_type>
    unordered_set<Key, Hash, KeyEqual>::insert(unordered_set::node_type &&nh) {
        // if nh is empty node handle, return false and the end iterator
        if(nh.next_ == nullptr && nh.prev_ == nullptr)
            return unordered_set::insert_return_type<unordered_set::iterator, unordered_set::node_type>(end(), nh);

        // if nh is not empty node handle, insert the node
        if(size() == max_load_factor()* bucket_count()-1)
            rehash(2 * max_bucket_count());

        auto hs = hash_(nh.val_);
        auto bindex = bucket(hs); //index of the bucket
        if(start_[bindex] != hs){
            // add the node to the end of list
            list_.node_.prev_->next_ = &nh;
            nh.prev_ = &(list_.node_.prev_);
            list_.node_.prev_ = &nh;
            nh.next_ = &(list_.node_);

            start_[bindex].hash_ = hs;
            ++bsize_;
            start_[bindex].first_ = list_.node_.prev_;
            start_[bindex].last_ = list_.node_.prev_;
        }else{
            // handle collsion
            // go throw the elements in the bucket, use local iterator
            for(local_iterator li = begin(index); li != end(index); ++li){
                if(equal_(*li, nh.val_))
                    return unordered_set::insert_return_type<unordered_set::iterator, unordered_set::node_type>(li, nh);
                // element is not found in the bucket, insert the element at the end of list
                list_.node_.prev_->next_ = &nh;
                nh.prev_ = &(list_.node_.prev_);
                list_.node_.prev_ = &nh;
                nh.next_ = &(list_.node_);

                // update the last pointer of the bucket
                start_[bindex].last_ = list_.node_.prev_;
            }
        }

        return unordered_set::insert_return_type<unordered_set::iterator, unordered_set::node_type>(end(),nh);
    }

    template<class Key, class Hash, class KeyEqual>
    typename unordered_set<Key, Hash, KeyEqual>::iterator
    unordered_set<Key, Hash, KeyEqual>::insert(const_iterator hint, unordered_set::node_type &&nh) {
        // if nh is empty node handle, return false and the end iterator
        if(nh.next_ == nullptr && nh.prev_ == nullptr)
            return end();

        // if nh is not empty node handle, insert the node
        if(size() == max_load_factor()* bucket_count()-1)
            rehash(2 * max_bucket_count());

        auto hs = hash_(nh.val_);
        auto bindex = bucket(hs); //index of the bucket
        if(start_[bindex] != hs){
            // add the node to the end of list
            list_.node_.prev_->next_ = &nh;
            nh.prev_ = &(list_.node_.prev_);
            list_.node_.prev_ = &nh;
            nh.next_ = &(list_.node_);

            start_[bindex].hash_ = hs;
            ++bsize_;
            start_[bindex].first_ = list_.node_.prev_;
            start_[bindex].last_ = list_.node_.prev_;
        }else{
            // handle collsion
            // go throw the elements in the bucket, use local iterator
            for(auto iter = hint; iter != hint-1; ++iter){
                if(equal_(*iter, nh.val_))
                    return iter;
                // element is not found in the bucket, insert the element at the end of list
                list_.node_.prev_->next_ = &nh;
                nh.prev_ = &(list_.node_.prev_);
                list_.node_.prev_ = &nh;
                nh.next_ = &(list_.node_);

                // update the last pointer of the bucket
                start_[bindex].last_ = list_.node_.prev_;
            }
        }

        return end();
    }

    template<class Key, class Hash, class KeyEqual>
    typename unordered_set<Key, Hash, KeyEqual>::size_type
    unordered_set<Key, Hash, KeyEqual>::bucket_size(unordered_set::size_type n) const {
        size_type s = 0;
        for(auto li=begin(n); li!=end(n); ++li)
            ++s;
        return s;
    }

    template<class Key, class Hash, class KeyEqual>
    typename unordered_set<Key, Hash, KeyEqual>::size_type
    unordered_set<Key, Hash, KeyEqual>::bucket(const Key &key) const {
        return hash_(key) % max_bucket_count();
    }

    template<class Key, class Hash, class KeyEqual>
    typename unordered_set<Key, Hash, KeyEqual>::iterator
    unordered_set<Key, Hash, KeyEqual>::erase(unordered_set::const_iterator pos) {
        size_type bindex = bucket(pos->val_);

        // if the bucket size is 1, clear the bucket
        if(bucket_size(bindex) == 1){
            start_[bindex].first_ = nullptr;
            start_[bindex].last_ = nullptr;
            --bsize_;
        } else{
            // if the pos node is the first node of bucket
            if(pos->val_ == start_[bindex].first_->val_){
                start_[bindex].first_ = pos->next_;
            }else if(pos->val_ == start_[bindex].last_->val_){
                start_[bindex].last_ = pos->prev_;
            }

        }
        return list_.erase(pos);
    }

}

#endif //STLCONTAINER_UNORDERED_SET_HPP
