// ///////////////////////////////////////////////////////////////////////////
// test_interop_eigen_inversion1.cpp by Victor Dods, created 2013/04/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_interop_eigen_inversion.hpp"

namespace Test {
namespace InteropEigen {
namespace Inversion {

void AddTests1 (Directory *parent)
{
    add_inversion_tests_1<double,2>(parent);
    add_inversion_tests_1<double,3>(parent);
    add_inversion_tests_1<double,4>(parent);
}

} // end of namespace Inversion
} // end of namespace InteropEigen
} // end of namespace Test