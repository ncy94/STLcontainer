//
// Created by NCY on 2019-08-16.
//

#ifndef STLCONTAINER_LIST_HPP
#define STLCONTAINER_LIST_HPP

#include "list_node.hpp"
#include "list_iterator.hpp"
#include <functional>
#include <cassert>

namespace sc::regular{

    using sc::utils::list_node;

    //doubly-linked list with sentinel node
    template <class T>
    class list{
    public:

        using value_type = T;

        using size_type = std::size_t;

        using reference = T&;

        using const_refernce = const T&;

        using pointer = T*;

        using const_pointer = const T*;

        using iterator = sc::utils::list_iterator<T>;

        using const_iterator = sc::utils::list_iterator<T const>;


        /*
         * constructors, assignments, destructor
         */

        //default constructor
        list(): node_(), size_(0) {
            node_.next_ = &node_;
            node_.prev_ = &node_;
        }

        //construct list of length count, default value if not specified
        explicit list(size_type count, const value_type& value = value_type());

        template <class InputIterator >
                list(InputIterator first, InputIterator last);

        list(const list& other);
        list(list&& other) noexcept ;
        list&operator=(const list& other);
        list&operator=(list&& other) noexcept ;

        ~list(){
            clear();
        }

        template <class InputIterator>
                void assign(InputIterator first, InputIterator last);

        void assign( size_type count, const T& value);

        /*
         * Element acess
         */

        reference front() { return *(node_.next());}

        const_refernce front() const{ return *(node_.next());}

        reference back() { return *(node_.prev());}

        const_refernce back() const{ return *(node_.prev());}

        /*
         * Iterators
         */

        iterator begin() { return iterator(node_.next());}

        const_iterator begin() const{ return const_iterator(node_.next());}

        const_iterator cbegin() const{ return const_iterator(node_.next());}

        //end returns the iterator points to the one-past-the-end element

        iterator end() { return iterator(node_);}

        const_iterator end() const{ return const_iterator(node_);}

        const_iterator cend() const{ return const_iterator(node_);}

        /*
         * Capacity
         */

        bool empty() const{ return size_ == 0;}

        size_type size() const{ return size_;}

        size_type max_size() const{ return std::numeric_limits<std::ptrdiff_t >::max();}

        /*
         * Modifiers
         */

        void clear();

        // insert one value before pos
        // returns the iterator points to the inserted element
        iterator insert( const_iterator pos, const T& value);

        iterator insert( const_iterator pos, T&& value);

        // insert copies of values before pos
        // returns the iterator points to the first inserted value
        iterator insert( const_iterator pos, size_type count, const T& value);

        // insert elements from range [first, last) before pos
        template <class InputIt>
                iterator insert( const_iterator pos, InputIt first, InputIt last);

        template <class... Args>
                iterator emplace(const_iterator pos, Args&&... args);

        // erase the element at pos, return the iterator following the last element
        // if pos refers to the last element, end() iterator is returned.
        iterator erase( const_iterator pos);

        // erase elements in range [first, last)
        iterator erase( const_iterator first, const_iterator last);

        void push_back( const T& value);
        void push_back( T&& value);

        // return reference to the inserted value: since C++ 17
        template <class... Args>
                reference emplace_back(Args&&... args);

        void pop_back();

        void push_front( const T& value);
        void push_front( T&& value);

        // resize the container to contain count elements.
        // if the current size is greater than count, the container is trimmed
        // if the current size is less than count, additional elements of either
        // default value or copies of value are appended.
        void resize( size_type count);
        void resize( size_type count, const value_type& value);

        void swap( list& other);

        /*
         * Operations
         */

        // merge two sorted lists
        void merge( list&& other);

        template <class Compare>
                void merge( list&& other, Compare comp);

        // splice: transfers elements from one list to another
        // No elements are copied or moved, only the
        // internal pointers of the list nodes are re-pointed
        void splice( const_iterator pos, list&& other);

        //transfer the element pointed to by it, into *this at the position before pos
        void splice( const_iterator pos, list&& other, const_iterator it);

        //transfer the elements in the range[first,last), into *this at the position before pos
        void splice( const_iterator pos, list&& other, const_iterator first, const_iterator last);

        // remove all elements of value, return the removed number
        size_type remove( const T& value);

        // reverse the order of the elements
        void reverse();

        // remove all the duplicates
        void unique();

        // this sort uses operator> to compare elements
        void sort();

        template <class Compare>
                void sort( Compare comp);

        /*
         * Non-member functions
         */

