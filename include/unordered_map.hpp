//
// Created by NCY on 2019-08-28.
//

#ifndef STLCONTAINER_UNORDERED_MAP_HPP
#define STLCONTAINER_UNORDERED_MAP_HPP

/*
 * The implementation corresponds to Microsoft
 * Visual Studio standard library of Dinkumware.
 *
 * This implementation has problem that if
 * user-defined hash function throws, erasure
 * would be undefined-behaviour.
 *
 * This implementation uses the sc::regular::list
 * which is implemented in this library
 */

#include "functional"
#include "list.hpp"
#include "bucket.hpp"

namespace sc::regular{

    template <
            class Key,
            class T,
            class Hash = std::hash<Key>,
            class KeyEqual = std::equal_to<Key>
    >
    class unordered_map{

    public:
        using key_type = Key;

        using value_type = std::pair<const Key, T>;

        using size_type = std::size_t ;

        using bucket_type = sc::utils::bucket<Key>;

        using diffrence_type = std::ptrdiff_t ;

        using hasher = Hash;

        using key_equal = KeyEqual;

        using reference = value_type &;

        using const_refrence = const value_type &;

        using pointer = value_type *;

        using const_pointer = const value_type *;

        using iterator = typename list<value_type >::iterator ;

        using const_iterator = typename list<value_type >::iterator;

        //An iterator type whose category, value, difference, pointer and
        //reference types are the same as iterator. This iterator
        //can be used to iterate through a single bucket but not across buckets
        using local_iterator = iterator;

        using const_local_iterator = const_iterator;

        using node_type = list_node<value_type >;

        template <class Iter, class NodeTye>
        struct insert_return_type{
            Iter posisiton;
            bool inserted;
            NodeTye node;
        };

        explicit unordered_map (size_type bucket_count,
                                const Hash& hash = Hash(),
                                const key_equal& equal = key_equal());

        template <class InputIt>
        unordered_map( InputIt first, InputIt last, size_type bucket_count, const Hash& hash);

        // copy/move constructor
        unordered_map( const unordered_map& other);
        unordered_map( unordered_map&& other) noexcept ;

        // copy/move assignment operator
        unordered_map&operator=(unordered_map other);

        ~unordered_map();

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

        void swap( unordered_map& other) noexcept ;

        // unlinks the node that contains the element pointed to by position
        // and returns a node handle that owns it
        // the second function overloaded by finding x
        node_type extract( const_iterator position);
        node_type extract( const key_type& x);

        template <class H2, class P2>
        void merge(unordered_map<Key, H2, P2>& source);

        template <class H2, class P2>
        void merge(unordered_map<Key, H2, P2>&& source);

        /*
         * Look-up
         */
        T& at(const Key& key);
        const T& at( const Key& key) const;

        T& operator[]( const Key& key);
        T&operator[]( Key&& key);

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

        bool friend operator==(const unordered_map& lhs, const unordered_map& rhs);

        bool friend operator!=(const unordered_map& lhs, const unordered_map& rhs);


    private:
        // the start position of the bucket array
        bucket_type* start_;
        // the end position of the bucket array
        bucket_type* end_;
        // the bi-directional linked list that hold the Keys
        list<value_type > list_;

        // the number of buckets that has elements
        size_type bsize_;
        // the maximum load factor
        float mlf_;

        Hash hash_;
        KeyEqual equal_;
    };


    template<class Key, class T, class Hash, class KeyEqual>
    unordered_map<Key, T, Hash, KeyEqual>::unordered_map(
            unordered_map::size_type bucket_count,
            const Hash &hash,const key_equal &equal) : hash_(hash), equal_(equal), bsize_(0), mlf_(0.f), list_(bucket_count)
    {
        start_ = static_cast<bucket_type *>(::operator new(bucket_count * sizeof(bucket_type)));
        end_ = start_ + bucket_count* sizeof(bucket_type);

    }

