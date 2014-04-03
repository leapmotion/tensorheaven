// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/tensorproduct.hpp by Victor Dods, created 2013/07/22
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_TENSORPRODUCT_HPP_
#define TENH_CONCEPTUAL_TENSORPRODUCT_HPP_

#include "tenh/core.hpp"

#include <type_traits>

#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/dual.hpp"
#include "tenh/conceptual/embeddableintensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/meta/typle.hpp"
#include "tenh/meta/typle_utility.hpp"

namespace Tenh {

inline std::string asterisk_separated_terse_string_of (Typle_t<> const &)
{
    return "";
}

template <typename Head_, typename... BodyTypes_>
std::string asterisk_separated_terse_string_of (Typle_t<Head_,BodyTypes_...> const &)
{
    typedef Typle_t<BodyTypes_...> BodyTyple;
    std::string body_string(asterisk_separated_terse_string_of(BodyTyple()));
    return terse_string_of<Head_>() + (body_string.empty() ? "" : '*' + body_string);
}

template <typename Head_, typename... BodyTypes_>
typename std::enable_if<!TypleIsUniform_f<Typle_t<Head_,BodyTypes_...>>::V ||
                        TypesAreEqual_f<Typle_t<BodyTypes_...>,Typle_t<>>::V,
                        std::string>::type terse_string_of_tensor_product (Typle_t<Head_,BodyTypes_...> const &)
{
    return '(' + asterisk_separated_terse_string_of(Typle_t<Head_,BodyTypes_...>()) + ')';
}

template <typename Head_, typename... BodyTypes_>
typename std::enable_if<TypleIsUniform_f<Typle_t<Head_,BodyTypes_...>>::V &&
                        !TypesAreEqual_f<Typle_t<BodyTypes_...>,Typle_t<>>::V,
                        std::string>::type terse_string_of_tensor_product (Typle_t<Head_,BodyTypes_...> const &)
{
    return "tensor<" + FORMAT((Length_f<Typle_t<Head_,BodyTypes_...>>::V)) + '>' + terse_string_of<Head_>();
}

// ///////////////////////////////////////////////////////////////////////////
// TensorProduct_c
// ///////////////////////////////////////////////////////////////////////////

// generic tensor product of formal symbols (e.g. identifiers, builtin C++ types, etc)
template <typename FactorTyple_>
struct TensorProduct_c
{
    static_assert(IsTyple_f<FactorTyple_>::V, "FactorTyple_ must be a Typle_t");

    typedef Typle_t<> ParentTyple;

    typedef FactorTyple_ FactorTyple;
    static Uint32 const ORDER = Length_f<FactorTyple>::V;

