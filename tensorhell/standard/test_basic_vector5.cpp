// ///////////////////////////////////////////////////////////////////////////
// test_basic_vector5.cpp by Victor Dods, created 2013/04/02
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

void AddTests5 (Directory &parent)
{
    {
        Directory &dir = parent.GetSubDirectory("Vector");
        add_particular_tests_for_scalar_Vec<float>(dir);
        add_particular_tests_for_scalar_Vec<double>(dir);
        add_particular_tests_for_scalar_Vec<complex<float> >(dir);
        add_particular_tests_for_scalar_Vec<complex<double> >(dir);
    }
}

} // end of namespace Vector
} // end of namespace Basic
} // end of namespace Test
