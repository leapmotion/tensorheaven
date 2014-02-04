#ifndef TENSORHELL_ADHOC_TEST_HPP_
#define TENSORHELL_ADHOC_TEST_HPP_

#include <cassert>
#include <iostream>

#include "tenh/core.hpp"

#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/diagonalbased2tensorproduct.hpp"
#include "tenh/conceptual/dual.hpp"
#include "tenh/conceptual/exteriorpower.hpp"
#include "tenh/conceptual/symmetricpower.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/conceptual/utility/conceptualinheritancegraph.hpp"
#include "tenh/expressiontemplate_eval.hpp"
#include "tenh/memberarray.hpp"
#include "tenh/implementation/alt.hpp"
#include "tenh/implementation/diagonal2tensor.hpp"
#include "tenh/implementation/directsum.hpp"
#include "tenh/implementation/euclideanembedding.hpp"
#include "tenh/implementation/innerproduct.hpp"
#include "tenh/implementation/sym.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/tensorproduct.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/vee.hpp"
#include "tenh/implementation/wedge.hpp"
#include "tenh/interop/eigen.hpp"
#include "tenh/interop/eigen_invert.hpp"
#include "tenh/proceduralarray.hpp"
#include "tenh/utility/polynomial.hpp"
#include "tenh/utility/polynomial_utility.hpp"

struct X
{
    static std::string type_as_string () { return "X"; }
};

struct Y
{
    static std::string type_as_string () { return "Y"; }
};

struct Z
{
    static std::string type_as_string () { return "Z"; }
};

using namespace Tenh;

template<Uint32 ORDER, typename Vector, typename Scalar>
ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER,Vector>,Scalar> test_vector_power (const ImplementationOf_t<Vector,Scalar> &input)
{
//    STATIC_ASSERT(IS_BASED_VECTORSPACE_UNIQUELY(Vector), MUST_BE_BASED_VECTOR_SPACE);
    typedef ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER,Vector>,Scalar> Sym;
    typedef ImplementationOf_t<Vector,Scalar> Vec;

    Sym result(fill_with(1));

    for (typename Sym::ComponentIndex it; it.is_not_at_end(); ++it)
    {
        //result[it] = Scalar(1);
        typename Sym::MultiIndex m = Sym::template bundle_index_map<typename Sym::MultiIndex::IndexTyple, typename Sym::ComponentIndex>(it);
        for (Uint32 i = 0; i < Sym::MultiIndex::LENGTH; ++i)
        {
            result[it] *= input[typename Vec::ComponentIndex(m.value_of_index(i, DONT_CHECK_RANGE))];
        }
    }
    return result;
}

template <typename BasedVectorSpace, Uint32 ORDER>
void test_tensor_printing (std::ostream &out)
{
    typedef typename TensorPowerOfBasedVectorSpace_f<ORDER,BasedVectorSpace>::T TPow;
    typedef ImplementationOf_t<TPow,float> T;
    T t(Static<WithoutInitialization>::SINGLETON);
    for (typename T::ComponentIndex i; i.is_not_at_end(); ++i)
        t[i] = static_cast<float>(i.value());

    out << FORMAT_VALUE(t) << '\n';
}

template <typename Scalar_, Uint32 DIMENSION_, Uint32 K_>
Scalar_ standard_basis_vector_generator (ComponentIndex_t<DIMENSION_> const &i)
{
    return i.value() == K_ ? Scalar_(1) : Scalar_(0);
}

template <Uint32 DIMENSION_, Uint32 K_>
struct StandardBasisVectorGeneratorId
{
    static std::string type_as_string ()
    {
        return "StandardBasisVectorGenerator<" + FORMAT(DIMENSION_) + ',' + FORMAT(K_) + '>';
    }
};

template <typename Scalar_, Uint32 DIMENSION_, Uint32 K_>
struct StandardBasisComponentGenerator_f
{
    typedef ComponentGenerator_t<Scalar_,DIMENSION_,
                                 standard_basis_vector_generator<Scalar_,DIMENSION_,K_>,
                                 StandardBasisVectorGeneratorId<DIMENSION_,K_> > T;
};

template <typename BasedVectorSpace_, typename Scalar_, Uint32 K_>
struct StandardBasisVector_f
{
private:
    typedef typename StandardBasisComponentGenerator_f<Scalar_,DimensionOf_f<BasedVectorSpace_>::V,K_>::T ComponentGenerator;
public:
    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_,UseProceduralArray_t<ComponentGenerator> > T;
};

