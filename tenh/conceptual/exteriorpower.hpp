// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/exteriorpower.hpp by Victor Dods, created 2013/08/15
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_EXTERIORPOWER_HPP_
#define TENH_CONCEPTUAL_EXTERIORPOWER_HPP_

#include "tenh/core.hpp"

#include "tenh/meta/typelist_utility.hpp"
#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/embeddableintensorpower.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/mathutil.hpp"

namespace Tenh {

// exterior power of formal symbols (e.g. identifiers, builtin C++ types, etc)
template <typename Factor_, Uint32 ORDER_>
struct ExteriorPower_c
{
    typedef EmptyTypeList ParentTypeList;

    static Uint32 const ORDER = ORDER_;
    typedef Factor_ Factor;
    typedef typename TypeListWithMultiplicity_t<Factor,ORDER>::T FactorTypeList;
    typedef typename DualOf_f<ExteriorPower_c>::T Dual; // the dual is not the exterior power of Factor::Dual

    static std::string type_as_string ()
    {
        return "ExteriorPower_c<" + TypeStringOf_t<Factor_>::eval() + ',' + AS_STRING(ORDER_) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<ExteriorPower_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsExteriorPower_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsExteriorPower_f<ExteriorPower_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(ExteriorPower);
// special convenience macros
#define IS_EXTERIOR_POWER_UNIQUELY(Concept) HasUniqueExteriorPowerStructure_f<Concept>::V
#define AS_EXTERIOR_POWER(Concept) UniqueExteriorPowerStructureOf_f<Concept>::T


// FactorTypeList_ must be a TypeList_t of VectorSpace_c types
template <typename Factor_, Uint32 ORDER_>
struct ExteriorPowerOfVectorSpaces_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_VECTOR_SPACE_UNIQUELY(Factor_), MUST_BE_VECTOR_SPACE), };
    typedef ExteriorPower_c<Factor_,ORDER_> As_ExteriorPower;
    typedef VectorSpace_c<typename Factor_::Field,BinomialCoefficient_t<Factor_::DIM, ORDER_>::V,ExteriorPower_c<typename Factor_::Id,ORDER_> > As_VectorSpace;
    typedef EmbeddableInTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces_c<Factor_,ORDER_> > As_EmbeddableInTensorPowerOfVectorSpaces;
public:
    typedef TypeList_t<As_ExteriorPower,
            TypeList_t<As_VectorSpace,
            TypeList_t<As_EmbeddableInTensorPowerOfVectorSpaces> > > ParentTypeList;

    typedef typename As_ExteriorPower::FactorTypeList FactorTypeList;
    typedef typename As_VectorSpace::Field Field;
    static Uint32 const DIM = As_VectorSpace::DIM;
    typedef typename As_VectorSpace::Id Id;
    typedef typename DualOf_f<ExteriorPowerOfVectorSpaces_c>::T Dual; // the dual is not the exterior power of Factor::Dual
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "ExteriorPowerOfVectorSpaces_c<" + TypeStringOf_t<Factor_>::eval() + ',' + AS_STRING(ORDER_) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<ExteriorPowerOfVectorSpaces_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsExteriorPowerOfVectorSpaces_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsExteriorPowerOfVectorSpaces_f<ExteriorPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(ExteriorPowerOfVectorSpaces);
// special convenience macros
#define IS_EXTERIOR_POWER_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueExteriorPowerOfVectorSpacesStructure_f<Concept>::V
#define AS_EXTERIOR_POWER_OF_VECTOR_SPACES(Concept) UniqueExteriorPowerOfVectorSpacesStructureOf_f<Concept>::T


// Factor_ must be a Basis_c type
template <typename Factor_, Uint32 ORDER_>
struct ExteriorPowerOfBases_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASIS_UNIQUELY(Factor_), MUST_BE_BASIS) };
    typedef ExteriorPower_c<Factor_,ORDER_> As_ExteriorPower;
    typedef Basis_c<ExteriorPower_c<Factor_,ORDER_> > As_Basis;
public:
    typedef TypeList_t<As_ExteriorPower, TypeList_t<As_Basis> > ParentTypeList;

    typedef typename As_Basis::Id Id;
    typedef typename DualOf_f<ExteriorPowerOfBases_c>::T Dual; // the dual is not the exterior power of Factor::Dual
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "ExteriorPowerOfBases_c<" + TypeStringOf_t<Factor_>::eval() + ',' + AS_STRING(ORDER_) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<ExteriorPowerOfBases_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsExteriorPowerOfBases_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsExteriorPowerOfBases_f<ExteriorPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(ExteriorPowerOfBases);
// special convenience macros
#define IS_EXTERIOR_POWER_OF_BASES_UNIQUELY(Concept) HasUniqueExteriorPowerOfBasesStructure_f<Concept>::V
#define AS_EXTERIOR_POWER_OF_BASES(Concept) UniqueExteriorPowerOfBasesStructureOf_f<Concept>::T


template <typename ExteriorPowerOfVectorSpaces_, typename Basis_>
struct BasedExteriorPowerOfVectorSpaces_c
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IS_EXTERIOR_POWER_OF_VECTOR_SPACES_UNIQUELY(ExteriorPowerOfVectorSpaces_), MUST_BE_EXTERIOR_POWER_OF_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(IS_BASIS_UNIQUELY(Basis_), MUST_BE_BASIS),
    };
    typedef ExteriorPowerOfVectorSpaces_ As_ExteriorPowerOfVectorSpaces;
    typedef BasedVectorSpace_c<ExteriorPowerOfVectorSpaces_,Basis_> As_BasedVectorSpace;
