// ///////////////////////////////////////////////////////////////////////////
// test_homogeneouspolynomials.hpp by Ted Nitz, created 2013/10/01
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(TEST_HOMOGENEOUSPOLYNOMIALS_HPP_)
#define TEST_HOMOGENEOUSPOLYNOMIALS_HPP_

#include "randomize.hpp"
#include "test.hpp"

#include <sstream>
#include <complex>

#include "tenh/utility/homogeneouspolynomial.hpp"
#include "tenh/utility/polynomial_utility.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

namespace Lvd {
namespace TestSystem {

struct Directory;

} // end of namespace TestSystem
} // end of namespace Lvd

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace HomogeneousPolynomials {

template <typename Scalar_, typename BasedVectorSpace_, Uint32 DEGREE_>
void constructor_without_initialization (Context const &context)
{
    typedef Tenh::HomogeneousPolynomial<DEGREE_,BasedVectorSpace_,Scalar_> Polynomial;

    Polynomial v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
}

template <typename Scalar_, typename BasedVectorSpace_, Uint32 DEGREE_>
void constructor_fill_with (Context const &context)
{
    typedef Tenh::HomogeneousPolynomial<DEGREE_,BasedVectorSpace_,Scalar_> Polynomial;
    typedef Tenh::PreallocatedArray_t<Scalar_,Polynomial::DIMENSION> ArrayType;

    Scalar_ fill = context.DataAs<Scalar_>();
    Polynomial v(Tenh::fill_with(fill));
    ArrayType array = v.as_array();
    for (typename ArrayType::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        assert_eq(array[i], fill);
    }
}

template <typename Scalar_, typename BasedVectorSpace_, Uint32 DEGREE_>
void add_random_polynomials_and_check_result (Context const &context)
{
    typedef Tenh::HomogeneousPolynomial<DEGREE_,BasedVectorSpace_,Scalar_> Polynomial;
    typedef typename Polynomial::Vector Vector;
    typedef Tenh::PreallocatedArray_t<Scalar_,Polynomial::DIMENSION> ArrayType;

    Polynomial roly(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Polynomial poly(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
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

    //std::cerr << roly+poly << '\n';
    assert_about_eq((roly+poly).evaluate(v), (roly.evaluate(v) + poly.evaluate(v)));
}

template <typename Scalar_, typename BasedVectorSpace_, Uint32 DEGREE1_, Uint32 DEGREE2_>
void multiply_random_polynomials_and_check_result (Context const &context)
{
    typedef Tenh::HomogeneousPolynomial<DEGREE1_,BasedVectorSpace_,Scalar_> Polynomial1;
    typedef Tenh::HomogeneousPolynomial<DEGREE2_,BasedVectorSpace_,Scalar_> Polynomial2;
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

    //std::cerr << roly*poly << '\n';
    assert_about_eq((roly*poly).evaluate(v), (roly.evaluate(v) * poly.evaluate(v)));
}

template <typename Scalar_, typename BasedVectorSpace_, Uint32 DEGREE_>
void multiply_random_polynomial_by_scalar_and_check_result (Context const &context)
{
    typedef Tenh::HomogeneousPolynomial<DEGREE_,BasedVectorSpace_,Scalar_> Polynomial;
    typedef typename Polynomial::Vector Vector;
    typedef Tenh::PreallocatedArray_t<Scalar_,Polynomial::DIMENSION> ArrayType;

    Polynomial poly(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Scalar_ a;
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

template <typename Scalar_, typename BasedVectorSpace_>
void add_addition_tests (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory(FORMAT("Addition Tests"))
                           .GetSubDirectory(Tenh::type_string_of<BasedVectorSpace_>());

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1", add_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 2", add_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,2>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 3", add_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,3>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 4", add_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,4>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 5", add_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,5>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 6", add_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,6>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 7", add_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,7>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 8", add_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,8>, RESULT_NO_ERROR);
}

template <typename Scalar_, typename BasedVectorSpace_>
void add_multiplication_tests (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory(FORMAT("Multiplication Tests"))
                           .GetSubDirectory(Tenh::type_string_of<BasedVectorSpace_>());

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 1", multiply_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,1,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 2", multiply_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,1,2>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 3", multiply_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,1,3>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 1 and 5", multiply_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,1,5>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 2 and 1", multiply_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,2,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 2 and 2", multiply_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,2,2>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 2 and 4", multiply_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,2,4>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 3 and 1", multiply_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,3,1>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "Degree 3 and 4", multiply_random_polynomials_and_check_result<Scalar_,BasedVectorSpace_,3,4>, RESULT_NO_ERROR);
}


template <typename Scalar_, Uint32 DIM_, Uint32 DEGREE_>
void add_particular_tests (Directory &parent)
{
    // Directory &scalar_dir = parent.GetSubDirectory(FORMAT("Scalar_ = " << Tenh::type_string_of<Scalar_>()));
    // Directory &dim_dir = scalar_dir.GetSubDirectory(FORMAT("DIM_ = " << DIM_));
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM_,Tenh::Generic>,Tenh::Basis_c<Tenh::Generic>> BasedVectorSpace;
    Directory &degree_dir = parent.GetSubDirectory(FORMAT("HomogeneousPolynomial<" << DEGREE_ << ',' << Tenh::type_string_of<BasedVectorSpace>() << ',' << Tenh::type_string_of<Scalar_>() << '>'));
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(degree_dir, "constructor_without_initialization", constructor_without_initialization<Scalar_,BasedVectorSpace,DEGREE_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(degree_dir, "constructor_fill_with", constructor_fill_with<Scalar_,BasedVectorSpace,DEGREE_>, new Context::Data<Scalar_>(42), RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(degree_dir, "multiply_random_polynomial_by_scalar_and_check_result", multiply_random_polynomial_by_scalar_and_check_result<Scalar_,BasedVectorSpace,DEGREE_>, RESULT_NO_ERROR);
}

void AddTests0 (Lvd::TestSystem::Directory &parent);
void AddTests1 (Lvd::TestSystem::Directory &parent);
void AddTests2 (Lvd::TestSystem::Directory &parent);
void AddTests3 (Lvd::TestSystem::Directory &parent);
void AddTests4 (Lvd::TestSystem::Directory &parent);
void AddTests5 (Lvd::TestSystem::Directory &parent);

} // end of namespace HomogeneousPolynomials
} // end of namespace Test

#endif // !defined(TEST_HOMOGENEOUSPOLYNOMIALS_HPP_)
