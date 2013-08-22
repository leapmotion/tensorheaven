// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/symmetricpower.hpp by Victor Dods, created 2013/07/29
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_
#define TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_

#include "tenh/core.hpp"
#include "tenh/mathutil.hpp"

#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/tensorpower.hpp" // for TypeListWithMultiplicity_t and probably nothing else
#include "tenh/conceptual/vectorspace.hpp"

namespace Tenh {

// symmetric power of formal symbols (e.g. identifiers, builtin C++ types, etc)
template <typename Factor_, Uint32 ORDER_>
struct SymmetricPower_c
{
    typedef EmptyTypeList ParentTypeList;

    static Uint32 const ORDER = ORDER_;
    typedef Factor_ Factor;
    typedef typename TypeListWithMultiplicity_t<Factor,ORDER>::T FactorTypeList;
    typedef typename DualOf_c<SymmetricPower_c>::T Dual; // the dual is not the symmetric power of Factor::Dual

    static std::string type_as_string ()
    {
        return "SymmetricPower_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<SymmetricPower_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsSymmetricPower_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsSymmetricPower_f<SymmetricPower_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(SymmetricPower);
// special convenience macros
#define IS_SYMMETRIC_POWER_UNIQUELY(Concept) HasUniqueSymmetricPowerStructure_f<Concept>::V
#define AS_SYMMETRIC_POWER(Concept) UniqueSymmetricPowerStructureOf_f<Concept>::T


// FactorTypeList_ must be a TypeList_t of VectorSpace_c types
template <typename Factor_, Uint32 ORDER_>
struct SymmetricPowerOfVectorSpaces_c
{
private:
    typedef SymmetricPower_c<Factor_,ORDER_> As_SymmetricPower;
    typedef VectorSpace_c<typename Factor_::Field,BinomialCoefficient_t<Factor_::DIM + ORDER_ - 1, ORDER_>::V,SymmetricPower_c<typename Factor_::Id,ORDER_> > As_VectorSpace;
public:
    typedef TypeList_t<As_SymmetricPower, TypeList_t<As_VectorSpace> > ParentTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM(HasVectorSpaceStructure_f<Factor_>::V, MUST_BE_VECTOR_SPACE),
    };

    typedef typename As_SymmetricPower::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_SymmetricPower::ORDER;
    typedef typename As_VectorSpace::Field Field;
    static Uint32 const DIM = As_VectorSpace::DIM;
    typedef typename As_VectorSpace::Id Id;
    typedef typename DualOf_c<SymmetricPowerOfVectorSpaces_c>::T Dual; // the dual is not the symmetric power of Factor::Dual
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "SymmetricPowerOfVectorSpaces_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<SymmetricPowerOfVectorSpaces_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsSymmetricPowerOfVectorSpaces_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsSymmetricPowerOfVectorSpaces_f<SymmetricPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(SymmetricPowerOfVectorSpaces);
// special convenience macros
#define IS_SYMMETRIC_POWER_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueSymmetricPowerOfVectorSpacesStructure_f<Concept>::V
#define AS_SYMMETRIC_POWER_OF_VECTOR_SPACES(Concept) UniqueSymmetricPowerOfVectorSpacesStructureOf_f<Concept>::T


// Factor_ must be a Basis_c type
template <typename Factor_, Uint32 ORDER_>
struct SymmetricPowerOfBases_c
{
private:
    typedef SymmetricPower_c<Factor_,ORDER_> As_SymmetricPower;
    typedef Basis_c<SymmetricPower_c<Factor_,ORDER_> > As_Basis;
public:
    typedef TypeList_t<As_SymmetricPower, TypeList_t<As_Basis> > ParentTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM(HasBasisStructure_f<Factor_>::V, MUST_BE_BASIS)
    };

    //typedef typename As_SymmetricPower::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_SymmetricPower::ORDER;
    typedef typename As_Basis::Id Id;
    typedef typename DualOf_c<SymmetricPowerOfBases_c>::T Dual; // the dual is not the symmetric power of Factor::Dual
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "SymmetricPowerOfBases_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<SymmetricPowerOfBases_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsSymmetricPowerOfBases_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsSymmetricPowerOfBases_f<SymmetricPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(SymmetricPowerOfBases);
// special convenience macros
#define IS_SYMMETRIC_POWER_OF_BASES_UNIQUELY(Concept) HasUniqueSymmetricPowerOfBasesStructure_f<Concept>::V
#define AS_SYMMETRIC_POWER_OF_BASES(Concept) UniqueSymmetricPowerOfBasesStructureOf_f<Concept>::T


/*
// TODO: figure out if this struct is necessary
template <typename SymmetricPowerOfVectorSpaces, typename Basis_>
struct BasedSymmetricPowerOfVectorSpaces_c
{
private:
    typedef SymmetricPowerOfVectorSpaces As_SymmetricPowerOfVectorSpaces;
    typedef BasedSymmetricProductOfVectorSpaces_c<SymmetricPowerOfVectorSpaces,Basis_> As_BasedSymmetricProductOfVectorSpaces;
public:
    typedef TypeList_t<As_SymmetricPowerOfVectorSpaces, TypeList_t<As_BasedSymmetricProductOfVectorSpaces> > ParentTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM(HasSymmetricPowerOfVectorSpacesStructure_f<SymmetricPowerOfVectorSpaces>::V, MUST_BE_SYMMETRIC_POWER_OF_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(HasBasisStructure_f<Basis_>::V, MUST_BE_BASIS),
    };

    //typedef typename As_SymmetricPowerOfVectorSpaces::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_SymmetricPowerOfVectorSpaces::ORDER;
    typedef typename As_BasedSymmetricProductOfVectorSpaces::Field Field;
    static Uint32 const DIM = As_BasedSymmetricProductOfVectorSpaces::DIM;
    typedef typename As_BasedSymmetricProductOfVectorSpaces::Id Id;
    typedef typename As_BasedSymmetricProductOfVectorSpaces::Basis Basis;
    typedef typename DualOf_c<BasedSymmetricPowerOfVectorSpaces_c>::T Dual; // relies on the template specialization below

    static std::string type_as_string ()
    {
        return "BasedSymmetricPowerOfVectorSpaces_c<" + TypeStringOf_t<SymmetricPowerOfVectorSpaces>::eval() + ','
                                                      + TypeStringOf_t<Basis>::eval() + '>';
    }
};

template <typename SymmetricPowerOfVectorSpaces, typename Basis_>
struct IsConcept_f<BasedSymmetricPowerOfVectorSpaces_c<SymmetricPowerOfVectorSpaces, Basis_> >
{ static bool const V = true; };

template <typename T> struct IsBasedSymmetricPowerOfVectorSpaces_f { static bool const V = false; };
template <typename SymmetricPowerOfVectorSpaces, typename Basis> struct IsBasedSymmetricPowerOfVectorSpaces_f<BasedSymmetricPowerOfVectorSpaces_c<SymmetricPowerOfVectorSpaces,Basis> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(BasedSymmetricPowerOfVectorSpaces);
// special convenience macros
#define IS_BASED_SYMMETRIC_POWER_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueBasedSymmetricPowerOfVectorSpacesStructure_f<Concept>::V
#define AS_BASED_SYMMETRIC_POWER_OF_VECTOR_SPACES(Concept) UniqueBasedSymmetricPowerOfVectorSpacesStructureOf_f<Concept>::T

template <typename SymmetricPowerOfVectorSpaces, typename Basis>
struct DualOf_c<BasedSymmetricPowerOfVectorSpaces_c<SymmetricPowerOfVectorSpaces,Basis> >
{
    // NOTE: not really sure how to define this, because the dual of a symmetric power isn't really
    // the symmetric power of the dual (this requires a particular identification)
    typedef BasedTensorPowerOfVectorSpaces_c<typename DualOf_c<TensorPowerOfVectorSpaces>::T,typename DualOf_c<Basis>::T> T;
};
*/

// Factor_ must be a BasedVectorSpace_c type
template <typename Factor_, Uint32 ORDER_>
struct SymmetricPowerOfBasedVectorSpaces_c
{
private:
    typedef SymmetricPowerOfVectorSpaces_c<typename AS_VECTOR_SPACE(Factor_),ORDER_> As_SymmetricPowerOfVectorSpaces;
    typedef BasedVectorSpace_c<SymmetricPowerOfVectorSpaces_c<typename AS_VECTOR_SPACE(Factor_),ORDER_>,
                               SymmetricPowerOfBases_c<typename Factor_::Basis,ORDER_> > As_BasedVectorSpace;
    typedef EmbeddableInTensorProductOfBasedVectorSpaces_c<TensorPowerOfBasedVectorSpaces_c<Factor_, ORDER_> > As_EmbeddableInTensorProductOfBasedVectorSpaces;
public:
    typedef TypeList_t<As_SymmetricPowerOfVectorSpaces, TypeList_t<As_BasedVectorSpace, TypeList_t<As_EmbeddableInTensorProductOfBasedVectorSpaces> > > ParentTypeList;

    typedef typename As_EmbeddableInTensorProductOfBasedVectorSpaces::FactorTypeList FactorTypeList;

    enum { STATIC_ASSERT_IN_ENUM(AllFactorsAreBasedVectorSpaces_f<FactorTypeList>::V, ALL_FACTORS_MUST_BE_BASED_VECTOR_SPACES) };

    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList> TensorProductOfBasedVectorSpaces;
    static Uint32 const ORDER = As_SymmetricPowerOfVectorSpaces::ORDER;
    typedef typename As_BasedVectorSpace::Field Field;
    static Uint32 const DIM = As_BasedVectorSpace::DIM;
    typedef typename As_BasedVectorSpace::Id Id;
    typedef typename As_BasedVectorSpace::Basis Basis;
    typedef typename DualOf_c<SymmetricPowerOfBasedVectorSpaces_c>::T Dual; // the dual is not the symmetric power of Factor::Dual
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "SymmetricProductOfBasedVectorSpaces_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<SymmetricPowerOfBasedVectorSpaces_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsSymmetricPowerOfBasedVectorSpaces_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsSymmetricPowerOfBasedVectorSpaces_f<SymmetricPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(SymmetricPowerOfBasedVectorSpaces);
// special convenience macros
#define IS_SYMMETRIC_POWER_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueSymmetricPowerOfBasedVectorSpacesStructure_f<Concept>::V
#define AS_SYMMETRIC_POWER_OF_BASED_VECTOR_SPACES(Concept) UniqueSymmetricPowerOfBasedVectorSpacesStructureOf_f<Concept>::T

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_
