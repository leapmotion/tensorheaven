// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/scalarbased2tensorproduct.hpp by Victor Dods, created 2013/12/30
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_SCALARBASED2TENSORPRODUCT_HPP_
#define TENH_CONCEPTUAL_SCALARBASED2TENSORPRODUCT_HPP_

#include "tenh/core.hpp"

#include <type_traits>

#include "tenh/componentindex.hpp" // technically not conceptual code, but close enough.
#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/diagonalbased2tensorproduct.hpp"
#include "tenh/conceptual/embeddableintensorproduct.hpp"
#include "tenh/conceptual/linearembedding.hpp"
#include "tenh/conceptual/symmetricpower.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/multiindex.hpp" // technically not conceptual code, but close enough.

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// Scalar2TensorProduct_c
// ///////////////////////////////////////////////////////////////////////////

// formal "scalar 2-tensor product" of symbols
template <typename Factor0_, typename Factor1_>
struct Scalar2TensorProduct_c
{
    typedef EmptyTypeList ParentTypeList;

    typedef Factor0_ Factor0;
    typedef Factor1_ Factor1;

    static std::string type_as_string ()
    {
        return "Scalar2TensorProduct_c<" + type_string_of<Factor0_>() + ',' + type_string_of<Factor1_>() + '>';
    }
};

template <typename Factor0_, typename Factor1_>
struct IsConcept_f<Scalar2TensorProduct_c<Factor0_,Factor1_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsScalar2TensorProduct_f
{
    static bool const V = false;
private:
    IsScalar2TensorProduct_f();
};

template <typename Factor0_, typename Factor1_>
struct IsScalar2TensorProduct_f<Scalar2TensorProduct_c<Factor0_,Factor1_> >
{
    static bool const V = true;
private:
    IsScalar2TensorProduct_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Scalar2TensorProduct);
// special convenience macros
#define IS_SCALAR_2_TENSOR_PRODUCT_UNIQUELY(Concept) HasUniqueScalar2TensorProductStructure_f<Concept>::V
#define AS_SCALAR_2_TENSOR_PRODUCT(Concept) UniqueScalar2TensorProductStructureOf_f<Concept>::T

// ///////////////////////////////////////////////////////////////////////////
// Scalar2TensorProductOfBases_c
// ///////////////////////////////////////////////////////////////////////////

template <typename Factor0_, typename Factor1_>
struct Scalar2TensorProductOfBases_c
{
private:
    static_assert(IS_BASIS_UNIQUELY(Factor0_), "Factor 0 must be a basis.");
    static_assert(IS_BASIS_UNIQUELY(Factor1_), "Factor 1 must be a basis.");
    typedef Basis_c<Scalar2TensorProduct_c<Factor0_,Factor1_> > As_Basis;
    typedef Scalar2TensorProduct_c<Factor0_,Factor1_> As_Scalar2TensorProduct;
public:
    typedef TypeList_t<As_Basis,
            TypeList_t<As_Scalar2TensorProduct> > ParentTypeList;

    typedef typename As_Basis::Id Id;
    typedef Factor0_ Factor0;
    typedef Factor1_ Factor1;

    static std::string type_as_string ()
    {
        return "Scalar2TensorProductOfBases_c<" + type_string_of<Factor0_>() + ',' + type_string_of<Factor1_>() + '>';
    }
};

template <typename Factor0_, typename Factor1_>
struct IsConcept_f<Scalar2TensorProductOfBases_c<Factor0_,Factor1_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsScalar2TensorProductOfBases_f
{
    static bool const V = false;
private:
    IsScalar2TensorProductOfBases_f();
};

template <typename Factor0_, typename Factor1_>
struct IsScalar2TensorProductOfBases_f<Scalar2TensorProductOfBases_c<Factor0_,Factor1_> >
{
    static bool const V = true;
private:
    IsScalar2TensorProductOfBases_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Scalar2TensorProductOfBases);