    static std::string type_as_string (bool verbose)
    {
        if (verbose)
            return "TensorProduct_c<" + type_string_of<FactorTyple_>() + '>';
        else
            return terse_string_of_tensor_product(FactorTyple_());
    }
};

template <typename FactorTyple_>
struct IsConcept_f<TensorProduct_c<FactorTyple_>>
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsTensorProduct_f
{
    static bool const V = false;
private:
    IsTensorProduct_f();
};
template <typename FactorTyple_> struct IsTensorProduct_f<TensorProduct_c<FactorTyple_>>
{
    static bool const V = true;
private:
    IsTensorProduct_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorProduct);
// special convenience macros
#define IS_TENSOR_PRODUCT_UNIQUELY(Concept) HasUniqueTensorProductStructure_f<Concept>::V
#define AS_TENSOR_PRODUCT(Concept) UniqueTensorProductStructureOf_f<Concept>::T

template <typename FactorTyple_>
struct DualOf_f<TensorProduct_c<FactorTyple_>>
{
    typedef TensorProduct_c<typename DualOf_f<FactorTyple_>::T> T;
private:
    DualOf_f();
};

// property IDs

struct Order; // could this be a forward declaration? Looks like yes.
struct FactorTyple; // could this be a forward declaration? Looks like yes.
struct TensorPowerFactor; // could this be a forward declaration? Looks like yes.

// BaseProperty_f accessors

template <typename FactorTyple_>
struct BaseProperty_f<TensorProduct_c<FactorTyple_>,Order>
{
    typedef Value_t<Uint32,Length_f<FactorTyple_>::V> T;
private:
    BaseProperty_f();
};

template <typename FactorTyple_>
struct BaseProperty_f<TensorProduct_c<FactorTyple_>,FactorTyple>
{
    typedef FactorTyple_ T;
private:
    BaseProperty_f();
};

// this function is only well-defined if FactorTyple_ is uniform and has at least one element
template <typename FactorTyple_>
struct BaseProperty_f<TensorProduct_c<FactorTyple_>,TensorPowerFactor>
{
private:
    static bool const THERE_IS_A_UNIQUE_FACTOR = Length_f<FactorTyple_>::V >= 1 &&
                                                 TypleIsUniform_f<FactorTyple_>::V;
    BaseProperty_f();
public:
    typedef typename If_f<THERE_IS_A_UNIQUE_FACTOR,
                          typename Head_f<FactorTyple_>::T,
                          NullValue>::T T;
};

// named property accessors

template <typename Concept_> struct OrderOf_f
{
    static Uint32 const V = PropertyValue_f<Concept_,Order>::V;
private:
    OrderOf_f();
};
template <typename Concept_> struct FactorTypleOf_f
{
    typedef typename Property_f<Concept_,FactorTyple>::T T;
private:
    FactorTypleOf_f();
};
template <typename Concept_> struct TensorPowerFactorOf_f
{
    typedef typename Property_f<Concept_,TensorPowerFactor>::T T;
private:
    TensorPowerFactorOf_f();
};

// ///////////////////////////////////////////////////////////////////////////
// TensorProductOfVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////


template <typename FactorTyple_>
struct TensorProductOfVectorSpaces_c
{
    static_assert(IsTyple_f<FactorTyple_>::V, "FactorTyple_ must be a Typle_t");
    static_assert(AllTypesHaveUniqueVectorSpaceStructures_f<FactorTyple_>::V, "all factors must have unique vector space structures");
    static_assert(AllTypesHaveSameScalarField_f<FactorTyple_>::V, "all factor vector spaces must have the same scalar field");
private:
    typedef TensorProduct_c<FactorTyple_> As_TensorProduct;
    typedef VectorSpace_c<typename ScalarFieldOf_f<typename Head_f<FactorTyple_>::T>::T,
                          ProductOfDimensions_f<FactorTyple_>::V,
                          TensorProduct_c<typename IdsOfTyple_f<FactorTyple_>::T>> As_VectorSpace;
    typedef EmbeddableInTensorProductOfVectorSpaces_c<TensorProductOfVectorSpaces_c> As_EmbeddableInTensorProductOfVectorSpaces;
public:
    typedef Typle_t<As_TensorProduct,As_VectorSpace,As_EmbeddableInTensorProductOfVectorSpaces> ParentTyple;

    typedef typename As_VectorSpace::Id Id;

    static std::string type_as_string (bool verbose)
    {
        if (verbose)
            return "TensorProductOfVectorSpaces_c<" + type_string_of<FactorTyple_>() + '>';
        else
            return terse_string_of_tensor_product(FactorTyple_());
    }
};

template <typename FactorTyple_>
struct IsConcept_f<TensorProductOfVectorSpaces_c<FactorTyple_>>
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsTensorProductOfVectorSpaces_f
{
    static bool const V = false;
private:
    IsTensorProductOfVectorSpaces_f();
};
template <typename FactorTyple_> struct IsTensorProductOfVectorSpaces_f<TensorProductOfVectorSpaces_c<FactorTyple_>>
{
    static bool const V = true;
private:
    IsTensorProductOfVectorSpaces_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorProductOfVectorSpaces);
// special convenience macros
#define IS_TENSOR_PRODUCT_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueTensorProductOfVectorSpacesStructure_f<Concept>::V
#define AS_TENSOR_PRODUCT_OF_VECTOR_SPACES(Concept) UniqueTensorProductOfVectorSpacesStructureOf_f<Concept>::T

template <typename FactorTyple_>
struct DualOf_f<TensorProductOfVectorSpaces_c<FactorTyple_>>
{
    typedef TensorProductOfVectorSpaces_c<typename DualOf_f<FactorTyple_>::T> T;
private:
    DualOf_f();
};

// ///////////////////////////////////////////////////////////////////////////
// TensorProductOfBases_c
// ///////////////////////////////////////////////////////////////////////////

template <typename FactorTyple_>
struct TensorProductOfBases_c
{
    static_assert(IsTyple_f<FactorTyple_>::V, "FactorTyple_ must be a Typle_t");
    static_assert(AllTypesHaveBasisStructures_f<FactorTyple_>::V, "all factors must be bases");
    static_assert(AllTypesHaveUniqueBasisStructures_f<FactorTyple_>::V, "all factors must have unique basis structures");
private:
    typedef TensorProduct_c<FactorTyple_> As_TensorProduct;
    typedef Basis_c<TensorProduct_c<typename IdsOfTyple_f<FactorTyple_>::T>> As_Basis;
public:
    typedef Typle_t<As_TensorProduct,As_Basis> ParentTyple;

