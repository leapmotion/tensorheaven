// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/diagonalbased2tensorproduct.hpp by Victor Dods, created 2013/07/24
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_DIAGONALBASED2TENSORPRODUCT_HPP_
#define TENH_CONCEPTUAL_DIAGONALBASED2TENSORPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/embeddableintensorproduct.hpp"
#include "tenh/conceptual/tensorproduct.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// Diagonal2TensorProduct_c
// ///////////////////////////////////////////////////////////////////////////

// formal "diagonal 2-tensor product" of symbols
template <typename Factor1_, typename Factor2_>
struct Diagonal2TensorProduct_c
{
    typedef EmptyTypeList ParentTypeList;

    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;

    static std::string type_as_string ()
    {
        return "Diagonal2TensorProduct_c<" + type_string_of<Factor1_>() + ',' + type_string_of<Factor2_>() + '>';
    }
};

template <typename Factor1_, typename Factor2_>
struct IsConcept_f<Diagonal2TensorProduct_c<Factor1_, Factor2_> >
{ static bool const V = true; };

template <typename T> struct IsDiagonal2TensorProduct_f { static bool const V = false; };

template <typename Factor1_, typename Factor2_>
struct IsDiagonal2TensorProduct_f<Diagonal2TensorProduct_c<Factor1_, Factor2_> >
{ static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Diagonal2TensorProduct);
// special convenience macros
#define IS_DIAGONAL_2_TENSOR_PRODUCT_UNIQUELY(Concept) HasUniqueDiagonal2TensorProductStructure_f<Concept>::V
#define AS_DIAGONAL_2_TENSOR_PRODUCT(Concept) UniqueDiagonal2TensorProductStructureOf_f<Concept>::T

// ///////////////////////////////////////////////////////////////////////////
// Diagonal2TensorProductOfBases_c
// ///////////////////////////////////////////////////////////////////////////

template <typename Factor1_, typename Factor2_>
struct Diagonal2TensorProductOfBases_c
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IS_BASIS_UNIQUELY(Factor1_), MUST_BE_BASIS, FACTOR1),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IS_BASIS_UNIQUELY(Factor2_), MUST_BE_BASIS, FACTOR2)
    };
    typedef Basis_c<Diagonal2TensorProduct_c<Factor1_,Factor2_> > As_Basis;
    typedef Diagonal2TensorProduct_c<Factor1_, Factor2_> As_Diagonal2TensorProduct;
public:
    typedef TypeList_t<As_Basis,
            TypeList_t<As_Diagonal2TensorProduct> > ParentTypeList;

    static Uint32 const ORDER = 2;
    typedef typename As_Basis::Id Id;
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;

    static std::string type_as_string ()
    {
        return "Diagonal2TensorProductOfBases_c<" + type_string_of<Factor1_>() + ',' + type_string_of<Factor2_>() + '>';
    }
};

template <typename Factor1_, typename Factor2_>
struct IsConcept_f<Diagonal2TensorProductOfBases_c<Factor1_, Factor2_> >
{ static bool const V = true; };

template <typename T> struct IsDiagonal2TensorProductOfBases_f { static bool const V = false; };

template <typename Factor1_, typename Factor2_>
struct IsDiagonal2TensorProductOfBases_f<Diagonal2TensorProductOfBases_c<Factor1_, Factor2_> >
{ static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Diagonal2TensorProductOfBases);
// special convenience macros
#define IS_DIAGONAL_2_TENSOR_PRODUCT_OF_BASES_UNIQUELY(Concept) HasUniqueDiagonal2TensorProductOfBasesStructure_f<Concept>::V
#define AS_DIAGONAL_2_TENSOR_PRODUCT_OF_BASES(Concept) UniqueDiagonal2TensorProductOfBasesStructureOf_f<Concept>::T

// ///////////////////////////////////////////////////////////////////////////
// Diagonal2TensorProductOfBasedVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////

