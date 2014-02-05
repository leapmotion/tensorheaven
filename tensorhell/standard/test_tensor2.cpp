// ///////////////////////////////////////////////////////////////////////////
// test_tensor2.cpp by Ted Nitz, created 2013/04/04
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_tensor2.hpp"

#include <utility> // for std::pair
#include <complex>

#include "tenh/multiindex.hpp"
#include "tenh/tensor2.hpp"
#include "tenh/vector.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Tensor2 {

template <typename Type_>
void constructor_without_initialization (Context const &context)
{
    Type_ d(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
}

template <typename Type_>
void constructor_fill_with (Context const &context)
{
    typedef typename Type_::Scalar Scalar;

    Scalar fill = context.DataAs<Scalar>();
    Type_ d(fill);

    for (typename Type_::Index i; i.is_not_at_end(); ++i)
    {
        assert_eq(d[i], fill);
    }
}

template <typename Type_>
void test_read_access (Context const &context)
{
    typedef typename Type_::Scalar Scalar;
    typedef typename Type_::Factor1 V1;
    typedef typename Type_::Factor2 V2;

    std::pair<Type_,Scalar> p = context.DataAs<std::pair<Type_,Scalar> >();
    Type_ t = p.first;
    Scalar s = p.second;

    for (typename V1::Index i; i.is_not_at_end(); ++i)
    {
        for (typename V2::Index j; j.is_not_at_end(); ++j)
        {
            Tenh::MultiIndex_t<Tenh::TypeList_t<typename V1::Index,
                Tenh::TypeList_t<typename V2::Index> > > ci(i,j);
            assert_eq(t[ci], t.component(i,j));
            assert_eq(t[ci], s);
            assert_eq(t.scalar_factor_for_component(ci), Scalar(1));
            assert(!t.component_is_procedural_zero(ci));
        }
    }
}

template <typename Type_>
void test_write_access (Context const &context)
{
    typedef typename Type_::Scalar Scalar;
    typedef typename Type_::Factor1 V1;
    typedef typename Type_::Factor2 V2;

    std::pair<Type_,Scalar> p = context.DataAs<std::pair<Type_,Scalar> >();
    Type_ t = p.first;

    for (typename V1::Index i; i.is_not_at_end(); ++i)
    {
        for (typename V2::Index j; j.is_not_at_end(); ++j)
        {
            t.set_component(i, j, Scalar(1));
            assert_eq(t.component(i,j), Scalar(1));

            t.set_component(i, j, Scalar(2));
            assert_eq(t.component(i,j), Scalar(2));
        }
    }
}

template <typename Scalar, Uint32 ROWS, Uint32 COLS>
void add_particular_tests (Directory *parent)
{
    typedef Tenh::Vector_t<Scalar,ROWS> V1;
    typedef Tenh::Vector_t<Scalar,COLS> V2;
    typedef Tenh::Tensor2_t<V1, V2> Tensor2;

    Directory *tensor2
        = new Directory(Tenh::type_string_of<Tenh::Tensor2_t<V1,V2> >(), parent);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2, "constructor_without_initialization",
        constructor_without_initialization<Tensor2>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2, "constructor_fill_with",
        constructor_fill_with<Tensor2>, new Context::Data<Scalar>(42), RESULT_NO_ERROR);

    Scalar s(1);
    Tensor2 t(s);
    std::pair<Tensor2,Scalar> p(t,s);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2, "test_read_access", test_read_access<Tensor2>,
        new Context::Data<std::pair<Tensor2,Scalar> >(p), RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2, "test_write_access",
        test_write_access<Tensor2>, new Context::Data<std::pair<Tensor2,Scalar> >(p), RESULT_NO_ERROR);
}

template <typename Scalar>
void add_particular_tests_for_scalar (Directory *parent)
{
    add_particular_tests<Scalar,1,1>(parent);
    add_particular_tests<Scalar,1,5>(parent);
    add_particular_tests<Scalar,8,1>(parent);
    add_particular_tests<Scalar,100,100>(parent);
}

void AddTests (Directory *parent)
{
    Directory *tensor2 = new Directory("Tensor2_t", parent);

    add_particular_tests_for_scalar<Sint32>(tensor2);
    add_particular_tests_for_scalar<float>(tensor2);
    add_particular_tests_for_scalar<double>(tensor2);
    add_particular_tests_for_scalar<complex<float> >(tensor2);
    add_particular_tests_for_scalar<complex<double> >(tensor2);
}

} // end of namespace Tensor2
} // end of namespace Test