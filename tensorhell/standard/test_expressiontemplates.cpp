// ///////////////////////////////////////////////////////////////////////////
// test_expressiontemplates.cpp by Ted Nitz, created 2013/04/11
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_expressiontemplates.hpp"
#include "randomize.hpp"

#include <utility> // for std::pair
#include <complex>

#include "tenh/expression_templates.hpp"
#include "tenh/expression_templates_utility.hpp"
#include "tenh/expressiontemplate_eval.hpp"
#include "tenh/meta/typelist.hpp"
#include "tenh/multiindex.hpp"
#include "tenh/tensor2.hpp"
#include "tenh/tensor2antisymmetric.hpp"
#include "tenh/tensor2symmetric.hpp"
#include "tenh/vector.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace ExpressionTemplates {

template <typename Tensor>
void test_Tensor2_t (Context const &context)
{
    typedef typename Tensor::Factor1 Factor1;
    typedef typename Tensor::Factor2 Factor2;
    typedef typename Tensor::Scalar Scalar;
    
    Tensor a(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Tensor b(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    randomize(a);
    randomize(b);
    
    Tenh::TypedIndex_t<Tensor,'i'> i;
    Tenh::TypedIndex_t<Factor1,'j'> j;
    Tenh::TypedIndex_t<Factor2,'k'> k;
    
    assert_about_eq(static_cast<Scalar>(a(i)*b(i)), static_cast<Scalar>(a(j|k)*b(j|k)));
}

template <typename Scalar, Uint32 DIM>
void add_particular_tests (Directory *parent)
{
    typedef Tenh::Vector_t<Scalar,DIM> V;
    typedef Tenh::Tensor2_t<V, V> Tensor2;
    typedef Tenh::Tensor2Antisymmetric_t<V, V> Tensor2Antisymmetric;
    typedef Tenh::Tensor2Symmetric_t<V, V> Tensor2Symmetric;

    Directory *tensor2
        = new Directory(Tenh::TypeStringOf_t<Tensor2>::eval(), parent);

    Directory *tensor2antisymmetric
        = new Directory(Tenh::TypeStringOf_t<Tensor2Antisymmetric>::eval(), parent);

    Directory *tensor2symmetric
        = new Directory(Tenh::TypeStringOf_t<Tensor2Symmetric>::eval(), parent);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2, "test",
        test_Tensor2_t<Tensor2>, RESULT_NO_ERROR);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2antisymmetric, "test",
        test_Tensor2_t<Tensor2Antisymmetric>, RESULT_NO_ERROR);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2symmetric, "test",
        test_Tensor2_t<Tensor2Symmetric>, RESULT_NO_ERROR);


}

template <typename Scalar>
void add_particular_tests_for_scalar (Directory *parent)
{
    add_particular_tests<Scalar,2>(parent);
    add_particular_tests<Scalar,3>(parent);
    add_particular_tests<Scalar,4>(parent);
    add_particular_tests<Scalar,5>(parent);
    // add_particular_tests<Scalar,6>(parent);
    // add_particular_tests<Scalar,7>(parent);
    // add_particular_tests<Scalar,8>(parent);
    // add_particular_tests<Scalar,9>(parent);
    // add_particular_tests<Scalar,100>(parent);
}

void AddTests (Directory *parent)
{
    Directory *expression_templates = new Directory("ExpressionTemplates", parent);
    add_particular_tests_for_scalar<float>(expression_templates);
    add_particular_tests_for_scalar<double>(expression_templates);
    // add_particular_tests_for_scalar<complex<float> >(expression_templates);
    // add_particular_tests_for_scalar<complex<double> >(expression_templates);



}

} // end of namespace ExpressionTemplates
} // end of namespace Test