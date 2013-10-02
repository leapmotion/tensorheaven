#include "conceptual_prototype.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 9
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_poly_in_2_dim ()
{
    std::cout << "Polynomials in 2 dimensions." << '\n';
    typedef MultivariatePolynomial<2,2,X> PolyType;
    PolyType::SymDual w(0.0f);
    MultivariatePolynomial<1,2,X>::SymDual x(1);
    w[PolyType::SymDual::ComponentIndex(0, CHECK_RANGE)] = 0; // x^2
    w[PolyType::SymDual::ComponentIndex(1, CHECK_RANGE)] = 1; // xy
    w[PolyType::SymDual::ComponentIndex(2, CHECK_RANGE)] = 0; // y^2
    std::cout << FORMAT_VALUE(w) << '\n';
    PolyType roly(w,MultivariatePolynomial<1,2,X>(x,1)), poly(Static<WithoutInitialization>::SINGLETON);
    poly = poly + MultivariatePolynomial<1,2,X>(x,1);
    PolyType::Vector v(tuple(1.0f,2.0f));
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

