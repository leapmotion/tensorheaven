// ///////////////////////////////////////////////////////////////////////////
// test_homogeneouspolynomials.cpp by Victor Dods, created 2013/04/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_homogeneouspolynomials.hpp"
#include "randomize.hpp"

#include <sstream>
#include <complex>

#include "applications/homogeneouspolynomial.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace HomogeneousPolynomials {

template <typename Scalar, Uint32 DIM, Uint32 DEGREE>
void constructor_without_initialization (Context const &context)
{
    typedef HomogeneousPolynomial<DEGREE,DIM,PolynomialBasisId,Scalar> Polynomial;

    Polynomial v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
}

template <typename Scalar, Uint32 DIM, Uint32 DEGREE>
void constructor_fill_with (Context const &context)
{
    typedef HomogeneousPolynomial<DEGREE,DIM,PolynomialBasisId,Scalar> Polynomial;
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
void add_random_polynomials_and_check_result (Context const &context)
{
    typedef HomogeneousPolynomial<DEGREE,DIM,PolynomialBasisId,Scalar> Polynomial;
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

    assert_about_eq((roly+poly).evaluate(v), (roly.evaluate(v) + poly.evaluate(v)));
}

template <typename Scalar, Uint32 DIM, Uint32 DEGREE1, Uint32 DEGREE2>
void multiply_random_polynomials_and_check_result (Context const &context)
{
    typedef HomogeneousPolynomial<DEGREE1,DIM,PolynomialBasisId,Scalar> Polynomial1;
    typedef HomogeneousPolynomial<DEGREE2,DIM,PolynomialBasisId,Scalar> Polynomial2;
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

template <typename Scalar, Uint32 DIM, Uint32 DEGREE>
void multiply_random_polynomial_by_scalar_and_check_result (Context const &context)
{
    typedef HomogeneousPolynomial<DEGREE,DIM,PolynomialBasisId,Scalar> Polynomial;
    typedef typename Polynomial::Vector Vector;
    typedef Tenh::PreallocatedArray_t<Scalar,Polynomial::DIMENSION> ArrayType;

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
void add_addition_tests_for_dimension (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory(FORMAT("Addition Tests"));
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1", add_random_polynomials_and_check_result<Scalar,DIM,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 2", add_random_polynomials_and_check_result<Scalar,DIM,2>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 3", add_random_polynomials_and_check_result<Scalar,DIM,3>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 4", add_random_polynomials_and_check_result<Scalar,DIM,4>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 5", add_random_polynomials_and_check_result<Scalar,DIM,5>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 6", add_random_polynomials_and_check_result<Scalar,DIM,6>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 7", add_random_polynomials_and_check_result<Scalar,DIM,7>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 8", add_random_polynomials_and_check_result<Scalar,DIM,8>, RESULT_NO_ERROR);
}

template <typename Scalar, Uint32 DIM>
void add_multiplication_tests_for_dimension (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory(FORMAT("Multiplication Tests"));
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 1", multiply_random_polynomials_and_check_result<Scalar,DIM,1,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 2", multiply_random_polynomials_and_check_result<Scalar,DIM,1,2>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 3", multiply_random_polynomials_and_check_result<Scalar,DIM,1,3>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 5", multiply_random_polynomials_and_check_result<Scalar,DIM,1,5>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 2 and 1", multiply_random_polynomials_and_check_result<Scalar,DIM,2,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 2 and 2", multiply_random_polynomials_and_check_result<Scalar,DIM,2,2>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 2 and 4", multiply_random_polynomials_and_check_result<Scalar,DIM,2,4>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 3 and 1", multiply_random_polynomials_and_check_result<Scalar,DIM,3,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 3 and 4", multiply_random_polynomials_and_check_result<Scalar,DIM,3,4>, RESULT_NO_ERROR);
}


template <typename Scalar, Uint32 DIM, Uint32 DEGREE>
void add_particular_tests (Directory &parent)
{
    Directory &scalar_dir = parent.GetSubDirectory(FORMAT("Scalar = " << Tenh::type_string_of<Scalar>()));
    Directory &dim_dir = scalar_dir.GetSubDirectory(FORMAT("DIM = " << DIM));
    Directory &degree_dir = dim_dir.GetSubDirectory(FORMAT("HomogeneousPolynomial<" << DEGREE << ',' << DIM << ',' << Tenh::type_string_of<PolynomialBasisId>() << ',' << Tenh::type_string_of<Scalar>() << '>'));
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(degree_dir, "constructor_without_initialization", constructor_without_initialization<Scalar,DIM,DEGREE>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(degree_dir, "constructor_fill_with", constructor_fill_with<Scalar,DIM,DEGREE>, new Context::Data<Scalar>(42), RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(degree_dir, "multiply_random_polynomial_by_scalar_and_check_result", multiply_random_polynomial_by_scalar_and_check_result<Scalar,DIM,DEGREE>, RESULT_NO_ERROR);
}

void AddTests (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory("HomogeneousPolynomial");

    // the reason all the different combinations are enumerated by hand is
    // so that this source file can be easily broken into several source
    // files, reducing the compilation resources required and allowing
    // parallel compilation.

    // // add_particular_tests_for_scalar<Tenh::Sint8>(dir);
    // // add_particular_tests_for_scalar<Tenh::Sint32>(dir);
    // // add_particular_tests_for_scalar<Tenh::Sint64>(dir);

    // TODO: re-add tests for integral types once integral types are working

    add_particular_tests<float,1,1>(dir);
    add_particular_tests<float,1,2>(dir);
    add_particular_tests<float,1,3>(dir);
    add_particular_tests<float,2,1>(dir);
    add_particular_tests<float,2,2>(dir);
    add_particular_tests<float,2,3>(dir);
    add_particular_tests<float,3,1>(dir);
    add_particular_tests<float,3,2>(dir);
    add_particular_tests<float,3,3>(dir);

    add_particular_tests<double,1,1>(dir);
    add_particular_tests<double,1,2>(dir);
    add_particular_tests<double,1,3>(dir);
    add_particular_tests<double,2,1>(dir);
    add_particular_tests<double,2,2>(dir);
    add_particular_tests<double,2,3>(dir);
    add_particular_tests<double,3,1>(dir);
    add_particular_tests<double,3,2>(dir);
    add_particular_tests<double,3,3>(dir);

    add_particular_tests<complex<float>,1,1>(dir);
    add_particular_tests<complex<float>,1,2>(dir);
    add_particular_tests<complex<float>,1,3>(dir);
    add_particular_tests<complex<float>,2,1>(dir);
    add_particular_tests<complex<float>,2,2>(dir);
    add_particular_tests<complex<float>,2,3>(dir);
    add_particular_tests<complex<float>,3,1>(dir);
    add_particular_tests<complex<float>,3,2>(dir);
    add_particular_tests<complex<float>,3,3>(dir);

    add_particular_tests<complex<double>,1,1>(dir);
    add_particular_tests<complex<double>,1,2>(dir);
    add_particular_tests<complex<double>,1,3>(dir);
    add_particular_tests<complex<double>,2,1>(dir);
    add_particular_tests<complex<double>,2,2>(dir);
    add_particular_tests<complex<double>,2,3>(dir);
    add_particular_tests<complex<double>,3,1>(dir);
    add_particular_tests<complex<double>,3,2>(dir);
    add_particular_tests<complex<double>,3,3>(dir);
}

} // end of namespace Vector
} // end of namespace Test
