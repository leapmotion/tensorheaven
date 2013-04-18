// ///////////////////////////////////////////////////////////////////////////
// test_euclideanembeddinginverse.cpp by Victor Dods, created 2013/04/18
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_euclideanembeddinginverse.hpp"

#include <iostream> // TEMP
#include <string>

#include "tenh/euclideanembedding.hpp"
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

// this ensures that EuclideanEmbeddingInverse_t<VectorType> is the actual inverse of EuclideanEmbedding_t<VectorType>
template <typename VectorType>
void verify_on_vector_type (Context const &context)
{
    //typedef typename VectorType::Scalar Scalar;
    typedef VectorType V;

    Tenh::TypedIndex_t<typename V::WithStandardEuclideanBasis,'p'> p;
    Tenh::TypedIndex_t<V,'q'> q;
    Tenh::TypedIndex_t<typename V::WithStandardEuclideanBasis,'r'> r;
    Tenh::TypedIndex_t<V,'s'> s;
    Tenh::EuclideanEmbedding_t<V> e;
    Tenh::EuclideanEmbeddingInverse_t<V> e_inv;
    Tenh::Tensor2Diagonal_t<V,V> identity_on_V(1); // TODO: could be a "constant tensor" (see various implementations of EuclideanEmbedding_t)
    // should really be InnerProduct_t<V>, once InnerProduct_t is implemented as a bona-fide Tensor_i.
//     Tenh::Tensor2Diagonal_t<typename V::WithStandardEuclideanBasis,
//                             typename V::WithStandardEuclideanBasis> identity_on_V_WithStandardEuclideanBasis(1);
    
    std::cerr << "\tEuclideanEmbeddingInverse - verify_on_vector_type<" << Tenh::TypeStringOf_t<V>::eval() << '\n';
    std::cerr << FORMAT_VALUE(e_inv(q|r)*e(r|s) - identity_on_V(q|s)) << '\n';
    // the following line produces THE worst compile error i have ever seen (over 20000 lines in GCC)
//     std::cerr << FORMAT_VALUE(e_inv(q|r)*e(r|s) - identity_on_V_WithStandardEuclideanBasis(q|s)) << '\n';
    // the following line is correct
//     std::cerr << FORMAT_VALUE(e(p|q)*e_inv(q|r) - identity_on_V_WithStandardEuclideanBasis(p|r)) << '\n';
}

template <typename VectorType>
void add_test_for_verify_on_vector_type (Directory *parent)
{
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     Tenh::TypeStringOf_t<VectorType>::eval(),
                                     verify_on_vector_type<VectorType>,
                                     RESULT_NO_ERROR);
}

// vector types and tensor types having identical factor types
template <typename Scalar, Uint32 DIM>
void add_tests_for_verify_on_vector_type (Directory *parent)
{
    typedef Tenh::Vector_t<Scalar,DIM> V;
    typedef Tenh::Tensor2Antisymmetric_t<V,V> A;
    typedef Tenh::Tensor2Symmetric_t<V,V> S;
    
    add_test_for_verify_on_vector_type<V>(parent);
    add_test_for_verify_on_vector_type<A>(parent);
    add_test_for_verify_on_vector_type<S>(parent);
//     add_test_for_verify_on_vector_type<Tenh::Tensor2Antisymmetric_t<A> >(parent); // TODO: allow zero-dimensional vector spaces
    add_test_for_verify_on_vector_type<Tenh::Tensor2Antisymmetric_t<S> >(parent);
    add_test_for_verify_on_vector_type<Tenh::Tensor2Symmetric_t<A> >(parent);
    add_test_for_verify_on_vector_type<Tenh::Tensor2Symmetric_t<S> >(parent);
}

void AddTests (Directory *parent)
{
    Directory *euclidean_embedding_inverse = new Directory("EuclideanEmbeddingInverse", parent);
    
    {
        Directory *d = new Directory("verify_as_vector", euclidean_embedding_inverse);
        
//         add_tests_for_verify_on_vector_type<float,1>(d); // TODO: allow 0-dimensional vector spaces
        add_tests_for_verify_on_vector_type<float,2>(d);
        add_tests_for_verify_on_vector_type<float,3>(d);
        add_tests_for_verify_on_vector_type<float,4>(d);
        
//         add_tests_for_verify_on_vector_type<double,1>(d); // TODO: allow 0-dimensional vector spaces
        add_tests_for_verify_on_vector_type<double,2>(d);
        add_tests_for_verify_on_vector_type<double,3>(d);
        add_tests_for_verify_on_vector_type<double,4>(d);
    }
}

} // end of namespace EuclideanEmbeddingInverse
} // end of namespace Test