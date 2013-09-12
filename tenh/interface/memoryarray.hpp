// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/memoryarray.hpp by Victor Dods, created 2013/09/11
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_MEMORYARRAY_HPP_
#define TENH_INTERFACE_MEMORYARRAY_HPP_

#include "tenh/core.hpp"

#include "tenh/interface/array.hpp"

namespace Tenh {

// compile-time interface for fixed-length array of a given component type,
// layed out contiguously in memory.
template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_>
struct MemoryArray_i
    :
    public Array_i<Derived_,Component_,COMPONENT_COUNT_,MUTABLE_COMPONENTS>
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!Lvd::Meta::TypesAreEqual<Derived_,NullType>::v), DERIVED_MUST_NOT_BE_NULL_TYPE)
    };

    typedef Array_i<Derived_,Component_,COMPONENT_COUNT_,MUTABLE_COMPONENTS> Parent_Array_i;

    typedef typename Parent_Array_i::Derived Derived;
    typedef typename Parent_Array_i::Component Component;
    using Parent_Array_i::COMPONENT_COUNT;
    typedef typename Parent_Array_i::ComponentIndex ComponentIndex;
    using Parent_Array_i::COMPONENTS_ARE_IMMUTABLE;
    typedef typename Parent_Array_i::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_Array_i::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;

    using Parent_Array_i::operator[];
    using Parent_Array_i::as_derived;

    // access to the raw data (WARNING: if Component isn't a POD type, then this isn't "everything")
    Uint32 allocation_size_in_bytes () const { return COMPONENT_COUNT*sizeof(Component); }
    Component const *pointer_to_allocation () const { return as_derived().Derived::pointer_to_allocation(); }
    Component *pointer_to_allocation () { return as_derived().Derived::pointer_to_allocation(); }

    static std::string type_as_string ()
    {
        return "MemoryArray_i<" + TypeStringOf_t<Derived>::eval() + ','
                                + TypeStringOf_t<Component>::eval() + ','
                                + AS_STRING(COMPONENT_COUNT) + '>';
    }
};

template <typename T> struct IsMemoryArray_i { static bool const V = false; };
template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_> struct IsMemoryArray_i<MemoryArray_i<Derived_,Component_,COMPONENT_COUNT_> > { static bool const V = true; };

template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_> struct IsArray_i<MemoryArray_i<Derived_,Component_,COMPONENT_COUNT_> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_INTERFACE_MEMORYARRAY_HPP_
