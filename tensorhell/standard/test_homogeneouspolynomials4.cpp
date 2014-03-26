// ///////////////////////////////////////////////////////////////////////////
// test_homogeneouspolynomials4.cpp by Ted Nitz, created 2013/04/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_homogeneouspolynomials.hpp"

namespace Test {
namespace HomogeneousPolynomials {

void AddTests4 (Directory &parent)
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

    {
        static Uint32 const DIM = 1;
        typedef Tenh::VectorSpace_c<Tenh::RealField,DIM,Tenh::Generic> VectorSpace;
        typedef Tenh::BasedVectorSpace_c<VectorSpace,Tenh::Basis_c<Tenh::Generic>> BasedVectorSpace;
        add_addition_tests<double,BasedVectorSpace>(dir);
        add_multiplication_tests<double,BasedVectorSpace>(dir);
    }
}

} // end of namespace Vector
} // end of namespace Test
