// ///////////////////////////////////////////////////////////////////////////
// test_expressiontemplates.cpp by Ted Nitz, created 2013/04/11
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_expressiontemplates.hpp"

#include <utility> // for std::pair
#include <complex>

#include "tenh/expression_templates.hpp"
#include "tenh/expression_templates_utility.hpp"
#include "tenh/expressiontemplate_eval.hpp"
#include "tenh/meta/typelist.hpp"
#include "tenh/multiindex.hpp"
#include "tenh/tensor2.hpp"
#include "tenh/tensor2antisymmetric.hpp"
#include "tenh/vector.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace ExpressionTemplates {

    template <typename Scalar, Uint32 DIM>
    void test_Tensor2Antisymmetric_t (Context const &context)
    {
        typedef Tenh::Vector_t<Scalar,DIM> Vector;
        typedef Tenh::Tensor2Antisymmetric_t<Vector> Tensor2Antisymmetric;

        Tensor2Antisymmetric a(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
        Tensor2Antisymmetric b(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
        for (typename Tensor2Antisymmetric::Index i; i.is_not_at_end(); ++i)
        {
            a[i] = i.value() + 1;
            b[i] = sqrt(i.value()) + 5;
        }

        float hand_computed_value = 0.0f;
        for (typename Tensor2Antisymmetric::MultiIndex i; i.is_not_at_end(); ++i)
        {
            hand_computed_value += a[i]*b[i];
        }
        
        Tenh::TypedIndex_t<Tensor2Antisymmetric,'i'> i;
        
        assert_eq(hand_computed_value, a(i)*b(i));
        
        Tenh::TypedIndex_t<Vector,'j'> j;
        Tenh::TypedIndex_t<Vector,'k'> k;
        
        assert_eq(hand_computed_value, a(j|k)*b(j|k));
    }

    template <typename Scalar, Uint32 DIM>
    void add_particular_tests (Directory *parent)
    {
        typedef Tenh::Vector_t<Scalar,DIM> V;
        typedef Tenh::Tensor2Antisymmetric_t<V, V> Tensor2Antisymmetric;

        Directory *tensor2antisymmetric
            = new Directory(Tenh::TypeStringOf_t<Tensor2Antisymmetric>::eval(), parent);

        LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2antisymmetric, "test",
            test_Tensor2Antisymmetric_t<Scalar, DIM>, RESULT_NO_ERROR);

    }

    template <typename Scalar>
    void add_particular_tests_for_scalar (Directory *parent)
    {
        add_particular_tests<Scalar,2>(parent);
        add_particular_tests<Scalar,8>(parent);
        add_particular_tests<Scalar,100>(parent);
    }

    void AddTests (Directory *parent)
    {
        Directory *expression_templates = new Directory("ExpressionTemplates", parent);
        add_particular_tests_for_scalar<float>(expression_templates);


    }

} // end of namespace ExpressionTemplates
} // end of namespace Test