// special convenience macros
#define IS_SCALAR_2_TENSOR_PRODUCT_OF_BASES_UNIQUELY(Concept) HasUniqueScalar2TensorProductOfBasesStructure_f<Concept>::V
#define AS_SCALAR_2_TENSOR_PRODUCT_OF_BASES(Concept) UniqueScalar2TensorProductOfBasesStructureOf_f<Concept>::T

// ///////////////////////////////////////////////////////////////////////////
// Scalar2TensorProductOfBasedVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////

template <typename Factor0_, typename Factor1_>
struct Scalar2TensorProductOfBasedVectorSpaces_c
{
private:
    static_assert(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor0_), "Factor 0 must be a based vector space.");
    static_assert(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor1_), "Factor 1 must be a based vector space.");
    static_assert((TypesAreEqual_f<typename ScalarFieldOf_f<Factor0_>::T,typename ScalarFieldOf_f<Factor1_>::T>::V), "Both factors must have the same scalar field.");
    typedef TypeList_t<Factor0_,TypeList_t<Factor1_> > FactorTypeList;

    typedef VectorSpace_c<typename ScalarFieldOf_f<Factor0_>::T,
                          1,
                          Scalar2TensorProduct_c<Factor0_,Factor1_> > UnderlyingVectorSpace;

    typedef BasedVectorSpace_c<UnderlyingVectorSpace,
                               Scalar2TensorProductOfBases_c<typename BasisOf_f<Factor0_>::T,
                                                             typename BasisOf_f<Factor1_>::T> > As_BasedVectorSpace;
    typedef EmbeddableInTensorProductOfBasedVectorSpaces_c<TensorProductOfBasedVectorSpaces_c<FactorTypeList>,
                                                           TensorProductOfVectorSpaces_c<FactorTypeList> > As_EmbeddableInTensorProductOfBasedVectorSpaces;
public:
    typedef TypeList_t<As_BasedVectorSpace,
            TypeList_t<As_EmbeddableInTensorProductOfBasedVectorSpaces> > ParentTypeList;

    typedef typename As_BasedVectorSpace::Id Id;
    typedef Factor0_ Factor0;
    typedef Factor1_ Factor1;

    static std::string type_as_string ()
    {
        return "Scalar2TensorProductOfBasedVectorSpaces_c<" + type_string_of<Factor0_>() + ',' + type_string_of<Factor1_>() + '>';
    }
};

template <typename Factor0_, typename Factor1_>
struct IsConcept_f<Scalar2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsScalar2TensorProductOfBasedVectorSpaces_f
{
    static bool const V = false;
private:
    IsScalar2TensorProductOfBasedVectorSpaces_f();
};

template <typename Factor0_, typename Factor1_>
struct IsScalar2TensorProductOfBasedVectorSpaces_f<Scalar2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_> >
{
    static bool const V = true;
private:
    IsScalar2TensorProductOfBasedVectorSpaces_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Scalar2TensorProductOfBasedVectorSpaces);
// special convenience macros
#define IS_SCALAR_2_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueScalar2TensorProductOfBasedVectorSpacesStructure_f<Concept>::V
#define AS_SCALAR_2_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(Concept) UniqueScalar2TensorProductOfBasedVectorSpacesStructureOf_f<Concept>::T

// TODO: verify that this mathematical claim is true
template <typename Factor0, typename Factor1>
struct DualOf_f<Scalar2TensorProductOfBasedVectorSpaces_c<Factor0,Factor1> >
{
    typedef Scalar2TensorProductOfBasedVectorSpaces_c<typename DualOf_f<Factor0>::T,typename DualOf_f<Factor1>::T> T;
private:
    DualOf_f();
};

// convenience metafunction for using a FactorTypeList_ instead of two separate factors
template <typename FactorTypeList_>
struct Scalar2TensorProductOfBasedVectorSpaces_f
{
private:
    static_assert(FactorTypeList_::LENGTH == 2, "FactorTypeList in scalar 2 tensor product must have length exactly 2");
    typedef typename FactorTypeList_::HeadType Factor0;
    typedef typename FactorTypeList_::BodyTypeList::HeadType Factor1;
    Scalar2TensorProductOfBasedVectorSpaces_f();
public:
    typedef Scalar2TensorProductOfBasedVectorSpaces_c<Factor0,Factor1> T;
};

