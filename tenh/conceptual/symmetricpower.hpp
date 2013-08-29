// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/symmetricpower.hpp by Victor Dods, created 2013/07/29
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_
#define TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_

#include "tenh/core.hpp"
#include "tenh/mathutil.hpp"

#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/embeddableintensorpower.hpp"
#include "tenh/conceptual/tensorpower.hpp"
#include "tenh/conceptual/vectorspace.hpp"

namespace Tenh {

// symmetric power of formal symbols (e.g. identifiers, builtin C++ types, etc)
template <typename Factor_, Uint32 ORDER_>
struct SymmetricPower_c
{
    typedef EmptyTypeList ParentTypeList;

    static Uint32 const ORDER = ORDER_;
    typedef Factor_ Factor;
    typedef typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T FactorTypeList;

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


template <typename Factor_, Uint32 ORDER_>
struct SymmetricPowerOfVectorSpace_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_VECTOR_SPACE_UNIQUELY(Factor_), MUST_BE_VECTOR_SPACE), };
    typedef SymmetricPower_c<Factor_,ORDER_> As_SymmetricPower;
    typedef VectorSpace_c<typename Factor_::Field,BinomialCoefficient_t<AS_VECTOR_SPACE(Factor_)::DIMENSION + ORDER_ - 1, ORDER_>::V,SymmetricPower_c<typename Factor_::Id,ORDER_> > As_VectorSpace;
    typedef typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T FactorTypeList;
    typedef EmbeddableInTensorPowerOfVectorSpace_c<TensorPowerOfVectorSpace_c<Factor_,ORDER_>,
                                                    TensorProductOfVectorSpaces_c<FactorTypeList> > As_EmbeddableInTensorPowerOfVectorSpace;
public:
    typedef TypeList_t<As_SymmetricPower,
            TypeList_t<As_VectorSpace,
            TypeList_t<As_EmbeddableInTensorPowerOfVectorSpace> > > ParentTypeList;

    typedef typename As_VectorSpace::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "SymmetricPowerOfVectorSpace_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER_) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<SymmetricPowerOfVectorSpace_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsSymmetricPowerOfVectorSpace_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsSymmetricPowerOfVectorSpace_f<SymmetricPowerOfVectorSpace_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(SymmetricPowerOfVectorSpace);
// special convenience macros
#define IS_SYMMETRIC_POWER_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueSymmetricPowerOfVectorSpaceStructure_f<Concept>::V
#define AS_SYMMETRIC_POWER_OF_VECTOR_SPACES(Concept) UniqueSymmetricPowerOfVectorSpaceStructureOf_f<Concept>::T


// Factor_ must be a Basis_c type
template <typename Factor_, Uint32 ORDER_>
struct SymmetricPowerOfBasis_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASIS_UNIQUELY(Factor_), MUST_BE_BASIS) };
    typedef SymmetricPower_c<Factor_,ORDER_> As_SymmetricPower;
    typedef Basis_c<SymmetricPower_c<Factor_,ORDER_> > As_Basis;
public:
    typedef TypeList_t<As_SymmetricPower,
            TypeList_t<As_Basis> > ParentTypeList;

    static Uint32 const ORDER = As_SymmetricPower::ORDER;
    typedef typename As_Basis::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "SymmetricPowerOfBasis_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<SymmetricPowerOfBasis_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsSymmetricPowerOfBasis_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsSymmetricPowerOfBasis_f<SymmetricPowerOfBasis_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(SymmetricPowerOfBasis);
// special convenience macros
#define IS_SYMMETRIC_POWER_OF_BASES_UNIQUELY(Concept) HasUniqueSymmetricPowerOfBasisStructure_f<Concept>::V
#define AS_SYMMETRIC_POWER_OF_BASES(Concept) UniqueSymmetricPowerOfBasisStructureOf_f<Concept>::T


template <typename SymmetricPowerOfVectorSpace_, typename Basis_>
struct BasedSymmetricPowerOfVectorSpace_c
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IS_SYMMETRIC_POWER_OF_VECTOR_SPACES_UNIQUELY(SymmetricPowerOfVectorSpace_), MUST_BE_SYMMETRIC_POWER_OF_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(IS_BASIS_UNIQUELY(Basis_), MUST_BE_BASIS),
    };
    typedef SymmetricPowerOfVectorSpace_ As_SymmetricPowerOfVectorSpace;
    typedef BasedVectorSpace_c<SymmetricPowerOfVectorSpace_,Basis_> As_BasedVectorSpace;