template <typename Factor1_, typename Factor2_>
struct Diagonal2TensorProductOfBasedVectorSpaces_c
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor1_), MUST_BE_BASED_VECTOR_SPACE, FACTOR1),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor2_), MUST_BE_BASED_VECTOR_SPACE, FACTOR2),
        STATIC_ASSERT_IN_ENUM((TypesAreEqual<typename ScalarFieldOf_f<Factor1_>::T,typename ScalarFieldOf_f<Factor2_>::T>::V), ALL_FACTORS_MUST_HAVE_SAME_FIELD),
    };
    typedef TypeList_t<Factor1_,TypeList_t<Factor2_> > FactorTypeList;

    typedef VectorSpace_c<typename ScalarFieldOf_f<Factor1_>::T,
                          (DimensionOf_f<Factor1_>::V < DimensionOf_f<Factor2_>::V) ? DimensionOf_f<Factor1_>::V : DimensionOf_f<Factor2_>::V,
                          Diagonal2TensorProduct_c<Factor1_,Factor2_> > UnderlyingVectorSpace;

    typedef BasedVectorSpace_c<UnderlyingVectorSpace,
                               Diagonal2TensorProductOfBases_c<typename BasisOf_f<Factor1_>::T,
                                                               typename BasisOf_f<Factor2_>::T> > As_BasedVectorSpace;
    typedef EmbeddableInTensorProductOfBasedVectorSpaces_c<TensorProductOfBasedVectorSpaces_c<FactorTypeList>,
                                                           TensorProductOfVectorSpaces_c<FactorTypeList> > As_EmbeddableInTensorProductOfBasedVectorSpaces;
public:
    typedef TypeList_t<As_BasedVectorSpace,
            TypeList_t<As_EmbeddableInTensorProductOfBasedVectorSpaces> > ParentTypeList;

    typedef typename As_BasedVectorSpace::Id Id;
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;

    static std::string type_as_string ()
    {
        return "Diagonal2TensorProductOfBasedVectorSpaces_c<" + type_string_of<Factor1_>() + ',' + type_string_of<Factor2_>() + '>';
    }
};

template <typename Factor1_, typename Factor2_>
struct IsConcept_f<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> >
{ static bool const V = true; };

template <typename T> struct IsDiagonal2TensorProductOfBasedVectorSpaces_f { static bool const V = false; };

template <typename Factor1_, typename Factor2_>
struct IsDiagonal2TensorProductOfBasedVectorSpaces_f<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> >
{ static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Diagonal2TensorProductOfBasedVectorSpaces);
// special convenience macros
#define IS_DIAGONAL_2_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueDiagonal2TensorProductOfBasedVectorSpacesStructure_f<Concept>::V
#define AS_DIAGONAL_2_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(Concept) UniqueDiagonal2TensorProductOfBasedVectorSpacesStructureOf_f<Concept>::T

// TODO: verify that this mathematical claim is true
template <typename Factor1, typename Factor2>
struct DualOf_f<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> >
{
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<typename DualOf_f<Factor1>::T,typename DualOf_f<Factor2>::T> T;
};

// convenience metafunction for using a FactorTypeList_ instead of two separate factors
template <typename FactorTypeList_>
struct Diagonal2TensorProductOfBasedVectorSpaces_f
{
    enum { STATIC_ASSERT_IN_ENUM(FactorTypeList_::LENGTH == 2, LENGTH_MUST_BE_EXACTLY_2) };
    typedef typename FactorTypeList_::HeadType Factor1;
    typedef typename FactorTypeList_::BodyTypeList::HeadType Factor2;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> T;
};

// specialization for FactorTypeListOf_f
template <typename Factor1_, typename Factor2_>
struct BaseProperty_f<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,FactorTypeList>
{
    typedef TypeList_t<Factor1_,TypeList_t<Factor2_> > T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_DIAGONALBASED2TENSORPRODUCT_HPP_
