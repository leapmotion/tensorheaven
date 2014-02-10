// ///////////////////////////////////////////////////////////////////////////
// tenh/memberarray.hpp by Victor Dods, created 2013/04/07
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_MEMBERARRAY_HPP_
#define TENH_MEMBERARRAY_HPP_

#include <stdexcept>

#include "tenh/core.hpp"

#include "tenh/interface/memoryarray.hpp"

namespace Tenh {

template <Uint32 SIZE_>
struct ArraySize_f
{
    static const Uint32 V = SIZE_;
private:
    ArraySize_f();
};

#if _WIN32
// Don't shoot the messenger.
template<>
struct ArraySize_f<0>
{
    static const Uint32 V = 1;
private:
    ArraySize_f();
};
#endif

// fixed-length array of a given component type, which must be a POD type
// (the allocation_size_in_bytes and pointer_to_allocation methods require this).
template <typename Component_, Uint32 COMPONENT_COUNT_, ComponentsAreConst COMPONENTS_ARE_CONST_ = ComponentsAreConst::FALSE, typename Derived_ = NullType>
struct MemberArray_t
    :
    public MemoryArray_i<typename DerivedType_f<Derived_,MemberArray_t<Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_,Derived_> >::T,
                         Component_,
                         COMPONENT_COUNT_,
                         COMPONENTS_ARE_CONST_>
{
    typedef MemoryArray_i<typename DerivedType_f<Derived_,MemberArray_t<Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_,Derived_> >::T,
                          Component_,
                          COMPONENT_COUNT_,
                          COMPONENTS_ARE_CONST_> Parent_MemoryArray_i;

    typedef typename Parent_MemoryArray_i::Component Component;
    using Parent_MemoryArray_i::COMPONENT_COUNT;
    using Parent_MemoryArray_i::COMPONENT_QUALIFIER;
    typedef typename Parent_MemoryArray_i::ComponentIndex ComponentIndex;
    typedef typename Parent_MemoryArray_i::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_MemoryArray_i::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;
    using Parent_MemoryArray_i::COMPONENTS_ARE_CONST;
    typedef typename Parent_MemoryArray_i::QualifiedComponent QualifiedComponent;

// this is to allow 0-component arrays to work (necessary for 0-dimensional vectors)
#ifdef __clang_version__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtautological-compare"
#endif // __clang_version__

    explicit MemberArray_t (WithoutInitialization const &) { }
    template <typename T_>
    explicit MemberArray_t (FillWith_t<T_> const &fill_with)
    {
        for (Uint32 i = 0; i < COMPONENT_COUNT; ++i)
            m_component[i] = Component_(fill_with.value());
    }

#ifdef __clang_version__
#pragma GCC diagnostic pop
#endif // __clang_version__

    template <ComponentsAreConst OTHER_COMPONENTS_ARE_CONST_, typename OtherDerived_>
    MemberArray_t (MemberArray_t<Component_,COMPONENT_COUNT_,OTHER_COMPONENTS_ARE_CONST_,OtherDerived_> const &m)
    {
        memcpy(&m_component[0], m.pointer_to_allocation(), allocation_size_in_bytes());
    }
    // TODO: tuple constructor?

    ComponentAccessConstReturnType operator [] (ComponentIndex const &i) const
    {
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_component[i.value()];
    }
    ComponentAccessNonConstReturnType operator [] (ComponentIndex const &i)
    {
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_component[i.value()];
    }

    // access to the raw data
    using Parent_MemoryArray_i::allocation_size_in_bytes;
    Component_ const *pointer_to_allocation () const { return &m_component[0]; }
    QualifiedComponent *pointer_to_allocation () { return &m_component[0]; }
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
        return "MemberArray_t<" + type_string_of<Component_>() + ','
                                + FORMAT(COMPONENT_COUNT_) + ','
                                + FORMAT(COMPONENTS_ARE_CONST_) + '>';
    }

protected:

    Component m_component[ArraySize_f<COMPONENT_COUNT_>::V];

private:

    // this has no definition, and is designed to generate a compiler error if used (use the one accepting WithoutInitialization instead).
    MemberArray_t ();
};

template <typename T> struct IsMemberArray_t
{
    static bool const V = false;
private:
    IsMemberArray_t();
};
template <typename Component_, Uint32 COMPONENT_COUNT_, ComponentsAreConst COMPONENTS_ARE_CONST_, typename Derived_> struct IsMemberArray_t<MemberArray_t<Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_,Derived_> >
{
    static bool const V = true;
private:
    IsMemberArray_t();
};

template <typename Component_, Uint32 COMPONENT_COUNT_, ComponentsAreConst COMPONENTS_ARE_CONST_, typename Derived_> struct IsArray_i<MemberArray_t<Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_,Derived_> >
{
    static bool const V = true;
private:
    IsArray_i();
};
template <typename Component_, Uint32 COMPONENT_COUNT_, ComponentsAreConst COMPONENTS_ARE_CONST_, typename Derived_> struct IsMemoryArray_i<MemberArray_t<Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_,Derived_> >
{
    static bool const V = true;
private:
    IsMemoryArray_i();
};

} // end of namespace Tenh

#endif // TENH_MEMBERARRAY_HPP_
