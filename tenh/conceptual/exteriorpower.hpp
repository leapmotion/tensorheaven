// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/exteriorpower.hpp by Victor Dods, created 2013/08/15
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_EXTERIORPOWER_HPP_
#define TENH_CONCEPTUAL_EXTERIORPOWER_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/embeddableintensorproduct.hpp"
#include "tenh/conceptual/tensorpower.hpp" // for TypeListWithMultiplicity_t and probably nothing else
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
    typedef typename DualOf_c<ExteriorPower_c>::T Dual; // the dual is not the exterior power of Factor::Dual

    static std::string type_as_string ()
    {
        return "ExteriorPower_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
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
    typedef ExteriorPower_c<Factor_,ORDER_> As_ExteriorPower;
    typedef VectorSpace_c<typename Factor_::Field,BinomialCoefficient_t<Factor_::DIM, ORDER_>::V,ExteriorPower_c<typename Factor_::Id,ORDER_> > As_VectorSpace;
public:
    typedef TypeList_t<As_ExteriorPower, TypeList_t<As_VectorSpace> > ParentTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM(HasVectorSpaceStructure_f<Factor_>::V, MUST_BE_VECTOR_SPACE),
    };

    typedef typename As_ExteriorPower::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_ExteriorPower::ORDER;
    typedef typename As_VectorSpace::Field Field;
    static Uint32 const DIM = As_VectorSpace::DIM;
    typedef typename As_VectorSpace::Id Id;
    typedef typename DualOf_c<ExteriorPowerOfVectorSpaces_c>::T Dual; // the dual is not the exterior power of Factor::Dual
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "ExteriorPowerOfVectorSpaces_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
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
    typedef ExteriorPower_c<Factor_,ORDER_> As_ExteriorPower;
    typedef Basis_c<ExteriorPower_c<Factor_,ORDER_> > As_Basis;
public:
    typedef TypeList_t<As_ExteriorPower, TypeList_t<As_Basis> > ParentTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM(HasBasisStructure_f<Factor_>::V, MUST_BE_BASIS)
    };

    //typedef typename As_ExteriorPower::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_ExteriorPower::ORDER;
    typedef typename As_Basis::Id Id;
    typedef typename DualOf_c<ExteriorPowerOfBases_c>::T Dual; // the dual is not the exterior power of Factor::Dual
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "ExteriorPowerOfBases_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
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


template <typename ExteriorPowerOfVectorSpaces, typename Basis_>
struct BasedExteriorPowerOfVectorSpaces_c
{
private:
    typedef ExteriorPowerOfVectorSpaces As_ExteriorPowerOfVectorSpaces;
    typedef BasedVectorSpace_c<ExteriorPowerOfVectorSpaces,Basis_> As_BasedVectorSpace;
//     typedef BasedExteriorProductOfVectorSpaces_c<ExteriorPowerOfVectorSpaces,Basis_> As_BasedExteriorProductOfVectorSpaces;
public:
    typedef TypeList_t<As_ExteriorPowerOfVectorSpaces, TypeList_t<As_BasedVectorSpace> > ParentTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM(HasExteriorPowerOfVectorSpacesStructure_f<ExteriorPowerOfVectorSpaces>::V, MUST_BE_EXTERIOR_POWER_OF_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(HasBasisStructure_f<Basis_>::V, MUST_BE_BASIS),
    };

    //typedef typename As_ExteriorPowerOfVectorSpaces::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_ExteriorPowerOfVectorSpaces::ORDER;
    typedef typename As_BasedVectorSpace::Field Field;
    static Uint32 const DIM = As_BasedVectorSpace::DIM;
    typedef typename As_BasedVectorSpace::Id Id;
    typedef typename As_BasedVectorSpace::Basis Basis;
    typedef typename DualOf_c<BasedExteriorPowerOfVectorSpaces_c>::T Dual; // relies on the template specialization below

    static std::string type_as_string ()
    {
        return "BasedExteriorPowerOfVectorSpaces_c<" + TypeStringOf_t<ExteriorPowerOfVectorSpaces>::eval() + ','
                                                     + TypeStringOf_t<Basis>::eval() + '>';
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

template <typename ExteriorPowerOfVectorSpaces, typename Basis>
struct DualOf_c<BasedExteriorPowerOfVectorSpaces_c<ExteriorPowerOfVectorSpaces,Basis> >
{
    typedef BasedExteriorPowerOfVectorSpaces_c<typename DualOf_c<ExteriorPowerOfVectorSpaces>::T,typename DualOf_c<Basis>::T> T;
};


// Factor_ must be a BasedVectorSpace_c type
template <typename Factor_, Uint32 ORDER_>
struct ExteriorPowerOfBasedVectorSpaces_c
{
private:
    typedef ExteriorPowerOfVectorSpaces_c<typename AS_VECTOR_SPACE(Factor_),ORDER_> As_ExteriorPowerOfVectorSpaces;
    typedef BasedVectorSpace_c<ExteriorPowerOfVectorSpaces_c<typename AS_VECTOR_SPACE(Factor_),ORDER_>,
                               ExteriorPowerOfBases_c<typename Factor_::Basis,ORDER_> > As_BasedVectorSpace;
    typedef EmbeddableInTensorProductOfBasedVectorSpaces_c<TensorPowerOfBasedVectorSpaces_c<Factor_,ORDER_> > As_EmbeddableInTensorProductOfBasedVectorSpaces;
public:
    typedef TypeList_t<As_ExteriorPowerOfVectorSpaces, TypeList_t<As_BasedVectorSpace, TypeList_t<As_EmbeddableInTensorProductOfBasedVectorSpaces> > > ParentTypeList;

    enum { STATIC_ASSERT_IN_ENUM(HasBasedVectorSpaceStructure_f<Factor_>::V, MUST_BE_BASED_VECTOR_SPACE), };

    typedef typename As_EmbeddableInTensorProductOfBasedVectorSpaces::FactorTypeList FactorTypeList;

    enum { STATIC_ASSERT_IN_ENUM(AllFactorsAreBasedVectorSpaces_f<FactorTypeList>::V, ALL_FACTORS_MUST_BE_BASED_VECTOR_SPACES) };

    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList> TensorProductOfBasedVectorSpaces;
    static Uint32 const ORDER = As_ExteriorPowerOfVectorSpaces::ORDER;
    typedef typename As_BasedVectorSpace::Field Field;
    static Uint32 const DIM = As_BasedVectorSpace::DIM;
    typedef typename As_BasedVectorSpace::Id Id;
    typedef typename As_BasedVectorSpace::Basis Basis;
    typedef typename DualOf_c<ExteriorPowerOfBasedVectorSpaces_c>::T Dual; // relies on the template specialization below
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "ExteriorProductOfBasedVectorSpaces_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
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

template <typename Factor, Uint32 ORDER>
struct DualOf_c<ExteriorPowerOfBasedVectorSpaces_c<Factor,ORDER> >
{
    typedef BasedExteriorPowerOfVectorSpaces_c<typename DualOf_c<ExteriorPowerOfVectorSpaces_c<typename AS_VECTOR_SPACE(Factor),ORDER> >::T,
                                               typename DualOf_c<typename ExteriorPowerOfBasedVectorSpaces_c<Factor,ORDER>::Basis>::T> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_EXTERIORPOWER_HPP_
