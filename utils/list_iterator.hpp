//
// Created by NCY on 2019-08-16.
//

#ifndef STLCONTAINER_LIST_ITERATOR_HPP
#define STLCONTAINER_LIST_ITERATOR_HPP

#include "iterator_base.hpp"

namespace sc::utils{

    template <class T>
    class list_iterator : public iterator_base<T, list_iterator<T>>{

    };
}

#endif //STLCONTAINER_LIST_ITERATOR_HPP
