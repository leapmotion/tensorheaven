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
struct TensorPowerOfVectorSpaces_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_VECTOR_SPACE_UNIQUELY(Factor_), MUST_BE_VECTOR_SPACE), };
    typedef TensorPower_c<Factor_,ORDER_> As_TensorPower;
    typedef TensorProductOfVectorSpaces_c<typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T> As_TensorProductOfVectorSpaces;
    typedef typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T FactorTypeList;
    typedef EmbeddableInTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces_c,
                                                    TensorProductOfVectorSpaces_c<FactorTypeList> > As_EmbeddableInTensorPowerOfVectorSpaces;
public:
    typedef TypeList_t<As_TensorPower,
            TypeList_t<As_TensorProductOfVectorSpaces,
            TypeList_t<As_EmbeddableInTensorPowerOfVectorSpaces> > > ParentTypeList;

    typedef typename As_TensorProductOfVectorSpaces::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "TensorPowerOfVectorSpaces_c<" + TypeStringOf_t<Factor_>::eval() + ',' + AS_STRING(ORDER_) + '>';
    }
};

template<typename Factor_, Uint32 ORDER_>
struct IsConcept_f<TensorPowerOfVectorSpaces_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsTensorPowerOfVectorSpaces_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsTensorPowerOfVectorSpaces_f<TensorPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorPowerOfVectorSpaces);
// special convenience macros
#define IS_TENSOR_POWER_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueTensorPowerOfVectorSpacesStructure_f<Concept>::V
#define AS_TENSOR_POWER_OF_VECTOR_SPACES(Concept) UniqueTensorPowerOfVectorSpacesStructureOf_f<Concept>::T

template <typename Factor, Uint32 ORDER>
struct DualOf_f<TensorPowerOfVectorSpaces_c<Factor,ORDER> >
{
    typedef TensorPowerOfVectorSpaces_c<typename DualOf_f<Factor>::T,ORDER> T;
};


// Factor_ must be a Basis_c type
template <typename Factor_, Uint32 ORDER_>
struct TensorPowerOfBases_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASIS_UNIQUELY(Factor_), MUST_BE_BASIS) };
    typedef TensorPower_c<Factor_,ORDER_> As_TensorPower;
    typedef TensorProductOfBases_c<typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T > As_TensorPowerOfBases;
public:
    typedef TypeList_t<As_TensorPower, TypeList_t<As_TensorPowerOfBases> > ParentTypeList;

    typedef typename As_TensorPowerOfBases::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "TensorPowerOfBases_c<" + TypeStringOf_t<Factor_>::eval() + ',' + AS_STRING(ORDER_) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<TensorPowerOfBases_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsTensorPowerOfBases_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsTensorPowerOfBases_f<TensorPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorPowerOfBases);
// special convenience macros
#define IS_TENSOR_POWER_OF_BASES_UNIQUELY(Concept) HasUniqueTensorPowerOfBasesStructure_f<Concept>::V
#define AS_TENSOR_POWER_OF_BASES(Concept) UniqueTensorPowerOfBasesStructureOf_f<Concept>::T

template <typename Factor, Uint32 ORDER>
struct DualOf_f<TensorPowerOfBases_c<Factor,ORDER> >
{
    typedef TensorPowerOfBases_c<typename DualOf_f<Factor>::T,ORDER> T;
};


template <typename TensorPowerOfVectorSpaces_, typename Basis_>
struct BasedTensorPowerOfVectorSpaces_c
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IS_TENSOR_POWER_OF_VECTOR_SPACES_UNIQUELY(TensorPowerOfVectorSpaces_), MUST_BE_TENSOR_POWER_OF_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(IS_BASIS_UNIQUELY(Basis_), MUST_BE_BASIS),
    };
    typedef TensorPowerOfVectorSpaces_ As_TensorPowerOfVectorSpaces;
    typedef BasedTensorProductOfVectorSpaces_c<TensorPowerOfVectorSpaces_,Basis_> As_BasedTensorProductOfVectorSpaces;
