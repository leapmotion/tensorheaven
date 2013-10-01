// ///////////////////////////////////////////////////////////////////////////
// test_multivariatepolynomials.cpp by Ted Nitz, created 2013/10/01
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_multivariatepolynomials.hpp"
#include "randomize.hpp"

#include <sstream>
#include <complex>

#include "applications/polynomial.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace MultivariatePolynomials {

template <typename Scalar, Uint32 DIM, Uint32 DEGREE>
void constructor_without_initialization (Context const &context)
{
    typedef MultivariatePolynomial<DEGREE,DIM,PolynomialBasisId,Scalar> Polynomial;

    Polynomial v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
}

template <typename Scalar, Uint32 DIM, Uint32 DEGREE>
void constructor_fill_with (Context const &context)
{
    typedef MultivariatePolynomial<DEGREE,DIM,PolynomialBasisId,Scalar> Polynomial;
    typedef Tenh::PreallocatedArray_t<Scalar,Polynomial::DIMENSION> ArrayType;

    Scalar fill = context.DataAs<Scalar>();
    Polynomial v(fill);
    ArrayType array = v.as_array();
    for (typename ArrayType::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        assert_eq(array[i], fill);
    }
}

template <typename Scalar, Uint32 DIM, Uint32 DEGREE>
void multiply_random_polynomials_and_check_result (Context const &context)
{
    typedef MultivariatePolynomial<DEGREE,DIM,PolynomialBasisId,Scalar> Polynomial;
    typedef typename Polynomial::Vector Vector;
    typedef Tenh::PreallocatedArray_t<Scalar,Polynomial::DIMENSION> ArrayType;

    Polynomial roly(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON), poly(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    ArrayType array = roly.as_array();
    for (typename ArrayType::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        Tenh::randomize(array[i]);
    }

    array = poly.as_array();
    for (typename ArrayType::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        Tenh::randomize(array[i]);
    }

    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        Tenh::randomize(v[i]);
    }

    assert_about_eq((roly*poly).evaluate(v), (roly.evaluate(v) * poly.evaluate(v)));
}


template <typename Scalar, Uint32 DIM, Uint32 DEGREE>
void add_particular_tests_for_dimension_and_degree (Directory *parent)
{
    Directory *vector = new Directory(FORMAT("MultivariatePolynomial<" << DEGREE << ',' << DIM << ',' << Tenh::type_string_of<PolynomialBasisId>() << ',' << Tenh::type_string_of<Scalar>() << '>'), parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(vector, "constructor_without_initialization", constructor_without_initialization<Scalar,DIM,DEGREE>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(vector, "constructor_fill_with", constructor_fill_with<Scalar,DIM,DEGREE>, new Context::Data<Scalar>(42), RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(vector, "multiply_random_polynomials_and_check_result", multiply_random_polynomials_and_check_result<Scalar,DIM,DEGREE>, RESULT_NO_ERROR);
}

template <typename Scalar, Uint32 DIM>
void add_particular_tests_for_dimension (Directory *parent)
{
    add_particular_tests_for_dimension_and_degree<Scalar,DIM,1>(parent);
    add_particular_tests_for_dimension_and_degree<Scalar,DIM,2>(parent);
    add_particular_tests_for_dimension_and_degree<Scalar,DIM,4>(parent);

}

template <typename Scalar>
void add_particular_tests_for_scalar (Directory *parent)
{
    add_particular_tests_for_dimension<Scalar,1>(parent);
    add_particular_tests_for_dimension<Scalar,2>(parent);
    add_particular_tests_for_dimension<Scalar,4>(parent);

}

void AddTests (Directory *parent)
{
    Directory *vector = new Directory("MultivariatePolynomial", parent);
    // add_particular_tests_for_scalar<Tenh::Sint8>(vector);
    // add_particular_tests_for_scalar<Tenh::Uint32>(vector);
    // add_particular_tests_for_scalar<Tenh::Sint64>(vector);
    add_particular_tests_for_scalar<float>(vector);
    add_particular_tests_for_scalar<double>(vector);
    add_particular_tests_for_scalar<complex<float> >(vector);
    add_particular_tests_for_scalar<complex<double> >(vector);
}

} // end of namespace Vector
} // end of namespace Test
