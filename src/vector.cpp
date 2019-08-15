#include "vector.hpp"
#include <functional>

template <class T>
using vector = sc::regular::vector<T>;

// construct the container with given size
template <class T>
vector<T>::vector(size_type size) {
    start_ = ::operator new(size * sizeof(size_type));
    finish_ = start_;
    end_ = start_ + size;

}

// copy constructor
template <class T>
vector<T>::vector(const vector& other) {
    start_ = static_cast<pointer >(::operator new(other.max_size() * sizeof(size_type)));
    end_ = start_ + other.max_size();
    try {
        finish_ = std::uninitialized_copy(other.start_, other.finish_, finish_);
    }catch (...){
        ::operator delete(start_);
        throw;
    }

}

// move constructor
template <class T>
vector<T>::vector(sc::regular::vector<T> && other) noexcept {
    start_ = other.start_;
    other.start_ = nullptr;
    end_ = other.end_;
    other.end_ = nullptr;
    finish_ = other.finish_;
    other.finish_ = nullptr;
}

// copy assignment
template <class T>
vector<T>& vector<T>::operator=(const vector<T> & other) {
    start_ = static_cast<pointer>(::operator new(other.max_size() * sizeof(size_type)));
    end_ = start_ + other.max_size();
    try {
        finish_ = std::uninitialized_copy(other.start_, other.finish_, finish_);
    }catch (...){
        ::operator delete(start_);
        throw;
    }

}

// move assignment
template <class T>
vector<T>& vector<T>::operator=(sc::regular::vector<T> &&other) noexcept {
    start_ = other.start_;
    other.start_ = nullptr;
    end_ = other.end_;
    other.end_ = nullptr;
    finish_ = other.finish_;
    other.finish_ = nullptr;
}