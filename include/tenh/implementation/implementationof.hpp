// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/implementationof.hpp by Victor Dods, created 2013/09/08
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_IMPLEMENTATIONOF_HPP_
#define TENH_IMPLEMENTATION_IMPLEMENTATIONOF_HPP_

#include "tenh/core.hpp"

#include "tenh/componentqualifier.hpp"
#include "tenh/conceptual/dual.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/memberarray.hpp"
#include "tenh/preallocatedarray.hpp"
#include "tenh/proceduralarray.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// forward declaration of the ImplementationOf_t template and related values
// ///////////////////////////////////////////////////////////////////////////

// these are for the UseArrayType parameter in ImplementationOf_t

template <bool COMPONENTS_ARE_CONST_>
struct UseMemberArray_t { static std::string type_as_string () { return "UseMemberArray_t<" + FORMAT(COMPONENTS_ARE_CONST_) + '>'; } };
template <bool COMPONENTS_ARE_CONST_> struct DualOf_f<UseMemberArray_t<COMPONENTS_ARE_CONST_> >
{
    typedef UseMemberArray_t<COMPONENTS_ARE_CONST_> T;
private:
    DualOf_f();
};

template <typename T> struct IsUseMemberArray_f
{
    static bool const V = false;
private:
    IsUseMemberArray_f();
};
template <bool COMPONENTS_ARE_CONST_> struct IsUseMemberArray_f<UseMemberArray_t<COMPONENTS_ARE_CONST_> >
{
    static bool const V = true;
private:
    IsUseMemberArray_f();
};

template <bool COMPONENTS_ARE_CONST_>
struct UsePreallocatedArray_t { static std::string type_as_string () { return "UsePreallocatedArray_t<" + FORMAT(COMPONENTS_ARE_CONST_) + '>'; } };
template <bool COMPONENTS_ARE_CONST_> struct DualOf_f<UsePreallocatedArray_t<COMPONENTS_ARE_CONST_> >
{
    typedef UsePreallocatedArray_t<COMPONENTS_ARE_CONST_> T;
private:
    DualOf_f();
};

template <typename T> struct IsUsePreallocatedArray_f
{
    static bool const V = false;
private:
    IsUsePreallocatedArray_f();
};
template <bool COMPONENTS_ARE_CONST_> struct IsUsePreallocatedArray_f<UsePreallocatedArray_t<COMPONENTS_ARE_CONST_> >
{
    static bool const V = true;
private:
    IsUsePreallocatedArray_f();
};

template <typename ComponentGenerator_>
struct UseProceduralArray_t
{
    enum { STATIC_ASSERT_IN_ENUM(IsComponentGenerator_t<ComponentGenerator_>::V, MUST_BE_COMPONENT_GENERATOR) };

    typedef ComponentGenerator_ ComponentGenerator;

    static std::string type_as_string ()
    {
        return "UseProceduralArray_t<" + type_string_of<ComponentGenerator_>() + '>';
    }
};

template <typename T> struct IsUseProceduralArray_f
{
    static bool const V = false;
private:
    IsUseProceduralArray_f();
};
template <typename ComponentGenerator_> struct IsUseProceduralArray_f<UseProceduralArray_t<ComponentGenerator_> >
{
    static bool const V = true;
private:
    IsUseProceduralArray_f();
};

// the default is UseMemberArray_t (internal storage).  each ImplementationOf_t must
// have a "typedef Concept_ Concept" and a "typedef UseArrayType_ UseArrayType".
template <typename Concept_,
          typename Scalar_,
          typename UseArrayType_ = UseMemberArray_t<COMPONENTS_ARE_NONCONST>,
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

// because there will be so many template specializations of ImplementationOf_t, all
// with what would be identical type_as_string functions, just do it via metafunction once.
template <typename Concept_, typename Scalar_, typename UseArrayType_, typename Derived_>
struct TypeStringOf_t<ImplementationOf_t<Concept_,Scalar_,UseArrayType_,Derived_> >
{
    static std::string eval ()
    {
        return "ImplementationOf_t<" + type_string_of<Concept_>() + ','
                                     + type_string_of<Scalar_>() + ','
                                     + type_string_of<UseArrayType_>() + ',';
    }
};

template <typename T_> struct ComponentQualifierOfArrayType_f;

template <bool COMPONENTS_ARE_CONST_>
struct ComponentQualifierOfArrayType_f<UseMemberArray_t<COMPONENTS_ARE_CONST_> >
{
    static ComponentQualifier const V = COMPONENTS_ARE_CONST_ ? COMPONENTS_ARE_CONST_MEMORY : COMPONENTS_ARE_NONCONST_MEMORY;
};

template <bool COMPONENTS_ARE_CONST_>
struct ComponentQualifierOfArrayType_f<UsePreallocatedArray_t<COMPONENTS_ARE_CONST_> >
{
    static ComponentQualifier const V = COMPONENTS_ARE_CONST_ ? COMPONENTS_ARE_CONST_MEMORY : COMPONENTS_ARE_NONCONST_MEMORY;
};

template <typename ComponentGenerator_>
struct ComponentQualifierOfArrayType_f<UseProceduralArray_t<ComponentGenerator_> >
{
    static ComponentQualifier const V = COMPONENTS_ARE_PROCEDURAL;
};

