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

template <typename Scalar, Uint32 DIM, Uint32 DEGREE1, Uint32 DEGREE2>
void multiply_random_polynomials_and_check_result (Context const &context)
{
    typedef MultivariatePolynomial<DEGREE1,DIM,PolynomialBasisId,Scalar> Polynomial1;
    typedef MultivariatePolynomial<DEGREE2,DIM,PolynomialBasisId,Scalar> Polynomial2;
    typedef typename Polynomial1::Vector Vector;
    typedef typename Polynomial1::CoefficientArray ArrayType1;
    typedef typename Polynomial2::CoefficientArray ArrayType2;

    Polynomial1 roly(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Polynomial2 poly(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    ArrayType1 array1 = roly.as_array();

    for (typename ArrayType1::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        Tenh::randomize(array1[i]);
    }

    ArrayType2 array2 = poly.as_array();
    for (typename ArrayType2::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        Tenh::randomize(array2[i]);
    }

    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        Tenh::randomize(v[i]);
    }

    assert_about_eq((roly*poly).evaluate(v), (roly.evaluate(v) * poly.evaluate(v)));
}

template <typename Scalar, Uint32 DIM, Uint32 DEGREE1, Uint32 DEGREE2>
void add_random_polynomials_and_check_result (Context const &context)
{
    typedef MultivariatePolynomial<DEGREE1,DIM,PolynomialBasisId,Scalar> Polynomial1;
    typedef MultivariatePolynomial<DEGREE2,DIM,PolynomialBasisId,Scalar> Polynomial2;
    typedef typename Polynomial1::Vector Vector;
    typedef typename Polynomial1::CoefficientArray ArrayType1;
    typedef typename Polynomial2::CoefficientArray ArrayType2;

    Polynomial1 roly(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Polynomial2 poly(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    ArrayType1 array1 = roly.as_array();

    for (typename ArrayType1::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        Tenh::randomize(array1[i]);
    }

    ArrayType2 array2 = poly.as_array();
    for (typename ArrayType2::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        Tenh::randomize(array2[i]);
    }

    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        Tenh::randomize(v[i]);
    }

    assert_about_eq((roly+poly).evaluate(v), (roly.evaluate(v) + poly.evaluate(v)));
}

template <typename Scalar, Uint32 DIM, Uint32 DEGREE>
void multiply_random_polynomial_by_scalar_and_check_result (Context const &context)
{
    typedef MultivariatePolynomial<DEGREE,DIM,PolynomialBasisId,Scalar> Polynomial;
    typedef typename Polynomial::Vector Vector;
    typedef typename Polynomial::CoefficientArray ArrayType;

    Polynomial poly(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Scalar a;
    ArrayType array = poly.as_array();

    Tenh::randomize(a);

    for (typename ArrayType::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        Tenh::randomize(array[i]);
    }

    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        Tenh::randomize(v[i]);
    }

    assert_about_eq((a*poly).evaluate(v), (a * poly.evaluate(v)));
}

template <typename Scalar, Uint32 DIM>
void add_addition_tests_for_dimension (Directory *parent)
{
    Directory *poly = new Directory(FORMAT("Addition Tests"), parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 1 and 0", add_random_polynomials_and_check_result<Scalar,DIM,1,0>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 1 and 1", add_random_polynomials_and_check_result<Scalar,DIM,1,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 1 and 4", add_random_polynomials_and_check_result<Scalar,DIM,1,4>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 1 and 5", add_random_polynomials_and_check_result<Scalar,DIM,1,5>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 3 and 1", add_random_polynomials_and_check_result<Scalar,DIM,3,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 3 and 3", add_random_polynomials_and_check_result<Scalar,DIM,3,3>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 3 and 6", add_random_polynomials_and_check_result<Scalar,DIM,3,6>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 8 and 1", add_random_polynomials_and_check_result<Scalar,DIM,8,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 8 and 2", add_random_polynomials_and_check_result<Scalar,DIM,8,2>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 8 and 8", add_random_polynomials_and_check_result<Scalar,DIM,8,8>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 8 and 10", add_random_polynomials_and_check_result<Scalar,DIM,8,10>, RESULT_NO_ERROR);
}

template <typename Scalar, Uint32 DIM>
void add_multiplication_tests_for_dimension (Directory *parent)
{
    Directory *poly = new Directory(FORMAT("Multiplication Tests"), parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 1 and 1", multiply_random_polynomials_and_check_result<Scalar,DIM,1,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 1 and 2", multiply_random_polynomials_and_check_result<Scalar,DIM,1,2>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 1 and 3", multiply_random_polynomials_and_check_result<Scalar,DIM,1,3>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 1 and 5", multiply_random_polynomials_and_check_result<Scalar,DIM,1,5>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 2 and 1", multiply_random_polynomials_and_check_result<Scalar,DIM,2,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 2 and 2", multiply_random_polynomials_and_check_result<Scalar,DIM,2,2>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 2 and 4", multiply_random_polynomials_and_check_result<Scalar,DIM,2,4>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 3 and 1", multiply_random_polynomials_and_check_result<Scalar,DIM,3,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "Degree 3 and 4", multiply_random_polynomials_and_check_result<Scalar,DIM,3,4>, RESULT_NO_ERROR);
}


template <typename Scalar, Uint32 DIM, Uint32 DEGREE>
void add_particular_tests_for_dimension_and_degree (Directory *parent)
{
    Directory *poly = new Directory(FORMAT("HomogeneousPolynomial<" << DEGREE << ',' << DIM << ',' << Tenh::type_string_of<PolynomialBasisId>() << ',' << Tenh::type_string_of<Scalar>() << '>'), parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "constructor_without_initialization", constructor_without_initialization<Scalar,DIM,DEGREE>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "constructor_fill_with", constructor_fill_with<Scalar,DIM,DEGREE>, new Context::Data<Scalar>(42), RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(poly, "multiply_random_polynomial_by_scalar_and_check_result", multiply_random_polynomial_by_scalar_and_check_result<Scalar,DIM,DEGREE>, RESULT_NO_ERROR);
}

template <typename Scalar, Uint32 DIM>
void add_particular_tests_for_dimension (Directory *parent)
{
    Directory *poly = new Directory(FORMAT("DIM = " << DIM), parent);
    add_particular_tests_for_dimension_and_degree<Scalar,DIM,1>(poly);
    add_particular_tests_for_dimension_and_degree<Scalar,DIM,2>(poly);
    add_particular_tests_for_dimension_and_degree<Scalar,DIM,3>(poly);
    add_addition_tests_for_dimension<Scalar,DIM>(poly);
    add_multiplication_tests_for_dimension<Scalar,DIM>(poly);
}

template <typename Scalar>
void add_particular_tests_for_scalar (Directory *parent)
{
    Directory *poly = new Directory(FORMAT("Scalar = " << Tenh::type_string_of<Scalar>()), parent);
    add_particular_tests_for_dimension<Scalar,1>(poly);
    add_particular_tests_for_dimension<Scalar,2>(poly);
    add_particular_tests_for_dimension<Scalar,3>(poly);

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
