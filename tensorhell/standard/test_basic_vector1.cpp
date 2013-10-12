// ///////////////////////////////////////////////////////////////////////////
// test_basic_vector1.cpp by Victor Dods, created 2013/04/02
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

void AddTests1 (Directory *parent)
{
    {
        Directory *dir = parent->GetSubDirectory("Vector");
        add_particular_tests_for_scalar_Vector<Sint32>(dir);
        add_particular_tests_for_scalar_Vector<Uint32>(dir);
        add_particular_tests_for_scalar_Vector<Sint64>(dir);
        add_particular_tests_for_scalar_Vector<Uint64>(dir);
    }
}

} // end of namespace Vector
} // end of namespace Basic
} // end of namespace Test
