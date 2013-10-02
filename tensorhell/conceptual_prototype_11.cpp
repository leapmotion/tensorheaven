#include "conceptual_prototype.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 11
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_poly_in_4_dim ()
{
    std::cout << "Polynomials in 4 dimensions." << '\n';
    typedef MultivariatePolynomial<2,4,X> PolyType;
    PolyType::SymDual w(0.0f);
    MultivariatePolynomial<1,4,X>::SymDual x(1);
    w[PolyType::SymDual::ComponentIndex(0, CHECK_RANGE)] = 1; // x^2
    w[PolyType::SymDual::ComponentIndex(1, CHECK_RANGE)] = 0; // xy
    w[PolyType::SymDual::ComponentIndex(2, CHECK_RANGE)] = 5; // y^2
    w[PolyType::SymDual::ComponentIndex(3, CHECK_RANGE)] = 2; // xz
    w[PolyType::SymDual::ComponentIndex(4, CHECK_RANGE)] = 6; // yz
    w[PolyType::SymDual::ComponentIndex(5, CHECK_RANGE)] = 3; // z^2
    w[PolyType::SymDual::ComponentIndex(6, CHECK_RANGE)] = 2; // xw
    w[PolyType::SymDual::ComponentIndex(7, CHECK_RANGE)] = 6; // yw
    w[PolyType::SymDual::ComponentIndex(8, CHECK_RANGE)] = 6; // zw
    w[PolyType::SymDual::ComponentIndex(9, CHECK_RANGE)] = 3; // w^2
    std::cout << FORMAT_VALUE(w) << '\n';
    PolyType roly(w,MultivariatePolynomial<1,4,X>(x,3));
    PolyType poly(0);
    poly = poly + MultivariatePolynomial<1,4,X>(x,3);
    PolyType::Vector v(tuple(1.0f,2.0f,3.0f,4.0f));
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

void test_polynomial_serialization ()
{
    std::cout << "serializing Polynomials in 4 dimensions." << '\n';
    typedef MultivariatePolynomial<2,4,X> PolyType;
    PolyType::SymDual w(0.0f);
    MultivariatePolynomial<1,4,X>::SymDual x(1);
    w[PolyType::SymDual::ComponentIndex(0, CHECK_RANGE)] = 1; // x^2
    w[PolyType::SymDual::ComponentIndex(1, CHECK_RANGE)] = 0; // xy
    w[PolyType::SymDual::ComponentIndex(2, CHECK_RANGE)] = 5; // y^2
    w[PolyType::SymDual::ComponentIndex(3, CHECK_RANGE)] = 2; // xz
    w[PolyType::SymDual::ComponentIndex(4, CHECK_RANGE)] = 6; // yz
    w[PolyType::SymDual::ComponentIndex(5, CHECK_RANGE)] = 3; // z^2
    w[PolyType::SymDual::ComponentIndex(6, CHECK_RANGE)] = 2; // xw
    w[PolyType::SymDual::ComponentIndex(7, CHECK_RANGE)] = 6; // yw
    w[PolyType::SymDual::ComponentIndex(8, CHECK_RANGE)] = 6; // zw
    w[PolyType::SymDual::ComponentIndex(9, CHECK_RANGE)] = 3; // w^2
    std::cout << FORMAT_VALUE(w) << '\n';
    PolyType roly(w,MultivariatePolynomial<1,4,X>(x,3));
    PolyType poly(Static<WithoutInitialization>::SINGLETON);

    poly.as_array().copy_from(roly.as_array());
    std::cout << FORMAT_VALUE(roly) << '\n';
    std::cout << FORMAT_VALUE(poly) << '\n';
    std::cout << '\n';
}

void test_polynomial_multiplication ()
{
    std::cout << "Polynomial multiplication tests." << '\n';
    typedef MultivariatePolynomial<1,2,X> PolyType;
    PolyType::SymDual x(0.0f),y(0.0f);
    x[PolyType::SymDual::ComponentIndex(0, CHECK_RANGE)] = 1.0f;
    y[PolyType::SymDual::ComponentIndex(1, CHECK_RANGE)] = 1.0f;

    PolyType X(x,0), Y(y,0);

    std::cout << X << '\n';
    std::cout << Y << '\n';
    std::cout << X*X << '\n';
    std::cout << Y*Y << '\n';
    std::cout << X*Y << '\n';
    std::cout << (X*X)*(Y*Y) + 3 << '\n';
    std::cout << (X*Y)*(X*Y) << '\n';
    std::cout << 17 + X*X*Y*Y << '\n';
}

void test_tuple_initialization ()
{
    std::cout << "testing tuple-based initialization\n";
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,12,X>,Basis_c<X> > BVS;
    typedef ImplementationOf_t<BVS,float,UseMemberArray> U;
    float array[12];
    typedef ImplementationOf_t<BVS,float,UsePreallocatedArray> V;
    U u(tuple(1.0f, 2.0f, 3.0f, 4.0f) | tuple(5.0f, 6.0f, 7.0f, 8.0f) | tuple(9.0f, 10.0f, 11.0f, 12.0f));
    V v(tuple(1.0f, 2.0f, 3.0f, 4.0f) | tuple(5.0f, 6.0f, 7.0f, 8.0f) | tuple(9.0f, 10.0f, 11.0f, 12.0f), &array[0]);
    std::cout << FORMAT_VALUE(u) << '\n';
    std::cout << FORMAT_VALUE(v) << '\n';
    std::cout << '\n';
}