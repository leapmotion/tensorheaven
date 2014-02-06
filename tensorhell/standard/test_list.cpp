// ///////////////////////////////////////////////////////////////////////////
// test_list.cpp by Victor Dods, created 2013/04/03
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_list.hpp"

#include <utility> // for std::pair

#include "tenh/meta/tuple.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace List { // HIPPO TODO change this to Tuple

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

void AddTests (Directory &parent)
{
    Directory &tuple = parent.GetSubDirectory("Tuple_t");

    LVD_ADD_TEST_CASE_FUNCTION(tuple, test_EmptyTuple, RESULT_NO_ERROR);
    add_particular_tests_for_tuple<Tenh::Typle_t<>>(tuple);
    add_particular_tests_for_tuple<Tenh::Typle_t<Sint32> >(tuple);
    add_particular_tests_for_tuple<Tenh::Typle_t<Sint32,Sint8>>(tuple);
    add_leading_and_trailing_tuple_tests(tuple);
    LVD_ADD_TEST_CASE_FUNCTION(tuple, test_tuple_concatenation, RESULT_NO_ERROR);
}

} // end of namespace List
} // end of namespace Test
