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

template <typename Tensor2Type>
void add_euclidean_embedding_tests_for_Tensor2Type (Directory *parent)
{
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     Tenh::TypeStringOf_t<Tensor2Type>::eval(),
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
//     add_euclidean_embedding_tests_for_Tensor2Type<Tenh::Tensor2Antisymmetric_t<A> >(parent); // TODO: allow zero-dimensional vector spaces
    add_euclidean_embedding_tests_for_Tensor2Type<Tenh::Tensor2Antisymmetric_t<S> >(parent);
    add_euclidean_embedding_tests_for_Tensor2Type<Tenh::Tensor2Symmetric_t<A> >(parent);
    add_euclidean_embedding_tests_for_Tensor2Type<Tenh::Tensor2Symmetric_t<S> >(parent);
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

template <typename Tensor2Type, typename InverseTensor2Type>
void test_invert_tensor2 (Context const &context)
{
    typedef typename Tensor2Type::Scalar Scalar;
    typedef typename Tenh::AssociatedFloatingPointType_t<Scalar>::T AssociatedFloatingPointType;
    typedef typename Tensor2Type::Factor1 Factor1;
    typedef typename Tensor2Type::Factor2 Factor2;

    assert_eq(Factor1::dim(), Factor2::dim());
    Tensor2Type t(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    InverseTensor2Type t_inverse(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    randomize(t); // will be invertible with full measure
    Tenh::invert_tensor2(t, t_inverse);
    Tenh::TypedIndex_t<Factor1,'i'> i;
    Tenh::TypedIndex_t<Factor1,'j'> j;
    Tenh::TypedIndex_t<Factor2,'k'> k;
    Tenh::TypedIndex_t<Factor2,'l'> l;

    Tenh::Tensor2Diagonal_t<Factor1,Factor1> identity_on_Factor1(1);
    Tenh::Tensor2Diagonal_t<Factor2,Factor2> identity_on_Factor2(1);
    typedef Tenh::Tensor2Diagonal_t<Factor1,Factor1> D1;
    typedef Tenh::Tensor2Diagonal_t<Factor2,Factor2> D2;
    D1 inner_product_inverse_squared_on_Factor1(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    D2 inner_product_inverse_squared_on_Factor2(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    // manually populate the result tensor
    Lvd::Meta::Assert<(D1::Index::COMPONENT_COUNT == Factor1::Index::COMPONENT_COUNT)>();
    Lvd::Meta::Assert<(D2::Index::COMPONENT_COUNT == Factor2::Index::COMPONENT_COUNT)>();
    // NOTE: this is only the inverse if the inner product is diagonal (which it currently is)
    for (typename D1::Index i; i.is_not_at_end(); ++i)
    {
        inner_product_inverse_squared_on_Factor1[i] =
            Scalar(1) /
            Tenh::sqr(Tenh::InnerProduct_t<Factor1,typename Factor1::Basis>::component(typename Factor1::Index(i.value())));
    }
    for (typename D2::Index i; i.is_not_at_end(); ++i)
    {
        inner_product_inverse_squared_on_Factor2[i] =
            Scalar(1) /
            Tenh::sqr(Tenh::InnerProduct_t<Factor2,typename Factor2::Basis>::component(typename Factor2::Index(i.value())));
    }

//     std::cerr << FORMAT_VALUE(t(i|k)*inner_product_inverse_squared_on_Factor2(k|l)*t_inverse(l|j)) << '\n';
//     std::cerr << FORMAT_VALUE((t(i|k)*inner_product_inverse_squared_on_Factor2(k|l)*t_inverse(l|j) - identity_on_Factor1(i|j)).squared_norm()) << '\n' << '\n';
//     std::cerr << FORMAT_VALUE(t_inverse(k|i)*inner_product_inverse_squared_on_Factor1(i|j)*t(j|l)) << '\n';
//     std::cerr << FORMAT_VALUE((t_inverse(k|i)*inner_product_inverse_squared_on_Factor1(i|j)*t(j|l) - identity_on_Factor2(k|l)).squared_norm()) << '\n';
    assert_leq((t(i|k)*inner_product_inverse_squared_on_Factor2(k|l)*t_inverse(l|j) - identity_on_Factor1(i|j)).squared_norm(), numeric_limits<AssociatedFloatingPointType>::epsilon());
    assert_leq((t_inverse(k|i)*inner_product_inverse_squared_on_Factor1(i|j)*t(j|l) - identity_on_Factor2(k|l)).squared_norm(), numeric_limits<AssociatedFloatingPointType>::epsilon());
}

template <typename Tensor2Type, typename InverseTensor2Type>
void add_inversion_test_for_Tensor2Type (Directory *parent)
{
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     Tenh::TypeStringOf_t<Tensor2Type>::eval(),
                                     test_invert_tensor2<Tensor2Type,InverseTensor2Type>,
                                     RESULT_NO_ERROR);
}

template <typename Scalar, Uint32 DIM>
void add_inversion_tests_1 (Directory *parent)
{
    typedef Tenh::Vector_t<Scalar,DIM> V;
    typedef Tenh::Tensor2_t<V,V> T;
    typedef Tenh::Tensor2Antisymmetric_t<V> A;
    typedef Tenh::Tensor2Diagonal_t<V,V> D;
    typedef Tenh::Tensor2Symmetric_t<V> S;

    add_inversion_test_for_Tensor2Type<T,T>(parent);
    if (V::dim() % 2 == 0) // antisymmetric can only be invertible for even dimension
        add_inversion_test_for_Tensor2Type<A,A>(parent);
    add_inversion_test_for_Tensor2Type<D,D>(parent);
    add_inversion_test_for_Tensor2Type<S,S>(parent);
//     add_inversion_test_for_Tensor2Type<Tenh::Tensor2Antisymmetric_t<A> >(parent); // TODO: allow zero-dimensional vector spaces
    if (S::dim() % 2 == 0) // antisymmetric can only be invertible for even dimension
        add_inversion_test_for_Tensor2Type<Tenh::Tensor2Antisymmetric_t<S>,Tenh::Tensor2Antisymmetric_t<S> >(parent);
    add_inversion_test_for_Tensor2Type<Tenh::Tensor2Symmetric_t<A>,Tenh::Tensor2Symmetric_t<A> >(parent);
    add_inversion_test_for_Tensor2Type<Tenh::Tensor2Symmetric_t<S>,Tenh::Tensor2Symmetric_t<S> >(parent);
}

template <typename Factor1, typename Factor2>
void add_inversion_test_2 (Directory *parent)
{
    add_inversion_test_for_Tensor2Type<Tenh::Tensor2_t<Factor1,Factor2>,Tenh::Tensor2_t<Factor2,Factor1> >(parent);
    add_inversion_test_for_Tensor2Type<Tenh::Tensor2_t<Factor2,Factor1>,Tenh::Tensor2_t<Factor1,Factor2> >(parent);
}

void AddTests (Directory *parent)
{
    Directory *interop_eigen = new Directory("interop_eigen", parent);

    {
        Directory *euclidean_embedding = new Directory("euclideanly_embedded_EigenMatrix_from_and_to", interop_eigen);

        add_embedding_tests_1<float,2>(euclidean_embedding);
        add_embedding_tests_1<float,3>(euclidean_embedding);
        add_embedding_tests_1<float,4>(euclidean_embedding);

        add_embedding_tests_1<double,2>(euclidean_embedding);
        add_embedding_tests_1<double,3>(euclidean_embedding);
        add_embedding_tests_1<double,4>(euclidean_embedding);

        add_embedding_tests_2<float,1,1>(euclidean_embedding);
        add_embedding_tests_2<float,1,3>(euclidean_embedding);
        add_embedding_tests_2<float,1,4>(euclidean_embedding);
    //     add_embedding_tests_2<float,3,1>(euclidean_embedding); // this causes a static assert in Eigen
        add_embedding_tests_2<float,3,3>(euclidean_embedding);
        add_embedding_tests_2<float,3,4>(euclidean_embedding);
    //     add_embedding_tests_2<float,4,1>(euclidean_embedding); // this causes a static assert in Eigen
        add_embedding_tests_2<float,4,3>(euclidean_embedding);
        add_embedding_tests_2<float,4,4>(euclidean_embedding);

        add_embedding_tests_2<double,1,1>(euclidean_embedding);
        add_embedding_tests_2<double,1,3>(euclidean_embedding);
        add_embedding_tests_2<double,1,4>(euclidean_embedding);
    //     add_embedding_tests_2<double,3,1>(euclidean_embedding); // this causes a static assert in Eigen
        add_embedding_tests_2<double,3,3>(euclidean_embedding);
        add_embedding_tests_2<double,3,4>(euclidean_embedding);
    //     add_embedding_tests_2<double,4,1>(euclidean_embedding); // this causes a static assert in Eigen
        add_embedding_tests_2<double,4,3>(euclidean_embedding);
        add_embedding_tests_2<double,4,4>(euclidean_embedding);
    }

    {
        Directory *inversion = new Directory("inversion", interop_eigen);

        add_inversion_tests_1<float,2>(inversion);
        add_inversion_tests_1<float,3>(inversion);
        add_inversion_tests_1<float,4>(inversion);

        add_inversion_tests_1<double,2>(inversion);
        add_inversion_tests_1<double,3>(inversion);
        add_inversion_tests_1<double,4>(inversion);

        // add tests for factors having matching dimensions
        typedef Tenh::Vector_t<float,1> Float1;          // dim = 1
        typedef Tenh::Vector_t<float,2> Float2;          // dim = 2
        typedef Tenh::Vector_t<float,3> Float3;          // dim = 3
        typedef Tenh::Vector_t<float,4> Float4;          // dim = 4
        typedef Tenh::Vector_t<float,5> Float5;          // dim = 5
        typedef Tenh::Tensor2_t<Float1,Float1> Float1x1; // dim = 1
        typedef Tenh::Tensor2_t<Float1,Float2> Float1x2; // dim = 2
        typedef Tenh::Tensor2_t<Float1,Float3> Float1x3; // dim = 3
        typedef Tenh::Tensor2_t<Float2,Float1> Float2x1; // dim = 2
        typedef Tenh::Tensor2_t<Float2,Float3> Float2x3; // dim = 6
        typedef Tenh::Tensor2_t<Float2,Float5> Float2x5; // dim = 10
        typedef Tenh::Tensor2_t<Float3,Float1> Float3x1; // dim = 3
        typedef Tenh::Tensor2_t<Float3,Float2> Float3x2; // dim = 6
        typedef Tenh::Tensor2Antisymmetric_t<Float2> A2; // dim = 1
        typedef Tenh::Tensor2Antisymmetric_t<Float3> A3; // dim = 3
        typedef Tenh::Tensor2Antisymmetric_t<Float4> A4; // dim = 6
        typedef Tenh::Tensor2Antisymmetric_t<Float5> A5; // dim = 10
        typedef Tenh::Tensor2Symmetric_t<Float1> S1;     // dim = 1
        typedef Tenh::Tensor2Symmetric_t<Float2> S2;     // dim = 3
        typedef Tenh::Tensor2Symmetric_t<Float3> S3;     // dim = 6
        typedef Tenh::Tensor2Symmetric_t<Float4> S4;     // dim = 10

        // dim = 1
        add_inversion_test_2<Float1,Float1x1>(inversion);
        add_inversion_test_2<Float1,A2>(inversion);
        add_inversion_test_2<Float1,S1>(inversion);
        add_inversion_test_2<Float1x1,A2>(inversion);
        add_inversion_test_2<Float1x1,S1>(inversion);
        add_inversion_test_2<A2,S1>(inversion);

        // dim = 2
        add_inversion_test_2<Float2,Float1x2>(inversion);
        add_inversion_test_2<Float2,Float2x1>(inversion);
        add_inversion_test_2<Float1x2,Float2x1>(inversion);

        // dim = 3
        add_inversion_test_2<Float3,Float1x3>(inversion);
        add_inversion_test_2<Float3,Float3x1>(inversion);
        add_inversion_test_2<Float3,A3>(inversion);
        add_inversion_test_2<Float3,S2>(inversion);
        add_inversion_test_2<Float1x3,Float3x1>(inversion);
        add_inversion_test_2<Float1x3,A3>(inversion);
        add_inversion_test_2<Float1x3,S2>(inversion);
        add_inversion_test_2<A3,S2>(inversion);

        // dim = 6
        add_inversion_test_2<Float2x3,Float3x2>(inversion);
        add_inversion_test_2<Float2x3,A4>(inversion);
        add_inversion_test_2<Float2x3,S3>(inversion);
        add_inversion_test_2<Float3x2,A4>(inversion);
        add_inversion_test_2<Float3x2,S3>(inversion);
        add_inversion_test_2<A4,S3>(inversion);

        // dim = 10
        add_inversion_test_2<Float2x5,A5>(inversion);
        add_inversion_test_2<Float2x5,S4>(inversion);
        add_inversion_test_2<A5,S4>(inversion);
    }
}

} // end of namespace InteropEigen
} // end of namespace Test