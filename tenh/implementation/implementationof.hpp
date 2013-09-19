// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/implementationof.hpp by Victor Dods, created 2013/09/08
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_IMPLEMENTATIONOF_HPP_
#define TENH_IMPLEMENTATION_IMPLEMENTATIONOF_HPP_

#include "tenh/core.hpp"

#include "tenh/immutablearray.hpp"
#include "tenh/memberarray.hpp"
#include "tenh/preallocatedarray.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// forward declaration of the ImplementationOf_t template and related values
// ///////////////////////////////////////////////////////////////////////////

// these are for the UseArrayType parameter in ImplementationOf_t

struct UseMemberArray { static std::string type_as_string () { return "UseMemberArray"; } };
template <> struct DualOf_f<UseMemberArray> { typedef UseMemberArray T; };

struct UsePreallocatedArray { static std::string type_as_string () { return "UsePreallocatedArray"; } };
template <> struct DualOf_f<UsePreallocatedArray> { typedef UsePreallocatedArray T; };

template <typename ComponentGenerator_>
struct UseImmutableArray_t
{
    enum { STATIC_ASSERT_IN_ENUM(IsComponentGenerator_t<ComponentGenerator_>::V, MUST_BE_COMPONENT_GENERATOR) };

    typedef ComponentGenerator_ ComponentGenerator;

    static std::string type_as_string ()
    {
        return "UseImmutableArray_t<" + TypeStringOf_t<ComponentGenerator_>::eval() + '>';
    }
};
// use of this will require a template specialization of DualOf_f for ComponentGenerator_
template <typename ComponentGenerator_>
struct DualOf_f<UseImmutableArray_t<ComponentGenerator_> >
{
    typedef UseImmutableArray_t<typename DualOf_f<ComponentGenerator_>::T> T;
};

template <typename T> struct IsUseImmutableArray_t { static bool const V = false; };
template <typename ComponentGenerator_> struct IsUseImmutableArray_t<UseImmutableArray_t<ComponentGenerator_> > { static bool const V = true; };

// used by ImplementationOf_t to provide the COMPONENTS_ARE_IMMUTABLE parameter value
template <typename T> struct ComponentsAreImmutable_f { static bool const V = false; };
template <typename ComponentGenerator_> struct ComponentsAreImmutable_f<UseImmutableArray_t<ComponentGenerator_> > { static bool const V = true; };

// the default is UseMemberArray (internal storage).  each ImplementationOf_t must
// have a "typedef Concept_ Concept".
template <typename Concept_,
          typename Scalar_,
          typename UseArrayType = UseMemberArray>
struct ImplementationOf_t;

// ///////////////////////////////////////////////////////////////////////////
// metafunction for deciding which structure to use for component access
// ///////////////////////////////////////////////////////////////////////////

// a template metafunction for figuring out which type of Array_i to use
// (one of MemberArray_t, PreallocatedArray_t, ImmutableArray_t)
template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          typename UseArrayType_,
          typename Derived_ = NullType>
struct ArrayStorage_f;

// template specialization for use of MemberArray_t
template <typename Component_, Uint32 COMPONENT_COUNT_, typename Derived_>
struct ArrayStorage_f<Component_,COMPONENT_COUNT_,UseMemberArray,Derived_>
{
    typedef MemberArray_t<Component_,COMPONENT_COUNT_,Derived_> T;
};

template <typename Component_, Uint32 COMPONENT_COUNT_, typename Derived_>
struct ArrayStorage_f<Component_,COMPONENT_COUNT_,UsePreallocatedArray,Derived_>
{
    typedef PreallocatedArray_t<Component_,COMPONENT_COUNT_,Derived_> T;
};

template <typename Component_, Uint32 COMPONENT_COUNT_, typename ComponentGenerator_, typename Derived_>
struct ArrayStorage_f<Component_,COMPONENT_COUNT_,UseImmutableArray_t<ComponentGenerator_>,Derived_>
{
    typedef ImmutableArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_> T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_IMPLEMENTATIONOF_HPP_
