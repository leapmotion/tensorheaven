#include "adhoc_test.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 10
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_poly_in_3_dim ()
{
    std::cout << "Polynomials in 3 dimensions." << '\n';
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,Basis_c<IdX> > BasedVectorSpace;
    typedef MultivariatePolynomial<2,BasedVectorSpace> PolyType;
    PolyType::SymDual w(fill_with(0));
    MultivariatePolynomial<1,BasedVectorSpace>::SymDual x(fill_with(1));
    w[PolyType::SymDual::ComponentIndex(0, CHECK_RANGE)] = 1; // x^2
    w[PolyType::SymDual::ComponentIndex(1, CHECK_RANGE)] = 0; // xy
    w[PolyType::SymDual::ComponentIndex(2, CHECK_RANGE)] = 5; // y^2
    w[PolyType::SymDual::ComponentIndex(3, CHECK_RANGE)] = 2; // xz
    w[PolyType::SymDual::ComponentIndex(4, CHECK_RANGE)] = 6; // yz
    w[PolyType::SymDual::ComponentIndex(5, CHECK_RANGE)] = 3; // z^2
    std::cout << FORMAT_VALUE(w) << '\n';
    PolyType roly(w,MultivariatePolynomial<1,BasedVectorSpace>(x,3));
    PolyType poly(fill_with(0));
    poly = poly + MultivariatePolynomial<1,BasedVectorSpace>(x,3);
    PolyType::Vector v(tuple(1.0f,2.0f,3.0f));
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

