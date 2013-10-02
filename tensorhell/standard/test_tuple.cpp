// ///////////////////////////////////////////////////////////////////////////
// test_tuple.cpp by Victor Dods, created 2013/10/01
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_tuple.hpp"

#include <string>

#include "tenh/meta/tuple.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Tuple {

void test_0_tuple (Context const &context)
{
    Tenh::Tuple_f<>::T t(Tenh::tuple());
    assert_eq(t.length(), 0);
}

void test_1_tuple (Context const &context)
{
    float x0 = 3.0f; // arbitrary value
    Tenh::Tuple_f<float>::T t(Tenh::tuple(x0));
    assert_eq(t.length(), 1);
    assert_eq(t.el<0>(), x0);
}

void test_2_tuple (Context const &context)
{
    float x0 = 3.0f; // arbitrary value
    int x1 = 42;     // arbitrary value
    Tenh::Tuple_f<float,int>::T t(Tenh::tuple(x0, x1));
    assert_eq(t.length(), 2);
    assert_eq(t.el<0>(), x0);
    assert_eq(t.el<1>(), x1);
}

void test_3_tuple (Context const &context)
{
    float x0 = 3.0f;         // arbitrary value
    int x1 = 42;             // arbitrary value
    std::string x2 = "blah"; // arbitrary value
    Tenh::Tuple_f<float,int,std::string>::T t(Tenh::tuple(x0, x1, x2));
    assert_eq(t.length(), 3);
    assert_eq(t.el<0>(), x0);
    assert_eq(t.el<1>(), x1);
    assert_eq(t.el<2>(), x2);
}

void test_4_tuple (Context const &context)
{
    float x0 = 3.0f;         // arbitrary value
    int x1 = 42;             // arbitrary value
    std::string x2 = "blah"; // arbitrary value
    double x3 = 1.234;       // arbitrary value
    Tenh::Tuple_f<float,int,std::string,double>::T t(Tenh::tuple(x0, x1, x2, x3));
    assert_eq(t.length(), 4);
    assert_eq(t.el<0>(), x0);
    assert_eq(t.el<1>(), x1);
    assert_eq(t.el<2>(), x2);
    assert_eq(t.el<3>(), x3);
}

void test_5_tuple (Context const &context)
{
    float x0 = 3.0f;         // arbitrary value
    int x1 = 42;             // arbitrary value
    std::string x2 = "blah"; // arbitrary value
    double x3 = 1.234;       // arbitrary value
    bool x4 = true;          // arbitrary value
    Tenh::Tuple_f<float,int,std::string,double,bool>::T t(Tenh::tuple(x0, x1, x2, x3, x4));
    assert_eq(t.length(), 5);
    assert_eq(t.el<0>(), x0);
    assert_eq(t.el<1>(), x1);
    assert_eq(t.el<2>(), x2);
    assert_eq(t.el<3>(), x3);
    assert_eq(t.el<4>(), x4);
}

void test_6_tuple (Context const &context)
{
    float x0 = 3.0f;         // arbitrary value
    int x1 = 42;             // arbitrary value
    std::string x2 = "blah"; // arbitrary value
    double x3 = 1.234;       // arbitrary value
    bool x4 = true;          // arbitrary value
    Sint8 x5 = 16;           // arbitrary value
    Tenh::Tuple_f<float,int,std::string,double,bool,Sint8>::T t(Tenh::tuple(x0, x1, x2, x3, x4, x5));
    assert_eq(t.length(), 6);
    assert_eq(t.el<0>(), x0);
    assert_eq(t.el<1>(), x1);
    assert_eq(t.el<2>(), x2);
    assert_eq(t.el<3>(), x3);
    assert_eq(t.el<4>(), x4);
    assert_eq(t.el<5>(), x5);
}

void test_7_tuple (Context const &context)
{
    float x0 = 3.0f;         // arbitrary value
    int x1 = 42;             // arbitrary value
    std::string x2 = "blah"; // arbitrary value
    double x3 = 1.234;       // arbitrary value
    bool x4 = true;          // arbitrary value
    Sint8 x5 = 16;           // arbitrary value
    Uint32 x6 = 0xFFFFFFFF;  // arbitrary value
    Tenh::Tuple_f<float,int,std::string,double,bool,Sint8,Uint32>::T t(Tenh::tuple(x0, x1, x2, x3, x4, x5, x6));
    assert_eq(t.length(), 7);
    assert_eq(t.el<0>(), x0);
    assert_eq(t.el<1>(), x1);
    assert_eq(t.el<2>(), x2);
    assert_eq(t.el<3>(), x3);
    assert_eq(t.el<4>(), x4);
    assert_eq(t.el<5>(), x5);
    assert_eq(t.el<6>(), x6);
}

void test_8_tuple (Context const &context)
{
    float x0 = 3.0f;         // arbitrary value
    int x1 = 42;             // arbitrary value
    std::string x2 = "blah"; // arbitrary value
    double x3 = 1.234;       // arbitrary value
    bool x4 = true;          // arbitrary value
    Sint8 x5 = 16;           // arbitrary value
    Uint32 x6 = 0xFFFFFFFF;  // arbitrary value
    Sint16 x7 = -257;           // arbitrary value
    Tenh::Tuple_f<float,int,std::string,double,bool,Sint8,Uint32,Sint16>::T t(Tenh::tuple(x0, x1, x2, x3, x4, x5, x6, x7));
    assert_eq(t.length(), 8);
    assert_eq(t.el<0>(), x0);
    assert_eq(t.el<1>(), x1);
    assert_eq(t.el<2>(), x2);
    assert_eq(t.el<3>(), x3);
    assert_eq(t.el<4>(), x4);
    assert_eq(t.el<5>(), x5);
    assert_eq(t.el<6>(), x6);
    assert_eq(t.el<7>(), x7);
}

void AddTests (Directory *parent)
{
    Directory *list = new Directory("tuple", parent);

    LVD_ADD_TEST_CASE_FUNCTION(list, test_0_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(list, test_1_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(list, test_2_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(list, test_3_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(list, test_4_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(list, test_5_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(list, test_6_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(list, test_7_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(list, test_8_tuple, RESULT_NO_ERROR);
}

} // end of namespace Tuple
} // end of namespace Test