public:
    typedef TypeList_t<As_TensorPowerOfVectorSpaces,
            TypeList_t<As_BasedTensorProductOfVectorSpaces> > ParentTypeList;

    typedef typename As_BasedTensorProductOfVectorSpaces::Id Id;

    static std::string type_as_string ()
    {
        return "BasedTensorPowerOfVectorSpaces_c<" + TypeStringOf_t<TensorPowerOfVectorSpaces_>::eval() + ','
                                                   + TypeStringOf_t<Basis_>::eval() + '>';
    }
};

template <typename TensorPowerOfVectorSpaces_, typename Basis_>
struct IsConcept_f<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces_, Basis_> >
{ static bool const V = true; };

template <typename T> struct IsBasedTensorPowerOfVectorSpaces_f { static bool const V = false; };
template <typename TensorPowerOfVectorSpaces_, typename Basis_> struct IsBasedTensorPowerOfVectorSpaces_f<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces_,Basis_> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(BasedTensorPowerOfVectorSpaces);
// special convenience macros
#define IS_BASED_TENSOR_POWER_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueBasedTensorPowerOfVectorSpacesStructure_f<Concept>::V
#define AS_BASED_TENSOR_POWER_OF_VECTOR_SPACES(Concept) UniqueBasedTensorPowerOfVectorSpacesStructureOf_f<Concept>::T

template <typename TensorPowerOfVectorSpaces, typename Basis>
struct DualOf_f<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis> >
{
    typedef BasedTensorPowerOfVectorSpaces_c<typename DualOf_f<TensorPowerOfVectorSpaces>::T,typename DualOf_f<Basis>::T> T;
};


// TODO: Consider what the ParentTypeList should actually be here.
// Factor_ must be a BasedVectorSpace_c type
template <typename Factor_, Uint32 ORDER_>
struct TensorPowerOfBasedVectorSpaces_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor_), MUST_BE_BASED_VECTOR_SPACE), };
    typedef typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T FactorTypeList;

    typedef TensorProductOfBasedVectorSpaces_c<typename UniformTypeListOfLength_t<Factor_,ORDER_>::T> As_TensorProductOfBasedVectorSpaces;
    typedef BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces_c<Factor_,ORDER_>,
                                             TensorPowerOfBases_c<typename Factor_::Basis,ORDER_> > As_BasedTensorPowerOfVectorSpaces;
    typedef EmbeddableInTensorPowerOfBasedVectorSpaces_c<TensorPowerOfBasedVectorSpaces_c,
                                                         TensorPowerOfVectorSpaces_c<Factor_,ORDER_>,
                                                         TensorProductOfBasedVectorSpaces_c<FactorTypeList>,
                                                         TensorProductOfVectorSpaces_c<FactorTypeList> > As_EmbeddableInTensorPowerOfBasedVectorSpaces;
public:
    typedef TypeList_t<As_BasedTensorPowerOfVectorSpaces,
            TypeList_t<As_TensorProductOfBasedVectorSpaces,
            TypeList_t<As_EmbeddableInTensorPowerOfBasedVectorSpaces> > > ParentTypeList;

    typedef typename As_BasedTensorPowerOfVectorSpaces::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "TensorPowerOfBasedVectorSpaces_c<" + TypeStringOf_t<Factor_>::eval() + ',' + AS_STRING(ORDER_) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<TensorPowerOfBasedVectorSpaces_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsTensorPowerOfBasedVectorSpaces_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsTensorPowerOfBasedVectorSpaces_f<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorPowerOfBasedVectorSpaces);
// special convenience macros
#define IS_TENSOR_POWER_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueTensorPowerOfBasedVectorSpacesStructure_f<Concept>::V
#define AS_TENSOR_POWER_OF_BASED_VECTOR_SPACES(Concept) UniqueTensorPowerOfBasedVectorSpacesStructureOf_f<Concept>::T

template <typename Factor, Uint32 ORDER>
struct DualOf_f<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> >
{
    typedef TensorPowerOfBasedVectorSpaces_c<typename DualOf_f<Factor>::T,ORDER> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_TENSORPOWER_HPP_
