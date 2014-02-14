// ///////////////////////////////////////////////////////////////////////////
// test_tuple.cpp by Victor Dods, created 2013/04/03
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_tuple.hpp"

#include <utility> // for std::pair

#include "tenh/tuple.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Tuple { // HIPPO TODO change this to Tuple

void test_EmptyTuple (Context const &context)
{
    assert((Tenh::TypesAreEqual_f<Tenh::EmptyTuple::BodyTyple,Tenh::Typle_t<>>()));
    assert_eq(Tenh::EmptyTuple::length(), 0);
    assert((Tenh::TypesAreEqual_f<Tenh::EmptyTuple::LeadingTupleType_f<0>::T,Tenh::EmptyTuple>()));
    assert((Tenh::TypesAreEqual_f<Tenh::EmptyTuple::TrailingTupleType_f<0>::T,Tenh::EmptyTuple>()));
}

template <typename Typle>
void test_length (Context const &context)
{
    typedef Tenh::Tuple_t<Typle> Tuple;
    assert_eq(Tuple::length(), Tenh::Length_f<Typle>::V);
}

template <typename Typle>
void test_default_construction (Context const &context)
{
    typedef Tenh::Tuple_t<Typle> Tuple;
    Tuple l;
    assert_neq(&l, static_cast<Tuple *>(NULL)); // sort of dummy code so that l is not unused
}

template <typename Typle>
void add_particular_tests_for_tuple (Directory &parent)
{
    Directory &tuple = parent.GetSubDirectory(Tenh::type_string_of<Tenh::Tuple_t<Typle> >());
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(tuple, "length", test_length<Typle>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(tuple, "default_construction", test_default_construction<Typle>, RESULT_NO_ERROR);
}

template <typename Typle, Uint32 INDEX>
void test_leading_tuple (Context const &context)
{
    typedef Tenh::Tuple_t<Typle> Tuple;
    typedef typename Tenh::LeadingTyple_f<Typle,INDEX>::T LeadingTyple;
    typedef Tenh::Tuple_t<LeadingTyple> LeadingTuple;

    assert((Tenh::TypesAreEqual_f<typename Tuple::template LeadingTupleType_f<INDEX>::T,LeadingTuple>()));

    typedef pair<Tuple,LeadingTuple> TestData;
    TestData const &d = context.DataAs<TestData>();
    Tuple const &l = d.first;
    LeadingTuple const &expected_leading_tuple = d.second;
    assert_eq(l.template leading_tuple<INDEX>(), expected_leading_tuple);
}

template <typename Typle, Uint32 INDEX>
void test_trailing_tuple (Context const &context)
{
    typedef Tenh::Tuple_t<Typle> Tuple;
    typedef typename Tenh::TrailingTyple_f<Typle,INDEX>::T TrailingTyple;
    typedef Tenh::Tuple_t<TrailingTyple> TrailingTuple;

    assert((Tenh::TypesAreEqual_f<typename Tuple::template TrailingTupleType_f<INDEX>::T,TrailingTuple>()));

    typedef pair<Tuple,TrailingTuple> TestData;
    TestData const &d = context.DataAs<TestData>();
    Tuple const &l = d.first;
    TrailingTuple const &expected_trailing_tuple = d.second;
    assert_eq(l.template trailing_tuple<INDEX>(), expected_trailing_tuple);
}

void add_leading_and_trailing_tuple_length_0_test (Directory &parent)
{
    typedef Tenh::Typle_t<> Typle;
    typedef Tenh::Tuple_t<Tenh::Typle_t<>> Tuple;
    Tuple l;
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<Tuple>() << "::leading_tuple<" << 0 << '>'),
                                     test_leading_tuple<Typle,0>,
                                     new Context::Data<pair<Tuple,Tenh::EmptyTuple> >(pair<Tuple,Tenh::EmptyTuple>(l, Tenh::Static<Tenh::EmptyTuple>::SINGLETON)),
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<Tuple>() << "::trailing_tuple<" << 0 << '>'),
                                     test_trailing_tuple<Typle,0>,
                                     new Context::Data<pair<Tuple,Tenh::EmptyTuple> >(pair<Tuple,Tenh::EmptyTuple>(l, Tenh::Static<Tenh::EmptyTuple>::SINGLETON)),
                                     RESULT_NO_ERROR);
}

void add_leading_and_trailing_tuple_length_1_test (Directory &parent)
{
    typedef Tenh::Typle_t<char> Typle;
    typedef Tenh::Tuple_t<Typle> Tuple;
    Tuple l('a');
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<Tuple>() << "::leading_tuple<" << 0 << '>'),
                                     test_leading_tuple<Typle,0>,
                                     new Context::Data<pair<Tuple,Tenh::EmptyTuple> >(pair<Tuple,Tenh::EmptyTuple>(l, Tenh::Static<Tenh::EmptyTuple>::SINGLETON)),
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<Tuple>() << "::trailing_tuple<" << 0 << '>'),
                                     test_trailing_tuple<Typle,0>,
                                     new Context::Data<pair<Tuple,Tuple> >(pair<Tuple,Tuple>(l, l)),
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<Tuple>() << "::leading_tuple<" << 1 << '>'),
                                     test_leading_tuple<Typle,1>,
                                     new Context::Data<pair<Tuple,Tuple> >(pair<Tuple,Tuple>(l, l)),
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<Tuple>() << "::trailing_tuple<" << 1 << '>'),
                                     test_trailing_tuple<Typle,1>,
                                     new Context::Data<pair<Tuple,Tenh::EmptyTuple> >(pair<Tuple,Tenh::EmptyTuple>(l, Tenh::Static<Tenh::EmptyTuple>::SINGLETON)),
                                     RESULT_NO_ERROR);
}