public:
    typedef TypeList_t<As_SymmetricPowerOfVectorSpace,
            TypeList_t<As_BasedVectorSpace> > ParentTypeList;

    typedef typename As_BasedVectorSpace::Id Id;

    static std::string type_as_string ()
    {
        return "BasedSymmetricPowerOfVectorSpace_c<" + TypeStringOf_t<SymmetricPowerOfVectorSpace_>::eval() + ','
                                                     + TypeStringOf_t<Basis_>::eval() + '>';
    }
};

template <typename SymmetricPowerOfVectorSpace, typename Basis_>
struct IsConcept_f<BasedSymmetricPowerOfVectorSpace_c<SymmetricPowerOfVectorSpace, Basis_> >
{ static bool const V = true; };

template <typename T> struct IsBasedSymmetricPowerOfVectorSpace_f { static bool const V = false; };
template <typename SymmetricPowerOfVectorSpace, typename Basis> struct IsBasedSymmetricPowerOfVectorSpace_f<BasedSymmetricPowerOfVectorSpace_c<SymmetricPowerOfVectorSpace,Basis> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(BasedSymmetricPowerOfVectorSpace);
// special convenience macros
#define IS_BASED_SYMMETRIC_POWER_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueBasedSymmetricPowerOfVectorSpaceStructure_f<Concept>::V
#define AS_BASED_SYMMETRIC_POWER_OF_VECTOR_SPACES(Concept) UniqueBasedSymmetricPowerOfVectorSpaceStructureOf_f<Concept>::T

// TODO: thinka bout this
// template <typename SymmetricPowerOfVectorSpace, typename Basis>
// struct DualOf_f<BasedSymmetricPowerOfVectorSpace_c<SymmetricPowerOfVectorSpace,Basis> >
// {
//     typedef BasedSymmetricPowerOfVectorSpace_c<typename DualOf_f<SymmetricPowerOfVectorSpace>::T,typename DualOf_f<Basis>::T> T;
// };


// Factor_ must be a BasedVectorSpace_c type
template <typename Factor_, Uint32 ORDER_>
struct SymmetricPowerOfBasedVectorSpace_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor_), MUST_BE_BASED_VECTOR_SPACE) };
    typedef BasedSymmetricPowerOfVectorSpace_c<SymmetricPowerOfVectorSpace_c<Factor_,ORDER_>, SymmetricPowerOfBasis_c<typename AS_BASED_VECTOR_SPACE(Factor_)::Basis, ORDER_> > As_BasedSymmetricPowerOfVectorSpace;
    typedef typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T FactorTypeList;
    typedef EmbeddableInTensorPowerOfBasedVectorSpace_c<TensorPowerOfBasedVectorSpace_c<Factor_,ORDER_>,
                                                         TensorPowerOfVectorSpace_c<Factor_,ORDER_>,
                                                         TensorProductOfBasedVectorSpaces_c<FactorTypeList>,
                                                         TensorProductOfVectorSpaces_c<FactorTypeList> > As_EmbeddableInTensorPowerOfBasedVectorSpace;
public:
    typedef TypeList_t<As_BasedSymmetricPowerOfVectorSpace,
            TypeList_t<As_EmbeddableInTensorPowerOfBasedVectorSpace> > ParentTypeList;

    typedef typename As_BasedSymmetricPowerOfVectorSpace::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "SymmetricPowerOfBasedVectorSpace_c<" + TypeStringOf_t<Factor_>::eval() + ',' + AS_STRING(ORDER_) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<SymmetricPowerOfBasedVectorSpace_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsSymmetricPowerOfBasedVectorSpace_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsSymmetricPowerOfBasedVectorSpace_f<SymmetricPowerOfBasedVectorSpace_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(SymmetricPowerOfBasedVectorSpace);
// special convenience macros
#define IS_SYMMETRIC_POWER_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueSymmetricPowerOfBasedVectorSpaceStructure_f<Concept>::V
#define AS_SYMMETRIC_POWER_OF_BASED_VECTOR_SPACES(Concept) UniqueSymmetricPowerOfBasedVectorSpaceStructureOf_f<Concept>::T

template <typename Factor, Uint32 ORDER>
struct DualOf_f<SymmetricPowerOfBasedVectorSpace_c<Factor,ORDER> >
{
    typedef SymmetricPowerOfBasedVectorSpace_c<typename DualOf_f<Factor>::T,ORDER> T;
};


} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_
