// ///////////////////////////////////////////////////////////////////////////
// test_interop_eigen_inversion.hpp by Victor Dods, created 2013/04/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(TEST_INTEROP_EIGEN_INVERSION_HPP_)
#define TEST_INTEROP_EIGEN_INVERSION_HPP_

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
namespace Inversion {

using namespace Lvd;
using namespace std;
using namespace TestSystem;

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
    Assert<(D1::Index::COMPONENT_COUNT == Factor1::Index::COMPONENT_COUNT)>();
    Assert<(D2::Index::COMPONENT_COUNT == Factor2::Index::COMPONENT_COUNT)>();
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
                                     Tenh::type_string_of<Tensor2Type>(),
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

void AddTests (Lvd::TestSystem::Directory *parent);
void AddTests0 (Lvd::TestSystem::Directory *parent);
void AddTests1 (Lvd::TestSystem::Directory *parent);
void AddTests2 (Lvd::TestSystem::Directory *parent);
void AddTests3 (Lvd::TestSystem::Directory *parent);
void AddTests4 (Lvd::TestSystem::Directory *parent);
void AddTests5 (Lvd::TestSystem::Directory *parent);
void AddTests6 (Lvd::TestSystem::Directory *parent);
void AddTests7 (Lvd::TestSystem::Directory *parent);

} // end of namespace Inversion
} // end of namespace InteropEigen
} // end of namespace Test

#endif // !defined(TEST_INTEROP_EIGEN_INVERSION_HPP_)