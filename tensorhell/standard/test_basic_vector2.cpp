// ///////////////////////////////////////////////////////////////////////////
// test_basic_vector2.cpp by Victor Dods, created 2013/04/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_basic_vector.hpp"

// this should include nothing besides test_basic_vector.hpp

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Basic {
namespace Vector {

void AddTests2 (Directory &parent)
{
    {
        Directory &dir = parent.GetSubDirectory("Vector");
        add_particular_tests_for_scalar_Vector<float>(dir);
        add_particular_tests_for_scalar_Vector<double>(dir);
        add_particular_tests_for_scalar_Vector<complex<float>>(dir);
        add_particular_tests_for_scalar_Vector<complex<double>>(dir);
    }
}

} // end of namespace Vector
} // end of namespace Basic
} // end of namespace Test
