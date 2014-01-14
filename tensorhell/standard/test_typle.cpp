// ///////////////////////////////////////////////////////////////////////////
// test_typle.cpp by Victor Dods, created 2014/01/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_typle.hpp"

#include "tenh/meta/typle.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Typle {

template <typename Typle_>
void test_length (Context const &context)
{
    Uint32 expected_length = context.DataAs<Uint32>();
    assert_eq(Typle_::length(), expected_length);
}

template <typename Typle_, Uint32 EXPECTED_LENGTH>
void add_particular_tests_for_typle (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory(Tenh::type_string_of<Typle_>());
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "length", test_length<Typle_>, new Context::Data<Uint32>(EXPECTED_LENGTH), RESULT_NO_ERROR);
}

// template <typename TypeList, Uint32 INDEX, typename ExpectedLeadingTypeList>
// void test_leading_type_list (Context const &context)
// {
//     assert((Tenh::TypesAreEqual_f<typename Tenh::LeadingTypeList_f<TypeList,INDEX>::T,ExpectedLeadingTypeList>()));
// }

// template <typename TypeList, Uint32 INDEX, typename ExpectedTrailingTypeList>
// void test_trailing_type_list (Context const &context)
// {
//     assert((Tenh::TypesAreEqual_f<typename Tenh::TrailingTypeList_f<TypeList,INDEX>::T,ExpectedTrailingTypeList>()));
// }

// template <typename TypeList, Uint32 INDEX, typename ExpectedLeadingTypeList, typename ExpectedTrailingTypeList>
// void add_leading_and_trailing_type_list_test (Directory &parent)
// {
//     LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
//                                      FORMAT("LeadingTypeList_f<" << Tenh::type_string_of<TypeList>() << ',' << INDEX << '>'),
//                                      test_leading_type_list<TypeList,INDEX,ExpectedLeadingTypeList>,
//                                      RESULT_NO_ERROR);
//     LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
//                                      FORMAT("TrailingTypeList_f<" << Tenh::type_string_of<TypeList>() << ',' << INDEX << '>'),
//                                      test_trailing_type_list<TypeList,INDEX,ExpectedTrailingTypeList>,
//                                      RESULT_NO_ERROR);
// }

// void add_leading_and_trailing_type_list_tests (Directory &parent)
// {
//     // length 0 type list
//     add_leading_and_trailing_type_list_test<Tenh::EmptyTypeList,
//                                             0,
//                                             Tenh::EmptyTypeList, // expected leading type list
//                                             Tenh::EmptyTypeList  // expected trailing type list
//                                             >(parent);

//     // length 1 type list
//     add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char>,
//                                             0,
//                                             Tenh::EmptyTypeList,   // expected leading type list
//                                             Tenh::TypeList_t<char> // expected trailing type list
//                                             >(parent);
//     add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char>,
//                                             1,
//                                             Tenh::TypeList_t<char>, // expected leading type list
//                                             Tenh::EmptyTypeList     // expected trailing type list
//                                             >(parent);

//     // length 2 type list
//     add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int> >,
//                                             0,
//                                             Tenh::EmptyTypeList,                           // expected leading type list
//                                             Tenh::TypeList_t<char,Tenh::TypeList_t<int> >  // expected trailing type list
//                                             >(parent);
//     add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int> >,
//                                             1,
//                                             Tenh::TypeList_t<char>,                        // expected leading type list
//                                             Tenh::TypeList_t<int>                          // expected trailing type list
//                                             >(parent);
//     add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int> >,
//                                             2,
//                                             Tenh::TypeList_t<char,Tenh::TypeList_t<int> >, // expected leading type list
//                                             Tenh::EmptyTypeList                            // expected trailing type list
//                                             >(parent);

//     // length 3 type list
//     add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int,Tenh::TypeList_t<double> > >,
//                                             0,
//                                             Tenh::EmptyTypeList,                                                     // expected leading type list
//                                             Tenh::TypeList_t<char,Tenh::TypeList_t<int,Tenh::TypeList_t<double> > >  // expected trailing type list
//                                             >(parent);
//     add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int,Tenh::TypeList_t<double> > >,
//                                             1,
//                                             Tenh::TypeList_t<char>,                                                  // expected leading type list
//                                             Tenh::TypeList_t<int,Tenh::TypeList_t<double> >                          // expected trailing type list
//                                             >(parent);
//     add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int,Tenh::TypeList_t<double> > >,
//                                             2,
//                                             Tenh::TypeList_t<char,Tenh::TypeList_t<int> >,                           // expected leading type list
//                                             Tenh::TypeList_t<double>                                                 // expected trailing type list
//                                             >(parent);
//     add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int,Tenh::TypeList_t<double> > >,
//                                             3,
//                                             Tenh::TypeList_t<char,Tenh::TypeList_t<int,Tenh::TypeList_t<double> > >, // expected leading type list
//                                             Tenh::EmptyTypeList                                                      // expected trailing type list
//                                             >(parent);
// }

void AddTests (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory("Typle_t");

    add_particular_tests_for_typle<Tenh::Typle_t<>,0>(dir);
    add_particular_tests_for_typle<Tenh::Typle_t<Sint32>,1>(dir);
    add_particular_tests_for_typle<Tenh::Typle_t<Sint32,Sint8>,2>(dir);
    add_particular_tests_for_typle<Tenh::Typle_t<Sint32,Sint8,double>,3>(dir);

    // add_leading_and_trailing_type_list_tests(dir);
}

} // end of namespace Typle
} // end of namespace Test
