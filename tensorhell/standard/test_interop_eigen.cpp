// ///////////////////////////////////////////////////////////////////////////
// test_interop_eigen.cpp by Victor Dods, created 2013/04/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_interop_eigen.hpp"

#include "randomize.hpp"
#include "tenh/expressiontemplate_eval.hpp"
#include "tenh/interop/eigen.hpp"
#include "tenh/interop/eigen_selfadjointeigendecomp.hpp"
#include "tenh/interop/eigen_svd.hpp"
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
namespace InteropEigen {

/*
// do eigen decomposition in the "packed with symmetries" form, then unpack,
// and compare to unpacked decomposition.
template <typename Factor>
void test_self_adjoint_eigen_decomp (Context const &context)
{
    typedef Tenh::Tensor2Symmetric_t<Factor> S;
    S s(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    // TODO: real random S value
    for (typename S::Index i; i.is_not_at_end(); ++i)
        s[i] = i.value() + 1;

    typedef Tenh::Tensor2Diagonal_t<typename Factor::WithStandardEuclideanBasis,
                                    typename Factor::WithStandardEuclideanBasis> D;
    typedef Tenh::Tensor2_t<typename Factor::WithStandardEuclideanBasis,Factor> U;
    D eigenvalues(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    U eigenvectors(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    // diagonalize
    diagonalize_Tensor2Symmetric(s, d, u);
    // verify that
}

template <typename Factor>
void add_particular_self_adjoint_eigen_decomp_test (Directory *parent)
{
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     "test_self_adjoint_eigen_decomp<" + Tenh::TypeStringOf_t<Factor>::eval() + '>',
                                     test_self_adjoint_eigen_decomp,
                                     RESULT_NO_ERROR);
}
*/

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

// template <typename Tensor2Type>
// void test_invert_tensor2 (Context const &context)
// {
//     assert_eq(Tensor2Type::Factor1::DIM, Tensor2Type::Factor2::DIM);
//     Tensor2Type t(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
//     Tensor2Type t_inverse(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
//     randomize(t); // will be invertible with full measure
//     Tenh::invert_tensor2(t, t_inverse);
//     std::cerr << FORMAT_VALUE(t) << '\n';
//     std::cerr << FORMAT_VALUE(t_inverse) << '\n';
// }

template <typename Tensor2Type>
void add_euclidean_embedding_tests_for_Tensor2Type (Directory *parent)
{
    Directory *d = new Directory("euclidean embedding of " + Tenh::TypeStringOf_t<Tensor2Type>::eval(), parent);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(d,
                                     "test_euclideanly_embedded_EigenMatrix_from_and_to",
                                     test_euclideanly_embedded_EigenMatrix_from_and_to<Tensor2Type>,
                                     RESULT_NO_ERROR);
}

// template <typename Tensor2Type>
// void add_inversion_test_for_Tensor2Type (Directory *parent)
// {
//     Directory *d = new Directory("inversion of " + Tenh::TypeStringOf_t<Tensor2Type>::eval(), parent);
//
//     LVD_ADD_NAMED_TEST_CASE_FUNCTION(d,
//                                      "test_invert_tensor2",
//                                      test_invert_tensor2<Tensor2Type>,
//                                      RESULT_NO_ERROR);
// }

template <typename Scalar, Uint32 DIM>
void add_tests_1 (Directory *parent)
{
    typedef Tenh::Vector_t<Scalar,DIM> V;
    typedef Tenh::Tensor2Antisymmetric_t<V> A;
    typedef Tenh::Tensor2Symmetric_t<V> S;

    add_euclidean_embedding_tests_for_Tensor2Type<A>(parent);
    add_euclidean_embedding_tests_for_Tensor2Type<S>(parent);
//     add_euclidean_embedding_tests_for_Tensor2Type<Tenh::Tensor2Antisymmetric_t<A> >(parent); // TODO: allow zero-dimensional vector spaces
    add_euclidean_embedding_tests_for_Tensor2Type<Tenh::Tensor2Antisymmetric_t<S> >(parent);
    add_euclidean_embedding_tests_for_Tensor2Type<Tenh::Tensor2Symmetric_t<A> >(parent);
    add_euclidean_embedding_tests_for_Tensor2Type<Tenh::Tensor2Symmetric_t<S> >(parent);

//     add_inversion_test_for_Tensor2Type<A>(parent);
//     add_inversion_test_for_Tensor2Type<S>(parent);
// //     add_inversion_test_for_Tensor2Type<Tenh::Tensor2Antisymmetric_t<A> >(parent); // TODO: allow zero-dimensional vector spaces
//     add_inversion_test_for_Tensor2Type<Tenh::Tensor2Antisymmetric_t<S> >(parent);
//     add_inversion_test_for_Tensor2Type<Tenh::Tensor2Symmetric_t<A> >(parent);
//     add_inversion_test_for_Tensor2Type<Tenh::Tensor2Symmetric_t<S> >(parent);
}

template <typename Scalar, Uint32 DIM1, Uint32 DIM2>
void add_tests_2 (Directory *parent)
{
    typedef Tenh::Vector_t<Scalar,DIM1> V1;
    typedef Tenh::Vector_t<Scalar,DIM2> V2;
    typedef Tenh::Tensor2_t<V1,V2> T;
    typedef Tenh::Tensor2Diagonal_t<V1,V2> D;

    add_euclidean_embedding_tests_for_Tensor2Type<T>(parent);
    add_euclidean_embedding_tests_for_Tensor2Type<D>(parent);
}

void AddTests (Directory *parent)
{
    Directory *interop_eigen = new Directory("interop_eigen", parent);

    add_tests_1<float,2>(interop_eigen);
    add_tests_1<float,3>(interop_eigen);
    add_tests_1<float,4>(interop_eigen);

    add_tests_1<double,2>(interop_eigen);
    add_tests_1<double,3>(interop_eigen);
    add_tests_1<double,4>(interop_eigen);

    add_tests_2<float,1,1>(interop_eigen);
    add_tests_2<float,1,3>(interop_eigen);
    add_tests_2<float,1,4>(interop_eigen);
//     add_tests_2<float,3,1>(interop_eigen); // this causes a static assert in Eigen
    add_tests_2<float,3,3>(interop_eigen);
    add_tests_2<float,3,4>(interop_eigen);
//     add_tests_2<float,4,1>(interop_eigen); // this causes a static assert in Eigen
    add_tests_2<float,4,3>(interop_eigen);
    add_tests_2<float,4,4>(interop_eigen);

    add_tests_2<double,1,1>(interop_eigen);
    add_tests_2<double,1,3>(interop_eigen);
    add_tests_2<double,1,4>(interop_eigen);
//     add_tests_2<double,3,1>(interop_eigen); // this causes a static assert in Eigen
    add_tests_2<double,3,3>(interop_eigen);
    add_tests_2<double,3,4>(interop_eigen);
//     add_tests_2<double,4,1>(interop_eigen); // this causes a static assert in Eigen
    add_tests_2<double,4,3>(interop_eigen);
    add_tests_2<double,4,4>(interop_eigen);
}

} // end of namespace InteropEigen
} // end of namespace Test