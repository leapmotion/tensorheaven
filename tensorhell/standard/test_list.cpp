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
    assert((Lvd::Meta::TypesAreEqual<Tenh::EmptyList::BodyTypeList,Tenh::EmptyTypeList>()));
    assert_eq(Tenh::EmptyList::length(), 0);
    assert((Lvd::Meta::TypesAreEqual<Tenh::EmptyList::LeadingListType_t<0>::T,Tenh::EmptyList>()));
    assert((Lvd::Meta::TypesAreEqual<Tenh::EmptyList::TrailingListType_t<0>::T,Tenh::EmptyList>()));
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
    Directory *list = new Directory(Tenh::TypeStringOf_t<Tenh::List_t<TypeList> >::eval(), parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(list, "test_length", test_length<TypeList>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(list, "test_default_construction", test_default_construction<TypeList>, RESULT_NO_ERROR);
}

template <typename TypeList, Uint32 INDEX>
void test_leading_list (Context const &context)
{
    typedef Tenh::List_t<TypeList> List;
    typedef typename TypeList::template LeadingTypeList_t<INDEX>::T LeadingTypeList;
    typedef Tenh::List_t<LeadingTypeList> LeadingList;

    assert((Lvd::Meta::TypesAreEqual<typename List::template LeadingListType_t<INDEX>::T,LeadingList>()));

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

    assert((Lvd::Meta::TypesAreEqual<typename List::template TrailingListType_t<INDEX>::T,TrailingList>()));

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
                                     FORMAT(Tenh::TypeStringOf_t<List>::eval() << "::leading_list<" << 0 << '>'),
                                     test_leading_list<TypeList,0>,
                                     new Context::Data<pair<List,Tenh::EmptyList> >(pair<List,Tenh::EmptyList>(l, Tenh::Static_t<Tenh::EmptyList>::SINGLETON)),
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::TypeStringOf_t<List>::eval() << "::trailing_list<" << 0 << '>'),
                                     test_trailing_list<TypeList,0>,
                                     new Context::Data<pair<List,Tenh::EmptyList> >(pair<List,Tenh::EmptyList>(l, Tenh::Static_t<Tenh::EmptyList>::SINGLETON)),
                                     RESULT_NO_ERROR);
}

void add_leading_and_trailing_list_length_1_test (Directory *parent)
{
    typedef Tenh::TypeList_t<char> TypeList;
    typedef Tenh::List_t<TypeList> List;
    List l('a');
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::TypeStringOf_t<List>::eval() << "::leading_list<" << 0 << '>'),
                                     test_leading_list<TypeList,0>,
                                     new Context::Data<pair<List,Tenh::EmptyList> >(pair<List,Tenh::EmptyList>(l, Tenh::Static_t<Tenh::EmptyList>::SINGLETON)),
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::TypeStringOf_t<List>::eval() << "::trailing_list<" << 0 << '>'),
                                     test_trailing_list<TypeList,0>,
                                     new Context::Data<pair<List,List> >(pair<List,List>(l, l)),
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::TypeStringOf_t<List>::eval() << "::leading_list<" << 1 << '>'),
                                     test_leading_list<TypeList,1>,
                                     new Context::Data<pair<List,List> >(pair<List,List>(l, l)),
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     FORMAT(Tenh::TypeStringOf_t<List>::eval() << "::trailing_list<" << 1 << '>'),
                                     test_trailing_list<TypeList,1>,
                                     new Context::Data<pair<List,Tenh::EmptyList> >(pair<List,Tenh::EmptyList>(l, Tenh::Static_t<Tenh::EmptyList>::SINGLETON)),
                                     RESULT_NO_ERROR);
}

void add_leading_and_trailing_list_tests (Directory *parent)
{
    add_leading_and_trailing_list_length_0_test(parent);
    add_leading_and_trailing_list_length_1_test(parent);
}

void AddTests (Directory *parent)
{
    Directory *list = new Directory("List_t", parent);

    LVD_ADD_TEST_CASE_FUNCTION(list, test_EmptyList, RESULT_NO_ERROR);
    add_particular_tests_for_list<Tenh::EmptyTypeList>(list);
    add_particular_tests_for_list<Tenh::TypeList_t<int> >(list);
    add_particular_tests_for_list<Tenh::TypeList_t<int,Tenh::TypeList_t<char> > >(list);
    add_leading_and_trailing_list_tests(list);
}

} // end of namespace List
} // end of namespace Test
