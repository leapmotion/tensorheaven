// ///////////////////////////////////////////////////////////////////////////
// test_multivariatepolynomials.cpp by Ted Nitz, created 2013/10/01
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_multivariatepolynomials.hpp"
#include "randomize.hpp"

#include <sstream>
#include <complex>

#include "applications/polynomial.hpp"
#include "applications/polynomial_utility.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace MultivariatePolynomials {

template <typename Scalar, typename BasedVectorSpace_, Uint32 DEGREE>
void constructor_without_initialization (Context const &context)
{
    typedef MultivariatePolynomial<DEGREE,BasedVectorSpace_,Scalar> Polynomial;

    Polynomial v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
}

template <typename Scalar_, typename BasedVectorSpace_, Uint32 DEGREE_>
void constructor_fill_with (Context const &context)
{
    typedef MultivariatePolynomial<DEGREE_,BasedVectorSpace_,Scalar_> Polynomial;
    typedef Tenh::PreallocatedArray_t<Scalar_,Polynomial::DIMENSION> ArrayType;

    Scalar_ fill = context.DataAs<Scalar_>();
    Polynomial v(Tenh::fill_with<Scalar_>(fill));
    ArrayType array = v.as_array();
    for (typename ArrayType::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        assert_eq(array[i], fill);
    }
}

template <typename Scalar, typename BasedVectorSpace_, Uint32 DEGREE1, Uint32 DEGREE2>
void multiply_random_polynomials_and_check_result (Context const &context)
{
    typedef MultivariatePolynomial<DEGREE1,BasedVectorSpace_,Scalar> Polynomial1;
    typedef MultivariatePolynomial<DEGREE2,BasedVectorSpace_,Scalar> Polynomial2;
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

    std::cerr << roly*poly << '\n';
    assert_about_eq((roly*poly).evaluate(v), (roly.evaluate(v) * poly.evaluate(v)));
}

template <typename Scalar, typename BasedVectorSpace_, Uint32 DEGREE1, Uint32 DEGREE2>
void add_random_polynomials_and_check_result (Context const &context)
{
    typedef MultivariatePolynomial<DEGREE1,BasedVectorSpace_,Scalar> Polynomial1;
    typedef MultivariatePolynomial<DEGREE2,BasedVectorSpace_,Scalar> Polynomial2;
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

    std::cerr << roly+poly << '\n';
    assert_about_eq((roly+poly).evaluate(v), (roly.evaluate(v) + poly.evaluate(v)));
}

template <typename Scalar, typename BasedVectorSpace_, Uint32 DEGREE>
void multiply_random_polynomial_by_scalar_and_check_result (Context const &context)
{
    typedef MultivariatePolynomial<DEGREE,BasedVectorSpace_,Scalar> Polynomial;
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

template <typename Scalar, typename BasedVectorSpace_>
void add_addition_tests (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory(FORMAT("Addition Tests"))
                           .GetSubDirectory(Tenh::type_string_of<BasedVectorSpace_>());

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 0", add_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,1,0>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 1", add_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,1,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 4", add_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,1,4>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 5", add_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,1,5>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 3 and 1", add_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,3,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 3 and 3", add_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,3,3>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 3 and 6", add_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,3,6>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 8 and 1", add_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,8,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 8 and 2", add_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,8,2>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 8 and 8", add_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,8,8>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 8 and 10", add_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,8,10>, RESULT_NO_ERROR);
}

template <typename Scalar, typename BasedVectorSpace_>
void add_multiplication_tests (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory(FORMAT("Multiplication Tests"))
                           .GetSubDirectory(Tenh::type_string_of<BasedVectorSpace_>());

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 1", multiply_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,1,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 2", multiply_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,1,2>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 3", multiply_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,1,3>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 5", multiply_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,1,5>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 2 and 1", multiply_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,2,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 2 and 2", multiply_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,2,2>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 2 and 4", multiply_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,2,4>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 3 and 1", multiply_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,3,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 3 and 4", multiply_random_polynomials_and_check_result<Scalar,BasedVectorSpace_,3,4>, RESULT_NO_ERROR);
}


template <typename Scalar_, Uint32 DIM_, Uint32 DEGREE_>
void add_particular_tests (Directory &parent)
{
    // Directory &scalar_dir = parent.GetSubDirectory(FORMAT("Scalar = " << Tenh::type_string_of<Scalar_>()));
    // Directory &dim_dir = scalar_dir.GetSubDirectory(FORMAT("DIM = " << DIM_));
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM_,Tenh::Generic>,Tenh::Basis_c<Tenh::Generic> > BasedVectorSpace;
    Directory &degree_dir = parent.GetSubDirectory(FORMAT("MultivariatePolynomial<" << DEGREE_ << ',' << Tenh::type_string_of<BasedVectorSpace>() << ',' << Tenh::type_string_of<Scalar_>() << '>'));
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(degree_dir, "constructor_without_initialization", constructor_without_initialization<Scalar_,BasedVectorSpace,DEGREE_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(degree_dir, "constructor_fill_with", constructor_fill_with<Scalar_,BasedVectorSpace,DEGREE_>, new Context::Data<Scalar_>(42), RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(degree_dir, "multiply_random_polynomial_by_scalar_and_check_result", multiply_random_polynomial_by_scalar_and_check_result<Scalar_,BasedVectorSpace,DEGREE_>, RESULT_NO_ERROR);
}

void AddTests (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory("MultivariatePolynomial");

    // the reason all the different combinations are enumerated by hand is
    // so that this source file can be easily broken into several source
    // files, reducing the compilation resources required and allowing
    // parallel compilation.

    // // add_particular_tests_for_scalar<Tenh::Sint8>(dir);
    // // add_particular_tests_for_scalar<Tenh::Uint32>(dir);
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

    {
        static Uint32 const DIM = 1;
        typedef Tenh::VectorSpace_c<Tenh::RealField,DIM,Tenh::Generic> VectorSpace;
        typedef Tenh::BasedVectorSpace_c<VectorSpace,Tenh::Basis_c<Tenh::Generic> > BasedVectorSpace;
        add_addition_tests<float,BasedVectorSpace>(dir);
        add_multiplication_tests<float,BasedVectorSpace>(dir);
    }

    {
        static Uint32 const DIM = 3;
        typedef Tenh::VectorSpace_c<Tenh::RealField,DIM,Tenh::Generic> VectorSpace;
        typedef Tenh::BasedVectorSpace_c<VectorSpace,Tenh::Basis_c<Tenh::Generic> > BasedVectorSpace;
        add_addition_tests<float,BasedVectorSpace>(dir);
        add_multiplication_tests<float,BasedVectorSpace>(dir);
    }
}

} // end of namespace Vector
} // end of namespace Test