// specialization for FactorTypeListOf_f
template <typename Factor0_, typename Factor1_>
struct BaseProperty_f<Scalar2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,FactorTypeList>
{
    typedef TypeList_t<Factor0_,TypeList_t<Factor1_> > T;
private:
    BaseProperty_f();
};

// ///////////////////////////////////////////////////////////////////////////
// helper functions for easing use of the type system
// ///////////////////////////////////////////////////////////////////////////

// some helper macros for the following different versions of diag2
#define BOTH_FACTORS_ARE_BASES_UNIQUELY(Factor0, Factor1) (IS_BASIS_UNIQUELY(Factor0) && IS_BASIS_UNIQUELY(Factor1))
#define BOTH_FACTORS_ARE_BASED_VECTOR_SPACES_UNIQUELY(Factor0, Factor1) (IS_BASED_VECTOR_SPACE_UNIQUELY(Factor0) && IS_BASED_VECTOR_SPACE_UNIQUELY(Factor1))

// formal scalar 2-tensor product
template <typename Factor0_, typename Factor1_>
typename std::enable_if<(!BOTH_FACTORS_ARE_BASES_UNIQUELY(Factor0_,Factor1_) &&
                         !BOTH_FACTORS_ARE_BASED_VECTOR_SPACES_UNIQUELY(Factor0_,Factor1_)),
                        Scalar2TensorProduct_c<Factor0_,Factor1_> >::type
    scalar2 (Factor0_ const &, Factor1_ const &)
{
    return Scalar2TensorProduct_c<Factor0_,Factor1_>();
}

// scalar 2-tensor product of bases
template <typename Factor0_, typename Factor1_>
typename std::enable_if<(BOTH_FACTORS_ARE_BASES_UNIQUELY(Factor0_,Factor1_) &&
                         !BOTH_FACTORS_ARE_BASED_VECTOR_SPACES_UNIQUELY(Factor0_,Factor1_)),
                        Scalar2TensorProductOfBases_c<Factor0_,Factor1_> >::type
    scalar2 (Factor0_ const &, Factor1_ const &)
{
    return Scalar2TensorProductOfBases_c<Factor0_,Factor1_>();
}

// scalar 2-tensor product of based vector spaces
template <typename Factor0_, typename Factor1_>
typename std::enable_if<(!BOTH_FACTORS_ARE_BASES_UNIQUELY(Factor0_,Factor1_) &&
                         BOTH_FACTORS_ARE_BASED_VECTOR_SPACES_UNIQUELY(Factor0_,Factor1_)),
                        Scalar2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_> >::type
    scalar2 (Factor0_ const &, Factor1_ const &)
{
    return Scalar2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>();
}

#undef BOTH_FACTORS_ARE_BASES_UNIQUELY
#undef BOTH_FACTORS_ARE_BASED_VECTOR_SPACES_UNIQUELY

// for now, just do scalar 2-tensor product of based vector spaces
template <typename Factor0_, typename Factor1_>
Scalar2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>
    scalar2 (TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor0_,TypeList_t<Factor1_> > > const &)
{
    return Scalar2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>();
}

// ///////////////////////////////////////////////////////////////////////////
// linear embedding of scalar 2-tensor into corresponding tensor product
// ///////////////////////////////////////////////////////////////////////////