    typedef typename As_Basis::Id Id;

    static std::string type_as_string (bool verbose)
    {
        if (verbose)
            return "TensorProductOfBases_c<" + type_string_of<FactorTyple_>() + '>';
        else
            return terse_string_of_tensor_product(FactorTyple_());
    }
};

template <typename FactorTyple_>
struct IsConcept_f<TensorProductOfBases_c<FactorTyple_>>
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsTensorProductOfBases_f
{
    static bool const V = false;
private:
    IsTensorProductOfBases_f();
};
template <typename FactorTyple_> struct IsTensorProductOfBases_f<TensorProductOfBases_c<FactorTyple_>>
{
    static bool const V = true;
private:
    IsTensorProductOfBases_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorProductOfBases);
// special convenience macros
#define IS_TENSOR_PRODUCT_OF_BASES_UNIQUELY(Concept) HasUniqueTensorProductOfBasesStructure_f<Concept>::V
#define AS_TENSOR_PRODUCT_OF_BASES(Concept) UniqueTensorProductOfBasesStructureOf_f<Concept>::T

template <typename FactorTyple_>
struct DualOf_f<TensorProductOfBases_c<FactorTyple_>>
{
    typedef TensorProductOfBases_c<typename DualOf_f<FactorTyple_>::T> T;
private:
    DualOf_f();
};

// ///////////////////////////////////////////////////////////////////////////
// BasedTensorProductOfVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////

// TODO: this should be an EmbeddableInTensorProductOfVectorSpaces
template <typename TensorProductOfVectorSpaces_, typename Basis_>
struct BasedTensorProductOfVectorSpaces_c
{
    static_assert(IS_TENSOR_PRODUCT_OF_VECTOR_SPACES_UNIQUELY(TensorProductOfVectorSpaces_), "TensorProductOfVectorSpaces_ must have unique tensor product of vector spaces structure");
    static_assert(IS_BASIS_UNIQUELY(Basis_), "Basis_ must have unique basis structure");
private:
    typedef TensorProductOfVectorSpaces_ As_TensorProductOfVectorSpaces;
    typedef BasedVectorSpace_c<TensorProductOfVectorSpaces_,Basis_> As_BasedVectorSpace;
public:
    typedef Typle_t<As_TensorProductOfVectorSpaces,As_BasedVectorSpace> ParentTyple;

    typedef typename As_BasedVectorSpace::Id Id;

