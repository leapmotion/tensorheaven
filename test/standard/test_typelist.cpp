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

#define FORMAT(x) static_cast<ostringstream &>(ostringstream().flush() << x).str()

void test_EmptyTypeList (Context const &context)
{
    assert((Lvd::Meta::TypesAreEqual<Tenh::EmptyTypeList::BodyTypeList,Tenh::EmptyTypeList>()));
    assert_eq(Tenh::EmptyTypeList::length(), 0);
    assert(!Tenh::EmptyTypeList::Contains_t<int>());
    assert((Lvd::Meta::TypesAreEqual<Tenh::EmptyTypeList::LeadingTypeList_t<0>::T,Tenh::EmptyTypeList>()));
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
    Directory *typelist = new Directory(Tenh::TypeStringOf_t<TypeList>::eval(), parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(typelist, "test_length", test_length<TypeList>, new Context::Data<Uint32>(EXPECTED_LENGTH), RESULT_NO_ERROR);
}

template <typename TypeList, Uint32 INDEX, typename ExpectedLeadingTypeList>
void test_leading_type_list (Context const &context)
{
    assert((Lvd::Meta::TypesAreEqual<typename TypeList::template LeadingTypeList_t<INDEX>::T,ExpectedLeadingTypeList>()));
}

void add_leading_type_list_tests (Directory *parent)
{
    typedef Tenh::TypeList_t<char> Tl1;
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent, 
                                     FORMAT(Tenh::TypeStringOf_t<Tl1>::eval() << "::LeadingTypeList_t<" << 0 << '>'),
                                     test_leading_type_list<Tl1,0,Tenh::EmptyTypeList>,
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent, 
                                     FORMAT(Tenh::TypeStringOf_t<Tl1>::eval() << "::LeadingTypeList_t<" << 1 << '>'),
                                     test_leading_type_list<Tl1,1,Tl1>,
                                     RESULT_NO_ERROR);

    typedef Tenh::TypeList_t<char,Tenh::TypeList_t<int> > Tl2;
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent, 
                                     FORMAT(Tenh::TypeStringOf_t<Tl2>::eval() << "::LeadingTypeList_t<" << 0 << '>'),
                                     test_leading_type_list<Tl2,0,Tenh::EmptyTypeList>,
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent, 
                                     FORMAT(Tenh::TypeStringOf_t<Tl2>::eval() << "::LeadingTypeList_t<" << 1 << '>'),
                                     test_leading_type_list<Tl2,1,Tl1>,
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent, 
                                     FORMAT(Tenh::TypeStringOf_t<Tl2>::eval() << "::LeadingTypeList_t<" << 2 << '>'),
                                     test_leading_type_list<Tl2,2,Tl2>,
                                     RESULT_NO_ERROR);

    typedef Tenh::TypeList_t<char,Tenh::TypeList_t<int,Tenh::TypeList_t<double> > > Tl3;
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent, 
                                     FORMAT(Tenh::TypeStringOf_t<Tl3>::eval() << "::LeadingTypeList_t<" << 0 << '>'),
                                     test_leading_type_list<Tl3,0,Tenh::EmptyTypeList>,
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent, 
                                     FORMAT(Tenh::TypeStringOf_t<Tl3>::eval() << "::LeadingTypeList_t<" << 1 << '>'),
                                     test_leading_type_list<Tl3,1,Tl1>,
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent, 
                                     FORMAT(Tenh::TypeStringOf_t<Tl3>::eval() << "::LeadingTypeList_t<" << 2 << '>'),
                                     test_leading_type_list<Tl3,2,Tl2>,
                                     RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent, 
                                     FORMAT(Tenh::TypeStringOf_t<Tl3>::eval() << "::LeadingTypeList_t<" << 3 << '>'),
                                     test_leading_type_list<Tl3,3,Tl3>,
                                     RESULT_NO_ERROR);
}

void AddTests (Directory *parent)
{
    Directory *typelist = new Directory("TypeList_t", parent);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(typelist, Tenh::TypeStringOf_t<Tenh::EmptyTypeList>::eval(), test_EmptyTypeList, RESULT_NO_ERROR);
    add_particular_tests_for_typelist<Tenh::TypeList_t<int>,1>(typelist);
    add_particular_tests_for_typelist<Tenh::TypeList_t<int,Tenh::TypeList_t<char> >,2>(typelist);

    add_leading_type_list_tests(typelist);
}

} // end of namespace TypeList
} // end of namespace Test
