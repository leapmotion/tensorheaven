// ///////////////////////////////////////////////////////////////////////////
// tenh/preallocatedarray.hpp by Victor Dods, created 2013/08/23
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_PREALLOCATEDARRAY_HPP_
#define TENH_PREALLOCATEDARRAY_HPP_

#include <stdexcept>

#include "tenh/core.hpp"

#include "tenh/interface/array.hpp"
#include "tenh/list.hpp"

namespace Tenh {

// fixed-length array of a given component type, which must be a POD type
// (the allocation_size_in_bytes and pointer_to_allocation methods require this).  this
// implementation of MemoryArray_i is a "map" to preexisting memory -- it just
// puts an MemoryArray_i interface on existing memory.
template <typename Component_, Uint32 COMPONENT_COUNT_, typename Derived_ = NullType>
struct PreallocatedArray_t
    :
    public MemoryArray_i<typename DerivedType_f<Derived_,PreallocatedArray_t<Component_,COMPONENT_COUNT_,Derived_> >::T,
                         Component_,
                         COMPONENT_COUNT_>
{
    typedef MemoryArray_i<typename DerivedType_f<Derived_,PreallocatedArray_t<Component_,COMPONENT_COUNT_,Derived_> >::T,
                          Component_,
                          COMPONENT_COUNT_> Parent_MemoryArray_i;

    typedef typename Parent_MemoryArray_i::Component Component;
    using Parent_MemoryArray_i::COMPONENT_COUNT;
    typedef typename Parent_MemoryArray_i::ComponentIndex ComponentIndex;

    explicit PreallocatedArray_t (WithoutInitialization const &) : m_pointer_to_allocation(NULL) { }
    explicit PreallocatedArray_t (Component *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        m_pointer_to_allocation(pointer_to_allocation)
    {
        if (check_pointer && m_pointer_to_allocation == NULL)
            throw std::invalid_argument("invalid pointer_to_allocation argument (must be non-NULL)");
    }

// this is to allow 0-component arrays to work (necessary for 0-dimensional vectors)
#ifdef __clang_version__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtautological-compare"
#endif // __clang_version__

    template <typename T_>
    PreallocatedArray_t (FillWith_t<T_> const &fill_with,
                         Component *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        m_pointer_to_allocation(pointer_to_allocation)
    {
        if (check_pointer && m_pointer_to_allocation == NULL)
            throw std::invalid_argument("invalid pointer_to_allocation argument (must be non-NULL)");
        for (Uint32 i = 0; i < COMPONENT_COUNT; ++i)
            m_pointer_to_allocation[i] = Component_(fill_with.value());
    }

#ifdef __clang_version__
#pragma GCC diagnostic pop
#endif // __clang_version__

    template <typename HeadType_, typename BodyTypeList_>
    PreallocatedArray_t (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x,
                         Component *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        m_pointer_to_allocation(pointer_to_allocation)
    {
        typedef TypeList_t<HeadType_,BodyTypeList_> TypeList;
        STATIC_ASSERT((TypeListIsUniform_t<TypeList>::V), TYPELIST_MUST_BE_UNIFORM);
        STATIC_ASSERT_TYPES_ARE_EQUAL(HeadType_,Component_);
        STATIC_ASSERT(TypeList::LENGTH == COMPONENT_COUNT, LENGTHS_MUST_BE_EQUAL);
        if (check_pointer && m_pointer_to_allocation == NULL)
            throw std::invalid_argument("invalid pointer_to_allocation argument (must be non-NULL)");
        memcpy(m_pointer_to_allocation, x.as_member_array().pointer_to_allocation(), allocation_size_in_bytes());
    }

    // the existence of this method is a necessary corollary of the WithoutInitialization constructor.
    void set_pointer_to_allocation (Component *pointer_to_allocation) { m_pointer_to_allocation = pointer_to_allocation; }

    Component const &operator [] (ComponentIndex const &i) const
    {
        assert(m_pointer_to_allocation != NULL && "you didn't initialize the pointer_to_allocation value");
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_pointer_to_allocation[i.value()];
    }
    Component &operator [] (ComponentIndex const &i)
    {
        assert(m_pointer_to_allocation != NULL && "you didn't initialize the pointer_to_allocation value");
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_pointer_to_allocation[i.value()];
    }

    // access to the raw data
    using Parent_MemoryArray_i::allocation_size_in_bytes;
    Component const *pointer_to_allocation () const { return m_pointer_to_allocation; }
    Component *pointer_to_allocation () { return m_pointer_to_allocation; }
    // this should really go in MemoryArray_i, but there were problems with casting
    // to private base classes.
    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        Uint8 const *ptr_to_alloc = reinterpret_cast<Uint8 const *>(pointer_to_allocation());
        Uint8 const *intersection_start = std::max(ptr, ptr_to_alloc);
        Uint8 const *intersection_end   = std::min(ptr+range, ptr_to_alloc+allocation_size_in_bytes());
        // return true iff the intersection range is positive
        return intersection_start < intersection_end;
    }

    static std::string type_as_string ()
    {
        return "PreallocatedArray_t<" + type_string_of<Component>() + ',' + AS_STRING(COMPONENT_COUNT) + '>';
    }

protected:

    // a pointer to the preallocated memory on which this structure operates
    Component *m_pointer_to_allocation;

private:

    // this has no definition, and is designed to generate a compiler error if used (use the one accepting WithoutInitialization instead).
    PreallocatedArray_t ();
};

template <typename T> struct IsPreallocatedArray_t
{
    static bool const V = false;
private:
    IsPreallocatedArray_t();
};
template <typename Component_, Uint32 COMPONENT_COUNT_, typename Derived_> struct IsPreallocatedArray_t<PreallocatedArray_t<Component_,COMPONENT_COUNT_,Derived_> >
{
    static bool const V = true;
private:
    IsPreallocatedArray_t();
};

template <typename Component_, Uint32 COMPONENT_COUNT_, typename Derived_> struct IsArray_i<PreallocatedArray_t<Component_,COMPONENT_COUNT_,Derived_> >
{
    static bool const V = true;
private:
    IsArray_i();
};
template <typename Component_, Uint32 COMPONENT_COUNT_, typename Derived_> struct IsMemoryArray_i<PreallocatedArray_t<Component_,COMPONENT_COUNT_,Derived_> >
{
    static bool const V = true;
private:
    IsMemoryArray_i();
};

} // end of namespace Tenh

#endif // TENH_PREALLOCATEDARRAY_HPP_
