// ///////////////////////////////////////////////////////////////////////////
// test.cpp by Victor Dods, created 2013/04/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test.hpp"

#include "test_homogeneouspolynomials.hpp"
#include "test_multivariatepolynomials.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

int main (int argc, char **argv, char **envp)
{
    Directory root;

    Test::HomogeneousPolynomials::AddTests0(root);
    Test::HomogeneousPolynomials::AddTests1(root);
    Test::HomogeneousPolynomials::AddTests2(root);
    Test::HomogeneousPolynomials::AddTests3(root);
    Test::HomogeneousPolynomials::AddTests4(root);
    Test::HomogeneousPolynomials::AddTests5(root);
    Test::MultivariatePolynomials::AddTests0(root);
    Test::MultivariatePolynomials::AddTests1(root);
    Test::MultivariatePolynomials::AddTests2(root);
    Test::MultivariatePolynomials::AddTests3(root);
    Test::MultivariatePolynomials::AddTests4(root);
    Test::MultivariatePolynomials::AddTests5(root);

    int failure_count = RunScheduled(argc, argv, envp, root);

    return (failure_count > 0) ? 1 : 0;
}
