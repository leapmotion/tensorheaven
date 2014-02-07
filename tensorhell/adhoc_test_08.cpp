#include "adhoc_test.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 8
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_poly_in_0_dim ()
{
    std::cout << "Polynomials in 0 dimensions." << '\n';
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,0,X>,Basis_c<X> > BasedVectorSpace;
    typedef MultivariatePolynomial<2,BasedVectorSpace> PolyType;
    PolyType::SymDual w(fill_with(0));
    MultivariatePolynomial<1,BasedVectorSpace>::SymDual x(fill_with(1));
    std::cout << FORMAT_VALUE(w) << '\n';
    PolyType roly(w,MultivariatePolynomial<1,BasedVectorSpace>(x,3));
    PolyType poly(fill_with(0));
    poly = poly + MultivariatePolynomial<1,BasedVectorSpace>(x,3);
    PolyType::Vector v(fill_with(0));
    std::cout << FORMAT_VALUE(roly) << '\n';
    roly.as_array().print(std::cout);
    std::cout << '\n';
    std::cout << FORMAT_VALUE(poly) << '\n';
    std::cout << FORMAT_VALUE(v) << '\n';
    std::cout << FORMAT_VALUE(roly.evaluate(v)) << '\n';
    std::cout << FORMAT_VALUE(poly.evaluate(v)) << '\n';
    std::cout << FORMAT_VALUE((poly - roly).evaluate(v)) << '\n';
    std::cout << FORMAT_VALUE((3.0f*roly).evaluate(v)) << '\n';
    std::cout << FORMAT_VALUE(roly*poly) << '\n';
    std::cout << FORMAT_VALUE((roly*poly).evaluate(v)) << '\n';
    std::cout << '\n';
}

void test_poly_in_1_dim ()
{
    std::cout << "Polynomials in 1 dimension." << '\n';
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,1,X>,Basis_c<X> > BasedVectorSpace;
    typedef MultivariatePolynomial<2,BasedVectorSpace> PolyType;
    PolyType::SymDual w(fill_with(0));
    MultivariatePolynomial<1,BasedVectorSpace>::SymDual x(fill_with(1));
    w[PolyType::SymDual::ComponentIndex(0, CheckRange::TRUE)] = 1; // x^2
    std::cout << FORMAT_VALUE(w) << '\n';
    PolyType roly(w,MultivariatePolynomial<1,BasedVectorSpace>(x,3));
    PolyType poly(fill_with(0));
    poly = poly + MultivariatePolynomial<1,BasedVectorSpace>(x,3);
    PolyType::Vector v(fill_with(1));
    std::cout << FORMAT_VALUE(roly) << '\n';
    roly.as_array().print(std::cout);
    std::cout << '\n';
    std::cout << FORMAT_VALUE(poly) << '\n';
    std::cout << FORMAT_VALUE(v) << '\n';
    std::cout << FORMAT_VALUE(roly.evaluate(v)) << '\n';
    std::cout << FORMAT_VALUE(poly.evaluate(v)) << '\n';
    std::cout << FORMAT_VALUE((poly - roly).evaluate(v)) << '\n';
    std::cout << FORMAT_VALUE((3.0f*roly).evaluate(v)) << '\n';
    std::cout << FORMAT_VALUE(roly*poly) << '\n';
    std::cout << FORMAT_VALUE((roly*poly).evaluate(v)) << '\n';
    std::cout << '\n';
}