template <typename Factor0_, typename Factor1_, typename Scalar_, bool ENABLE_EXCEPTIONS_>
struct LinearEmbedding_c<Scalar2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,
                         TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor0_,TypeList_t<Factor1_> > >,
                         Scalar_,
                         NaturalEmbedding,
                         ENABLE_EXCEPTIONS_>
{
private:
    typedef Scalar2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_> Scalar2;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor0_,TypeList_t<Factor1_> > > Tensor2;
    static bool const FACTOR0DIM_LEQ_FACTOR1DIM = DimensionOf_f<Factor0_>::V <= DimensionOf_f<Factor1_>::V;
    static Uint32 const DIM = DimensionOf_f<Factor0_>::V <= DimensionOf_f<Factor1_>::V ?
                              DimensionOf_f<Factor0_>::V :
                              DimensionOf_f<Factor1_>::V;
public:
    typedef ComponentIndex_t<DimensionOf_f<Scalar2>::V> Scalar2ComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Tensor2>::V> Tensor2ComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Factor0_>::V> Factor0ComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Factor1_>::V> Factor1ComponentIndex;

    struct CoembedIndexIterator
    {
        CoembedIndexIterator (Scalar2ComponentIndex const &) : m() { }
        void operator ++ () { m.increment_by(DimensionOf_f<Factor1_>::V+1, DONT_CHECK_RANGE); }
        bool is_not_at_end () const { return m.value() < (DimensionOf_f<Factor1_>::V+1)*DIM; }
        Scalar_ scale_factor () const { return Scalar_(1); }
        typedef Tensor2ComponentIndex ComponentIndexReturnType;
        ComponentIndexReturnType const &component_index () const { return m; }
    private:
        Tensor2ComponentIndex m;
    };

private:
    typedef MultiIndex_t<TypeList_t<Factor0ComponentIndex,
                         TypeList_t<Factor1ComponentIndex> > > Tensor2MultiIndex;
public:
    static bool embedded_component_is_procedural_zero (Tensor2ComponentIndex const &i)
    {
        Tensor2MultiIndex m(i); // does the row-major indexing conversion
        // it's a procedural zero if the component is off the diagonal.
        return m.template el<0>().value() != m.template el<1>().value();
    }
    static Scalar_ scalar_factor_for_embedded_component (Tensor2ComponentIndex const &i)
    {
        if (ENABLE_EXCEPTIONS_ && embedded_component_is_procedural_zero(i))
            throw std::domain_error(FORMAT(i.value()) + " is not in the domain of scalar_factor_for_embedded_component");

        return Scalar_(1);
    }
    static Scalar2ComponentIndex source_component_index_for_embedded_component (Tensor2ComponentIndex const &i)
    {
        if (ENABLE_EXCEPTIONS_ && embedded_component_is_procedural_zero(i))
            throw std::domain_error(FORMAT(i.value()) + " is not in the domain of scalar_factor_for_embedded_component");

        // there is only one component for a scalar 2-tensor -- zero.
        return Scalar2ComponentIndex(0, DONT_CHECK_RANGE);
    }
};

// ///////////////////////////////////////////////////////////////////////////
// linear embedding of scalar 2-tensor of uniform factor type into
// corresponding 2nd symmetric power
// ///////////////////////////////////////////////////////////////////////////

