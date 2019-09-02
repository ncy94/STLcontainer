# C++ STL container library implementation

## Overview

This library implements STL containers in efficient and understandable codes. The purpose of writing this library is to practise and study. The main knowledge used in this library is: memory management, template programming, object-oriented programming. 

The APIs for each container corresponds those in C++ standard library. The author tries his best to achieve the same robustness and efficiency as the C++ standard library, so that this library can be used in a practical project (although it's usually undesirable to use a library over the standard one). If you're also trying to figure out how standard library works, but often gets bugged by the source code, this library might be of great help to you.

## Build instructions

This is a static library using cmake. To use this library, just link it with your executable.
```cmake
target_link_libraries(${target_name} STLcontainer)

```

If you're only going to test the library, the library provides codes that do the tests. To build the test project, unquote the last two lines in `CMakeLists.txt` as instructed, and try the following lines in the top directore:
```
mkdir build
cd build
cmake ..
make
```

## Container lists

- [x] vector
- [x] list
- [x] deque
- [x] unordered_set
- [X] unordered_map
- [ ] rbtree
- [ ] set
- [ ] map

## Interfaces

The interfaces strictly follows the standard library, which can be view at [cppreference.com](https://en.cppreference.com/w/cpp/container), in the container library section. 

The APIs follow a specific convention, which will be discussed respectively below. If standard differs for a specific function, I implemented the latest (up to **C++ 17** standard). For example, the `insert` function's first parameter is a `iterator` type before **C++ 11**, however it was changed to `const_iterator` afterwards. This library only implements the latest version. (i.e., the first parameter of which is `const_iterator`).

Because the containers implements in the same way as the standard library, the iterators/references validations are the same as standard. That is, resize of `vector` invalidates iterator/references; Modifications of other containers don't generally has this setback. A detailed explanation will be presented below.

All implementations has the same complexity as standard library.

#### limitations

- Allocator template parameter and construction from initializer list are not supported.
- Strong exception guarantee strictly follows the standard. No-throw guarantees are not perfectly implemented. However, the most necessary no-throw functions such as move constructors, swap are implemented. 
- `emplace`, `emplace_back` are left unimplemented
- 'sort' functions for `list` class are left unimplemented until `ubtree` is implemented.


### constructor, assignment operator and destructor

- Default constructors. The constructors maybe overloaded by different parameters for different container types. The most common default constructor takes in a `count` parameter which specifies the container size. 
- Construct from a range specified by two InputIterator of `[first, last)`
- copy constructor, move constructor
- copy/move assignment operator. The recently implemented containers use a [^1]**copy-and-swap idiom**

### element access, iterators
- Array-based containers(`vector, deque`) provide random access functions `operator []` and `at()`.
- Sequence containers provide `front()` and `back()` functions.
- All containers provide `begin()` and `end()` functions which returns a `iterator`/`cost_iterator` to the begin or end element. 

### Capacity
- `empty()` returns boolean value indicates whether the container is empty
- `size()` returns the number of elements in the container, while `capacity()` returns the maximum elements the current storage can hold. In some containers, `max_size()` has the same effect as `capacity()`
- `reserve()` reserves a specified storage, it has no effect if the specified memory storage is less than current allocation. `shrink_to_fit()` truncates the extra allocated memory

### Modifiers
- `clear()` clears all elements, memory allocation unchanged
- `insert()` inserts at a specified `const_iterator` location. references/iterators of `vector` will be invalidated after this function.
- `emplace()` construct the element in-place. references/iterators of `vector` will be invalidated after this function. `emplace_back()` doesn't invalidate reference/iterators.
- `push_back()`,`pop_back()` doesn't invalidates reference/iterators for all functions.
- `push_front()`,`pop_front()` apply to `deque` and other node-based containers.
- `resize()` if the new size is larger than current, new memory allocated; if the new size is smaller, extra contents are truncated. 
- `swap()` swap the elements of two containers. Provides no-throw guarantee.
- `erase()` erase the element at the location specified by a `const_iterator`. Not provided for `vector`

### Non-member functions
- compare operators `>`, `<`, `>=`, `<=`. Lexically compare the elements in two containers. If two containers has the same number of elements, each element is compared in a forward order. The first un-equal element decides the result.
- `operator==()`,`operator!=()` compares whether the elements are exactly the same. For associative containers, the comparator and equal functions are also compared.
- `std::swap()` effectively use the `swap()` function

### Operations (only for list)
- `merge()` merge two sorted lists
- `splice()` moves elements from another list
- `remove()` removes an element based on value
- `reverse()` reverse the order of elements
- `unique()` removes duplicates 
- `sort()` sorts the elements

### Bucket interface (only for unordered associative containers)
- local `begin()` and `end()` functions which points to the begin and end of a bucket.
- `bucket_count()` returns the number of buckets
- `max_bucket_count()` returns the maximum number of bucket, it's effectively the length of array for buckets
- `bucket_size()` the number of elements in a bucket.

### Hash policy (only for unordered associative containers)
- `load_factor()` returns the average number of elements per bucket, that is, `size()` divided by `bucket_count()`.
- `max_load_factor()` returns the value which is set on construction. Default value is `1.0`, can be overloaded with a parameter which changes the `max_load_factor`
- `rehash()` effectively reallocates the array of **buckets**. Automatically invokes when `load_factor()` reaches the maximum. This regenerates the hash table.
- `reserve()`reserves space for at least the specified number of **elements**. This regenerates the hash table. 
- `hash_function()` returns the hash function.
- `key_eq()` returns the function used to compare keys for equality.

## References
[1] https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom



