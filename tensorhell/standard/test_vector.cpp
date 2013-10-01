// ///////////////////////////////////////////////////////////////////////////
// test_vector.cpp by Victor Dods, created 2013/04/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_vector.hpp"

#include <sstream>
#include <complex>

#include "tenh/vector.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Vector {

template <typename Scalar, Uint32 DIM>
void constructor_without_initialization (Context const &context)
{
    typedef Tenh::Vector_t<Scalar,DIM> Vector;

    Vector v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
}

template <typename Scalar, Uint32 DIM>
void constructor_fill_with (Context const &context)
{
    typedef Tenh::Vector_t<Scalar,DIM> Vector;

    Scalar fill = context.DataAs<Scalar>();
    Vector v(fill);
    for (typename Vector::Index i; i.is_not_at_end(); ++i)
    {
        assert_eq(v[i], fill);
    }
}

template <typename Scalar>
void check_filled_values (Context const &context)
{
    typedef Tenh::Vector_t<Scalar,4> Vector;

    Vector v(0,2,4,6);
    for (typename Vector::Index i; i.is_not_at_end(); ++i)
    {
        assert_eq(v[i], Scalar(2*i.value()));
    }
}

template <typename Scalar, Uint32 DIM>
void add_particular_tests (Directory *parent)
{
    Directory *vector = new Directory(FORMAT("Vector_t<" << Tenh::type_string_of<Scalar>() << ',' << DIM << '>'), parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(vector, "constructor_without_initialization", constructor_without_initialization<Scalar,DIM>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(vector, "constructor_fill_with", constructor_fill_with<Scalar,DIM>, new Context::Data<Scalar>(42), RESULT_NO_ERROR);
}

template <typename Scalar>
void add_particular_tests_for_scalar (Directory *parent)
{
    add_particular_tests<Scalar,1>(parent);
    add_particular_tests<Scalar,2>(parent);
    add_particular_tests<Scalar,100>(parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent, FORMAT("check_filled_values<" << Tenh::type_string_of<Scalar>() << ">"), check_filled_values<Scalar>, RESULT_NO_ERROR);

}

void AddTests (Directory *parent)
{
    Directory *vector = new Directory("Vector_t", parent);
    add_particular_tests_for_scalar<Sint8>(vector);
    add_particular_tests_for_scalar<Uint32>(vector);
    add_particular_tests_for_scalar<Sint64>(vector);
    add_particular_tests_for_scalar<float>(vector);
    add_particular_tests_for_scalar<double>(vector);
    add_particular_tests_for_scalar<complex<float> >(vector);
    add_particular_tests_for_scalar<complex<double> >(vector);
}

} // end of namespace Vector
} // end of namespace Test