template <typename Factor_, typename Scalar_, bool ENABLE_EXCEPTIONS_>
struct LinearEmbedding_c<Scalar2TensorProductOfBasedVectorSpaces_c<Factor_,Factor_>,
                         SymmetricPowerOfBasedVectorSpace_c<2,Factor_>,
                         Scalar_,
                         NaturalEmbedding,
                         ENABLE_EXCEPTIONS_>
{
private:
    typedef Scalar2TensorProductOfBasedVectorSpaces_c<Factor_,Factor_> Diag2;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,Factor_> Sym2;
    static Uint32 const DIM = DimensionOf_f<Factor_>::V;
public:
    typedef ComponentIndex_t<DimensionOf_f<Diag2>::V> Scalar2ComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Sym2>::V> Sym2ComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Factor_>::V> FactorComponentIndex;

    struct CoembedIndexIterator
    {
        CoembedIndexIterator (Scalar2ComponentIndex const &) : m(), m_increment(2) { }
        void operator ++ () { m.increment_by(m_increment, DONT_CHECK_RANGE); ++m_increment; } // increment triangularly
        bool is_not_at_end () const { return m.is_not_at_end(); }
        Scalar_ scale_factor () const { return Scalar_(1); }
        typedef Sym2ComponentIndex ComponentIndexReturnType;
        ComponentIndexReturnType const &component_index () const { return m; }
    private:
        Sym2ComponentIndex m;
        Uint32 m_increment;
    };

    static bool embedded_component_is_procedural_zero (Sym2ComponentIndex const &i)
    {
        // Sym2 uses the lower-triangular indexing (which doesn't depend on the dimension of Factor_)
        Uint32 t = index_of_greatest_simplicial_number_leq(i.value(), 2);
        // std::cout << FORMAT_VALUE(i.value()) << ", " << FORMAT_VALUE(t) << '\n';
        FactorComponentIndex row(t-1, DONT_CHECK_RANGE);
        FactorComponentIndex col(i.value() - t*(t-1)/2, DONT_CHECK_RANGE);
        // off-diagonals are procedural zeros
        return row.value() != col.value();
    }
    static Scalar_ scalar_factor_for_embedded_component (Sym2ComponentIndex const &i)
    {
        if (ENABLE_EXCEPTIONS_ && embedded_component_is_procedural_zero(i))
            throw std::domain_error(FORMAT(i.value()) + " is not in the domain of scalar_factor_for_embedded_component");

        return Scalar_(1);
    }
    static Scalar2ComponentIndex source_component_index_for_embedded_component (Sym2ComponentIndex const &i)
    {
        if (ENABLE_EXCEPTIONS_ && embedded_component_is_procedural_zero(i))
            throw std::domain_error(FORMAT(i.value()) + " is not in the domain of scalar_factor_for_embedded_component");

        return Scalar2ComponentIndex(0, DONT_CHECK_RANGE);
    }
};

// ///////////////////////////////////////////////////////////////////////////
// linear embedding of scalar 2-tensor into corresponding diagonal 2-tensor
// ///////////////////////////////////////////////////////////////////////////

template <typename Factor0_, typename Factor1_, typename Scalar_, bool ENABLE_EXCEPTIONS_>
struct LinearEmbedding_c<Scalar2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,
                         Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,
                         Scalar_,
                         NaturalEmbedding,
                         ENABLE_EXCEPTIONS_>
{
private:
    typedef Scalar2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_> Scalar2;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_> Diag2;
    static bool const FACTOR0DIM_LEQ_FACTOR1DIM = DimensionOf_f<Factor0_>::V <= DimensionOf_f<Factor1_>::V;
    static Uint32 const DIM = DimensionOf_f<Factor0_>::V <= DimensionOf_f<Factor1_>::V ?
                              DimensionOf_f<Factor0_>::V :
                              DimensionOf_f<Factor1_>::V;
public:
    typedef ComponentIndex_t<DimensionOf_f<Scalar2>::V> Scalar2ComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Diag2>::V> Diag2ComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Factor0_>::V> Factor0ComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Factor1_>::V> Factor1ComponentIndex;

    struct CoembedIndexIterator
    {
        CoembedIndexIterator (Scalar2ComponentIndex const &) : m() { }
        void operator ++ () { ++m; }
        bool is_not_at_end () const { return m.is_not_at_end(); }
        Scalar_ scale_factor () const { return Scalar_(1); }
        typedef Diag2ComponentIndex ComponentIndexReturnType;
        ComponentIndexReturnType const &component_index () const { return m; }
    private:
        Diag2ComponentIndex m;
    };

    // because this always returns false, there is no need for the other two functions to ever throw.
    static bool embedded_component_is_procedural_zero (Diag2ComponentIndex const &) { return false; }
    static Scalar_ scalar_factor_for_embedded_component (Diag2ComponentIndex const &) { return Scalar_(1); }
    static Scalar2ComponentIndex source_component_index_for_embedded_component (Diag2ComponentIndex const &i)
    {
        // there is only one component for a scalar 2-tensor -- zero.
        return Scalar2ComponentIndex(0, DONT_CHECK_RANGE);
    }
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_SCALARBASED2TENSORPRODUCT_HPP_
