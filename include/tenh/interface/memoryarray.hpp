// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/memoryarray.hpp by Victor Dods, created 2013/09/11
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_MEMORYARRAY_HPP_
#define TENH_INTERFACE_MEMORYARRAY_HPP_

#include "tenh/core.hpp"

#include <cstring>

#include "tenh/interface/array.hpp"

namespace Tenh {

static bool const COMPONENTS_ARE_CONST = true;
static bool const COMPONENTS_ARE_NONCONST = false;

// compile-time interface for fixed-length array of a given component type,
// layed out contiguously in memory.
template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_, bool COMPONENTS_ARE_CONST_ = COMPONENTS_ARE_NONCONST>
struct MemoryArray_i
    :
    public Array_i<Derived_,Component_,COMPONENT_COUNT_,(COMPONENTS_ARE_CONST_ ? COMPONENTS_ARE_CONST_MEMORY : COMPONENTS_ARE_NONCONST_MEMORY)>
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!TypesAreEqual_f<Derived_,NullType>::V), DERIVED_MUST_NOT_BE_NULL_TYPE)
    };

    typedef Array_i<Derived_,Component_,COMPONENT_COUNT_,(COMPONENTS_ARE_CONST_ ? COMPONENTS_ARE_CONST_MEMORY : COMPONENTS_ARE_NONCONST_MEMORY)> Parent_Array_i;

    typedef typename Parent_Array_i::Derived Derived;
    typedef typename Parent_Array_i::Component Component;
    using Parent_Array_i::COMPONENT_COUNT;
    using Parent_Array_i::COMPONENT_QUALIFIER;
    typedef typename Parent_Array_i::ComponentIndex ComponentIndex;
    typedef typename Parent_Array_i::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_Array_i::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;
    typedef typename Parent_Array_i::QualifiedComponent QualifiedComponent;

    static bool const COMPONENTS_ARE_CONST = COMPONENTS_ARE_CONST_;

    // start_offset is the index in this array at which copied components will start.
    // components_to_copy is the number of components of the other array to copy into this one.
    template <typename OtherDerived_, Uint32 OTHER_COMPONENT_COUNT_, ComponentQualifier OTHER_COMPONENT_QUALIFIER_>
    void copy_from (Array_i<OtherDerived_,Component_,OTHER_COMPONENT_COUNT_,OTHER_COMPONENT_QUALIFIER_> const &a,
                    Uint32 start_offset = 0,
                    Uint32 components_to_copy = OTHER_COMPONENT_COUNT_,
                    CheckRange check_range = CheckRange::TRUE)
    {
        if (bool(check_range) && start_offset >= COMPONENT_COUNT_)
            throw std::out_of_range("start_offset is outside of range");
        if (bool(check_range) && start_offset + components_to_copy > COMPONENT_COUNT_)
            throw std::out_of_range("range to copy exceeds this array size");

        typedef Array_i<OtherDerived_,Component_,OTHER_COMPONENT_COUNT_,OTHER_COMPONENT_QUALIFIER_> OtherArray;
        // these cases have actual memory that can be copied.  NOTE: this assumes that Component_ is a POD type
        if (OTHER_COMPONENT_QUALIFIER_ == COMPONENTS_ARE_CONST_MEMORY ||
            OTHER_COMPONENT_QUALIFIER_ == COMPONENTS_ARE_NONCONST_MEMORY)
        {
            // memmove is used so that the memory segments may overlap
            memmove(reinterpret_cast<void *>(&(*this)[ComponentIndex(start_offset, CheckRange::FALSE)]),
                    reinterpret_cast<void const *>(&a[typename OtherArray::ComponentIndex(0, CheckRange::FALSE)]),
                    sizeof(Component_)*components_to_copy);
        }
        // otherwise OTHER_COMPONENT_QUALIFIER_ is COMPONENTS_ARE_PROCEDURAL, and there is no memory
        // to copy, so iterate through and use individual element access.
        else
        {
            ComponentIndex this_i(start_offset, CheckRange::FALSE);
            for (typename OtherArray::ComponentIndex other_i; this_i.value() < components_to_copy; ++this_i, ++other_i)
                (*this)[this_i] = a[other_i];
        }
    }

    using Parent_Array_i::operator[];
    using Parent_Array_i::as_derived;

    // access to the raw data (WARNING: if Component isn't a POD type, then this isn't "everything")
    Uint32 allocation_size_in_bytes () const { return COMPONENT_COUNT*sizeof(Component); }
    Component_ const *pointer_to_allocation () const { return as_derived().pointer_to_allocation(); }
    QualifiedComponent *pointer_to_allocation () { return as_derived().pointer_to_allocation(); }

    static std::string type_as_string ()
    {
        return "MemoryArray_i<" + type_string_of<Derived_>() + ','
                                + type_string_of<Component_>() + ','
                                + FORMAT(COMPONENT_COUNT_) + ','
                                + FORMAT(COMPONENTS_ARE_CONST_) + '>';
    }
};

template <typename T> struct IsMemoryArray_i { static bool const V = false; };
template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_, bool COMPONENTS_ARE_CONST_> struct IsMemoryArray_i<MemoryArray_i<Derived_,Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_> > { static bool const V = true; };

template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_, bool COMPONENTS_ARE_CONST_> struct IsArray_i<MemoryArray_i<Derived_,Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_INTERFACE_MEMORYARRAY_HPP_
