// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/tensorpower.hpp by Victor Dods, created 2013/07/24
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_TENSORPOWER_HPP_
#define TENH_CONCEPTUAL_TENSORPOWER_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/embeddableintensorproduct.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/meta/typelist_utility.hpp"

namespace Tenh {

// generic tensor power of formal symbols (e.g. identifiers, builtin C++ types, etc)
template <typename Factor_, Uint32 ORDER_>
struct TensorPower_c
{
private:
    typedef TensorProduct_c<typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T> As_TensorProduct;
public:
    typedef TypeList_t<As_TensorProduct> ParentTypeList;

    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "TensorPower_c<" + TypeStringOf_t<Factor_>::eval() + ',' + AS_STRING(ORDER_) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<TensorPower_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsTensorPower_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsTensorPower_f<TensorPower_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorPower);
// special convenience macros
#define IS_TENSOR_POWER_UNIQUELY(Concept) HasUniqueTensorPowerStructure_f<Concept>::V
#define AS_TENSOR_POWER(Concept) UniqueTensorPowerStructureOf_f<Concept>::T

// the dual of a tensor power is naturally identified as the tensor power of the dual.
template <typename Factor, Uint32 ORDER>
struct DualOf_f<TensorPower_c<Factor,ORDER> >
{
    typedef TensorPower_c<typename DualOf_f<Factor>::T,ORDER> T;
};


template <typename Factor_, Uint32 ORDER_>
struct TensorPowerOfVectorSpace_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_VECTOR_SPACE_UNIQUELY(Factor_), MUST_BE_VECTOR_SPACE), };
    typedef TensorPower_c<Factor_,ORDER_> As_TensorPower;
    typedef TensorProductOfVectorSpaces_c<typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T> As_TensorProductOfVectorSpaces;
    typedef typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T FactorTypeList;
    typedef EmbeddableInTensorPowerOfVectorSpace_c<TensorPowerOfVectorSpace_c,
                                                    TensorProductOfVectorSpaces_c<FactorTypeList> > As_EmbeddableInTensorPowerOfVectorSpace;
public:
    typedef TypeList_t<As_TensorPower,
            TypeList_t<As_TensorProductOfVectorSpaces,
            TypeList_t<As_EmbeddableInTensorPowerOfVectorSpace> > > ParentTypeList;

    typedef typename As_TensorProductOfVectorSpaces::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "TensorPowerOfVectorSpace_c<" + TypeStringOf_t<Factor_>::eval() + ',' + AS_STRING(ORDER_) + '>';
    }
};

template<typename Factor_, Uint32 ORDER_>
struct IsConcept_f<TensorPowerOfVectorSpace_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsTensorPowerOfVectorSpace_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsTensorPowerOfVectorSpace_f<TensorPowerOfVectorSpace_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorPowerOfVectorSpace);
// special convenience macros
#define IS_TENSOR_POWER_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueTensorPowerOfVectorSpaceStructure_f<Concept>::V
#define AS_TENSOR_POWER_OF_VECTOR_SPACES(Concept) UniqueTensorPowerOfVectorSpaceStructureOf_f<Concept>::T

template <typename Factor, Uint32 ORDER>
struct DualOf_f<TensorPowerOfVectorSpace_c<Factor,ORDER> >
{
    typedef TensorPowerOfVectorSpace_c<typename DualOf_f<Factor>::T,ORDER> T;
};


// Factor_ must be a Basis_c type
template <typename Factor_, Uint32 ORDER_>
struct TensorPowerOfBasis_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASIS_UNIQUELY(Factor_), MUST_BE_BASIS) };
    typedef TensorPower_c<Factor_,ORDER_> As_TensorPower;
    typedef TensorProductOfBases_c<typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T > As_TensorPowerOfBasis;
public:
    typedef TypeList_t<As_TensorPower, TypeList_t<As_TensorPowerOfBasis> > ParentTypeList;

    typedef typename As_TensorPowerOfBasis::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "TensorPowerOfBasis_c<" + TypeStringOf_t<Factor_>::eval() + ',' + AS_STRING(ORDER_) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<TensorPowerOfBasis_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsTensorPowerOfBasis_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsTensorPowerOfBasis_f<TensorPowerOfBasis_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorPowerOfBasis);
// special convenience macros
#define IS_TENSOR_POWER_OF_BASES_UNIQUELY(Concept) HasUniqueTensorPowerOfBasisStructure_f<Concept>::V
#define AS_TENSOR_POWER_OF_BASES(Concept) UniqueTensorPowerOfBasisStructureOf_f<Concept>::T

template <typename Factor, Uint32 ORDER>
struct DualOf_f<TensorPowerOfBasis_c<Factor,ORDER> >
{
    typedef TensorPowerOfBasis_c<typename DualOf_f<Factor>::T,ORDER> T;
};