    static std::string type_as_string (bool verbose)
    {
        if (verbose)
            return "BasedTensorProductOfVectorSpaces_c<" + type_string_of<TensorProductOfVectorSpaces_>() + ','
                                                         + type_string_of<Basis_>() + '>';
        else
            return '(' + terse_string_of<TensorProductOfVectorSpaces_>() + ',' + terse_string_of<Basis_>() + ')';
    }
};

template <typename TensorProductOfVectorSpaces_, typename Basis_>
struct IsConcept_f<BasedTensorProductOfVectorSpaces_c<TensorProductOfVectorSpaces_, Basis_>>
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsBasedTensorProductOfVectorSpaces_f
{
    static bool const V = false;
private:
    IsBasedTensorProductOfVectorSpaces_f();
};
template <typename TensorProductOfVectorSpaces_, typename Basis_> struct IsBasedTensorProductOfVectorSpaces_f<BasedTensorProductOfVectorSpaces_c<TensorProductOfVectorSpaces_,Basis_>>
{
    static bool const V = true;
private:
    IsBasedTensorProductOfVectorSpaces_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(BasedTensorProductOfVectorSpaces);
// special convenience macros
#define IS_BASED_TENSOR_PRODUCT_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueBasedTensorProductOfVectorSpacesStructure_f<Concept>::V
#define AS_BASED_TENSOR_PRODUCT_OF_VECTOR_SPACES(Concept) UniqueBasedTensorProductOfVectorSpacesStructureOf_f<Concept>::T

template <typename TensorProductOfVectorSpaces, typename Basis>
struct DualOf_f<BasedTensorProductOfVectorSpaces_c<TensorProductOfVectorSpaces,Basis>>
{
    typedef BasedTensorProductOfVectorSpaces_c<typename DualOf_f<TensorProductOfVectorSpaces>::T,typename DualOf_f<Basis>::T> T;
private:
    DualOf_f();
};

// ///////////////////////////////////////////////////////////////////////////
// TensorProductOfBasedVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////

template <typename FactorTyple_>
struct TensorProductOfBasedVectorSpaces_c
{
    static_assert(IsTyple_f<FactorTyple_>::V, "FactorTyple_ must be a Typle_t");
    static_assert(AllTypesHaveUniqueBasedVectorSpaceStructures_f<FactorTyple_>::V, "all summands must have unique based vector space structures");
private:
    typedef typename PropertyOfEachInTyple_f<FactorTyple_,Basis>::T BasisTyple;
    typedef BasedTensorProductOfVectorSpaces_c<TensorProductOfVectorSpaces_c<FactorTyple_>,
                                               TensorProductOfBases_c<BasisTyple>> As_BasedTensorProductOfVectorSpaces;
    typedef EmbeddableInTensorProductOfBasedVectorSpaces_c<TensorProductOfBasedVectorSpaces_c,
                                                           TensorProductOfVectorSpaces_c<FactorTyple_>> As_EmbeddableInTensorProductOfBasedVectorSpaces;
public:
    typedef Typle_t<As_BasedTensorProductOfVectorSpaces,As_EmbeddableInTensorProductOfBasedVectorSpaces> ParentTyple;

    typedef typename As_BasedTensorProductOfVectorSpaces::Id Id;

    static std::string type_as_string (bool verbose)
    {
        if (verbose)
            return "TensorProductOfBasedVectorSpaces_c<" + type_string_of<FactorTyple_>() + '>';
        else
            return terse_string_of_tensor_product(FactorTyple_());
    }
};

template <typename FactorTyple_>
struct IsConcept_f<TensorProductOfBasedVectorSpaces_c<FactorTyple_>>
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsTensorProductOfBasedVectorSpaces_f
{
    static bool const V = false;
private:
    IsTensorProductOfBasedVectorSpaces_f();
};
template <typename FactorTyple_> struct IsTensorProductOfBasedVectorSpaces_f<TensorProductOfBasedVectorSpaces_c<FactorTyple_>>
{
    static bool const V = true;
private:
    IsTensorProductOfBasedVectorSpaces_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorProductOfBasedVectorSpaces);
// special convenience macros
#define IS_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueTensorProductOfBasedVectorSpacesStructure_f<Concept>::V
#define AS_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(Concept) UniqueTensorProductOfBasedVectorSpacesStructureOf_f<Concept>::T

// there is a natural identification of a tensor product of vector spaces with the
// tensor product of the dual of the vector spaces
template <typename FactorTyple_>
struct DualOf_f<TensorProductOfBasedVectorSpaces_c<FactorTyple_>>
{
    typedef TensorProductOfBasedVectorSpaces_c<typename DualOf_f<FactorTyple_>::T> T;
private:
    DualOf_f();
};

// ///////////////////////////////////////////////////////////////////////////
// meta-functions for taking tensor powers of stuff
// ///////////////////////////////////////////////////////////////////////////

template <Uint32 ORDER_, typename Factor_>
struct TensorPower_f
{
    typedef TensorProduct_c<typename UniformTyple_f<ORDER_,Factor_>::T> T;
private:
    TensorPower_f();
};

template <Uint32 ORDER_, typename Factor_>
struct TensorPowerOfVectorSpace_f
{
    static_assert(HasVectorSpaceStructure_f<Factor_>::V, "Factor_ must be a vector space");
    static_assert(IS_VECTOR_SPACE_UNIQUELY(Factor_), "Factor_ must have a unique vector space structure");
    typedef TensorProductOfVectorSpaces_c<typename UniformTyple_f<ORDER_,Factor_>::T> T;
private:
    TensorPowerOfVectorSpace_f();
};

template <Uint32 ORDER_, typename Factor_>
struct TensorPowerOfBasedVectorSpace_f
{
    static_assert(HasBasedVectorSpaceStructure_f<Factor_>::V, "Factor_ must be a based vector space");
    static_assert(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor_), "Factor_ must have a unique based vector space structure");
    typedef TensorProductOfBasedVectorSpaces_c<typename UniformTyple_f<ORDER_,Factor_>::T> T;
private:
    TensorPowerOfBasedVectorSpace_f();
};

// ///////////////////////////////////////////////////////////////////////////
// helper functions for easing use of the type system
// ///////////////////////////////////////////////////////////////////////////

// for now, just do tensor product of based vector spaces

template <typename LhsBasedVectorSpace_,
          typename RhsBasedVectorSpace_>
typename std::enable_if<(IS_BASED_VECTOR_SPACE_UNIQUELY(LhsBasedVectorSpace_) &&
                         IS_BASED_VECTOR_SPACE_UNIQUELY(RhsBasedVectorSpace_)),
                        TensorProductOfBasedVectorSpaces_c<Typle_t<LhsBasedVectorSpace_,
                                                                   RhsBasedVectorSpace_>>>::type
    operator * (LhsBasedVectorSpace_ const &, RhsBasedVectorSpace_ const &)
{
    return TensorProductOfBasedVectorSpaces_c<Typle_t<LhsBasedVectorSpace_,RhsBasedVectorSpace_>>();
}

template <typename FactorTyple_, typename BasedVectorSpace_>
TensorProductOfBasedVectorSpaces_c<typename Concat2Typles_f<FactorTyple_,
                                                                   Typle_t<BasedVectorSpace_>>::T>
    operator * (TensorProductOfBasedVectorSpaces_c<FactorTyple_> const &, BasedVectorSpace_ const &)
{
    return TensorProductOfBasedVectorSpaces_c<typename Concat2Typles_f<FactorTyple_,
                                                                              Typle_t<BasedVectorSpace_>>::T>();
}

template <typename FactorTyple_, typename BasedVectorSpace_>
TensorProductOfBasedVectorSpaces_c<typename HeadBodyTyple_f<BasedVectorSpace_,FactorTyple_>::T>
    operator * (BasedVectorSpace_ const &, TensorProductOfBasedVectorSpaces_c<FactorTyple_> const &)
{
    return TensorProductOfBasedVectorSpaces_c<typename HeadBodyTyple_f<BasedVectorSpace_,FactorTyple_>::T>();
}

template <typename LhsFactorTyple_, typename RhsFactorTyple_>
TensorProductOfBasedVectorSpaces_c<Typle_t<TensorProductOfBasedVectorSpaces_c<LhsFactorTyple_>,
                                           TensorProductOfBasedVectorSpaces_c<RhsFactorTyple_>>>
    operator * (TensorProductOfBasedVectorSpaces_c<LhsFactorTyple_> const &,
                TensorProductOfBasedVectorSpaces_c<RhsFactorTyple_> const &)
{
    return TensorProductOfBasedVectorSpaces_c<Typle_t<TensorProductOfBasedVectorSpaces_c<LhsFactorTyple_>,
                                                      TensorProductOfBasedVectorSpaces_c<RhsFactorTyple_>>>();
}

// tensor power of a bvs -- use as tensor<3>(bvs)
template <Uint32 ORDER_, typename Factor_>
typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T tensor (Factor_ const &)
{
    return typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T();
}

// ///////////////////////////////////////////////////////////////////////////
// linear embedding of tensor product into itself (needed for completeness)
// ///////////////////////////////////////////////////////////////////////////

// just use the identity embedding
template <typename FactorTyple_, typename Scalar_, WithExceptions WITH_EXCEPTIONS_>
struct LinearEmbedding_f<TensorProductOfBasedVectorSpaces_c<FactorTyple_>,
                         TensorProductOfBasedVectorSpaces_c<FactorTyple_>,
                         Scalar_,
                         NaturalEmbedding,
                         WITH_EXCEPTIONS_>
{
private:
    typedef TensorProductOfBasedVectorSpaces_c<FactorTyple_> TensorProduct;
    LinearEmbedding_f ();
public:
    typedef LinearEmbedding_c<TensorProduct,TensorProduct,Scalar_,IdentityEmbedding,WITH_EXCEPTIONS_> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_TENSORPRODUCT_HPP_