// ///////////////////////////////////////////////////////////////////////////
// metafunction for deciding which structure to use for component access
// ///////////////////////////////////////////////////////////////////////////

// a template metafunction for figuring out which type of Array_i to use
// (one of MemberArray_t, PreallocatedArray_t, ProceduralArray_t)
template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          typename UseArrayType_,// = UseMemberArray_t<COMPONENTS_ARE_NONCONST>,
          typename Derived_ = NullType>
struct ArrayStorage_f;

// template specialization for use of MemberArray_t
template <typename Component_, Uint32 COMPONENT_COUNT_, bool COMPONENTS_ARE_CONST_, typename Derived_>
struct ArrayStorage_f<Component_,COMPONENT_COUNT_,UseMemberArray_t<COMPONENTS_ARE_CONST_>,Derived_>
{
    typedef MemberArray_t<Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_,Derived_> T;
private:
    ArrayStorage_f();
};

template <typename Component_, Uint32 COMPONENT_COUNT_, bool COMPONENTS_ARE_CONST_, typename Derived_>
struct ArrayStorage_f<Component_,COMPONENT_COUNT_,UsePreallocatedArray_t<COMPONENTS_ARE_CONST_>,Derived_>
{
    typedef PreallocatedArray_t<Component_,COMPONENT_COUNT_,COMPONENTS_ARE_CONST_,Derived_> T;
private:
    ArrayStorage_f();
};

template <typename Component_, Uint32 COMPONENT_COUNT_, typename ComponentGenerator_, typename Derived_>
struct ArrayStorage_f<Component_,COMPONENT_COUNT_,UseProceduralArray_t<ComponentGenerator_>,Derived_>
{
    typedef ProceduralArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_> T;
private:
    ArrayStorage_f();
};

// ///////////////////////////////////////////////////////////////////////////
// helper metafunctions
// ///////////////////////////////////////////////////////////////////////////

// TODO: change these to use OnEach_f
template <typename Typle_>
struct ConceptOfEachTypeIn_f
{
    typedef typename HeadBodyTyple_f<typename Head_f<Typle_>::T::Concept,
                                     typename ConceptOfEachTypeIn_f<typename BodyTyple_f<Typle_>::T>::T>::T T;
private:
    ConceptOfEachTypeIn_f();
};

template <>
struct ConceptOfEachTypeIn_f<Typle_t<>>
{
    typedef Typle_t<> T;
private:
    ConceptOfEachTypeIn_f();
};

template <typename Typle_>
struct ScalarOfEachTypeIn_f
{
    typedef typename HeadBodyTyple_f<typename Head_f<Typle_>::T::Scalar,
                                     typename ScalarOfEachTypeIn_f<typename BodyTyple_f<Typle_>::T>::T>::T T;
private:
    ScalarOfEachTypeIn_f();
};

template <>
struct ScalarOfEachTypeIn_f<Typle_t<>>
{
    typedef Typle_t<> T;
private:
    ScalarOfEachTypeIn_f();
};

template <typename Typle_>
struct EachTypeUsesProceduralArray_f
{
    static bool const V = IsUseProceduralArray_f<typename Head_f<Typle_>::T::UseArrayType>::V &&
                          EachTypeUsesProceduralArray_f<typename BodyTyple_f<Typle_>::T>::V;
private:
    EachTypeUsesProceduralArray_f();
};

template <>
struct EachTypeUsesProceduralArray_f<Typle_t<>>
{
    static bool const V = true; // vacuously true
private:
    EachTypeUsesProceduralArray_f();
};

template <typename TensorProductOfBasedVectorSpacesTyple_>
struct EachTypeIsA2TensorProductOfBasedVectorSpaces_f
{
    static bool const V = IsTensorProductOfBasedVectorSpaces_f<typename Head_f<TensorProductOfBasedVectorSpacesTyple_>::T>::V &&
                          (Length_f<typename FactorTypleOf_f<typename Head_f<TensorProductOfBasedVectorSpacesTyple_>::T>::T>::V == 2) &&
                          EachTypeIsA2TensorProductOfBasedVectorSpaces_f<typename BodyTyple_f<TensorProductOfBasedVectorSpacesTyple_>::T>::V;
private:
    EachTypeIsA2TensorProductOfBasedVectorSpaces_f();
};

template <>
struct EachTypeIsA2TensorProductOfBasedVectorSpaces_f<Typle_t<>>
{
    static bool const V = true; // vacuously true
private:
    EachTypeIsA2TensorProductOfBasedVectorSpaces_f();
};

template <Uint32 N_, typename Typle_>
struct FactorNOfEachTypeIn_f
{
private:
    typedef typename FactorTypleOf_f<typename Head_f<Typle_>::T>::T FactorTypleOfHead;
    FactorNOfEachTypeIn_f();
public:
    typedef typename HeadBodyTyple_f<typename Element_f<FactorTypleOfHead,N_>::T,
                                     typename FactorNOfEachTypeIn_f<N_,typename BodyTyple_f<Typle_>::T>::T>::T T;
};

template <Uint32 N_>
struct FactorNOfEachTypeIn_f<N_,Typle_t<>>
{
    typedef Typle_t<> T;
private:
    FactorNOfEachTypeIn_f();
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_IMPLEMENTATIONOF_HPP_
