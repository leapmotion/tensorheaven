// ///////////////////////////////////////////////////////////////////////////
// test_typelist.cpp by Victor Dods, created 2013/04/03
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_typelist.hpp"

#include "tenh/meta/typelist.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace TypeList {

void test_EmptyTypeList (Context const &context)
{
    assert((TypesAreEqual_f<Tenh::EmptyTypeList::BodyTypeList,Tenh::EmptyTypeList>()));
    assert_eq(Tenh::EmptyTypeList::length(), 0);
    assert(!Tenh::EmptyTypeList::Contains_t<int>());
    assert((TypesAreEqual_f<Tenh::EmptyTypeList::LeadingTypeList_t<0>::T,Tenh::EmptyTypeList>()));
    assert((TypesAreEqual_f<Tenh::EmptyTypeList::TrailingTypeList_t<0>::T,Tenh::EmptyTypeList>()));
}

template <typename TypeList>
void test_length (Context const &context)
{
    Uint32 expected_length = context.DataAs<Uint32>();
    assert_eq(TypeList::length(), expected_length);
}

template <typename TypeList, Uint32 EXPECTED_LENGTH>
void add_particular_tests_for_typelist (Directory *parent)
{
    Directory *typelist = new Directory(Tenh::type_string_of<TypeList>(), parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(typelist, "test_length", test_length<TypeList>, new Context::Data<Uint32>(EXPECTED_LENGTH), RESULT_NO_ERROR);
}

template <typename TypeList, Uint32 INDEX, typename ExpectedLeadingTypeList>
void test_leading_type_list (Context const &context)
{
    assert((TypesAreEqual_f<typename TypeList::template LeadingTypeList_t<INDEX>::T,ExpectedLeadingTypeList>()));
}

template <typename TypeList, Uint32 INDEX, typename ExpectedTrailingTypeList>
void test_trailing_type_list (Context const &context)
{
    assert((TypesAreEqual_f<typename TypeList::template TrailingTypeList_t<INDEX>::T,ExpectedTrailingTypeList>()));
}

template <typename TypeList, Uint32 INDEX, typename ExpectedLeadingTypeList, typename ExpectedTrailingTypeList>
void add_leading_and_trailing_type_list_test (Directory *parent)
{
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<TypeList>() << "::LeadingTypeList_t<" << INDEX << '>'),
                                     test_leading_type_list<TypeList,INDEX,ExpectedLeadingTypeList>,
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<TypeList>() << "::TrailingTypeList_t<" << INDEX << '>'),
                                     test_trailing_type_list<TypeList,INDEX,ExpectedTrailingTypeList>,
                                     RESULT_NO_ERROR);
}

void add_leading_and_trailing_type_list_tests (Directory *parent)
{
    // length 0 type list
    add_leading_and_trailing_type_list_test<Tenh::EmptyTypeList,
                                            0,
                                            Tenh::EmptyTypeList, // expected leading type list
                                            Tenh::EmptyTypeList  // expected trailing type list
                                            >(parent);

    // length 1 type list
    add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char>,
                                            0,
                                            Tenh::EmptyTypeList,   // expected leading type list
                                            Tenh::TypeList_t<char> // expected trailing type list
                                            >(parent);
    add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char>,
                                            1,
                                            Tenh::TypeList_t<char>, // expected leading type list
                                            Tenh::EmptyTypeList     // expected trailing type list
                                            >(parent);

    // length 2 type list
    add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int> >,
                                            0,
                                            Tenh::EmptyTypeList,                           // expected leading type list
                                            Tenh::TypeList_t<char,Tenh::TypeList_t<int> >  // expected trailing type list
                                            >(parent);
    add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int> >,
                                            1,
                                            Tenh::TypeList_t<char>,                        // expected leading type list
                                            Tenh::TypeList_t<int>                          // expected trailing type list
                                            >(parent);
    add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int> >,
                                            2,
                                            Tenh::TypeList_t<char,Tenh::TypeList_t<int> >, // expected leading type list
                                            Tenh::EmptyTypeList                            // expected trailing type list
                                            >(parent);

    // length 3 type list
    add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int,Tenh::TypeList_t<double> > >,
                                            0,
                                            Tenh::EmptyTypeList,                                                     // expected leading type list
                                            Tenh::TypeList_t<char,Tenh::TypeList_t<int,Tenh::TypeList_t<double> > >  // expected trailing type list
                                            >(parent);
    add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int,Tenh::TypeList_t<double> > >,
                                            1,
                                            Tenh::TypeList_t<char>,                                                  // expected leading type list
                                            Tenh::TypeList_t<int,Tenh::TypeList_t<double> >                          // expected trailing type list
                                            >(parent);
    add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int,Tenh::TypeList_t<double> > >,
                                            2,
                                            Tenh::TypeList_t<char,Tenh::TypeList_t<int> >,                           // expected leading type list
                                            Tenh::TypeList_t<double>                                                 // expected trailing type list
                                            >(parent);
    add_leading_and_trailing_type_list_test<Tenh::TypeList_t<char,Tenh::TypeList_t<int,Tenh::TypeList_t<double> > >,
                                            3,
                                            Tenh::TypeList_t<char,Tenh::TypeList_t<int,Tenh::TypeList_t<double> > >, // expected leading type list
                                            Tenh::EmptyTypeList                                                      // expected trailing type list
                                            >(parent);
}

void AddTests (Directory *parent)
{
    Directory *typelist = new Directory("TypeList_t", parent);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(typelist, Tenh::type_string_of<Tenh::EmptyTypeList>(), test_EmptyTypeList, RESULT_NO_ERROR);
    add_particular_tests_for_typelist<Tenh::TypeList_t<Sint32>,1>(typelist);
    add_particular_tests_for_typelist<Tenh::TypeList_t<Sint32,Tenh::TypeList_t<Sint8> >,2>(typelist);

    add_leading_and_trailing_type_list_tests(typelist);
}

} // end of namespace TypeList
} // end of namespace Test
