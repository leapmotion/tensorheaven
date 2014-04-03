// ///////////////////////////////////////////////////////////////////////////
// test_interop_eigen_euclideanlyembedded.hpp by Victor Dods, created 2013/04/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(TEST_INTEROP_EIGEN_EUCLIDEANLYEMBEDDED_HPP_)
#define TEST_INTEROP_EIGEN_EUCLIDEANLYEMBEDDED_HPP_

#include "randomize.hpp"
#include "tenh/expressiontemplate_eval.hpp"
#include "tenh/interop/eigen.hpp"
#include "tenh/tensor2.hpp"
#include "tenh/tensor2antisymmetric.hpp"
#include "tenh/tensor2diagonal.hpp"
#include "tenh/tensor2symmetric.hpp"
#include "tenh/vector.hpp"
#include "test.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

namespace Lvd {
namespace TestSystem {

struct Directory;

} // end of namespace TestSystem
} // end of namespace Lvd

namespace Test {
namespace InteropEigen {
namespace EuclideanlyEmbedded {

using namespace Lvd;
using namespace std;
using namespace TestSystem;

template <typename Tensor2Type>
void test_euclideanly_embedded_EigenMatrix_from_and_to (Context const &context)
{
    typedef typename Tensor2Type::Scalar Scalar;
    typedef typename Tenh::AssociatedFloatingPointType_t<Scalar>::T AssociatedFloatingPointType;

    Tensor2Type t(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    // TODO: real random value
    for (typename Tensor2Type::Index i; i.is_not_at_end(); ++i)
        t[i] = i.value() + 1;

    typedef Eigen::Matrix<typename Tensor2Type::Scalar,Tensor2Type::Factor1::DIM,Tensor2Type::Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    EigenMatrix m(Tenh::euclideanly_embedded_EigenMatrix_from(t));

    Tensor2Type munged_t(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Tenh::euclideanly_embedded_EigenMatrix_to(m, munged_t);

    Tenh::TypedIndex_t<Tensor2Type,'i'> i;
//     std::cerr << FORMAT_VALUE(t) << FORMAT_VALUE(munged_t) << '\n';
    assert_leq((munged_t(i) - t(i)).squared_norm(), numeric_limits<AssociatedFloatingPointType>::epsilon());
}

template <typename Tensor2Type>
void add_euclidean_embedding_tests_for_Tensor2Type (Directory *parent)
{
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     Tenh::type_string_of<Tensor2Type>(),
                                     test_euclideanly_embedded_EigenMatrix_from_and_to<Tensor2Type>,
                                     RESULT_NO_ERROR);
}

template <typename Scalar, Uint32 DIM>
void add_embedding_tests_1 (Directory *parent)
{
    typedef Tenh::Vector_t<Scalar,DIM> V;
    typedef Tenh::Tensor2_t<V,V> T;
    typedef Tenh::Tensor2Antisymmetric_t<V> A;
    typedef Tenh::Tensor2Diagonal_t<V,V> D;
    typedef Tenh::Tensor2Symmetric_t<V> S;

    add_euclidean_embedding_tests_for_Tensor2Type<A>(parent);
    add_euclidean_embedding_tests_for_Tensor2Type<S>(parent);
//     add_euclidean_embedding_tests_for_Tensor2Type<Tenh::Tensor2Antisymmetric_t<A>>(parent); // TODO: allow zero-dimensional vector spaces
    add_euclidean_embedding_tests_for_Tensor2Type<Tenh::Tensor2Antisymmetric_t<S>>(parent);
    add_euclidean_embedding_tests_for_Tensor2Type<Tenh::Tensor2Symmetric_t<A>>(parent);
    add_euclidean_embedding_tests_for_Tensor2Type<Tenh::Tensor2Symmetric_t<S>>(parent);
}

template <typename Scalar, Uint32 DIM1, Uint32 DIM2>
void add_embedding_tests_2 (Directory *parent)
{
    typedef Tenh::Vector_t<Scalar,DIM1> V1;
    typedef Tenh::Vector_t<Scalar,DIM2> V2;
    typedef Tenh::Tensor2_t<V1,V2> T;
    typedef Tenh::Tensor2Diagonal_t<V1,V2> D;

    add_euclidean_embedding_tests_for_Tensor2Type<T>(parent);
    add_euclidean_embedding_tests_for_Tensor2Type<D>(parent);
}

// the point of splitting it up this way is to parallelize the intensive template compilation
void AddTests (Lvd::TestSystem::Directory *parent);
void AddTests0 (Lvd::TestSystem::Directory *parent);
void AddTests1 (Lvd::TestSystem::Directory *parent);
void AddTests2 (Lvd::TestSystem::Directory *parent);
void AddTests3 (Lvd::TestSystem::Directory *parent);
void AddTests4 (Lvd::TestSystem::Directory *parent);
void AddTests5 (Lvd::TestSystem::Directory *parent);
void AddTests6 (Lvd::TestSystem::Directory *parent);
void AddTests7 (Lvd::TestSystem::Directory *parent);

} // end of namespace EuclideanlyEmbedded
} // end of namespace InteropEigen
} // end of namespace Test

#endif // !defined(TEST_INTEROP_EIGEN_EUCLIDEANLYEMBEDDED_HPP_)