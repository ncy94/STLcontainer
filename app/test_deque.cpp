//
// Created by NCY on 2019-08-18.
//


#include "deque.hpp"

template <class T>
void do_test(){
    using sc::regular::deque;

    // test default constructor;
    deque<T> t(15);
}

int main(){
    do_test<int>();
}
