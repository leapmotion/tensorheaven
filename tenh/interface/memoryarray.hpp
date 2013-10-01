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
        STATIC_ASSERT_IN_ENUM((!TypesAreEqual_f<Derived_,NullType>::V), DERIVED_MUST_NOT_BE_NULL_TYPE)
    };

    typedef Array_i<Derived_,Component_,COMPONENT_COUNT_,MUTABLE_COMPONENTS> Parent_Array_i;

    typedef typename Parent_Array_i::Derived Derived;
    typedef typename Parent_Array_i::Component Component;
    using Parent_Array_i::COMPONENT_COUNT;
    typedef typename Parent_Array_i::ComponentIndex ComponentIndex;
    using Parent_Array_i::COMPONENTS_ARE_IMMUTABLE;
    typedef typename Parent_Array_i::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_Array_i::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;

    // start_offset is the index in this array at which copied components will start.
    // components_to_copy is the number of components of the other array to copy into this one.
    template <typename OtherDerived_, Uint32 OTHER_COMPONENT_COUNT_>
    void copy_from (Array_i<OtherDerived_,Component_,OTHER_COMPONENT_COUNT_,MUTABLE_COMPONENTS> const &a,
                    Uint32 start_offset = 0,
                    Uint32 components_to_copy = OTHER_COMPONENT_COUNT_,
                    bool check_range = CHECK_RANGE)
    {
        if (check_range && start_offset >= COMPONENT_COUNT_)
            throw std::out_of_range("start_offset is outside of range");
        if (check_range && start_offset + components_to_copy > COMPONENT_COUNT_)
            throw std::out_of_range("range to copy exceeds this array size");

        typedef Array_i<OtherDerived_,Component_,OTHER_COMPONENT_COUNT_,MUTABLE_COMPONENTS> OtherArray;
        // memmove is used so that the memory segments may overlap
        memmove(reinterpret_cast<void *>(&(*this)[ComponentIndex(start_offset, DONT_CHECK_RANGE)]),
                reinterpret_cast<void const *>(&a[typename OtherArray::ComponentIndex(0, DONT_CHECK_RANGE)]),
                sizeof(Component_)*components_to_copy);
    }
    // overload for immutable-component arrays (which don't correspond to actual memory,
    // so no memcpy/memmove is possible; each component has to be produced individually)
    template <typename OtherDerived_, Uint32 OTHER_COMPONENT_COUNT_>
    void copy_from (Array_i<OtherDerived_,Component_,OTHER_COMPONENT_COUNT_,IMMUTABLE_COMPONENTS> const &a,
                    Uint32 start_offset = 0,
                    Uint32 components_to_copy = OTHER_COMPONENT_COUNT_,
                    bool check_range = CHECK_RANGE)
    {
        if (check_range && start_offset >= COMPONENT_COUNT_)
            throw std::out_of_range("start_offset is outside of range");
        if (check_range && start_offset + components_to_copy > COMPONENT_COUNT_)
            throw std::out_of_range("range to copy exceeds this array size");

        typedef Array_i<OtherDerived_,Component_,OTHER_COMPONENT_COUNT_,MUTABLE_COMPONENTS> OtherArray;
        ComponentIndex this_i(start_offset, DONT_CHECK_RANGE);
        for (typename OtherArray::ComponentIndex other_i; this_i.value() < components_to_copy; ++this_i, ++other_i)
            (*this)[this_i] = a[other_i];
    }

    using Parent_Array_i::operator[];
    using Parent_Array_i::as_derived;

    // access to the raw data (WARNING: if Component isn't a POD type, then this isn't "everything")
    Uint32 allocation_size_in_bytes () const { return COMPONENT_COUNT*sizeof(Component); }
    Component const *pointer_to_allocation () const { return as_derived().Derived::pointer_to_allocation(); }
    Component *pointer_to_allocation () { return as_derived().Derived::pointer_to_allocation(); }

    static std::string type_as_string ()
    {
        return "MemoryArray_i<" + type_string_of<Derived>() + ','
                                + type_string_of<Component>() + ','
                                + AS_STRING(COMPONENT_COUNT) + '>';
    }
};

template <typename T> struct IsMemoryArray_i { static bool const V = false; };
template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_> struct IsMemoryArray_i<MemoryArray_i<Derived_,Component_,COMPONENT_COUNT_> > { static bool const V = true; };

template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_> struct IsArray_i<MemoryArray_i<Derived_,Component_,COMPONENT_COUNT_> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_INTERFACE_MEMORYARRAY_HPP_
