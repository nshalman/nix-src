#pragma once

#include <boost/container/small_vector.hpp>

#include "nix/expr/value.hh"

#ifdef __sun
// Fix for illumos: traceable_allocator<void> has private typedefs
// that Boost containers expect to be public. Provide a wrapper.
template <typename T>
struct illumos_traceable_allocator : public traceable_allocator<T> {
    using traceable_allocator<T>::traceable_allocator;
    template <class U> struct rebind { typedef illumos_traceable_allocator<U> other; };
};

template <>
struct illumos_traceable_allocator<void> {
public:
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
    typedef void        value_type;
    template <class U> struct rebind { typedef illumos_traceable_allocator<U> other; };
};
#endif

namespace nix {

/**
 * A GC compatible vector that may used a reserved portion of `nItems` on the stack instead of allocating on the heap.
 */
template <typename T, size_t nItems>
#ifdef __sun
using SmallVector = boost::container::small_vector<T, nItems, illumos_traceable_allocator<T>>;
#else
using SmallVector = boost::container::small_vector<T, nItems, traceable_allocator<T>>;
#endif

/**
 * A vector of value pointers. See `SmallVector`.
 */
template <size_t nItems>
using SmallValueVector = SmallVector<Value *, nItems>;

/**
 * A vector of values that must not be referenced after the vector is destroyed.
 *
 * See also `SmallValueVector`.
 */
template <size_t nItems>
using SmallTemporaryValueVector = SmallVector<Value, nItems>;

}