        friend bool operator==(const list& l1, const list& l2);

        friend bool operator!=(const list& l1, const list& l2);

        friend bool operator<(const list& l1, const list& l2);

        friend bool operator>(const list& l1, const list& l2);

        friend bool operator<=(const list& l1, const list& l2);

        friend bool operator>=(const list& l1, const list& l2);

        //friend std::swap(list& l1, list& l2);

    private:
        list_node<T> node_; // sentinel node
        size_type size_;
    };

    template <class T>
    list<T>::list(list::size_type count, const value_type &value):node_(), size_(0) {
        list_node<T>* tmp = &node_;
        while(size_ < count){
            tmp->next_ = new list_node(value);
            tmp->next->prev_ = tmp;
            tmp = tmp->next_;
            size_ += 1;
        }
        tmp->next_ = &node_;

    }

    template<class T>
    template<class InputIterator>
    list<T>::list(InputIterator first, InputIterator last): node_(), size_(0) {
        list_node<T>* tmp = &node_;
        for(auto iter = first; iter != last; ++iter){
            tmp->next_ = new list_node(iter->val_);
            tmp->next_->prev_ = tmp;
            tmp = tmp->next_;
            size_ += 1;
        }
        tmp->next_ = &node_;

    }

    template<class T>
    list<T>::list(const list &other): node_(), size_(0) {
        list_node<T>* des = &node_;
        list_node<T>* src = &(other.node_);
        while(size_ < other.size()){
            des->next_ = new list_node(src->val_);
            des->next_->prev_ = des;

            //proceed both nodes
            des = des->next_;
            src = src->next_;
            size_ += 1;
        }
        des->next_ = &node_;
        // src should points to the sentinel node at this time
        assert(src->next_ == other.end());
    }

    template<class T>
    list<T>::list(list &&other) noexcept {
        node_ = std::move(other.node_);
        size_ = other.size();
        other.size_ = 0;
    }

    template<class T>
    list<T> &list<T>::operator=(const list &other) {
        clear();
        list_node<T>* des = &node_;
        list_node<T>* src = &(other.node_);
        while(size_ < other.size()){
            des->next_ = new list_node(src->val_);
            des->next_->prev_ = des;

            //proceed both nodes
            des = des->next_;
            src = src->next_;
            size_ += 1;
        }
        des->next_ = &node_;
        // src should points to the sentinel node at this time
        assert(src->next_ == other.end());
    }

    template<class T>
    list<T> &list<T>::operator=(list &&other) noexcept {
        clear();
        node_ = std::move(other.node_);
        size_ = other.size();
        other.size_ = 0;
    }

    template<class T>
    template<class InputIterator>
    void list<T>::assign(InputIterator first, InputIterator last) {
        clear();
        list_node<T>* tmp = &node_;
        for(auto iter = first; iter != last; ++iter){
            tmp->next_ = new list_node(iter->val_);
            tmp->next_->prev_ = tmp;
            tmp = tmp->next_;
            size_ += 1;
        }
        tmp->next_ = &node_;

    }

    template<class T>
    void list<T>::assign(list::size_type count, const T &value) {
        clear();
        list_node<T>* tmp = &node_;
        for(int i=0; i<count; ++i){
            tmp->next_ = new list_node(value);
            tmp->next_->prev_ = tmp;
            tmp = tmp->next_;
            size_ += 1;
        }

        tmp->next_ = &node_;

    }

    template<class T>
    void list<T>::clear() {
        // how to declare a pointer?
        // delete from tail to head
        list_node<T>* tmp = node_.prev_;
        while(size_ > 0){
            tmp = tmp->prev_;
            delete(tmp->next_);
            --size_;
        }

        assert(size_ == 0 && node_.prev_==node_.next_);

    }

    template<class T>
    typename list<T>::iterator list<T>::insert(list::const_iterator pos, const T &value) {
        list_node<T>* current = &(*pos); // the node to be inserted before
        list_node<T>* prev = current->prev_; // the proceeding node of new node

        //insert a new node after prev
        prev->next_ = new list_node(value);
        prev->next_->prev_ = prev;

        //connect new node with current
        current->prev_ = prev->next_;
        prev->next_ = current;

        ++size_;

        return iterator(prev->next_);
    }

    template<class T>
    typename list<T>::iterator list<T>::insert(list::const_iterator pos, T &&value) {
        list_node<T>* current = &(*pos); // the node to be inserted before
        list_node<T>* prev = current->prev_; // the proceeding node of new node

        //insert a new node after prev
        current->prev_->next_ = new list_node(std::move(value));
        prev->next_->prev_ = prev;

        //connect new node with current
        current->prev_ = prev->next_;
        prev->next_ = current;

        ++size_;

        return iterator(prev->next_);
    }

    template<class T>
    typename list<T>::iterator list<T>::insert(list::const_iterator pos, list::size_type count, const T &value) {
        list_node<T>* current = &(*pos); // the node to be inserted before
        list_node<T>* prev = current->prev_; // the node used for iteration
        list_node<T>* res = current->prev_; // the prev of the node to be returned

        // insert i nodes after prev
        for(int i=0; i<count; ++i){
            prev->next_ = new list_node(value);
            prev->next_->prev_ = prev;

            //proceed to the next node
            prev = prev->next_;
            ++size_;
        }

        prev->next_ = current;
        current->prev_ = prev;

        //return the first inserted element
        return iterator(res->next_);
    }

    template<class T>
    template<class InputIt>
    typename list<T>::iterator list<T>::insert(list::const_iterator pos, InputIt first, InputIt last) {
        list_node<T>* current = &(*pos);
        list_node<T>* prev = current->prev_;
        list_node<T>* res = current->prev_;

        for(auto iter = first; iter != last; ++iter){
            prev->next_ = new list_node((*iter).val_);
            prev->next_->prev_ = prev;

            //proceed to the next node
            prev = prev->next_;
            ++size_;
        }

        prev->next_ = current;
        current->prev_ = prev;

        //return the first inserted element
        return iterator(res->next_);

    }

    template<class T>
    typename list<T>::iterator list<T>::erase(list::const_iterator pos) {
        // the node before pos
        // if pos is at the beginning, proc points to sentinel header node
        // if pos is at the end, deleting will return iterator to end()
        list_node<T>* proc = &(*(--pos));

        proc->next_ = proc->next_->next_;
        proc->next_->prev_ = proc;

        // delete the node at pos
        delete pos;

        return iterator(proc->next_);


    }

    template<class T>
    typename list<T>::iterator list<T>::erase(list::const_iterator first, list::const_iterator last) {
        // the node before first
        list_node<T>* proc = &(*(--first));

        for(auto iter=first; iter!=last; ++iter){
            //delete the node at iter
            delete iter;
        }

        proc->next_ = &(*last);
        last->prev_ = proc;

        return iterator(last);
    }

    template<class T>
    void list<T>::push_back(const T &value) {

        list_node<T>* old_last = node_.prev_;

        old_last->next_ = new list_node(value);
        old_last->next_->prev_ = old_last;

        old_last->next_->next_ = node_;
        node_.prev_ = old_last->next_;

        ++size_;

    }

    template<class T>
    void list<T>::push_back(T &&value) {

        list_node<T>* old_last = node_.prev_;

        old_last->next_ = new list_node(std::move(value));
        old_last->next_->prev_ = old_last;

        old_last->next_->next_ = &node_;
        node_.prev_ = old_last->next_;

    }

    template<class T>
    void list<T>::pop_back() {
        // if empty list, this function has no effect
        if(size_ == 0)
            return;

        list_node<T>* old_last = node_.prev_;
        old_last->prev_->next_ = &node_;
        node_.prev_ = old_last->prev_;

        delete old_last;
        --size_;
    }

    template<class T>
    void list<T>::push_front(const T &value) {
        list_node<T>* old_front = node_.next_;

        node_.next_ = new list_node(value);
        node_.next_->prev_ = &node_;

        node_.next_->next_ = old_front;
        old_front->prev_ = &node_;

        ++size_;

    }

    template<class T>
    void list<T>::push_front(T &&value) {
        list_node<T>* old_front = node_.next_;

        node_.next_ = new list_node(std::move(value));
        node_.next_->prev_ = &node_;

        node_.next_->next_ = old_front;
        old_front->prev_ = &node_;

        ++size_;

    }

    template<class T>
    void list<T>::resize(list::size_type count) {
        if(count == 0){
            clear();
            return;
        }

        if(size_ == count)
            return;

        if(size_ < count){
            while(size_ < count)
                push_back(0);

        } else{
            // find the new last element in the short path
            list_node<T>* current = node_.next_;
            if(size_ - count > count){
                for(int i=0; i<count; ++i){
                    current = current->next_;
                }
            } else{
                // should go one more step here
                current = current->prev_;
                for(int i=0; i<size_-count; ++i){
                    current = current->prev_;
                }
            }

            // record the new last node
            list_node<T>* new_last = current->prev_;

            // delete the trimmed nodes
            while(size_ > count){
                current = current->next_;
                delete current->prev_;
                --size_;
            }
            // current should be at the sentinel header node
            assert(current == &node_ && size_ == count);

            // should adjust the header node only after deletion is done
            node_.prev_ = new_last;
            new_last->next_ = &node_;
        }

    }