template <typename Scalar_, Uint32 DIM_>
Scalar_ counting_vector_generator (ComponentIndex_t<DIM_> const &i)
{
    return Scalar_(i.value() + 1);
}

template <Uint32 DIM_>
struct CountingVectorGeneratorId { static std::string type_as_string () { return "CountingVectorGenerator<" + FORMAT(DIM_) + '>'; } };

template <typename Scalar_, Uint32 DIM_, Uint32 K_>
void test_procedural_array_0 ()
{
    std::cout << "test_procedural_array_0<" << type_string_of<Scalar_>() << ',' + FORMAT(DIM_) << ',' << FORMAT(K_) << ">\n";
    typedef ComponentGenerator_t<Scalar_,
                                 DIM_,
                                 standard_basis_vector_generator<Scalar_,DIM_,K_>,
                                 StandardBasisVectorGeneratorId<DIM_,K_> > ComponentGenerator;
    typedef ProceduralArray_t<Scalar_,DIM_,ComponentGenerator> A;
    A a;
    std::cout << FORMAT_VALUE(a.type_as_string()) << '\n';
    for (typename A::ComponentIndex i; i.is_not_at_end(); ++i)
        std::cout << FORMAT_VALUE(i) << ", " << FORMAT_VALUE(a[i]) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, Uint32 DIM_>
void test_procedural_array_1 ()
{
    std::cout << "test_procedural_array_1<" << type_string_of<Scalar_>() << ',' << FORMAT(DIM_) << ">\n";
    typedef ComponentGenerator_t<Scalar_,
                                 DIM_,
                                 counting_vector_generator<Scalar_,DIM_>,
                                 CountingVectorGeneratorId<DIM_> > ComponentGenerator;
    typedef ProceduralArray_t<Scalar_,DIM_,ComponentGenerator> A;
    A a;
    std::cout << FORMAT_VALUE(a.type_as_string()) << '\n';
    for (typename A::ComponentIndex i; i.is_not_at_end(); ++i)
        std::cout << FORMAT_VALUE(i) << ", " << FORMAT_VALUE(a[i]) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, Uint32 DIM_>
void test_procedural_implementation_of_vector ()
{
    std::cout << "test_procedural_implementation_of_vector<" << type_string_of<Scalar_>() << ',' << FORMAT(DIM_) << ">\n";
}

template <typename Scalar_, Uint32 DIM_>
Scalar_ identity_matrix_generator (ComponentIndex_t<DIM_*DIM_> const &i)
{
    Uint32 row = i.value() / DIM_;
    Uint32 col = i.value() % DIM_;
    return row == col ? Scalar_(1) : Scalar_(0);
}

struct IdentityMatrixGeneratorId { static std::string type_as_string () { return "IdentityMatrixGenerator"; } };

template <typename Scalar_, typename BasedVectorSpace_>
void test_procedural_identity_tensor ()
{
    typedef Typle_t<BasedVectorSpace_,typename DualOf_f<BasedVectorSpace_>::T> FactorTyple;
    typedef TensorProductOfBasedVectorSpaces_c<FactorTyple> TensorProduct;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<TensorProduct>::V,
                                 identity_matrix_generator<Scalar_,DimensionOf_f<BasedVectorSpace_>::V>,
                                 IdentityMatrixGeneratorId> ComponentGenerator;
    typedef ImplementationOf_t<TensorProduct,Scalar_,UseProceduralArray_t<ComponentGenerator> > IdentityTensor;
    IdentityTensor identity_tensor;
    std::cout << "test_procedural_identity_tensor<" << type_string_of<Scalar_>() << ',' << type_string_of<BasedVectorSpace_>() << ">\n";
    std::cout << FORMAT_VALUE(type_string_of<IdentityTensor>()) << '\n';
    std::cout << FORMAT_VALUE(identity_tensor) << '\n';

    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_> Vector;
    Vector v(Static<WithoutInitialization>::SINGLETON);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        v[i] = static_cast<float>(i.value());

    AbstractIndex_c<'p'> p;
    AbstractIndex_c<'q'> q;
    AbstractIndex_c<'r'> r;
    std::cout << FORMAT_VALUE(v) << '\n';
    std::cout << FORMAT_VALUE(identity_tensor(p*q)*v(q)) << '\n';
    std::cout << FORMAT_VALUE(identity_tensor(p*q)*identity_tensor(q*r)) << '\n';

    std::cout << '\n';
}

template <typename Scalar_, typename VectorSpace_>
void test_standard_euclidean_inner_product ()
{
    std::cout << "test_standard_euclidean_inner_product<" << type_string_of<Scalar_>() << ',' << type_string_of<VectorSpace_>() << ">\n";
    typedef typename InnerProduct_f<BasedVectorSpace_c<VectorSpace_,OrthonormalBasis_c<X> >,StandardInnerProduct,Scalar_>::T InnerProduct;
    InnerProduct g;
    std::cout << FORMAT_VALUE(g) << '\n';
    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    std::cout << FORMAT_VALUE(g(P).split(P,i*j)) << '\n';
    std::cout << FORMAT_VALUE(g.split(i*j)) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, Uint32 ORDER_, typename BasedVectorSpace_, typename InnerProductId_>
void test_tensor_power_of_inner_product ()
{
    std::cout << "test_tensor_power_of_inner_product<" << type_string_of<Scalar_>() << ',' << FORMAT(ORDER_) << ',' << type_string_of<BasedVectorSpace_>() << ">\n";
    typedef typename InnerProduct_f<typename TensorPowerOfBasedVectorSpace_f<ORDER_,BasedVectorSpace_>::T,
                                    typename TensorPower_f<ORDER_,InnerProductId_>::T,
                                    Scalar_>::T InnerProduct;
    InnerProduct g;
    std::cout << FORMAT_VALUE(g) << '\n';
    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    std::cout << FORMAT_VALUE(g(P).split(P,i*j)) << '\n';
    std::cout << FORMAT_VALUE(g.split(i*j)) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, typename VectorSpace_>
void test_euclidean_embedding_of_standard_euclidean_space ()
{
    std::cout << "test_euclidean_embedding_of_standard_euclidean_space<" << type_string_of<Scalar_>() << ',' << type_string_of<VectorSpace_>() << ">\n";
    typedef typename EuclideanEmbedding_f<BasedVectorSpace_c<VectorSpace_,OrthonormalBasis_c<Generic> >,StandardInnerProduct,Scalar_>::T EuclideanEmbedding;
    EuclideanEmbedding e;
    std::cout << FORMAT_VALUE(e) << '\n';
    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    std::cout << FORMAT_VALUE(e(P).split(P,i*j)) << '\n';
    std::cout << FORMAT_VALUE(e.split(i*j)) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, Uint32 ORDER_, typename BasedVectorSpace_, typename InnerProductId_>
void test_tensor_power_of_euclidean_embedding ()
{
    std::cout << "test_tensor_power_of_euclidean_embedding<" << type_string_of<Scalar_>() << ',' << FORMAT(ORDER_) << ',' << type_string_of<BasedVectorSpace_>() << ">\n";
    typedef typename EuclideanEmbedding_f<typename TensorPowerOfBasedVectorSpace_f<ORDER_,BasedVectorSpace_>::T,
                                          typename TensorPower_f<ORDER_,InnerProductId_>::T,
                                          Scalar_>::T EuclideanEmbedding;
    EuclideanEmbedding e;
    std::cout << FORMAT_VALUE(e) << '\n';
    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    std::cout << FORMAT_VALUE(e(P).split(P,i*j)) << '\n';
    std::cout << FORMAT_VALUE(e.split(i*j)) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, Uint32 ORDER_, Uint32 DIMENSION_>
void test_sym ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,DIMENSION_,X>,Basis_c<X> > BasedVectorSpace;
    std::cout << "testing Sym_f<" << type_string_of<Scalar_>() << ',' << ORDER_ << ',' << DIMENSION_ << ">\n";
    typedef typename Sym_f<ORDER_,BasedVectorSpace,Scalar_>::T Sym;
    Sym sym;
    std::cout << FORMAT_VALUE(sym) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, Uint32 ORDER_, Uint32 DIMENSION_>
void test_alt ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,DIMENSION_,X>,Basis_c<X> > BasedVectorSpace;
    std::cout << "testing Alt_f<" << type_string_of<Scalar_>() << ',' << ORDER_ << ',' << DIMENSION_ << ">\n";
    typedef typename Alt_f<ORDER_,BasedVectorSpace,Scalar_>::T Alt;
    Alt alt;
    std::cout << FORMAT_VALUE(alt) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, typename EmbeddableAsTensorProduct_>
void test_split_index_to_index_order_1 ()
{
    typedef typename AS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(EmbeddableAsTensorProduct_)::TensorProductOfBasedVectorSpaces TensorProduct;
    STATIC_ASSERT(OrderOf_f<TensorProduct>::V == 1, UNSPECIFIED_MESSAGE);
    std::cout << "test_split_index_to_index_order_1<" + type_string_of<Scalar_>() + ',' + type_string_of<EmbeddableAsTensorProduct_>() + ">\n";
    typedef ImplementationOf_t<EmbeddableAsTensorProduct_,float> S;
    S s(Static<WithoutInitialization>::SINGLETON);
    for (typename S::ComponentIndex i; i.is_not_at_end(); ++i)
        s[i] = static_cast<Scalar_>(i.value() + 1);
    std::cout << FORMAT_VALUE(s) << '\n';
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    std::cout << FORMAT_VALUE(s(i).split(i,Typle_t<>()*k)) << '\n';
    std::cout << FORMAT_VALUE(s(i).split(i,j)) << '\n';
    std::cout << FORMAT_VALUE(s(i).split(i,j).split(j,Typle_t<>()*k)) << '\n';
    std::cout << "the following should be exactly zero\n";
    std::cout << FORMAT_VALUE(s(i).split(i,Typle_t<>()*k) - s(i).split(i,j).split(j,Typle_t<>()*k)) << '\n';
    std::cout << "the following should be exactly zero\n";
    std::cout << FORMAT_VALUE(s(i).split(i,j) - s(i).split(i,Typle_t<>()*k).bundle(Typle_t<>()*k,TensorProduct(),j)) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, typename EmbeddableAsTensorProduct_>
void test_split_index_to_index_order_2 ()
{
    typedef typename AS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(EmbeddableAsTensorProduct_)::TensorProductOfBasedVectorSpaces TensorProduct;
    STATIC_ASSERT(OrderOf_f<TensorProduct>::V == 2, UNSPECIFIED_MESSAGE);
    std::cout << "test_split_index_to_index_order_2<" + type_string_of<Scalar_>() + ',' + type_string_of<EmbeddableAsTensorProduct_>() + ">\n";
    typedef ImplementationOf_t<EmbeddableAsTensorProduct_,float> S;
    S s(Static<WithoutInitialization>::SINGLETON);
    for (typename S::ComponentIndex i; i.is_not_at_end(); ++i)
        s[i] = static_cast<Scalar_>(i.value() + 1);
    std::cout << FORMAT_VALUE(s) << '\n';
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'l'> l;
    std::cout << FORMAT_VALUE(s(i).split(i,k*l)) << '\n';
    std::cout << FORMAT_VALUE(s.split(k*l)) << '\n';
    std::cout << FORMAT_VALUE(s(i).split(i,j)) << '\n';
    std::cout << FORMAT_VALUE(s(i).split(i,j).split(j,k*l)) << '\n';
    std::cout << "the following should be exactly zero\n";
    std::cout << FORMAT_VALUE(s(i).split(i,k*l) - s(i).split(i,j).split(j,k*l)) << '\n';
    std::cout << "the following should be exactly zero\n";
    std::cout << FORMAT_VALUE(s.split(k*l) - s(i).split(i,j).split(j,k*l)) << '\n';
    std::cout << "the following should be exactly zero\n";
    std::cout << FORMAT_VALUE(s(i).split(i,j) - s(i).split(i,k*l).bundle(k*l,TensorProduct(),j)) << '\n';
    std::cout << "the following should be exactly zero\n";
    std::cout << FORMAT_VALUE(s(i).split(i,j) - s.split(k*l).bundle(k*l,TensorProduct(),j)) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, typename EmbeddableAsTensorProduct_>
void test_split_index_to_index_order_3 ()
{
    typedef typename AS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(EmbeddableAsTensorProduct_)::TensorProductOfBasedVectorSpaces TensorProduct;
    STATIC_ASSERT(OrderOf_f<TensorProduct>::V == 3, UNSPECIFIED_MESSAGE);
    std::cout << "test_split_index_to_index_order_3<" + type_string_of<Scalar_>() + ',' + type_string_of<EmbeddableAsTensorProduct_>() + ">\n";
    typedef ImplementationOf_t<EmbeddableAsTensorProduct_,float> S;
    S s(Static<WithoutInitialization>::SINGLETON);
    for (typename S::ComponentIndex i; i.is_not_at_end(); ++i)
        s[i] = static_cast<Scalar_>(i.value() + 1);
    std::cout << FORMAT_VALUE(s) << '\n';
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'l'> l;
    AbstractIndex_c<'m'> m;
    std::cout << FORMAT_VALUE(s(i).split(i,k*l*m)) << '\n';
    std::cout << FORMAT_VALUE(s(i).split(i,j)) << '\n';
    std::cout << FORMAT_VALUE(s(i).split(i,j).split(j,k*l*m)) << '\n';
    std::cout << "the following should be exactly zero\n";
    std::cout << FORMAT_VALUE(s(i).split(i,k*l*m) - s(i).split(i,j).split(j,k*l*m)) << '\n';
    std::cout << "the following should be exactly zero\n";
    std::cout << FORMAT_VALUE(s(i).split(i,j) - s(i).split(i,k*l*m).bundle(k*l*m,TensorProduct(),j)) << '\n';
    std::cout << '\n';
}

template <Uint32 DIM_>
void test_tensor_printing_0 ()
{
    typedef VectorSpace_c<RealField,DIM_,X> VSX;
    typedef Basis_c<X> BasisX;
    typedef BasedVectorSpace_c<VSX,BasisX> BasedX;
    test_tensor_printing<BasedX,1>(std::cout);
    test_tensor_printing<BasedX,2>(std::cout);
    test_tensor_printing<BasedX,3>(std::cout);
    test_tensor_printing<BasedX,4>(std::cout);
    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////////////////////
// 0
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_Basis_c ();
void test_VectorSpace_c ();
void test_BasedVectorSpace_c ();
void test_TensorProduct_c ();
void test_TensorProductOfVectorSpaces_c ();
void test_TensorProductOfBases_c ();
void test_BasedTensorProductStuff ();
void test_SymmetricPower_c ();
void test_SymmetricPowerOfVectorSpace_c ();
void test_SymmetricPowerOfBasis_c ();
void test_BasedSymmetricPowerStuff ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 1
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_vector_contraction ();
void test_tensor_contraction ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 2
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_scalar2tensor ();
void test_wedge ();
void test_vee ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 3
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_multiindex_sort ();
void test_multiindex_sort_2 ();
void test_zero_dimensional_vector_space ();
void test_Diagonal2Tensor ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 4
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_vector_power_thingy ();
void test_a_bunch_of_stuff ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 5
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_procedural_stuff ();
void test_inner_product_and_euclidean_stuff ();
void test_products_of_procedural_stuff ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 6
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_pretty_typestringof ();
void test_conceptual_inheritance_graph ();
void test_conceptual_property_accessors ();
void test_sym_0 ();
void test_alt_0 ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 7
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_eigen_map_of_2_tensor ();
void test_split_index_to_index_order_1 ();
void test_split_index_to_index_order_2 ();
void test_split_index_to_index_order_3 ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 8
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_poly_in_0_dim ();
void test_poly_in_1_dim ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 9
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_poly_in_2_dim ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 10
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_poly_in_3_dim ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 11
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_poly_in_4_dim ();
void test_polynomial_serialization ();
void test_polynomial_multiplication ();
void test_tuple_initialization ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 12
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_induced_inner_product ();
void test_direct_sums ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 13
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_zero_vector ();
void test_basis_vectors ();
void test_eval_value ();
void test_direct_sum_of_2tensors ();
void test_direct_sum_of_diagonal2tensors ();
void test_direct_sum_of_inner_products ();

// ///////////////////////////////////////////////////////////////////////////////////////////
// 14
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_embeddable_as_tensor_as_multilinear_form ();
void test_product_of_abstract_indices ();
void test_linear_solve_using_least_squares ();
void test_implementation_of_vector_construction_via_vector_i ();
void test_const_component_arrays ();
void test_identity_embedding ();
void test_diag2tensor_embedding ();
void test_scalar2tensor_embedding ();
void test_exterior_power_embedding ();
void test_symmetric_power_embedding ();
void test_embed_coembed_adjointness ();
void test_type_system_helper_functions ();

#endif // TENSORHELL_ADHOC_TEST_HPP_
