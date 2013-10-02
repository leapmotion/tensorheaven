// ///////////////////////////////////////////////////////////////////////////
// test_list.cpp by Victor Dods, created 2013/04/03
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_list.hpp"

#include <utility> // for std::pair

#include "tenh/list.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace List {

void test_EmptyList (Context const &context)
{
    assert((Tenh::TypesAreEqual_f<Tenh::EmptyList::BodyTypeList,Tenh::EmptyTypeList>()));
    assert_eq(Tenh::EmptyList::length(), 0);
    assert((Tenh::TypesAreEqual_f<Tenh::EmptyList::LeadingListType_t<0>::T,Tenh::EmptyList>()));
    assert((Tenh::TypesAreEqual_f<Tenh::EmptyList::TrailingListType_t<0>::T,Tenh::EmptyList>()));
}

template <typename TypeList>
void test_length (Context const &context)
{
    typedef Tenh::List_t<TypeList> List;
    assert_eq(List::length(), TypeList::length());
}

template <typename TypeList>
void test_default_construction (Context const &context)
{
    typedef Tenh::List_t<TypeList> List;
    List l;
    assert_neq(&l, static_cast<List *>(NULL)); // sort of dummy code so that l is not unused
}

template <typename TypeList>
void add_particular_tests_for_list (Directory *parent)
{
    Directory *list = new Directory(Tenh::type_string_of<Tenh::List_t<TypeList> >(), parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(list, "test_length", test_length<TypeList>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(list, "test_default_construction", test_default_construction<TypeList>, RESULT_NO_ERROR);
}

template <typename TypeList, Uint32 INDEX>
void test_leading_list (Context const &context)
{
    typedef Tenh::List_t<TypeList> List;
    typedef typename TypeList::template LeadingTypeList_t<INDEX>::T LeadingTypeList;
    typedef Tenh::List_t<LeadingTypeList> LeadingList;

    assert((Tenh::TypesAreEqual_f<typename List::template LeadingListType_t<INDEX>::T,LeadingList>()));

    typedef pair<List,LeadingList> TestData;
    TestData const &d = context.DataAs<TestData>();
    List const &l = d.first;
    LeadingList const &expected_leading_list = d.second;
    assert_eq(l.template leading_list<INDEX>(), expected_leading_list);
}

template <typename TypeList, Uint32 INDEX>
void test_trailing_list (Context const &context)
{
    typedef Tenh::List_t<TypeList> List;
    typedef typename TypeList::template TrailingTypeList_t<INDEX>::T TrailingTypeList;
    typedef Tenh::List_t<TrailingTypeList> TrailingList;

    assert((Tenh::TypesAreEqual_f<typename List::template TrailingListType_t<INDEX>::T,TrailingList>()));

    typedef pair<List,TrailingList> TestData;
    TestData const &d = context.DataAs<TestData>();
    List const &l = d.first;
    TrailingList const &expected_trailing_list = d.second;
    assert_eq(l.template trailing_list<INDEX>(), expected_trailing_list);
}

void add_leading_and_trailing_list_length_0_test (Directory *parent)
{
    typedef Tenh::EmptyTypeList TypeList;
    typedef Tenh::List_t<Tenh::EmptyTypeList> List;
    List l;
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<List>() << "::leading_list<" << 0 << '>'),
                                     test_leading_list<TypeList,0>,
                                     new Context::Data<pair<List,Tenh::EmptyList> >(pair<List,Tenh::EmptyList>(l, Tenh::Static<Tenh::EmptyList>::SINGLETON)),
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<List>() << "::trailing_list<" << 0 << '>'),
                                     test_trailing_list<TypeList,0>,
                                     new Context::Data<pair<List,Tenh::EmptyList> >(pair<List,Tenh::EmptyList>(l, Tenh::Static<Tenh::EmptyList>::SINGLETON)),
                                     RESULT_NO_ERROR);
}

