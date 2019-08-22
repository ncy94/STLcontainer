//
// Created by NCY on 2019-08-18.
//


#include "deque.hpp"
#include <iostream>

template <class T>
void print(const sc::regular::deque<T>& dq){
    for(typename sc::regular::deque<T>::const_iterator d = dq.begin(); d!= dq.end(); ++d){
    //for(auto d: dq){
        std::cout << *d <<' ';
    }
}

template <class T>
void do_test(){
    using sc::regular::deque;
    using iterator = typename sc::regular::deque<T>::iterator ;
    using const_iterator = typename sc::regular::deque<T>::const_iterator ;

    {
        // test default constructor;
        deque<T> t(15);
        for (int i = 0; i < 10; ++i)
            t.push_back(i);
        for (int i = 0; i < 10; ++i)
            t.push_front(i);

        //test copy/move
        deque<T> t2(t);
        deque<T> t3(std::move(t2));
        deque<T> t4 = t3;
        deque<T> t5 = std::move(t4);
        //print(t5);
    }

    {
        deque<T> t(5);
        int a[] ={1,2,3,4,5,6,7,8};
        t.assign(std::begin(a), std::end(a));
        t.clear();
        //print(t);
    }

    {
        deque<T> t(1);
        for(int i=0; i<99; i++)
            t.push_back(i);
        for(int i=0; i<90; i++)
            t.pop_front();
        print(t);


    }


}

int main(){
    do_test<int>();
}
