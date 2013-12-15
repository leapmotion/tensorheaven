// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/implementationof.hpp by Victor Dods, created 2013/09/08
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_IMPLEMENTATIONOF_HPP_
#define TENH_IMPLEMENTATION_IMPLEMENTATIONOF_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/dual.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/immutablearray.hpp"
#include "tenh/memberarray.hpp"
#include "tenh/preallocatedarray.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// forward declaration of the ImplementationOf_t template and related values
// ///////////////////////////////////////////////////////////////////////////

// these are for the UseArrayType parameter in ImplementationOf_t

struct UseMemberArray { static std::string type_as_string () { return "UseMemberArray"; } };
template <> struct DualOf_f<UseMemberArray>
{
    typedef UseMemberArray T;
private:
    DualOf_f();
};

struct UsePreallocatedArray { static std::string type_as_string () { return "UsePreallocatedArray"; } };
template <> struct DualOf_f<UsePreallocatedArray>
{
    typedef UsePreallocatedArray T;
private:
    DualOf_f();
};

template <typename ComponentGenerator_>
struct UseImmutableArray_t
{
    enum { STATIC_ASSERT_IN_ENUM(IsComponentGenerator_t<ComponentGenerator_>::V, MUST_BE_COMPONENT_GENERATOR) };

    typedef ComponentGenerator_ ComponentGenerator;

    static std::string type_as_string ()
    {
        return "UseImmutableArray_t<" + type_string_of<ComponentGenerator_>() + '>';
    }
};

template <typename T> struct IsUseImmutableArray_f
{
    static bool const V = false;
private:
    IsUseImmutableArray_f();
};
template <typename ComponentGenerator_> struct IsUseImmutableArray_f<UseImmutableArray_t<ComponentGenerator_> >
{
    static bool const V = true;
private:
    IsUseImmutableArray_f();
};

// used by ImplementationOf_t to provide the COMPONENTS_ARE_IMMUTABLE parameter value
template <typename T> struct ComponentsAreImmutable_f
{
    static bool const V = false;
private:
    ComponentsAreImmutable_f();
};
template <typename ComponentGenerator_> struct ComponentsAreImmutable_f<UseImmutableArray_t<ComponentGenerator_> >
{
    static bool const V = true;
private:
    ComponentsAreImmutable_f();
};

// the default is UseMemberArray (internal storage).  each ImplementationOf_t must
// have a "typedef Concept_ Concept" and a "typedef UseArrayType_ UseArrayType".
template <typename Concept_,
          typename Scalar_,
          typename UseArrayType_ = UseMemberArray,
          typename Derived_ = NullType>
struct ImplementationOf_t;

template <typename T_> struct IsImplementationOf_f { static bool const V = false; };
template <typename Concept_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_>
struct IsImplementationOf_f<ImplementationOf_t<Concept_,Scalar_,UseArrayType_,Derived_> >
{
    static bool const V = true;
};

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
private:
    ArrayStorage_f();
};

template <typename Component_, Uint32 COMPONENT_COUNT_, typename Derived_>
struct ArrayStorage_f<Component_,COMPONENT_COUNT_,UsePreallocatedArray,Derived_>
{
    typedef PreallocatedArray_t<Component_,COMPONENT_COUNT_,Derived_> T;
private:
    ArrayStorage_f();
};

template <typename Component_, Uint32 COMPONENT_COUNT_, typename ComponentGenerator_, typename Derived_>
struct ArrayStorage_f<Component_,COMPONENT_COUNT_,UseImmutableArray_t<ComponentGenerator_>,Derived_>
{
    typedef ImmutableArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_> T;
private:
    ArrayStorage_f();
};

// ///////////////////////////////////////////////////////////////////////////
// helper metafunctions
// ///////////////////////////////////////////////////////////////////////////

template <typename TypeList_>
struct ConceptOfEachTypeIn_f
{
    typedef TypeList_t<typename TypeList_::HeadType::Concept,
                       typename ConceptOfEachTypeIn_f<typename TypeList_::BodyTypeList>::T> T;
private:
    ConceptOfEachTypeIn_f();
};

template <>
struct ConceptOfEachTypeIn_f<EmptyTypeList>
{
    typedef EmptyTypeList T;
private:
    ConceptOfEachTypeIn_f();
};

template <typename TypeList_>
struct ScalarOfEachTypeIn_f
{
    typedef TypeList_t<typename TypeList_::HeadType::Scalar,
                       typename ScalarOfEachTypeIn_f<typename TypeList_::BodyTypeList>::T> T;
private:
    ScalarOfEachTypeIn_f();
};

template <>
struct ScalarOfEachTypeIn_f<EmptyTypeList>
{
    typedef EmptyTypeList T;
private:
    ScalarOfEachTypeIn_f();
};

template <typename TypeList_>
struct EachTypeUsesImmutableArray_f
{
    static bool const V = IsUseImmutableArray_f<typename TypeList_::HeadType::UseArrayType>::V &&
                          EachTypeUsesImmutableArray_f<typename TypeList_::BodyTypeList>::V;
private:
    EachTypeUsesImmutableArray_f();
};

template <>
struct EachTypeUsesImmutableArray_f<EmptyTypeList>
{
    static bool const V = true; // vacuously true
private:
    EachTypeUsesImmutableArray_f();
};

template <typename TensorProductOfBasedVectorSpacesTypeList_>
struct EachTypeIsA2TensorProductOfBasedVectorSpaces_f
{
    static bool const V = IsTensorProductOfBasedVectorSpaces_f<typename TensorProductOfBasedVectorSpacesTypeList_::HeadType>::V &&
                          (FactorTypeListOf_f<typename TensorProductOfBasedVectorSpacesTypeList_::HeadType>::T::LENGTH == 2) &&
                          EachTypeIsA2TensorProductOfBasedVectorSpaces_f<typename TensorProductOfBasedVectorSpacesTypeList_::BodyTypeList>::V;
private:
    EachTypeIsA2TensorProductOfBasedVectorSpaces_f();
};

template <>
struct EachTypeIsA2TensorProductOfBasedVectorSpaces_f<EmptyTypeList>
{
    static bool const V = true; // vacuously true
private:
    EachTypeIsA2TensorProductOfBasedVectorSpaces_f();
};

template <Uint32 N_, typename TypeList_>
struct FactorNOfEachTypeIn_f
{
private:
    typedef typename FactorTypeListOf_f<typename TypeList_::HeadType>::T FactorTypeListOfHead;
    FactorNOfEachTypeIn_f();
public:
    typedef TypeList_t<typename Element_f<FactorTypeListOfHead,N_>::T,
                       typename FactorNOfEachTypeIn_f<N_,typename TypeList_::BodyTypeList>::T> T;
};

template <Uint32 N_>
struct FactorNOfEachTypeIn_f<N_,EmptyTypeList>
{
    typedef EmptyTypeList T;
private:
    FactorNOfEachTypeIn_f();
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_IMPLEMENTATIONOF_HPP_
