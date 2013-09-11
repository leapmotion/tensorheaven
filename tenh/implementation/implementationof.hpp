// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/implementationof.hpp by Victor Dods, created 2013/09/08
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_IMPLEMENTATIONOF_HPP_
#define TENH_IMPLEMENTATION_IMPLEMENTATIONOF_HPP_

#include "tenh/core.hpp"

#include "tenh/memoryarray.hpp"
#include "tenh/preallocatedarray.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// forward declaration of the ImplementationOf_t template and related values
// ///////////////////////////////////////////////////////////////////////////

// for use in the USE_MEMBER_ARRAY_ template parameter for ImplementationOf_t
static bool const USE_MEMBER_ARRAY = true;        // uses MemoryArray_t for storage (default)
static bool const USE_PREALLOCATED_ARRAY = false; // uses PreallocatedArray_t for memory access

template <typename Concept_,
          typename Scalar_,
          bool USE_MEMBER_ARRAY_ = USE_MEMBER_ARRAY>
struct ImplementationOf_t;

// ///////////////////////////////////////////////////////////////////////////
// metafunction for deciding which structure to use for component access
// ///////////////////////////////////////////////////////////////////////////

// a template metafunction for figuring out which type of array to use (MemoryArray_t or PreallocatedArray_t)
template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          bool USE_MEMBER_ARRAY_,
          typename Derived_ = NullType>
struct ArrayStorage_f;

// template specialization for use of MemoryArray_t
template <typename Component_, Uint32 COMPONENT_COUNT_, typename Derived_>
struct ArrayStorage_f<Component_,COMPONENT_COUNT_,USE_MEMBER_ARRAY,Derived_>
{
    typedef MemoryArray_t<Component_,COMPONENT_COUNT_,Derived_> T;
};

template <typename Component_, Uint32 COMPONENT_COUNT_, typename Derived_>
struct ArrayStorage_f<Component_,COMPONENT_COUNT_,USE_PREALLOCATED_ARRAY,Derived_>
{
    typedef PreallocatedArray_t<Component_,COMPONENT_COUNT_,Derived_> T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_IMPLEMENTATIONOF_HPP_