    template<class T>
    void list<T>::resize(list::size_type count, const value_type &value) {
        if(count == 0){
            clear();
            return;
        }

        if(size_ == count)
            return;

        if(size_ < count){
            while(size_ < count)
                push_back(value);

        } else{
            // find the new last element in the short path
            list_node<T>* current = node_.next_;
            if(size_ - count > count){
                for(int i=0; i<count; ++i){
                    current = current->next_;
                }
            } else{
                // should go one more step here
                current = current->prev_;
                for(int i=0; i<size_-count; ++i){
                    current = current->prev_;
                }
            }

            // record the new last node
            list_node<T>* new_last = current->prev_;

            // delete the trimmed nodes
            while(size_ > count){
                current = current->next_;
                delete current->prev_;
                --size_;
            }
            // current should be at the sentinel header node
            assert(current == &node_ && size_ == count);

            // should adjust the header node only after deletion is done
            node_.prev_ = new_last;
            new_last->next_ = &node_;
        }

    }

    template<class T>
    void list<T>::swap(list &other) {
        auto tmp = std::move(node_);
        node_ = std::move(other.node_);
        other.node_ = tmp;

        std::swap(size_, other.size_);
    }

    // merge two sorted lists.
    // if two lists are not sorted, this function is undefined behaviour
    template<class T>
    void list<T>::merge(list &&other) {
        // if other and this points to the same object, this function has no effect
        if(other == *this)
            return;

        list_node<T>* current1 = node_.next_;
        list_node<T>* current2 = other.node_.next_;

        while(current1!= &node_ && current2 != &node_){
            if(current1->val_ > current2->val_){
                // record the next position of current2
                list_node<T>* new2 = current2->next_;

                //insert current2 before current1;
                current1->prev_->next_ = current2;
                current2->prev_ = current1->prev_;

                current1->prev_ = current2;
                current2->next_ = current1;

                // adjust the size of two lists
                ++size_;
                --other.size_;

                //proceed current2
                current2 = new2;
            }else{
                current1 = current1->next_;
            }

        }

        // if there'are still elements in other, move it at the end of current1;
        if(current2 != &node_){
            // now current1 points to header node
            assert(current1 == &node_);
            current1->prev_->next_ = current2;
            current2->prev_ = current1->prev_;

            node_.prev_ = other.node_.prev_;
            node_.prev_->next_ = &node_;

            //adjust the size
            size_ += other.size_;

            // make other list empty
            other.node_.next_ = &(other.node_);
            other.node_.prev_ = &(other.node_);
            other.size_ = 0;
        }


    }

    // precondition: two lists are sorted in this comparator sequence
    template<class T>
    template<class Compare>
    void list<T>::merge(list &&other, Compare comp) {

        // if other and this points to the same object, this function has no effect
        if(other == *this)
            return;

        list_node<T>* current1 = node_.next_;
        list_node<T>* current2 = other.node_.next_;

        while(current1!= &node_ && current2 != &node_){
            // compare current2 and current1 gets true (current2<current1 under std::less)
            if(comp(current2->val_,current1->val_)){
                // record the next position of current2
                list_node<T>* new2 = current2->next_;

                //insert current2 before current1;
                current1->prev_->next_ = current2;
                current2->prev_ = current1->prev_;

                current1->prev_ = current2;
                current2->next_ = current1;

                // adjust the size of two lists
                ++size_;
                --other.size_;

                //proceed current2
                current2 = new2;
            }else{
                current1 = current1->next_;
            }

        }

        // if there'are still elements in other, move it at the end of current1;
        if(current2 != &node_){
            // now current1 points to header node
            assert(current1 == &node_);
            current1->prev_->next_ = current2;
            current2->prev_ = current1->prev_;

            node_.prev_ = other.node_.prev_;
            node_.prev_->next_ = &node_;

            // adjust the size
            size_ += other.size_;

            // make other list empty
            other.node_.next_ = &(other.node_);
            other.node_.prev_ = &(other.node_);
            other.size_ = 0;
        }
    }

    template<class T>
    void list<T>::splice(list::const_iterator pos, list &&other) {
        // if other list is empty, this function has no effect
        if(other.size_ == 0)
            return;

        list_node<T>* current = &(*pos); // the node to be inserted before

        // insert the first node before pos
        current->prev_->next_ = other.node_.next_;
        other.node_.next_->prev_ = current->prev_;

        // link the last node to pos
        other.node_.prev_->next_ = current;
        current->prev_ = other.node_.prev_;

        // adjust the size
        size_ += other.size_;

        // make other list emtpy
        other.size_ = 0;
        other.node_.next_ = &(other.node_);
        other.node_.prev_ = &(other.node_);
    }

    // if it points to an element in *this, this function has undefined behaviour
    template<class T>
    void list<T>::splice(list::const_iterator pos, list &&other, list::const_iterator it) {
        list_node<T>* current = &(*pos);

        current->prev_->next_ = &(*it);
        it->prev_ = current->prev_;

        current->prev_ = &(*it);
        it->next_ = current; //note here unused

        ++size_;
        --other.size_;

    }

    // if [first,last) is in *this, this function has undefined behaviour
    template<class T>
    void list<T>::splice(list::const_iterator pos, list &&other, list::const_iterator first, list::const_iterator last) {
        list_node<T>* prev = first->prev; //record the previous node before first
        size_type old_size = size_;

        insert(pos,first,last);

        // connect the broken link in other;
        prev->next_ = &(*last);
        last->prev_ = prev;

        other.size_ -= (size_ - old_size);

    }

    template<class T>
    typename list<T>::size_type list<T>::remove(const T &value) {
        size_type old_size = size_;

        for(auto iter=begin(); iter!= end(); ++iter){
            if(iter->val_ == value){
                //remove the element at iter
                iter->prev_->nexrt_ = iter->next_;
                iter->next_->prev_ = iter->prev_;
                --size_;

                delete &(*iter);
            }
        }

        return size_-old_size;

    }

    template<class T>
    void list<T>::reverse() {
        for(auto iter= begin(); iter!= end(); ++iter){
            // record the next node;
            list_node<T>* next = iter->next_;

            // reverse the pointers between iter and iter's next
            iter->next_->prev_ = &(*iter);
            iter->prev_->next_ = iter->next;

            iter = next;
        }

        // the new tail's next is node_
        node_.next_->next_ = &(*node_);
        // the new head's prev is node_
        node_.prev_->prev_ = &(*node_);

        // exchange head and tail node
        list_node<T>* head = node_.next_;
        node_.next_ = node_.prev_;
        node_.prev_ = head;

    }

    template <class T>
    bool operator==(const list<T> &l1, const list<T> &l2) {
        if(l1.size_ == l2.size_) {
            for (auto iter1 = l1.begin(), iter2 = l2.begin(); iter1 != l1.end(); ++iter1, ++iter2){
                if(iter1->val_ != iter2->val_)
                    break;
            }
            return true;
        }
        return false;
    }

    template <class T>
    bool operator!=(const list<T> &l1, const list<T> &l2){
        return !(l1 == l2);
    }

    // lexically compare two lists
    template <class T>
    bool operator>(const list<T> &l1, const list<T> &l2){
        if(l1.size_ == l2.size_){
            for (auto iter1 = l1.begin(), iter2 = l2.begin(); iter1 != l1.end(); ++iter1, ++iter2){
                // if the current element is the same, compare the next
                if(iter1->val_ == iter2->val_)
                    continue;
                // the first different element determines the result
                return (iter1->val > iter2->val);
            }
            // if all elements are equal, return false
            return false;
        }

        // if two lists are of different length, the list with bigger length is bigger
        return (l1.size_ > l2.size_);
    }

    template <class T>
    bool operator<(const list<T> &l1, const list<T> &l2){
        if(l1.size_ == l2.size_){
            for (auto iter1 = l1.begin(), iter2 = l2.begin(); iter1 != l1.end(); ++iter1, ++iter2){
                // if the current element is the same, compare the next
                if(iter1->val_ == iter2->val_)
                    continue;
                // the first different element determines the result
                return (iter1->val < iter2->val);
            }
            // if all elements are equal, return false
            return false;
        }

        // if two lists are of different length, the list with bigger length is bigger
        return (l1.size_ < l2.size_);
    }

    template <class T>
    bool operator>=(const list<T> &l1, const list<T> &l2){
        return !(l1 < l2);
    }

    template <class T>
    bool operator<=(const list<T> &l1, const list<T> &l2){
        return !(l1 > l2);
    }


}

#endif //STLCONTAINER_LIST_HPP
