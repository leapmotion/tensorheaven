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

void test_uniform_tuples (Context const &context)
{
    Tenh::UniformTuple_f<int,0>::T t0(Tenh::uniform_tuple());
    Tenh::UniformTuple_f<int,1>::T t1(Tenh::uniform_tuple(0));
    Tenh::UniformTuple_f<int,2>::T t2(Tenh::uniform_tuple(0, 1));
    Tenh::UniformTuple_f<int,3>::T t3(Tenh::uniform_tuple(0, 1, 2));
    Tenh::UniformTuple_f<int,4>::T t4(Tenh::uniform_tuple(0, 1, 2, 3));
    Tenh::UniformTuple_f<int,5>::T t5(Tenh::uniform_tuple(0, 1, 2, 3, 4));
    Tenh::UniformTuple_f<int,6>::T t6(Tenh::uniform_tuple(0, 1, 2, 3, 4, 5));
    Tenh::UniformTuple_f<int,7>::T t7(Tenh::uniform_tuple(0, 1, 2, 3, 4, 5, 6));
    Tenh::UniformTuple_f<int,8>::T t8(Tenh::uniform_tuple(0, 1, 2, 3, 4, 5, 6, 7));

    assert_eq(t0.length(), 0);

    assert_eq(t1.length(), 1);
    assert_eq(t1.el<0>(), 0);

    assert_eq(t2.length(), 2);
    assert_eq(t2.el<0>(), 0);
    assert_eq(t2.el<1>(), 1);

    assert_eq(t3.length(), 3);
    assert_eq(t3.el<0>(), 0);
    assert_eq(t3.el<1>(), 1);
    assert_eq(t3.el<2>(), 2);

    assert_eq(t4.length(), 4);
    assert_eq(t4.el<0>(), 0);
    assert_eq(t4.el<1>(), 1);
    assert_eq(t4.el<2>(), 2);
    assert_eq(t4.el<3>(), 3);

    assert_eq(t5.length(), 5);
    assert_eq(t5.el<0>(), 0);
    assert_eq(t5.el<1>(), 1);
    assert_eq(t5.el<2>(), 2);
    assert_eq(t5.el<3>(), 3);
    assert_eq(t5.el<4>(), 4);

    assert_eq(t6.length(), 6);
    assert_eq(t6.el<0>(), 0);
    assert_eq(t6.el<1>(), 1);
    assert_eq(t6.el<2>(), 2);
    assert_eq(t6.el<3>(), 3);
    assert_eq(t6.el<4>(), 4);
    assert_eq(t6.el<5>(), 5);

    assert_eq(t7.length(), 7);
    assert_eq(t7.el<0>(), 0);
    assert_eq(t7.el<1>(), 1);
    assert_eq(t7.el<2>(), 2);
    assert_eq(t7.el<3>(), 3);
    assert_eq(t7.el<4>(), 4);
    assert_eq(t7.el<5>(), 5);
    assert_eq(t7.el<6>(), 6);

    assert_eq(t8.length(), 8);
    assert_eq(t8.el<0>(), 0);
    assert_eq(t8.el<1>(), 1);
    assert_eq(t8.el<2>(), 2);
    assert_eq(t8.el<3>(), 3);
    assert_eq(t8.el<4>(), 4);
    assert_eq(t8.el<5>(), 5);
    assert_eq(t8.el<6>(), 6);
    assert_eq(t8.el<7>(), 7);
}

void AddTests (Directory &parent)
{
    Directory &tuple_dir = parent.GetSubDirectory("tuple");

    LVD_ADD_TEST_CASE_FUNCTION(tuple_dir, test_0_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(tuple_dir, test_1_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(tuple_dir, test_2_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(tuple_dir, test_3_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(tuple_dir, test_4_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(tuple_dir, test_5_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(tuple_dir, test_6_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(tuple_dir, test_7_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(tuple_dir, test_8_tuple, RESULT_NO_ERROR);

    LVD_ADD_TEST_CASE_FUNCTION(tuple_dir, test_uniform_tuples, RESULT_NO_ERROR);
}

} // end of namespace Tuple
} // end of namespace Test
