// ///////////////////////////////////////////////////////////////////////////
// test_basic_vector3.cpp by Victor Dods, created 2013/04/02
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

void AddTests3 (Directory &parent)
{
    {
        Directory &dir = parent.GetSubDirectory("Vec");
        add_particular_tests_for_scalar_Vec<Sint8>(dir);
        add_particular_tests_for_scalar_Vec<Uint8>(dir);
        add_particular_tests_for_scalar_Vec<Sint16>(dir);
        add_particular_tests_for_scalar_Vec<Uint16>(dir);
    }
}

} // end of namespace Vector
} // end of namespace Basic
} // end of namespace Test
