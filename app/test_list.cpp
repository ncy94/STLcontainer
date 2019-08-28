//
// Created by NCY on 2019-08-16.
//

#include "list.hpp"
#include <iostream>

using sc::regular::list;

template <class T>
void print(const list<T>& l){
    int i=0;
    if(l.size() == 0)
        std::cout << "empty";
    else{
        for(auto iter = l.begin(); iter!= l.end(); ++iter, ++i)
            std::cout << iter->getValue() << ' ';
    }
    assert(i == l.size());

}

template <class T>
void do_test(){
    list<T> l(1);
    l.push_back(1);

    print(l);

    list<T> l2(l);


}

int main(){
    do_test<int>();
}