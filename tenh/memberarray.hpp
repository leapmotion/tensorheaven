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
};

#if _WIN32
template<>
struct ArraySize_f<0>
{
	static const Uint32 V = 1;
};
#endif

// fixed-length array of a given component type, which must be a POD type
// (the allocation_size_in_bytes and pointer_to_allocation methods require this).
template <typename Component_, Uint32 COMPONENT_COUNT_, typename Derived_ = NullType>
struct MemberArray_t
    :
    public MemoryArray_i<typename If<(TypesAreEqual_f<Derived_,NullType>::V),
                                     MemberArray_t<Component_,COMPONENT_COUNT_,Derived_>,
                                     Derived_>::T,
                         Component_,
                         COMPONENT_COUNT_>
{
    typedef MemoryArray_i<typename If<(TypesAreEqual_f<Derived_,NullType>::V),
                                      MemberArray_t<Component_,COMPONENT_COUNT_,Derived_>,
                                      Derived_>::T,
                          Component_,
                          COMPONENT_COUNT_> Parent_MemoryArray_i;

    typedef typename Parent_MemoryArray_i::Component Component;
    using Parent_MemoryArray_i::COMPONENT_COUNT;
    typedef typename Parent_MemoryArray_i::ComponentIndex ComponentIndex;

// this is to allow 0-component arrays to work (necessary for 0-dimensional vectors)
#ifdef __clang_version__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtautological-compare"
#endif // __clang_version__

    explicit MemberArray_t (WithoutInitialization const &) { }
    explicit MemberArray_t (Component const &fill_with) { for (Uint32 i = 0; i < COMPONENT_COUNT; ++i) m_component[i] = fill_with; }

#ifdef __clang_version__
#pragma GCC diagnostic pop
#endif // __clang_version__

    MemberArray_t (Component const &x0, Component const &x1) { STATIC_ASSERT((COMPONENT_COUNT == 2),COMPONENT_COUNT_DOES_NOT_MATCH_ARGUMENT_COUNT); m_component[0] = x0; m_component[1] = x1; }
    MemberArray_t (Component const &x0, Component const &x1, Component const &x2) { STATIC_ASSERT((COMPONENT_COUNT == 3),COMPONENT_COUNT_DOES_NOT_MATCH_ARGUMENT_COUNT); m_component[0] = x0; m_component[1] = x1; m_component[2] = x2; }
    MemberArray_t (Component const &x0, Component const &x1, Component const &x2, Component const &x3) { STATIC_ASSERT((COMPONENT_COUNT == 4),COMPONENT_COUNT_DOES_NOT_MATCH_ARGUMENT_COUNT); m_component[0] = x0; m_component[1] = x1; m_component[2] = x2; m_component[3] = x3; }

    Component const &operator [] (ComponentIndex const &i) const
    {
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_component[i.value()];
    }
    Component &operator [] (ComponentIndex const &i)
    {
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_component[i.value()];
    }

    // access to the raw data
    using Parent_MemoryArray_i::allocation_size_in_bytes;
    Component const *pointer_to_allocation () const { return &m_component[0]; }
    Component *pointer_to_allocation () { return &m_component[0]; }

    static std::string type_as_string ()
    {
        return "MemberArray_t<" + type_string_of<Component>() + ',' + AS_STRING(COMPONENT_COUNT) + '>';
    }

protected:

    Component m_component[ArraySize_f<COMPONENT_COUNT>::V];

private:

    // this has no definition, and is designed to generate a compiler error if used (use the one accepting WithoutInitialization instead).
    MemberArray_t ();
};

template <typename T> struct IsMemberArray_t { static bool const V = false; };
template <typename Component_, Uint32 COMPONENT_COUNT_, typename Derived_> struct IsMemberArray_t<MemberArray_t<Component_,COMPONENT_COUNT_,Derived_> > { static bool const V = true; };

template <typename Component_, Uint32 COMPONENT_COUNT_, typename Derived_> struct IsArray_i<MemberArray_t<Component_,COMPONENT_COUNT_,Derived_> > { static bool const V = true; };
template <typename Component_, Uint32 COMPONENT_COUNT_, typename Derived_> struct IsMemoryArray_i<MemberArray_t<Component_,COMPONENT_COUNT_,Derived_> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_MEMBERARRAY_HPP_