template <typename TensorPowerOfVectorSpace_, typename Basis_>
struct BasedTensorPowerOfVectorSpace_c
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IS_TENSOR_POWER_OF_VECTOR_SPACES_UNIQUELY(TensorPowerOfVectorSpace_), MUST_BE_TENSOR_POWER_OF_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(IS_BASIS_UNIQUELY(Basis_), MUST_BE_BASIS),
    };
    typedef TensorPowerOfVectorSpace_ As_TensorPowerOfVectorSpace;
    typedef BasedTensorProductOfVectorSpaces_c<TensorPowerOfVectorSpace_,Basis_> As_BasedTensorProductOfVectorSpaces;
public:
    typedef TypeList_t<As_TensorPowerOfVectorSpace,
            TypeList_t<As_BasedTensorProductOfVectorSpaces> > ParentTypeList;

    typedef typename As_BasedTensorProductOfVectorSpaces::Id Id;

    static std::string type_as_string ()
    {
        return "BasedTensorPowerOfVectorSpace_c<" + TypeStringOf_t<TensorPowerOfVectorSpace_>::eval() + ','
                                                   + TypeStringOf_t<Basis_>::eval() + '>';
    }
};

template <typename TensorPowerOfVectorSpace_, typename Basis_>
struct IsConcept_f<BasedTensorPowerOfVectorSpace_c<TensorPowerOfVectorSpace_, Basis_> >
{ static bool const V = true; };

template <typename T> struct IsBasedTensorPowerOfVectorSpace_f { static bool const V = false; };
template <typename TensorPowerOfVectorSpace_, typename Basis_> struct IsBasedTensorPowerOfVectorSpace_f<BasedTensorPowerOfVectorSpace_c<TensorPowerOfVectorSpace_,Basis_> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(BasedTensorPowerOfVectorSpace);
// special convenience macros
#define IS_BASED_TENSOR_POWER_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueBasedTensorPowerOfVectorSpaceStructure_f<Concept>::V
#define AS_BASED_TENSOR_POWER_OF_VECTOR_SPACES(Concept) UniqueBasedTensorPowerOfVectorSpaceStructureOf_f<Concept>::T

template <typename TensorPowerOfVectorSpace, typename Basis>
struct DualOf_f<BasedTensorPowerOfVectorSpace_c<TensorPowerOfVectorSpace,Basis> >
{
    typedef BasedTensorPowerOfVectorSpace_c<typename DualOf_f<TensorPowerOfVectorSpace>::T,typename DualOf_f<Basis>::T> T;
};


// TODO: Consider what the ParentTypeList should actually be here.
// Factor_ must be a BasedVectorSpace_c type
template <typename Factor_, Uint32 ORDER_>
struct TensorPowerOfBasedVectorSpace_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor_), MUST_BE_BASED_VECTOR_SPACE), };
    typedef typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T FactorTypeList;

    typedef TensorProductOfBasedVectorSpaces_c<typename UniformTypeListOfLength_t<Factor_,ORDER_>::T> As_TensorProductOfBasedVectorSpaces;
    typedef BasedTensorPowerOfVectorSpace_c<TensorPowerOfVectorSpace_c<Factor_,ORDER_>,
                                             TensorPowerOfBasis_c<typename Factor_::Basis,ORDER_> > As_BasedTensorPowerOfVectorSpace;
    typedef EmbeddableInTensorPowerOfBasedVectorSpace_c<TensorPowerOfBasedVectorSpace_c,
                                                         TensorPowerOfVectorSpace_c<Factor_,ORDER_>,
                                                         TensorProductOfBasedVectorSpaces_c<FactorTypeList>,
                                                         TensorProductOfVectorSpaces_c<FactorTypeList> > As_EmbeddableInTensorPowerOfBasedVectorSpace;
public:
    typedef TypeList_t<As_BasedTensorPowerOfVectorSpace,
            TypeList_t<As_TensorProductOfBasedVectorSpaces,
            TypeList_t<As_EmbeddableInTensorPowerOfBasedVectorSpace> > > ParentTypeList;

    typedef typename As_BasedTensorPowerOfVectorSpace::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "TensorPowerOfBasedVectorSpace_c<" + TypeStringOf_t<Factor_>::eval() + ',' + AS_STRING(ORDER_) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<TensorPowerOfBasedVectorSpace_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsTensorPowerOfBasedVectorSpace_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsTensorPowerOfBasedVectorSpace_f<TensorPowerOfBasedVectorSpace_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorPowerOfBasedVectorSpace);
// special convenience macros
#define IS_TENSOR_POWER_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueTensorPowerOfBasedVectorSpaceStructure_f<Concept>::V
#define AS_TENSOR_POWER_OF_BASED_VECTOR_SPACES(Concept) UniqueTensorPowerOfBasedVectorSpaceStructureOf_f<Concept>::T

template <typename Factor, Uint32 ORDER>
struct DualOf_f<TensorPowerOfBasedVectorSpace_c<Factor,ORDER> >
{
    typedef TensorPowerOfBasedVectorSpace_c<typename DualOf_f<Factor>::T,ORDER> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_TENSORPOWER_HPP_
