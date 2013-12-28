// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/diagonalbased2tensorproduct.hpp by Victor Dods, created 2013/07/24
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_DIAGONALBASED2TENSORPRODUCT_HPP_
#define TENH_CONCEPTUAL_DIAGONALBASED2TENSORPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/componentindex.hpp" // technically not conceptual code, but close enough.
#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/embeddableintensorproduct.hpp"
#include "tenh/conceptual/linearembedding.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/multiindex.hpp" // technically not conceptual code, but close enough.

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
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsDiagonal2TensorProduct_f
{
    static bool const V = false;
private:
    IsDiagonal2TensorProduct_f();
};

template <typename Factor1_, typename Factor2_>
struct IsDiagonal2TensorProduct_f<Diagonal2TensorProduct_c<Factor1_, Factor2_> >
{
    static bool const V = true;
private:
    IsDiagonal2TensorProduct_f();
};

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
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsDiagonal2TensorProductOfBases_f
{
    static bool const V = false;
private:
    IsDiagonal2TensorProductOfBases_f();
};

template <typename Factor1_, typename Factor2_>
struct IsDiagonal2TensorProductOfBases_f<Diagonal2TensorProductOfBases_c<Factor1_, Factor2_> >
{
    static bool const V = true;
private:
    IsDiagonal2TensorProductOfBases_f();
};

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
        STATIC_ASSERT_IN_ENUM((TypesAreEqual_f<typename ScalarFieldOf_f<Factor1_>::T,typename ScalarFieldOf_f<Factor2_>::T>::V), ALL_FACTORS_MUST_HAVE_SAME_FIELD),
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
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsDiagonal2TensorProductOfBasedVectorSpaces_f
{
    static bool const V = false;
private:
    IsDiagonal2TensorProductOfBasedVectorSpaces_f();
};

template <typename Factor1_, typename Factor2_>
struct IsDiagonal2TensorProductOfBasedVectorSpaces_f<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> >
{
    static bool const V = true;
private:
    IsDiagonal2TensorProductOfBasedVectorSpaces_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Diagonal2TensorProductOfBasedVectorSpaces);
// special convenience macros
#define IS_DIAGONAL_2_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueDiagonal2TensorProductOfBasedVectorSpacesStructure_f<Concept>::V
#define AS_DIAGONAL_2_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(Concept) UniqueDiagonal2TensorProductOfBasedVectorSpacesStructureOf_f<Concept>::T

// TODO: verify that this mathematical claim is true
template <typename Factor1, typename Factor2>
struct DualOf_f<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> >
{
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<typename DualOf_f<Factor1>::T,typename DualOf_f<Factor2>::T> T;
private:
    DualOf_f();
};

// convenience metafunction for using a FactorTypeList_ instead of two separate factors
template <typename FactorTypeList_>
struct Diagonal2TensorProductOfBasedVectorSpaces_f
{
private:
    enum { STATIC_ASSERT_IN_ENUM(FactorTypeList_::LENGTH == 2, LENGTH_MUST_BE_EXACTLY_2) };
    typedef typename FactorTypeList_::HeadType Factor1;
    typedef typename FactorTypeList_::BodyTypeList::HeadType Factor2;
    Diagonal2TensorProductOfBasedVectorSpaces_f();
public:
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> T;
};

// specialization for FactorTypeListOf_f
template <typename Factor1_, typename Factor2_>
struct BaseProperty_f<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,FactorTypeList>
{
    typedef TypeList_t<Factor1_,TypeList_t<Factor2_> > T;
private:
    BaseProperty_f();
};

// ///////////////////////////////////////////////////////////////////////////
// linear embedding of diagonal 2-tensor into corresponding tensor product
// ///////////////////////////////////////////////////////////////////////////

template <typename Factor1_, typename Factor2_, typename Scalar_>
struct LinearEmbedding_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,
                         TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,TypeList_t<Factor2_> > >,
                         Scalar_,
                         NaturalEmbedding>
{
private:
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> Diag2;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,TypeList_t<Factor2_> > > Tensor2;
    static bool const FACTOR1DIM_LEQ_FACTOR2DIM = DimensionOf_f<Factor1_>::V <= DimensionOf_f<Factor2_>::V;
public:

    typedef ComponentIndex_t<DimensionOf_f<Diag2>::V> Diag2ComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Tensor2>::V> Tensor2ComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Factor1_>::V> Factor1ComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Factor2_>::V> Factor2ComponentIndex;
    typedef MultiIndex_t<TypeList_t<Factor1ComponentIndex,
                         TypeList_t<Factor2ComponentIndex> > > Tensor2MultiIndex;

    static bool embedded_component_is_procedural_zero (Tensor2ComponentIndex const &i)
    {
        Tensor2MultiIndex m(i); // does the row-major indexing conversion
        // it's a procedural zero if the component is off the diagonal.
        return m.template el<0>().value() != m.template el<1>().value();
    }
    static Scalar_ scalar_factor_for_embedded_component (Tensor2ComponentIndex const &) { return Scalar_(1); }
    static Diag2ComponentIndex source_component_index_for_embedded_component (Tensor2ComponentIndex const &i)
    {
        assert(!embedded_component_is_procedural_zero(i)); // this may potentially slow stuff down too much
        Tensor2MultiIndex m(i); // does the row-major indexing conversion
        return m.template el<(FACTOR1DIM_LEQ_FACTOR2DIM ? 0 : 1)>();
    }

    static Uint32 term_count_for_coembedded_component (Diag2ComponentIndex const &) { return 1; }
    static Scalar_ scalar_factor_for_coembedded_component (Diag2ComponentIndex const &, Uint32) { return 1; }
    static Tensor2ComponentIndex source_component_index_for_coembedded_component (Diag2ComponentIndex const &i, Uint32)
    {
        return Tensor2MultiIndex(i.value(), i.value(), DONT_CHECK_RANGE).as_component_index();
    }
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_DIAGONALBASED2TENSORPRODUCT_HPP_