    // copy constructor
    template<class Key, class T, class Hash, class KeyEqual>
    unordered_map<Key, T, Hash, KeyEqual>::unordered_map(const unordered_map &other):
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
    template<class Key, class T, class Hash, class KeyEqual>
    unordered_map<Key, T, Hash, KeyEqual>::unordered_map(unordered_map &&other) noexcept {
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

    template<class Key, class T, class Hash, class KeyEqual>
    void unordered_map<Key, T, Hash, KeyEqual>::unordered_map::swap(unordered_map &other) noexcept {
        // by swapping the elements, the two containers are effectively swapped
        std::swap(hash_, other.hash_);
        std::swap(equal_, other.equal_);
        std::swap(bsize_, other.bsize_);
        std::swap(mlf_, other.mlf_);
        std::swap(list_, other.swap());
        std::swap(start_, other.start_);
        std::swap(end_, other.end_);
    }

    template<class Key, class T, class Hash, class KeyEqual>
    unordered_map<Key, T, Hash, KeyEqual>& unordered_map<Key, T, Hash, KeyEqual>::operator=(unordered_map other) {
        swap(*this, other);
        return *this;
    }

    template<class Key, class T, class Hash, class KeyEqual>
    unordered_map<Key, T, Hash, KeyEqual>::~unordered_map() {
        clear();
        ::operator delete(start_);
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::iterator unordered_map<Key, T, Hash, KeyEqual>::begin() noexcept {
        return list_.begin();
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::const_iterator unordered_map<Key, T, Hash, KeyEqual>::begin() const noexcept{
        return list_.begin();
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::const_iterator unordered_map<Key, T, Hash, KeyEqual>::cbegin() const noexcept{
        return list_.begin();
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::iterator unordered_map<Key, T, Hash, KeyEqual>::end() noexcept {
        return list_.end();
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::const_iterator unordered_map<Key, T, Hash, KeyEqual>::end() const noexcept{
        return list_.end();
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::const_iterator unordered_map<Key, T, Hash, KeyEqual>::cend() const noexcept{
        return list_.end();
    }

    template<class Key, class T, class Hash, class KeyEqual>
    void unordered_map<Key, T, Hash, KeyEqual>::clear() noexcept {
        list_.clear();
        std::destroy(start_, end_);
        bsize_ = 0;
    }

    /*
     * Bucket interface
     */
    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::local_iterator
    unordered_map<Key, T, Hash, KeyEqual>::begin(unordered_map::size_type n) {
        return start_[n].first_;
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::const_local_iterator
    unordered_map<Key, T, Hash, KeyEqual>::begin(unordered_map::size_type n) const{
        return start_[n].first_;
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::const_local_iterator
    unordered_map<Key, T, Hash, KeyEqual>::cbegin(unordered_map::size_type n) const{
        return start_[n].first_;
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::local_iterator
    unordered_map<Key, T, Hash, KeyEqual>::end(unordered_map::size_type n) {
        return start_[n].last_+1;
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::const_local_iterator
    unordered_map<Key, T, Hash, KeyEqual>::end(unordered_map::size_type n) const{
        return start_[n].last_+1;
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::const_local_iterator
    unordered_map<Key, T, Hash, KeyEqual>::cend(unordered_map::size_type n) const{
        return start_[n].last_+1;
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::size_type
    unordered_map<Key, T, Hash, KeyEqual>::bucket_size(unordered_map::size_type n) const {
        size_type s = 0;
        for(auto li=begin(n); li!=end(n); ++li)
            ++s;
        return s;
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::size_type
    unordered_map<Key, T, Hash, KeyEqual>::bucket(const Key &key) const {
        return hash_(key) % max_bucket_count();
    }

    /*
     * Rehash and Reserve
     */

    template<class Key, class T, class Hash, class KeyEqual>
    void unordered_map<Key, T, Hash, KeyEqual>::rehash(unordered_map::size_type count) {
        auto new_start = static_cast<key_type *>(::operator new(count));
        auto old_count = bucket_count();

        try {
            std::uninitialized_move(start_, end_, new_start);
        }catch (...){
            ::operator delete(new_start);
        }

        start_ = new_start;
        end_ = start_ + 2 * old_count;
    }

    template<class Key, class T, class Hash, class KeyEqual>
    void unordered_map<Key, T, Hash, KeyEqual>::reserve(unordered_map::size_type count){
        rehash(ceil(count / max_load_factor()));
    }

    template<class Key, class T, class Hash, class KeyEqual>
    std::pair<typename unordered_map<Key, T, Hash, KeyEqual>::iterator, bool>
    unordered_map<Key, T, Hash, KeyEqual>::insert(const unordered_map::value_type &value) {
        if(size() == max_load_factor()* bucket_count()-1)
            rehash(2 * max_bucket_count());

        auto hs = hash_(value.first);
        auto bindex = bucket(value.first); //index of the bucket
        if(bucket_count(bindex) == 0){
            list_.push_back(value);
            start_[bindex].hash_ = hs;
            ++bsize_;
            start_[bindex].first_ = list_.node_.prev_;
            start_[bindex].last_ = list_.node_.prev_;
            return std::pair<unordered_map::iterator, bool>(list_.end(), true);
        }else{
            // handle collsion
            // go throw the elements in the bucket, use local iterator
            for(local_iterator li = begin(index); li != end(index); ++li) {
                // if the key equals
                if (equal_((*li).first, value.first))
                    return std::pair<unordered_map::iterator, bool>(li, false);
            }
            // element is not found in the bucket, insert the element at the end of bucket
            list_.insert(start_[bindex].last_, value);
        }

        return std::pair<unordered_map::iterator, bool>(start_[bindex].last_, true);
    }

    template<class Key, class T, class Hash, class KeyEqual>
    std::pair<typename unordered_map<Key, T, Hash, KeyEqual>::iterator, bool>
    unordered_map<Key, T, Hash, KeyEqual>::insert(value_type &&value) {
        if(size() == max_load_factor()* bucket_count()-1)
            rehash(2 * max_bucket_count());

        auto hs = hash_(value.first);
        auto bindex = bucket(value.first); //index of the bucket
        if(bucket_count(bindex)){
            list_.push_back(std::move(value));
            start_[bindex].hash_ = hs;
            ++bsize_;
            start_[bindex].first_ = list_.node_.prev_;
            start_[bindex].last_ = list_.node_.prev_;
            return std::pair<unordered_map::iterator, bool>(list_.end(), true);
        }else{
            // handle collsion
            // go throw the elements in the bucket, use local iterator
            for(local_iterator li = begin(index); li != end(index); ++li) {
                if (equal_(li->first, value.first))
                    return std::pair<unordered_map::iterator, bool>(li, false);
            }
            // element is not found in the bucket, insert the element at the end of list
            list_.insert(start_[bindex].last_, std::move(value));

        }

        return std::pair<unordered_map::iterator, bool>(start_[bindex].last_, true);

    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::iterator
    unordered_map<Key, T, Hash, KeyEqual>::insert(unordered_map::const_iterator hint, const value_type &value) {
        if(size() == max_load_factor()* bucket_count()-1)
            rehash(2 * max_bucket_count());

        auto hs = hash_(value.first);
        auto bindex = bucket(value.first); //index of the bucket
        if(bucket_count(bindex)){
            list_.push_back(std::move(value));
            start_[bindex].hash_ = hs;
            ++bsize_;
            start_[bindex].first_ = list_.node_.prev_;
            start_[bindex].last_ = list_.node_.prev_;
            return std::pair<unordered_map::iterator, bool>(list_.end(), true);
        }else{
            // handle collsion
            // go throw the elements in the bucket, use local iterator
            for(auto iter = hint; iter != hint-1; ++iter) {
                if (equal_(iter->first, value.first))
                    return std::pair<unordered_map::iterator, bool>(iter, false);
            }
            // element is not found in the bucket, insert the element at the end of list
            list_.insert(start_[bindex].last_, std::move(value));

        }

        return std::pair<unordered_map::iterator, bool>(start_[bindex].last_, true);
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::iterator
    unordered_map<Key, T, Hash, KeyEqual>::insert(unordered_map::const_iterator hint, value_type &&value) {
        if(size() == max_load_factor()* bucket_count()-1)
            rehash(2 * max_bucket_count());

        auto hs = hash_(value.first);
        auto bindex = bucket(value.first); //index of the bucket
        if(bucket_count(bindex) == 0){
            list_.push_back(std::move(value));
            start_[bindex].hash_ = hs;
            ++bsize_;
            start_[bindex].first_ = list_.node_.prev_;
            start_[bindex].last_ = list_.node_.prev_;
            return std::pair<unordered_map::iterator, bool>(list_.end(), true);
        }else{
            // handle collsion
            // go throw the elements in the bucket, use local iterator
            for(auto iter = hint; iter != hint-1; ++iter) {
                if (equal_(iter->first, value.first))
                    return std::pair<unordered_map::iterator, bool>(iter, false);
            }
            // element is not found in the bucket, insert the element at the end of list
            list_.insert(start_[bindex].last_, std::move(value));

        }

        return std::pair<unordered_map::iterator, bool>(start_[bindex].last_, true);

    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::template insert_return_type<
            typename unordered_map<Key, T, Hash, KeyEqual>::iterator,
            typename unordered_map<Key, T, Hash, KeyEqual>::node_type>
    unordered_map<Key, T, Hash, KeyEqual>::insert(unordered_map::node_type &&nh) {
        using insert_return_type = typename unordered_map<Key, T, Hash, KeyEqual>::insert_return_type<
                typename unordered_map<Key, T, Hash, KeyEqual>::iterator,
                typename unordered_map<Key, T, Hash, KeyEqual>::node_type>;
        // if nh is empty node handle, return false and the end iterator
        if(nh.next_ == nullptr && nh.prev_ == nullptr)
            return insert_return_type(end(), nh);

        // if nh is not empty node handle, insert the node
        if(size() == max_load_factor()* bucket_count()-1)
            rehash(2 * max_bucket_count());

        auto hs = hash_(nh.val_.first);
        auto bindex = bucket(nh.val_.first); //index of the bucket
        if(bucket_count(bindex) == 0){
            // add the node to the end of list
            list_.node_.prev_->next_ = &nh;
            nh.prev_ = &(list_.node_.prev_);
            list_.node_.prev_ = &nh;
            nh.next_ = &(list_.node_);

            start_[bindex].hash_ = hs;
            ++bsize_;
            start_[bindex].first_ = list_.node_.prev_;
            start_[bindex].last_ = list_.node_.prev_;
            return insert_return_type(end(), nh);
        }else{
            // handle collsion
            // go throw the elements in the bucket, use local iterator
            for(local_iterator li = begin(index); li != end(index); ++li) {
                if (equal_(li->first, nh.val_.first))
                    return insert_return_type(li, nh);
            }
            // element is not found in the bucket, insert the element at the end of bucket
            start_[bindex].last_->prev_->next_ = &nh;
            nh.prev_ = &(start_[bindex].last_->prev_);
            start_[bindex].last_->prev_ = &nh;
            nh.next_ = &(start_[bindex].last_);

        }

        return insert_return_type(start_[bindex].last_,nh);

    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::iterator
    unordered_map<Key, T, Hash, KeyEqual>::insert(unordered_map::const_iterator hint, unordered_map::node_type &&nh) {
        // if nh is empty node handle, return false and the end iterator
        if(nh.next_ == nullptr && nh.prev_ == nullptr)
            return end();

        // if nh is not empty node handle, insert the node
        if(size() == max_load_factor()* bucket_count()-1)
            rehash(2 * max_bucket_count());

        auto hs = hash_(nh.val_.first);
        auto bindex = bucket(nh.val_.first); //index of the bucket
        if(bucket_count(bindex) == 0){
            // add the node to the end of list
            list_.node_.prev_->next_ = &nh;
            nh.prev_ = &(list_.node_.prev_);
            list_.node_.prev_ = &nh;
            nh.next_ = &(list_.node_);

            start_[bindex].hash_ = hs;
            ++bsize_;
            start_[bindex].first_ = list_.node_.prev_;
            start_[bindex].last_ = list_.node_.prev_;
            return end();
        }else{
            // handle collsion
            // go throw the elements in the bucket, use local iterator
            for(auto iter = hint; iter != hint-1 ; ++iter) {
                if (equal_(iter->first, nh.val_.first))
                    return iter;
            }
            // element is not found in the bucket, insert the element at the end of bucket
            start_[bindex].last_->prev_->next_ = &nh;
            nh.prev_ = &(start_[bindex].last_->prev_);
            start_[bindex].last_->prev_ = &nh;
            nh.next_ = &(start_[bindex].last_);

        }
        return start_[bindex].last_;
    }

    /*
     * Erase functions
     */

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::iterator
    unordered_map<Key, T, Hash, KeyEqual>::erase(unordered_map::const_iterator pos) {
        size_type bindex = bucket(pos->val_.first);

        // if the bucket size is 1, clear the bucket
        if(bucket_size(bindex) == 1){
            start_[bindex].first_ = nullptr;
            start_[bindex].last_ = nullptr;
            --bsize_;
        } else{
            // if the pos node is the first node of bucket
            if(equal_(pos->val_.first, start_[bindex].first_->val_.first)){
                start_[bindex].first_ = pos->next_;
            }
                // if the pos node is the last node of bucket
            else if(equal_(pos->val_.first, start_[bindex].last_->val_.first)){
                start_[bindex].last_ = pos->prev_;
            }

        }
        return list_.erase(pos);
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::iterator
    unordered_map<Key, T, Hash, KeyEqual>::erase(unordered_map::const_iterator first,
            unordered_map::const_iterator last) {
        for(auto iter=first; iter!=last; ++iter){
            auto bindex = bucket(iter->val_.first);

            // if the element only has one element
            if(bucket_size(bindex) == 1){
                start_[bindex].first_ = nullptr;
                start_[bindex].last_ = nullptr;
                --bsize_;
                continue;
            }

            // first element is the end of bucket
            if(iter == first && start_[bindex].last_ == &(*iter)){
                start_[bindex].last_ = start_[bindex].last_->prev_;
                continue ;
            }

            // last element is the start of bucket
            if(iter == last-1 && start_[bindex].first_ == &(*iter)){
                start_[bindex].first_ = start_[bindex].first_->next_;
                continue;
            }
        }

        return list_.erase(first, last);
    }

    template<class Key, class T, class Hash, class KeyEqual>
    typename unordered_map<Key, T, Hash, KeyEqual>::size_type
    unordered_map<Key, T, Hash, KeyEqual>::erase(const key_type &key) {
        size_type bindex = bucket(key);

        // if the bucket is empty, the container doesn't have this key
        if(bucket_size(bindex) == 0)
            return 0;
        // if the bucket size is 1, clear the bucket
        if(bucket_size(bindex) == 1){
            // erase the node from the list
            list_.erase(start_[bindex].first_);
            start_[bindex].first_ = nullptr;
            start_[bindex].last_ = nullptr;
            --bsize_;
        } else{
            // if the pos node is the first node of bucket
            if(equal_(key, start_[bindex].first_->val_)){
                start_[bindex].first_ = start_[bindex].first_->next_;
                list_.erase(start_[bindex].first_->prev_);
            }
                // if the pos node is the last node of bucket
            else if(equal_(key,start_[bindex].last_->val_)){
                start_[bindex].last_ = start_[bindex].last_->prev_;
                list_.erase(start_[bindex].last_->next_);
            }
        }
        return 1;
    }


}

#endif //STLCONTAINER_UNORDERED_MAP_HPP