void add_leading_and_trailing_list_length_1_test (Directory *parent)
{
    typedef Tenh::TypeList_t<char> TypeList;
    typedef Tenh::List_t<TypeList> List;
    List l('a');
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<List>() << "::leading_list<" << 0 << '>'),
                                     test_leading_list<TypeList,0>,
                                     new Context::Data<pair<List,Tenh::EmptyList> >(pair<List,Tenh::EmptyList>(l, Tenh::Static<Tenh::EmptyList>::SINGLETON)),
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<List>() << "::trailing_list<" << 0 << '>'),
                                     test_trailing_list<TypeList,0>,
                                     new Context::Data<pair<List,List> >(pair<List,List>(l, l)),
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<List>() << "::leading_list<" << 1 << '>'),
                                     test_leading_list<TypeList,1>,
                                     new Context::Data<pair<List,List> >(pair<List,List>(l, l)),
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::type_string_of<List>() << "::trailing_list<" << 1 << '>'),
                                     test_trailing_list<TypeList,1>,
                                     new Context::Data<pair<List,Tenh::EmptyList> >(pair<List,Tenh::EmptyList>(l, Tenh::Static<Tenh::EmptyList>::SINGLETON)),
                                     RESULT_NO_ERROR);
}

void add_leading_and_trailing_list_tests (Directory *parent)
{
    add_leading_and_trailing_list_length_0_test(parent);
    add_leading_and_trailing_list_length_1_test(parent);
}

void test_list_concatenation (Context const &context)
{
    Tenh::List_t<Tenh::EmptyTypeList> empty_list;
    Tenh::List_t<Tenh::TypeList_t<float> > list_1(3.0f);
    Tenh::List_t<Tenh::TypeList_t<char,
                 Tenh::TypeList_t<bool> > > list_2('a', true);

    Tenh::List_t<Tenh::TypeList_t<float,
                 Tenh::TypeList_t<float> > > list_1__list_1(3.0f, list_1);
    Tenh::List_t<Tenh::TypeList_t<float,
                 Tenh::TypeList_t<char,
                 Tenh::TypeList_t<bool> > > > list_1__list_2(3.0f, list_2);
    Tenh::List_t<Tenh::TypeList_t<char,
                 Tenh::TypeList_t<bool,
                 Tenh::TypeList_t<float> > > >
        list_2__list_1('a', Tenh::List_t<Tenh::TypeList_t<bool,
                                         Tenh::TypeList_t<float> > >(true, 3.0f));
    Tenh::List_t<Tenh::TypeList_t<char,
                 Tenh::TypeList_t<bool,
                 Tenh::TypeList_t<char,
                 Tenh::TypeList_t<bool> > > > >
        list_2__list_2('a', Tenh::List_t<Tenh::TypeList_t<bool,
                                         Tenh::TypeList_t<char,
                                         Tenh::TypeList_t<bool> > > >(true, list_2));

    assert_eq(empty_list, empty_list);
    assert_eq((empty_list|empty_list), empty_list);
    assert_eq((list_1|empty_list), list_1);
    assert_eq((list_2|empty_list), list_2);
    assert_eq((list_1|list_1), list_1__list_1);
    assert_eq((list_1|list_2), list_1__list_2);
    assert_eq((list_2|list_1), list_2__list_1);
    assert_eq((list_2|list_2), list_2__list_2);
}

void AddTests (Directory *parent)
{
    Directory *list = new Directory("List_t", parent);

    LVD_ADD_TEST_CASE_FUNCTION(list, test_EmptyList, RESULT_NO_ERROR);
    add_particular_tests_for_list<Tenh::EmptyTypeList>(list);
    add_particular_tests_for_list<Tenh::TypeList_t<Sint32> >(list);
    add_particular_tests_for_list<Tenh::TypeList_t<Sint32,Tenh::TypeList_t<Sint8> > >(list);
    add_leading_and_trailing_list_tests(list);
    LVD_ADD_TEST_CASE_FUNCTION(list, test_list_concatenation, RESULT_NO_ERROR);
}

} // end of namespace List
} // end of namespace Test
