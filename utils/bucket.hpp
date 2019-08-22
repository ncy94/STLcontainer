//
// Created by NCY on 2019-08-22.
//

#ifndef STLCONTAINER_BUCKET_HPP
#define STLCONTAINER_BUCKET_HPP

/*
 * This class represents the bucket of unordered set
 *
 * The implementation corresponds to Microsoft Visual Studio
 * C++ standard library
 *
 * references: http://bannalia.blogspot.com/2013/10/implementation-of-c-unordered.html
 */


// forward declare friend classes
namespace sc::regular{
    template <class> class list;
}


namespace sc::utils{

    template <class T>
    class bucket{
    private:
        template <class> friend class sc::regular::list;
        sc::utils::list_node<T>* first_;
        sc::utils::list_node<T>* last_;
        unsigned long hash_;
    };

}

#endif //STLCONTAINER_SET_NODE_HPP