public:
    typedef TypeList_t<As_ExteriorPowerOfVectorSpaces, TypeList_t<As_BasedVectorSpace> > ParentTypeList;

    typedef typename As_BasedVectorSpace::Field Field;
    static Uint32 const DIM = As_BasedVectorSpace::DIM;
    typedef typename As_BasedVectorSpace::Id Id;
    typedef typename As_BasedVectorSpace::Basis Basis;
    typedef typename DualOf_f<BasedExteriorPowerOfVectorSpaces_c>::T Dual; // relies on the template specialization below

    static std::string type_as_string ()
    {
        return "BasedExteriorPowerOfVectorSpaces_c<" + TypeStringOf_t<ExteriorPowerOfVectorSpaces_>::eval() + ','
                                                     + TypeStringOf_t<Basis_>::eval() + '>';
    }
};

template <typename ExteriorPowerOfVectorSpaces, typename Basis_>
struct IsConcept_f<BasedExteriorPowerOfVectorSpaces_c<ExteriorPowerOfVectorSpaces, Basis_> >
{ static bool const V = true; };

template <typename T> struct IsBasedExteriorPowerOfVectorSpaces_f { static bool const V = false; };
template <typename ExteriorPowerOfVectorSpaces, typename Basis> struct IsBasedExteriorPowerOfVectorSpaces_f<BasedExteriorPowerOfVectorSpaces_c<ExteriorPowerOfVectorSpaces,Basis> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(BasedExteriorPowerOfVectorSpaces);
// special convenience macros
#define IS_BASED_EXTERIOR_POWER_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueBasedExteriorPowerOfVectorSpacesStructure_f<Concept>::V
#define AS_BASED_EXTERIOR_POWER_OF_VECTOR_SPACES(Concept) UniqueBasedExteriorPowerOfVectorSpacesStructureOf_f<Concept>::T

// TODO: thinka bout this
// template <typename ExteriorPowerOfVectorSpaces, typename Basis>
// struct DualOf_f<BasedExteriorPowerOfVectorSpaces_c<ExteriorPowerOfVectorSpaces,Basis> >
// {
//     typedef BasedExteriorPowerOfVectorSpaces_c<typename DualOf_f<ExteriorPowerOfVectorSpaces>::T,typename DualOf_f<Basis>::T> T;
// };


// Factor_ must be a BasedVectorSpace_c type
template <typename Factor_, Uint32 ORDER_>
struct ExteriorPowerOfBasedVectorSpaces_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor_), MUST_BE_BASED_VECTOR_SPACE), };
    typedef BasedExteriorPowerOfVectorSpaces_c<ExteriorPowerOfVectorSpaces_c<Factor_,ORDER_>,ExteriorPowerOfBases_c<typename AS_BASED_VECTOR_SPACE(Factor_)::Basis, ORDER_> > As_BasedExteriorPowerOfVectorSpaces;
    typedef EmbeddableInTensorPowerOfBasedVectorSpaces_c<TensorPowerOfBasedVectorSpaces_c<Factor_,ORDER_> > As_EmbeddableInTensorPowerOfBasedVectorSpaces;
public:
    typedef TypeList_t<As_BasedExteriorPowerOfVectorSpaces,
             TypeList_t<As_EmbeddableInTensorPowerOfBasedVectorSpaces> > ParentTypeList;
    // TODO: Why the fuck is this broken?????????

    typedef typename As_EmbeddableInTensorPowerOfBasedVectorSpaces::FactorTypeList FactorTypeList;

    typedef typename As_BasedExteriorPowerOfVectorSpaces::Field Field;
    static Uint32 const DIM = As_BasedExteriorPowerOfVectorSpaces::DIM;
    typedef typename As_BasedExteriorPowerOfVectorSpaces::Id Id;
    typedef typename As_BasedExteriorPowerOfVectorSpaces::Basis Basis;
    typedef typename DualOf_f<ExteriorPowerOfBasedVectorSpaces_c>::T Dual; // relies on the template specialization below
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "ExteriorPowerOfBasedVectorSpaces_c<" + TypeStringOf_t<Factor_>::eval() + ',' + AS_STRING(ORDER_) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<ExteriorPowerOfBasedVectorSpaces_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsExteriorPowerOfBasedVectorSpaces_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsExteriorPowerOfBasedVectorSpaces_f<ExteriorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(ExteriorPowerOfBasedVectorSpaces);
// special convenience macros
#define IS_EXTERIOR_POWER_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueExteriorPowerOfBasedVectorSpacesStructure_f<Concept>::V
#define AS_EXTERIOR_POWER_OF_BASED_VECTOR_SPACES(Concept) UniqueExteriorPowerOfBasedVectorSpacesStructureOf_f<Concept>::T

template <typename Factor_, Uint32 ORDER_>
struct DualOf_f<ExteriorPowerOfBasedVectorSpaces_c<Factor_,ORDER_> >
{
    typedef BasedExteriorPowerOfVectorSpaces_c<typename DualOf_f<ExteriorPowerOfVectorSpaces_c<Factor_,ORDER_> >::T,
                                               typename DualOf_f<typename ExteriorPowerOfBasedVectorSpaces_c<Factor_,ORDER_>::Basis>::T> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_EXTERIORPOWER_HPP_