void add_leading_and_trailing_tuple_tests (Directory &parent)
{
    add_leading_and_trailing_tuple_length_0_test(parent);
    add_leading_and_trailing_tuple_length_1_test(parent);
}

void test_tuple_concatenation (Context const &context)
{
    Tenh::Tuple_t<Tenh::Typle_t<>> empty_tuple;
    Tenh::Tuple_t<Tenh::Typle_t<float>> tuple_1(3.0f);
    Tenh::Tuple_t<Tenh::Typle_t<char,bool>> tuple_2('a', true);

    Tenh::Tuple_t<Tenh::Typle_t<float,float>> tuple_1__tuple_1(3.0f, tuple_1);
    Tenh::Tuple_t<Tenh::Typle_t<float,char,bool>> tuple_1__tuple_2(3.0f, tuple_2);
    Tenh::Tuple_t<Tenh::Typle_t<char,bool,float>>
        tuple_2__tuple_1('a', Tenh::Tuple_t<Tenh::Typle_t<bool,float>>(true, 3.0f));
    Tenh::Tuple_t<Tenh::Typle_t<char,bool,char,bool>>
        tuple_2__tuple_2('a', Tenh::Tuple_t<Tenh::Typle_t<bool,char,bool>>(true, tuple_2));

    assert_eq(empty_tuple, empty_tuple);
    assert_eq((empty_tuple|empty_tuple), empty_tuple);
    assert_eq((tuple_1|empty_tuple), tuple_1);
    assert_eq((tuple_2|empty_tuple), tuple_2);
    assert_eq((tuple_1|tuple_1), tuple_1__tuple_1);
    assert_eq((tuple_1|tuple_2), tuple_1__tuple_2);
    assert_eq((tuple_2|tuple_1), tuple_2__tuple_1);
    assert_eq((tuple_2|tuple_2), tuple_2__tuple_2);
}

void test_0_tuple (Context const &context)
{
    auto t(Tenh::tuple());
    assert_eq(t.length(), 0);
}

void test_1_tuple (Context const &context)
{
    float x0 = 3.0f; // arbitrary value
    auto t(Tenh::tuple(x0));
    assert_eq(t.length(), 1);
    assert_eq(t.el<0>(), x0);
}

void test_2_tuple (Context const &context)
{
    float x0 = 3.0f; // arbitrary value
    int x1 = 42;     // arbitrary value
    auto t(Tenh::tuple(x0, x1));
    assert_eq(t.length(), 2);
    assert_eq(t.el<0>(), x0);
    assert_eq(t.el<1>(), x1);
}

void test_3_tuple (Context const &context)
{
    float x0 = 3.0f;         // arbitrary value
    int x1 = 42;             // arbitrary value
    std::string x2 = "blah"; // arbitrary value
    auto t(Tenh::tuple(x0, x1, x2));
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
    auto t(Tenh::tuple(x0, x1, x2, x3));
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
    auto t(Tenh::tuple(x0, x1, x2, x3, x4));
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
    auto t(Tenh::tuple(x0, x1, x2, x3, x4, x5));
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
    auto t(Tenh::tuple(x0, x1, x2, x3, x4, x5, x6));
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
    Sint16 x7 = -257;        // arbitrary value
    auto t(Tenh::tuple(x0, x1, x2, x3, x4, x5, x6, x7));
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
    Tenh::UniformTuple_f<int,0>::T t0(Tenh::uniform_tuple<int>());
    Tenh::UniformTuple_f<int,1>::T t1(Tenh::uniform_tuple<int>(0));
    Tenh::UniformTuple_f<int,2>::T t2(Tenh::uniform_tuple<int>(0, 1));
    Tenh::UniformTuple_f<int,3>::T t3(Tenh::uniform_tuple<int>(0, 1, 2));
    Tenh::UniformTuple_f<int,4>::T t4(Tenh::uniform_tuple<int>(0, 1, 2, 3));
    Tenh::UniformTuple_f<int,5>::T t5(Tenh::uniform_tuple<int>(0, 1, 2, 3, 4));
    Tenh::UniformTuple_f<int,6>::T t6(Tenh::uniform_tuple<int>(0, 1, 2, 3, 4, 5));
    Tenh::UniformTuple_f<int,7>::T t7(Tenh::uniform_tuple<int>(0, 1, 2, 3, 4, 5, 6));
    Tenh::UniformTuple_f<int,8>::T t8(Tenh::uniform_tuple<int>(0, 1, 2, 3, 4, 5, 6, 7));

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
    Directory &dir = parent.GetSubDirectory("Tuple_t");

    LVD_ADD_TEST_CASE_FUNCTION(dir, test_EmptyTuple, RESULT_NO_ERROR);
    add_particular_tests_for_tuple<Tenh::Typle_t<>>(dir);
    add_particular_tests_for_tuple<Tenh::Typle_t<Sint32> >(dir);
    add_particular_tests_for_tuple<Tenh::Typle_t<Sint32,Sint8>>(dir);
    add_leading_and_trailing_tuple_tests(dir);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_tuple_concatenation, RESULT_NO_ERROR);

    LVD_ADD_TEST_CASE_FUNCTION(dir, test_0_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_1_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_2_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_3_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_4_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_5_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_6_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_7_tuple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_8_tuple, RESULT_NO_ERROR);

    LVD_ADD_TEST_CASE_FUNCTION(dir, test_uniform_tuples, RESULT_NO_ERROR);
}

} // end of namespace Tuple
} // end of namespace Test
