// ///////////////////////////////////////////////////////////////////////////
// test_euclideanembeddinginverse.cpp by Victor Dods, created 2013/04/18
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_euclideanembeddinginverse.hpp"

#include <iostream> // TEMP
#include <string>

#include "tenh/euclideanembedding.hpp"
#include "tenh/expressiontemplate_eval.hpp"
#include "tenh/tensor2.hpp"
#include "tenh/tensor2antisymmetric.hpp"
#include "tenh/tensor2diagonal.hpp"
#include "tenh/tensor2symmetric.hpp"
#include "tenh/vector.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace EuclideanEmbeddingInverse {

// this ensures that EuclideanEmbeddingInverse_t<VectorType> is the actual inverse of EuclideanEmbedding_t<VectorType> (one composition)
template <typename VectorType>
void e_inv_e_composition (Context const &context)
{
    typedef typename VectorType::Scalar Scalar;
    typedef typename Tenh::AssociatedFloatingPointType_t<Scalar>::T AssociatedFloatingPointType;
    typedef VectorType V;

    Tenh::TypedIndex_t<typename V::WithStandardEuclideanBasis,'p'> p;
    Tenh::TypedIndex_t<V,'q'> q;
    Tenh::TypedIndex_t<typename V::WithStandardEuclideanBasis,'r'> r;
    Tenh::TypedIndex_t<V,'s'> s;
    Tenh::EuclideanEmbedding_t<V> e;
    Tenh::EuclideanEmbeddingInverse_t<V> e_inv;

    // NOTE: the following comments contain an important case motivating compile error message improvement
//     Tenh::Tensor2Diagonal_t<typename V::WithStandardEuclideanBasis,
//                             typename V::WithStandardEuclideanBasis> inner_product_on_V(1);
//     std::cerr << "\tEuclideanEmbeddingInverse - e_inv_e_composition<" << Tenh::TypeStringOf_t<V>::eval() << '\n';
//     std::cerr << FORMAT_VALUE(e_inv(q|r)*e(r|s) - identity_on_V(q|s)) << '\n';
    // the following line produces THE worst compile error i have ever seen (over 20000 lines in GCC)
//     std::cerr << FORMAT_VALUE(e_inv(q|r)*e(r|s) - inner_product_on_V(q|s)) << '\n';
    // the following line compiles
//     std::cerr << FORMAT_VALUE(e(p|q)*e_inv(q|r) - inner_product_on_V(p|r)) << '\n';

    // should really be InnerProduct_t<V> directly, once InnerProduct_t is implemented as a bona-fide Tensor_i.
    typedef Tenh::Tensor2Diagonal_t<V,V> D;
    D inner_product_inverse_on_V(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    // manually populate the result tensor
    Lvd::Meta::Assert<(D::Index::COMPONENT_COUNT == V::Index::COMPONENT_COUNT)>();
    // NOTE: this is only the inverse if the inner product is diagonal (which it currently is)
    for (typename D::Index i; i.is_not_at_end(); ++i)
        inner_product_inverse_on_V[i] = Scalar(1) / Tenh::InnerProduct_t<V,typename V::Basis>::component(typename V::Index(i.value()));

//     std::cerr << FORMAT_VALUE(e_inv(q|r)*e(r|s)) << '\n';
//     std::cerr << FORMAT_VALUE(inner_product_inverse_on_V) << '\n';
    // NOTE: the squared norm maybe isn't the correct measurement of error, but screw it for now.
    assert_leq((e_inv(q|r)*e(r|s) - inner_product_inverse_on_V(q|s)).squared_norm(),
               numeric_limits<AssociatedFloatingPointType>::epsilon());
}

// this ensures that EuclideanEmbeddingInverse_t<VectorType> is the actual inverse of EuclideanEmbedding_t<VectorType> (one composition)
template <typename VectorType>
void e_e_inv_composition (Context const &context)
{
    typedef typename VectorType::Scalar Scalar;
    typedef typename Tenh::AssociatedFloatingPointType_t<Scalar>::T AssociatedFloatingPointType;
    typedef VectorType V;

    Tenh::TypedIndex_t<typename V::WithStandardEuclideanBasis,'p'> p;
    Tenh::TypedIndex_t<V,'q'> q;
    Tenh::TypedIndex_t<typename V::WithStandardEuclideanBasis,'r'> r;
    Tenh::TypedIndex_t<V,'s'> s;
    Tenh::EuclideanEmbedding_t<V> e;
    Tenh::EuclideanEmbeddingInverse_t<V> e_inv;
    // TODO: could be a "constant tensor" (see various implementations of EuclideanEmbedding_t)
    Tenh::Tensor2Diagonal_t<typename V::WithStandardEuclideanBasis,
                            typename V::WithStandardEuclideanBasis> identity_on_V_WithStandardEuclideanBasis(1);

//     std::cerr << FORMAT_VALUE(e(p|q)*e_inv(q|r)) << '\n';
//     std::cerr << FORMAT_VALUE(identity_on_V_WithStandardEuclideanBasis) << '\n';
    // NOTE: the squared norm maybe isn't the correct measurement of error, but screw it for now.
    assert_leq((e(p|q)*e_inv(q|r) - identity_on_V_WithStandardEuclideanBasis(p|r)).squared_norm(),
               numeric_limits<AssociatedFloatingPointType>::epsilon());
}

template <typename VectorType>
void add_test_for_each_composition (Directory *parent)
{
    Directory *d = new Directory(Tenh::TypeStringOf_t<VectorType>::eval(), parent);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(d, "e_inv_e_composition", e_inv_e_composition<VectorType>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(d, "e_e_inv_composition", e_e_inv_composition<VectorType>, RESULT_NO_ERROR);
}

// vector types and tensor types having identical factor types
template <typename Scalar, Uint32 DIM>
void add_tests (Directory *parent)
{
    typedef Tenh::Vector_t<Scalar,DIM> V;
    typedef Tenh::Tensor2Antisymmetric_t<V,V> A;
    typedef Tenh::Tensor2Symmetric_t<V,V> S;

    add_test_for_each_composition<V>(parent);
    add_test_for_each_composition<A>(parent);
    add_test_for_each_composition<S>(parent);
//     add_test_for_each_composition<Tenh::Tensor2Antisymmetric_t<A> >(parent); // TODO: allow zero-dimensional vector spaces
    add_test_for_each_composition<Tenh::Tensor2Antisymmetric_t<S> >(parent);
    add_test_for_each_composition<Tenh::Tensor2Symmetric_t<A> >(parent);
    add_test_for_each_composition<Tenh::Tensor2Symmetric_t<S> >(parent);
}

void AddTests (Directory *parent)
{
    Directory *euclidean_embedding_inverse = new Directory("EuclideanEmbeddingInverse", parent);

    {
        Directory *d = new Directory("verify", euclidean_embedding_inverse);

//         add_tests<float,1>(d); // TODO: allow 0-dimensional vector spaces
        add_tests<float,2>(d);
        add_tests<float,3>(d);
        add_tests<float,4>(d);

//         add_tests<double,1>(d); // TODO: allow 0-dimensional vector spaces
        add_tests<double,2>(d);
        add_tests<double,3>(d);
        add_tests<double,4>(d);
    }
}

} // end of namespace EuclideanEmbeddingInverse
} // end of namespace Test