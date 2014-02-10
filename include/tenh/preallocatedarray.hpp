// ///////////////////////////////////////////////////////////////////////////
// tenh/preallocatedarray.hpp by Victor Dods, created 2013/08/23
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_PREALLOCATEDARRAY_HPP_
#define TENH_PREALLOCATEDARRAY_HPP_

#include <stdexcept>

#include "tenh/core.hpp"

#include "tenh/interface/array.hpp"
#include "tenh/tuple.hpp"

namespace Tenh {

// fixed-length array of a given component type, which must be a POD type
// (the allocation_size_in_bytes and pointer_to_allocation methods require this).  this
// implementation of MemoryArray_i is a "map" to preexisting memory -- it just
// puts an MemoryArray_i interface on existing memory.
template <typename Component_, Uint32 COMPONENT_COUNT_, ComponentsAreConst COMPONENTS_ARE_CONST_ = ComponentsAreConst::FALSE, typename Derived_ = NullType>
struct PreallocatedArray_t
    :
    public MemoryArray_i<typename DerivedType_f<Derived_,PreallocatedArray_t<Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_,Derived_> >::T,
                         Component_,
                         COMPONENT_COUNT_,
                         COMPONENTS_ARE_CONST_>
{
    typedef MemoryArray_i<typename DerivedType_f<Derived_,PreallocatedArray_t<Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_,Derived_> >::T,
                          Component_,
                          COMPONENT_COUNT_,
                          COMPONENTS_ARE_CONST_> Parent_MemoryArray_i;

    typedef typename Parent_MemoryArray_i::Component Component;
    using Parent_MemoryArray_i::COMPONENT_COUNT;
    using Parent_MemoryArray_i::COMPONENT_QUALIFIER;
    using Parent_MemoryArray_i::COMPONENTS_ARE_CONST;
    typedef typename Parent_MemoryArray_i::ComponentIndex ComponentIndex;
    typedef typename Parent_MemoryArray_i::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_MemoryArray_i::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;
    typedef typename Parent_MemoryArray_i::QualifiedComponent QualifiedComponent;

    explicit PreallocatedArray_t (WithoutInitialization const &) : m_pointer_to_allocation(nullptr) { }
    explicit PreallocatedArray_t (QualifiedComponent *pointer_to_allocation, CheckPointer check_pointer = CheckPointer::TRUE)
        :
        m_pointer_to_allocation(pointer_to_allocation)
    {
        if (bool(check_pointer) && m_pointer_to_allocation == nullptr)
            throw std::invalid_argument("invalid pointer_to_allocation argument (must be non-null)");
    }

// this is to allow 0-component arrays to work (necessary for 0-dimensional vectors)
#ifdef __clang_version__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtautological-compare"
#endif // __clang_version__

    template <typename T_>
    PreallocatedArray_t (FillWith_t<T_> const &fill_with,
                         QualifiedComponent *pointer_to_allocation, CheckPointer check_pointer = CheckPointer::TRUE)
        :
        m_pointer_to_allocation(pointer_to_allocation)
    {
        if (bool(check_pointer) && m_pointer_to_allocation == nullptr)
            throw std::invalid_argument("invalid pointer_to_allocation argument (must be non-null)");
        for (Uint32 i = 0; i < COMPONENT_COUNT; ++i)
            m_pointer_to_allocation[i] = Component_(fill_with.value());
    }

#ifdef __clang_version__
#pragma GCC diagnostic pop
#endif // __clang_version__

    template <typename Typle_>
    PreallocatedArray_t (Tuple_t<Typle_> const &x,
                         QualifiedComponent *pointer_to_allocation, CheckPointer check_pointer = CheckPointer::TRUE)
        :
        m_pointer_to_allocation(pointer_to_allocation)
    {
        static_assert(Length_f<Typle_>::V == COMPONENT_COUNT, "Tuple_t argument length must match that of array");
        static_assert(TypleIsUniform_f<Typle_>::V, "Tuple_t argument must have uniform Typle_ parameter");
        static_assert(TypesAreEqual_f<typename TypeOfUniformTyple_f<Typle_>::T,Component_>::V, "type of uniform Tuple_t must match Component_");
        if (bool(check_pointer) && m_pointer_to_allocation == nullptr)
            throw std::invalid_argument("invalid pointer_to_allocation argument (must be non-null)");
        memcpy(m_pointer_to_allocation, x.as_member_array().pointer_to_allocation(), allocation_size_in_bytes());
    }

    // the existence of this method is a necessary corollary of the WithoutInitialization constructor.
    void set_pointer_to_allocation (QualifiedComponent *pointer_to_allocation) { m_pointer_to_allocation = pointer_to_allocation; }

    ComponentAccessConstReturnType operator [] (ComponentIndex const &i) const
    {
        assert(m_pointer_to_allocation != nullptr && "you didn't initialize the pointer_to_allocation value");
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_pointer_to_allocation[i.value()];
    }
    ComponentAccessNonConstReturnType operator [] (ComponentIndex const &i)
    {
        assert(m_pointer_to_allocation != nullptr && "you didn't initialize the pointer_to_allocation value");
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_pointer_to_allocation[i.value()];
    }

    // access to the raw data
    using Parent_MemoryArray_i::allocation_size_in_bytes;
    Component_ const *pointer_to_allocation () const { return m_pointer_to_allocation; }
    QualifiedComponent *pointer_to_allocation () { return m_pointer_to_allocation; }
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
        return "PreallocatedArray_t<" + type_string_of<Component>() + ',' + FORMAT(COMPONENT_COUNT) + '>';
    }

protected:

    // a pointer to the preallocated memory on which this structure operates
    QualifiedComponent *m_pointer_to_allocation;

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
template <typename Component_, Uint32 COMPONENT_COUNT_, ComponentsAreConst COMPONENTS_ARE_CONST_, typename Derived_> struct IsPreallocatedArray_t<PreallocatedArray_t<Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_,Derived_> >
{
    static bool const V = true;
private:
    IsPreallocatedArray_t();
};

template <typename Component_, Uint32 COMPONENT_COUNT_, ComponentsAreConst COMPONENTS_ARE_CONST_, typename Derived_> struct IsArray_i<PreallocatedArray_t<Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_,Derived_> >
{
    static bool const V = true;
private:
    IsArray_i();
};
template <typename Component_, Uint32 COMPONENT_COUNT_, ComponentsAreConst COMPONENTS_ARE_CONST_, typename Derived_> struct IsMemoryArray_i<PreallocatedArray_t<Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_,Derived_> >
{
    static bool const V = true;
private:
    IsMemoryArray_i();
};

} // end of namespace Tenh

#endif // TENH_PREALLOCATEDARRAY_HPP_
