// ///////////////////////////////////////////////////////////////////////////
// test_typle.cpp by Victor Dods, created 2014/01/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_typle.hpp"

#include "tenh/meta/typle.hpp"
#include "tenh/meta/typle_utility.hpp"

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

template <typename T_> struct IsFloatingPointType_f { static bool const V = false; };
template <> struct IsFloatingPointType_f<float> { static bool const V = true; };
template <> struct IsFloatingPointType_f<double> { static bool const V = true; };
template <> struct IsFloatingPointType_f<long double> { static bool const V = true; };

MAKE_1_ARY_VALUE_EVALUATOR(IsFloatingPointType, bool);

void test_OnEach (Context const &context)
{
    typedef Tenh::Value_t<bool,true> True;
    typedef Tenh::Value_t<bool,false> False;
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::OnEach_f<Tenh::Typle_t<>,IsFloatingPointType_e>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::OnEach_f<Tenh::Typle_t<int>,IsFloatingPointType_e>::T,
                                        Tenh::Typle_t<False>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::OnEach_f<Tenh::Typle_t<float>,IsFloatingPointType_e>::T,
                                        Tenh::Typle_t<True>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::OnEach_f<Tenh::Typle_t<float,int>,IsFloatingPointType_e>::T,
                                        Tenh::Typle_t<True,False>>::V, "error");

    typedef Tenh::Hippo::OnEach_e<IsFloatingPointType_e> OnEach_IsFloatingPointType_e;
    static_assert(Tenh::TypesAreEqual_f<OnEach_IsFloatingPointType_e::Eval_f<Tenh::Typle_t<>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<OnEach_IsFloatingPointType_e::Eval_f<Tenh::Typle_t<int>>::T,
                                        Tenh::Typle_t<False>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<OnEach_IsFloatingPointType_e::Eval_f<Tenh::Typle_t<float>>::T,
                                        Tenh::Typle_t<True>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<OnEach_IsFloatingPointType_e::Eval_f<Tenh::Typle_t<float,int>>::T,
                                        Tenh::Typle_t<True,False>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_NegationOfPredicate (Context const &context)
{
    typedef Tenh::Value_t<bool,true> True;
    typedef Tenh::Value_t<bool,false> False;
    typedef Tenh::Hippo::NegationOfPredicate_e<IsFloatingPointType_e> IsntFloatingPointType_e;
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::OnEach_f<Tenh::Typle_t<>,IsntFloatingPointType_e>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::OnEach_f<Tenh::Typle_t<int>,IsntFloatingPointType_e>::T,
                                        Tenh::Typle_t<True>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::OnEach_f<Tenh::Typle_t<float>,IsntFloatingPointType_e>::T,
                                        Tenh::Typle_t<False>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::OnEach_f<Tenh::Typle_t<float,int>,IsntFloatingPointType_e>::T,
                                        Tenh::Typle_t<False,True>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_And (Context const &context)
{
    typedef Tenh::Value_t<bool,true> True;
    typedef Tenh::Value_t<bool,false> False;
    static_assert(Tenh::Hippo::And_f<Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::Hippo::And_f<Tenh::Typle_t<True>>::V, "error");
    static_assert(!Tenh::Hippo::And_f<Tenh::Typle_t<False>>::V, "error");
    static_assert(Tenh::Hippo::And_f<Tenh::Typle_t<True,True>>::V, "error");
    static_assert(!Tenh::Hippo::And_f<Tenh::Typle_t<False,True>>::V, "error");
    static_assert(!Tenh::Hippo::And_f<Tenh::Typle_t<False,False>>::V, "error");

    typedef Tenh::Hippo::And_e And_e;
    static_assert(And_e::Eval_f<Tenh::Typle_t<>>::V, "error");
    static_assert(And_e::Eval_f<Tenh::Typle_t<True>>::V, "error");
    static_assert(!And_e::Eval_f<Tenh::Typle_t<False>>::V, "error");
    static_assert(And_e::Eval_f<Tenh::Typle_t<True,True>>::V, "error");
    static_assert(!And_e::Eval_f<Tenh::Typle_t<False,True>>::V, "error");
    static_assert(!And_e::Eval_f<Tenh::Typle_t<False,False>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_Or (Context const &context)
{
    typedef Tenh::Value_t<bool,true> True;
    typedef Tenh::Value_t<bool,false> False;
    static_assert(!Tenh::Hippo::Or_f<Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::Hippo::Or_f<Tenh::Typle_t<True>>::V, "error");
    static_assert(!Tenh::Hippo::Or_f<Tenh::Typle_t<False>>::V, "error");
    static_assert(Tenh::Hippo::Or_f<Tenh::Typle_t<True,True>>::V, "error");
    static_assert(Tenh::Hippo::Or_f<Tenh::Typle_t<False,True>>::V, "error");
    static_assert(!Tenh::Hippo::Or_f<Tenh::Typle_t<False,False>>::V, "error");

    typedef Tenh::Hippo::Or_e Or_e;
    static_assert(!Or_e::Eval_f<Tenh::Typle_t<>>::V, "error");
    static_assert(Or_e::Eval_f<Tenh::Typle_t<True>>::V, "error");
    static_assert(!Or_e::Eval_f<Tenh::Typle_t<False>>::V, "error");
    static_assert(Or_e::Eval_f<Tenh::Typle_t<True,True>>::V, "error");
    static_assert(Or_e::Eval_f<Tenh::Typle_t<False,True>>::V, "error");
    static_assert(!Or_e::Eval_f<Tenh::Typle_t<False,False>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_EachTypeSatisfies (Context const &context)
{
    static_assert(Tenh::Hippo::EachTypeSatisfies_f<Tenh::Typle_t<>,IsFloatingPointType_e>::V, "error");
    static_assert(Tenh::Hippo::EachTypeSatisfies_f<Tenh::Typle_t<float>,IsFloatingPointType_e>::V, "error");
    static_assert(!Tenh::Hippo::EachTypeSatisfies_f<Tenh::Typle_t<int>,IsFloatingPointType_e>::V, "error");
    static_assert(Tenh::Hippo::EachTypeSatisfies_f<Tenh::Typle_t<float,double>,IsFloatingPointType_e>::V, "error");
    static_assert(!Tenh::Hippo::EachTypeSatisfies_f<Tenh::Typle_t<int,double>,IsFloatingPointType_e>::V, "error");

    typedef Tenh::Hippo::EachTypeSatisfies_e<IsFloatingPointType_e> EachTypeIsFloatingPointType_e;
    static_assert(EachTypeIsFloatingPointType_e::Eval_f<Tenh::Typle_t<>>::V, "error");
    static_assert(EachTypeIsFloatingPointType_e::Eval_f<Tenh::Typle_t<float>>::V, "error");
    static_assert(!EachTypeIsFloatingPointType_e::Eval_f<Tenh::Typle_t<int>>::V, "error");
    static_assert(EachTypeIsFloatingPointType_e::Eval_f<Tenh::Typle_t<float,double>>::V, "error");
    static_assert(!EachTypeIsFloatingPointType_e::Eval_f<Tenh::Typle_t<int,double>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_Sum (Context const &context)
{
    typedef Tenh::Value_t<int,0> Zero;
    typedef Tenh::Value_t<int,1> One;
    typedef Tenh::Value_t<int,3> Three;
    static_assert(Tenh::Hippo::Sum_f<Tenh::Typle_t<>,int>::V == 0, "error");
    static_assert(Tenh::Hippo::Sum_f<Tenh::Typle_t<Zero>,int>::V == 0, "error");
    static_assert(Tenh::Hippo::Sum_f<Tenh::Typle_t<Zero,One>,int>::V == 1, "error");
    static_assert(Tenh::Hippo::Sum_f<Tenh::Typle_t<One,One>,int>::V == 2, "error");
    static_assert(Tenh::Hippo::Sum_f<Tenh::Typle_t<One,One,Three>,int>::V == 5, "error");

    typedef Tenh::Hippo::Sum_e<int> IntSum_e;
    static_assert(IntSum_e::Eval_f<Tenh::Typle_t<>>::V == 0, "error");
    static_assert(IntSum_e::Eval_f<Tenh::Typle_t<Zero>>::V == 0, "error");
    static_assert(IntSum_e::Eval_f<Tenh::Typle_t<Zero,One>>::V == 1, "error");
    static_assert(IntSum_e::Eval_f<Tenh::Typle_t<One,One>>::V == 2, "error");
    static_assert(IntSum_e::Eval_f<Tenh::Typle_t<One,One,Three>>::V == 5, "error");

    // there is currently no runtime test code here, but that could change
}

void test_Product (Context const &context)
{
    typedef Tenh::Value_t<int,0> Zero;
    typedef Tenh::Value_t<int,1> One;
    typedef Tenh::Value_t<int,3> Three;
    static_assert(Tenh::Hippo::Product_f<Tenh::Typle_t<>,int>::V == 1, "error");
    static_assert(Tenh::Hippo::Product_f<Tenh::Typle_t<Zero>,int>::V == 0, "error");
    static_assert(Tenh::Hippo::Product_f<Tenh::Typle_t<Zero,One>,int>::V == 0, "error");
    static_assert(Tenh::Hippo::Product_f<Tenh::Typle_t<One,One>,int>::V == 1, "error");
    static_assert(Tenh::Hippo::Product_f<Tenh::Typle_t<One,One,Three>,int>::V == 3, "error");
    static_assert(Tenh::Hippo::Product_f<Tenh::Typle_t<Three,Three>,int>::V == 9, "error");
    static_assert(Tenh::Hippo::Product_f<Tenh::Typle_t<Three,Three,Three>,int>::V == 27, "error");

    typedef Tenh::Hippo::Product_e<int> IntProduct_e;
    static_assert(IntProduct_e::Eval_f<Tenh::Typle_t<>>::V == 1, "error");
    static_assert(IntProduct_e::Eval_f<Tenh::Typle_t<Zero>>::V == 0, "error");
    static_assert(IntProduct_e::Eval_f<Tenh::Typle_t<Zero,One>>::V == 0, "error");
    static_assert(IntProduct_e::Eval_f<Tenh::Typle_t<One,One>>::V == 1, "error");
    static_assert(IntProduct_e::Eval_f<Tenh::Typle_t<One,One,Three>>::V == 3, "error");
    static_assert(IntProduct_e::Eval_f<Tenh::Typle_t<Three,Three>>::V == 9, "error");
    static_assert(IntProduct_e::Eval_f<Tenh::Typle_t<Three,Three,Three>>::V == 27, "error");

    // there is currently no runtime test code here, but that could change
}

void test_Min (Context const &context)
{
    typedef Tenh::Value_t<int,0> Zero;
    typedef Tenh::Value_t<int,1> One;
    typedef Tenh::Value_t<int,3> Three;
    static_assert(Tenh::Hippo::Min_f<Tenh::Typle_t<>,int>::V == Tenh::NumericBound_t<int>::MAX, "error");
    static_assert(Tenh::Hippo::Min_f<Tenh::Typle_t<Zero,One>,int>::V == 0, "error");
    static_assert(Tenh::Hippo::Min_f<Tenh::Typle_t<One,One>,int>::V == 1, "error");
    static_assert(Tenh::Hippo::Min_f<Tenh::Typle_t<One,One,Three>,int>::V == 1, "error");
    static_assert(Tenh::Hippo::Min_f<Tenh::Typle_t<Three,Three>,int>::V == 3, "error");

    typedef Tenh::Hippo::Min_e<int> IntMin_e;
    static_assert(IntMin_e::Eval_f<Tenh::Typle_t<>>::V == std::numeric_limits<int>::max(), "error");
    static_assert(IntMin_e::Eval_f<Tenh::Typle_t<Zero,One>>::V == 0, "error");
    static_assert(IntMin_e::Eval_f<Tenh::Typle_t<One,One>>::V == 1, "error");
    static_assert(IntMin_e::Eval_f<Tenh::Typle_t<One,One,Three>>::V == 1, "error");
    static_assert(IntMin_e::Eval_f<Tenh::Typle_t<Three,Three>>::V == 3, "error");

    // there is currently no runtime test code here, but that could change
}

void test_Max (Context const &context)
{
    typedef Tenh::Value_t<int,0> Zero;
    typedef Tenh::Value_t<int,1> One;
    typedef Tenh::Value_t<int,3> Three;
    static_assert(Tenh::Hippo::Max_f<Tenh::Typle_t<>,int>::V == Tenh::NumericBound_t<int>::MIN, "error");
    static_assert(Tenh::Hippo::Max_f<Tenh::Typle_t<Zero,One>,int>::V == 1, "error");
    static_assert(Tenh::Hippo::Max_f<Tenh::Typle_t<One,One>,int>::V == 1, "error");
    static_assert(Tenh::Hippo::Max_f<Tenh::Typle_t<One,One,Three>,int>::V == 3, "error");
    static_assert(Tenh::Hippo::Max_f<Tenh::Typle_t<Three,Three>,int>::V == 3, "error");

    typedef Tenh::Hippo::Max_e<int> IntMax_e;
    static_assert(IntMax_e::Eval_f<Tenh::Typle_t<>>::V == std::numeric_limits<int>::min(), "error");
    static_assert(IntMax_e::Eval_f<Tenh::Typle_t<Zero,One>>::V == 1, "error");
    static_assert(IntMax_e::Eval_f<Tenh::Typle_t<One,One>>::V == 1, "error");
    static_assert(IntMax_e::Eval_f<Tenh::Typle_t<One,One,Three>>::V == 3, "error");
    static_assert(IntMax_e::Eval_f<Tenh::Typle_t<Three,Three>>::V == 3, "error");

    // there is currently no runtime test code here, but that could change
}

void test_Element (Context const &context)
{
    typedef Tenh::Typle_t<int,float,bool,char> T;
    // uncommenting the following line should cause a static assert about Typle_t<> having no elements
    //Tenh::Hippo::Element_f<Tenh::Typle_t<>,0>::T x;
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Element_f<T,0>::T,int>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Element_f<T,1>::T,float>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Element_f<T,2>::T,bool>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Element_f<T,3>::T,char>::V, "error");

    typedef Tenh::Hippo::Element_e<0> Element0_e;
    typedef Tenh::Hippo::Element_e<1> Element1_e;
    typedef Tenh::Hippo::Element_e<2> Element2_e;
    typedef Tenh::Hippo::Element_e<3> Element3_e;
    static_assert(Tenh::TypesAreEqual_f<Element0_e::Eval_f<T>::T,int>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Element1_e::Eval_f<T>::T,float>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Element2_e::Eval_f<T>::T,bool>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Element3_e::Eval_f<T>::T,char>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_Contains (Context const &context)
{
    static_assert(!Tenh::Hippo::Contains_f<Tenh::Typle_t<>,int>::V, "error");
    static_assert(Tenh::Hippo::Contains_f<Tenh::Typle_t<int,float,bool,char>,int>::V, "error");
    static_assert(Tenh::Hippo::Contains_f<Tenh::Typle_t<int,float,bool,char>,float>::V, "error");
    static_assert(!Tenh::Hippo::Contains_f<Tenh::Typle_t<int,float,bool,char>,double>::V, "error");

    typedef Tenh::Hippo::Contains_e<int> ContainsInt_e;
    typedef Tenh::Hippo::Contains_e<float> ContainsFloat_e;
    typedef Tenh::Hippo::Contains_e<double> ContainsDouble_e;
    static_assert(!ContainsInt_e::Eval_f<Tenh::Typle_t<>>::V, "error");
    static_assert(ContainsInt_e::Eval_f<Tenh::Typle_t<int,float,bool,char>>::V, "error");
    static_assert(ContainsFloat_e::Eval_f<Tenh::Typle_t<int,float,bool,char>>::V, "error");
    static_assert(!ContainsDouble_e::Eval_f<Tenh::Typle_t<int,float,bool,char>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_IndexOfFirstOccurrence (Context const &context)
{
    typedef Tenh::Typle_t<int,float,bool,float,char> T;
    static_assert(Tenh::Hippo::IndexOfFirstOccurrence_f<T,int>::V == 0, "error");
    static_assert(Tenh::Hippo::IndexOfFirstOccurrence_f<T,float>::V == 1, "error");
    static_assert(Tenh::Hippo::IndexOfFirstOccurrence_f<T,bool>::V == 2, "error");
    static_assert(Tenh::Hippo::IndexOfFirstOccurrence_f<T,char>::V == 4, "error");
    static_assert(Tenh::Hippo::IndexOfFirstOccurrence_f<T,double>::V == 5, "error");

    typedef Tenh::Hippo::IndexOfFirstOccurrence_e<int> IndexOfFirstOccurrenceOfInt_e;
    typedef Tenh::Hippo::IndexOfFirstOccurrence_e<float> IndexOfFirstOccurrenceOfFloat_e;
    typedef Tenh::Hippo::IndexOfFirstOccurrence_e<bool> IndexOfFirstOccurrenceOfBool_e;
    typedef Tenh::Hippo::IndexOfFirstOccurrence_e<char> IndexOfFirstOccurrenceOfChar_e;
    typedef Tenh::Hippo::IndexOfFirstOccurrence_e<double> IndexOfFirstOccurrenceOfDouble_e;
    static_assert(IndexOfFirstOccurrenceOfInt_e::Eval_f<T>::V == 0, "error");
    static_assert(IndexOfFirstOccurrenceOfFloat_e::Eval_f<T>::V == 1, "error");
    static_assert(IndexOfFirstOccurrenceOfBool_e::Eval_f<T>::V == 2, "error");
    static_assert(IndexOfFirstOccurrenceOfChar_e::Eval_f<T>::V == 4, "error");
    static_assert(IndexOfFirstOccurrenceOfDouble_e::Eval_f<T>::V == 5, "error");

    // there is currently no runtime test code here, but that could change
}

void test_TrailingTyple (Context const &context)
{
    typedef Tenh::Typle_t<int,float,bool,float,char> T;
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TrailingTyple_f<T,0>::T,T>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TrailingTyple_f<T,1>::T,Tenh::Typle_t<float,bool,float,char>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TrailingTyple_f<T,2>::T,Tenh::Typle_t<bool,float,char>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TrailingTyple_f<T,3>::T,Tenh::Typle_t<float,char>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TrailingTyple_f<T,4>::T,Tenh::Typle_t<char>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TrailingTyple_f<T,5>::T,Tenh::Typle_t<>>::V, "error");

    typedef Tenh::Hippo::TrailingTyple_e<0> TrailingTyple0_e;
    typedef Tenh::Hippo::TrailingTyple_e<1> TrailingTyple1_e;
    typedef Tenh::Hippo::TrailingTyple_e<2> TrailingTyple2_e;
    typedef Tenh::Hippo::TrailingTyple_e<3> TrailingTyple3_e;
    typedef Tenh::Hippo::TrailingTyple_e<4> TrailingTyple4_e;
    typedef Tenh::Hippo::TrailingTyple_e<5> TrailingTyple5_e;
    static_assert(Tenh::TypesAreEqual_f<TrailingTyple0_e::Eval_f<T>::T,T>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<TrailingTyple1_e::Eval_f<T>::T,Tenh::Typle_t<float,bool,float,char>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<TrailingTyple2_e::Eval_f<T>::T,Tenh::Typle_t<bool,float,char>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<TrailingTyple3_e::Eval_f<T>::T,Tenh::Typle_t<float,char>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<TrailingTyple4_e::Eval_f<T>::T,Tenh::Typle_t<char>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<TrailingTyple5_e::Eval_f<T>::T,Tenh::Typle_t<>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_LeadingTyple (Context const &context)
{
    typedef Tenh::Typle_t<int,float,bool,float,char> T;
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::LeadingTyple_f<T,0>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::LeadingTyple_f<T,1>::T,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::LeadingTyple_f<T,2>::T,Tenh::Typle_t<int,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::LeadingTyple_f<T,3>::T,Tenh::Typle_t<int,float,bool>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::LeadingTyple_f<T,4>::T,Tenh::Typle_t<int,float,bool,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::LeadingTyple_f<T,5>::T,T>::V, "error");

    typedef Tenh::Hippo::LeadingTyple_e<0> LeadingTyple0_e;
    typedef Tenh::Hippo::LeadingTyple_e<1> LeadingTyple1_e;
    typedef Tenh::Hippo::LeadingTyple_e<2> LeadingTyple2_e;
    typedef Tenh::Hippo::LeadingTyple_e<3> LeadingTyple3_e;
    typedef Tenh::Hippo::LeadingTyple_e<4> LeadingTyple4_e;
    typedef Tenh::Hippo::LeadingTyple_e<5> LeadingTyple5_e;
    static_assert(Tenh::TypesAreEqual_f<LeadingTyple0_e::Eval_f<T>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<LeadingTyple1_e::Eval_f<T>::T,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<LeadingTyple2_e::Eval_f<T>::T,Tenh::Typle_t<int,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<LeadingTyple3_e::Eval_f<T>::T,Tenh::Typle_t<int,float,bool>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<LeadingTyple4_e::Eval_f<T>::T,Tenh::Typle_t<int,float,bool,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<LeadingTyple5_e::Eval_f<T>::T,T>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_TypleRange (Context const &context)
{
    typedef Tenh::Typle_t<int,float,bool,float,char> T;
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,0,0>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,0,1>::T,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,0,2>::T,Tenh::Typle_t<int,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,0,3>::T,Tenh::Typle_t<int,float,bool>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,0,4>::T,Tenh::Typle_t<int,float,bool,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,0,5>::T,Tenh::Typle_t<int,float,bool,float,char>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,1,1>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,1,2>::T,Tenh::Typle_t<float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,1,3>::T,Tenh::Typle_t<float,bool>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,1,4>::T,Tenh::Typle_t<float,bool,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,1,5>::T,Tenh::Typle_t<float,bool,float,char>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,2,2>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,2,3>::T,Tenh::Typle_t<bool>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,2,4>::T,Tenh::Typle_t<bool,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,2,5>::T,Tenh::Typle_t<bool,float,char>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,3,3>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,3,4>::T,Tenh::Typle_t<float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,3,5>::T,Tenh::Typle_t<float,char>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,4,4>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,4,5>::T,Tenh::Typle_t<char>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_f<T,5,5>::T,Tenh::Typle_t<>>::V, "error");


    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<0,0>::Eval_f<T>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<0,1>::Eval_f<T>::T,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<0,2>::Eval_f<T>::T,Tenh::Typle_t<int,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<0,3>::Eval_f<T>::T,Tenh::Typle_t<int,float,bool>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<0,4>::Eval_f<T>::T,Tenh::Typle_t<int,float,bool,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<0,5>::Eval_f<T>::T,Tenh::Typle_t<int,float,bool,float,char>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<1,1>::Eval_f<T>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<1,2>::Eval_f<T>::T,Tenh::Typle_t<float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<1,3>::Eval_f<T>::T,Tenh::Typle_t<float,bool>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<1,4>::Eval_f<T>::T,Tenh::Typle_t<float,bool,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<1,5>::Eval_f<T>::T,Tenh::Typle_t<float,bool,float,char>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<2,2>::Eval_f<T>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<2,3>::Eval_f<T>::T,Tenh::Typle_t<bool>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<2,4>::Eval_f<T>::T,Tenh::Typle_t<bool,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<2,5>::Eval_f<T>::T,Tenh::Typle_t<bool,float,char>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<3,3>::Eval_f<T>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<3,4>::Eval_f<T>::T,Tenh::Typle_t<float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<3,5>::Eval_f<T>::T,Tenh::Typle_t<float,char>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<4,4>::Eval_f<T>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<4,5>::Eval_f<T>::T,Tenh::Typle_t<char>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::TypleRange_e<5,5>::Eval_f<T>::T,Tenh::Typle_t<>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_UniqueTypesIn (Context const &context)
{
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniqueTypesIn_f<Tenh::Typle_t<>>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniqueTypesIn_f<Tenh::Typle_t<int>>::T,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniqueTypesIn_f<Tenh::Typle_t<int,int>>::T,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniqueTypesIn_f<Tenh::Typle_t<int,float,int>>::T,Tenh::Typle_t<int,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniqueTypesIn_f<Tenh::Typle_t<int,float,int,char>>::T,Tenh::Typle_t<int,float,char>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniqueTypesIn_e::Eval_f<Tenh::Typle_t<>>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniqueTypesIn_e::Eval_f<Tenh::Typle_t<int>>::T,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniqueTypesIn_e::Eval_f<Tenh::Typle_t<int,int>>::T,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniqueTypesIn_e::Eval_f<Tenh::Typle_t<int,float,int>>::T,Tenh::Typle_t<int,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniqueTypesIn_e::Eval_f<Tenh::Typle_t<int,float,int,char>>::T,Tenh::Typle_t<int,float,char>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_OccurrenceCount (Context const &context)
{
    static_assert(Tenh::Hippo::OccurrenceCount_f<Tenh::Typle_t<>,int>::V == 0, "error");
    static_assert(Tenh::Hippo::OccurrenceCount_f<Tenh::Typle_t<int>,int>::V == 1, "error");
    static_assert(Tenh::Hippo::OccurrenceCount_f<Tenh::Typle_t<int,int>,int>::V == 2, "error");
    static_assert(Tenh::Hippo::OccurrenceCount_f<Tenh::Typle_t<int,char,int>,int>::V == 2, "error");
    static_assert(Tenh::Hippo::OccurrenceCount_f<Tenh::Typle_t<int,char,int>,char>::V == 1, "error");
    static_assert(Tenh::Hippo::OccurrenceCount_f<Tenh::Typle_t<int,char,int>,float>::V == 0, "error");

    static_assert(Tenh::Hippo::OccurrenceCount_e<int>::Eval_f<Tenh::Typle_t<>>::V == 0, "error");
    static_assert(Tenh::Hippo::OccurrenceCount_e<int>::Eval_f<Tenh::Typle_t<int>>::V == 1, "error");
    static_assert(Tenh::Hippo::OccurrenceCount_e<int>::Eval_f<Tenh::Typle_t<int,int>>::V == 2, "error");
    static_assert(Tenh::Hippo::OccurrenceCount_e<int>::Eval_f<Tenh::Typle_t<int,char,int>>::V == 2, "error");
    static_assert(Tenh::Hippo::OccurrenceCount_e<char>::Eval_f<Tenh::Typle_t<int,char,int>>::V == 1, "error");
    static_assert(Tenh::Hippo::OccurrenceCount_e<float>::Eval_f<Tenh::Typle_t<int,char,int>>::V == 0, "error");

    // there is currently no runtime test code here, but that could change
}

void test_ElementsHavingMultiplicity (Context const &context)
{
    typedef Tenh::Typle_t<int,char,int,float,float,double,double,double,int,bool> T;
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::ElementsHavingMultiplicity_f<T,1>::T,Tenh::Typle_t<char,bool>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::ElementsHavingMultiplicity_f<T,2>::T,Tenh::Typle_t<float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::ElementsHavingMultiplicity_f<T,3>::T,Tenh::Typle_t<int,double>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::ElementsHavingMultiplicity_f<T,4>::T,Tenh::Typle_t<>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::ElementsHavingMultiplicity_e<1>::Eval_f<T>::T,Tenh::Typle_t<char,bool>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::ElementsHavingMultiplicity_e<2>::Eval_f<T>::T,Tenh::Typle_t<float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::ElementsHavingMultiplicity_e<3>::Eval_f<T>::T,Tenh::Typle_t<int,double>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::ElementsHavingMultiplicity_e<4>::Eval_f<T>::T,Tenh::Typle_t<>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_ElementsOfTypleSatisfying (Context const &context)
{
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::ElementsOfTypleSatisfying_f<Tenh::Typle_t<int,char,int,float,float,double,double,double,int,bool>,
                                                                                 IsFloatingPointType_e>::T,
                                        Tenh::Typle_t<float,float,double,double,double>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::ElementsOfTypleSatisfying_f<Tenh::Typle_t<int,char,int,int,bool>,
                                                                                 IsFloatingPointType_e>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::ElementsOfTypleSatisfying_f<Tenh::Typle_t<int>,
                                                                                 IsFloatingPointType_e>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::ElementsOfTypleSatisfying_f<Tenh::Typle_t<float>,
                                                                                 IsFloatingPointType_e>::T,
                                        Tenh::Typle_t<float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::ElementsOfTypleSatisfying_f<Tenh::Typle_t<>,
                                                                                 IsFloatingPointType_e>::T,
                                        Tenh::Typle_t<>>::V, "error");

    typedef Tenh::Hippo::ElementsOfTypleSatisfying_e<IsFloatingPointType_e> ElementsOfTypleThatAreFloatingPointType_e;
    static_assert(Tenh::TypesAreEqual_f<ElementsOfTypleThatAreFloatingPointType_e::Eval_f<Tenh::Typle_t<int,char,int,float,float,double,double,double,int,bool>>::T,
                                        Tenh::Typle_t<float,float,double,double,double>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<ElementsOfTypleThatAreFloatingPointType_e::Eval_f<Tenh::Typle_t<int,char,int,int,bool>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<ElementsOfTypleThatAreFloatingPointType_e::Eval_f<Tenh::Typle_t<int>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<ElementsOfTypleThatAreFloatingPointType_e::Eval_f<Tenh::Typle_t<float>>::T,
                                        Tenh::Typle_t<float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<ElementsOfTypleThatAreFloatingPointType_e::Eval_f<Tenh::Typle_t<>>::T,
                                        Tenh::Typle_t<>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_ContainsDuplicates (Context const &context)
{
    static_assert(!Tenh::Hippo::ContainsDuplicates_f<Tenh::Typle_t<>>::V, "error");
    static_assert(!Tenh::Hippo::ContainsDuplicates_f<Tenh::Typle_t<int>>::V, "error");
    static_assert(!Tenh::Hippo::ContainsDuplicates_f<Tenh::Typle_t<int,char>>::V, "error");
    static_assert(Tenh::Hippo::ContainsDuplicates_f<Tenh::Typle_t<char,char>>::V, "error");
    static_assert(Tenh::Hippo::ContainsDuplicates_f<Tenh::Typle_t<int,char,int>>::V, "error");
    static_assert(!Tenh::Hippo::ContainsDuplicates_f<Tenh::Typle_t<int,char,float>>::V, "error");

    typedef Tenh::Hippo::ContainsDuplicates_e ContainsDuplicates_e;
    static_assert(!ContainsDuplicates_e::Eval_f<Tenh::Typle_t<>>::V, "error");
    static_assert(!ContainsDuplicates_e::Eval_f<Tenh::Typle_t<int>>::V, "error");
    static_assert(!ContainsDuplicates_e::Eval_f<Tenh::Typle_t<int,char>>::V, "error");
    static_assert(ContainsDuplicates_e::Eval_f<Tenh::Typle_t<char,char>>::V, "error");
    static_assert(ContainsDuplicates_e::Eval_f<Tenh::Typle_t<int,char,int>>::V, "error");
    static_assert(!ContainsDuplicates_e::Eval_f<Tenh::Typle_t<int,char,float>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_IsASubsetOf (Context const &context)
{
    static_assert(Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<>,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<>,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<>,Tenh::Typle_t<int,char>>::V, "error");

    static_assert(!Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<int>,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<int>,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<int>,Tenh::Typle_t<int,char>>::V, "error");

    static_assert(!Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<char>,Tenh::Typle_t<>>::V, "error");
    static_assert(!Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<char>,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<char>,Tenh::Typle_t<int,char>>::V, "error");

    static_assert(!Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<int,char>,Tenh::Typle_t<>>::V, "error");
    static_assert(!Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<int,char>,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<int,char>,Tenh::Typle_t<int,char>>::V, "error");

    // test order invariance
    static_assert(Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<char,int>,Tenh::Typle_t<int,char>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_f<Tenh::Typle_t<char,float,bool>,Tenh::Typle_t<float,bool,char>>::V, "error");


    static_assert(Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<>>::Eval_f<Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<int>>::Eval_f<Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<int,char>>::Eval_f<Tenh::Typle_t<>>::V, "error");

    static_assert(!Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<>>::Eval_f<Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<int>>::Eval_f<Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<int,char>>::Eval_f<Tenh::Typle_t<int>>::V, "error");

    static_assert(!Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<>>::Eval_f<Tenh::Typle_t<char>>::V, "error");
    static_assert(!Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<int>>::Eval_f<Tenh::Typle_t<char>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<int,char>>::Eval_f<Tenh::Typle_t<char>>::V, "error");

    static_assert(!Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<>>::Eval_f<Tenh::Typle_t<int,char>>::V, "error");
    static_assert(!Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<int>>::Eval_f<Tenh::Typle_t<int,char>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<int,char>>::Eval_f<Tenh::Typle_t<int,char>>::V, "error");

    // test order invariance
    static_assert(Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<int,char>>::Eval_f<Tenh::Typle_t<char,int>>::V, "error");
    static_assert(Tenh::Hippo::IsASubsetOf_e<Tenh::Typle_t<float,bool,char>>::Eval_f<Tenh::Typle_t<char,float,bool>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_AreEqualAsSets (Context const &context)
{
    static_assert(Tenh::Hippo::AreEqualAsSets_f<Tenh::Typle_t<>,Tenh::Typle_t<>>::V, "error");
    static_assert(!Tenh::Hippo::AreEqualAsSets_f<Tenh::Typle_t<int>,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::Hippo::AreEqualAsSets_f<Tenh::Typle_t<int>,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::Hippo::AreEqualAsSets_f<Tenh::Typle_t<int,char>,Tenh::Typle_t<char,int>>::V, "error");

    static_assert(Tenh::Hippo::AreEqualAsSets_e<Tenh::Typle_t<>>::Eval_f<Tenh::Typle_t<>>::V, "error");
    static_assert(!Tenh::Hippo::AreEqualAsSets_e<Tenh::Typle_t<>>::Eval_f<Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::Hippo::AreEqualAsSets_e<Tenh::Typle_t<int>>::Eval_f<Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::Hippo::AreEqualAsSets_e<Tenh::Typle_t<char,int>>::Eval_f<Tenh::Typle_t<int,char>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_HasNontrivialIntersectionAsSets (Context const &context)
{
    static_assert(!Tenh::Hippo::HasNontrivialIntersectionAsSets_f<Tenh::Typle_t<>,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::Hippo::HasNontrivialIntersectionAsSets_f<Tenh::Typle_t<int>,Tenh::Typle_t<int>>::V, "error");
    static_assert(!Tenh::Hippo::HasNontrivialIntersectionAsSets_f<Tenh::Typle_t<float>,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::Hippo::HasNontrivialIntersectionAsSets_f<Tenh::Typle_t<float,int>,Tenh::Typle_t<int,float>>::V, "error");
    static_assert(Tenh::Hippo::HasNontrivialIntersectionAsSets_f<Tenh::Typle_t<float,int,char>,Tenh::Typle_t<int,double>>::V, "error");

    static_assert(!Tenh::Hippo::HasNontrivialIntersectionAsSets_e<Tenh::Typle_t<>>::Eval_f<Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::Hippo::HasNontrivialIntersectionAsSets_e<Tenh::Typle_t<int>>::Eval_f<Tenh::Typle_t<int>>::V, "error");
    static_assert(!Tenh::Hippo::HasNontrivialIntersectionAsSets_e<Tenh::Typle_t<int>>::Eval_f<Tenh::Typle_t<float>>::V, "error");
    static_assert(Tenh::Hippo::HasNontrivialIntersectionAsSets_e<Tenh::Typle_t<int,float>>::Eval_f<Tenh::Typle_t<float,int>>::V, "error");
    static_assert(Tenh::Hippo::HasNontrivialIntersectionAsSets_e<Tenh::Typle_t<int,double>>::Eval_f<Tenh::Typle_t<float,int,char>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_IntersectionAsSets (Context const &context)
{
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::IntersectionAsSets_f<Tenh::Typle_t<>,Tenh::Typle_t<>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::IntersectionAsSets_f<Tenh::Typle_t<int>,Tenh::Typle_t<int>>::T,
                                        Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::IntersectionAsSets_f<Tenh::Typle_t<float>,Tenh::Typle_t<int>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::IntersectionAsSets_f<Tenh::Typle_t<float,int>,Tenh::Typle_t<int,float>>::T,
                                        Tenh::Typle_t<float,int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::IntersectionAsSets_f<Tenh::Typle_t<float,int,char>,Tenh::Typle_t<int,double>>::T,
                                        Tenh::Typle_t<int>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::IntersectionAsSets_e<Tenh::Typle_t<>>::Eval_f<Tenh::Typle_t<>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::IntersectionAsSets_e<Tenh::Typle_t<int>>::Eval_f<Tenh::Typle_t<int>>::T,
                                        Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::IntersectionAsSets_e<Tenh::Typle_t<int>>::Eval_f<Tenh::Typle_t<float>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::IntersectionAsSets_e<Tenh::Typle_t<int,float>>::Eval_f<Tenh::Typle_t<float,int>>::T,
                                        Tenh::Typle_t<float,int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::IntersectionAsSets_e<Tenh::Typle_t<int,double>>::Eval_f<Tenh::Typle_t<float,int,char>>::T,
                                        Tenh::Typle_t<int>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_SetSubtraction (Context const &context)
{
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<>,Tenh::Typle_t<>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<>,Tenh::Typle_t<int>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<>,Tenh::Typle_t<int,float>>::T,
                                        Tenh::Typle_t<>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<int>,Tenh::Typle_t<>>::T,
                                        Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<int>,Tenh::Typle_t<int>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<int>,Tenh::Typle_t<int,float>>::T,
                                        Tenh::Typle_t<>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<int,float>,Tenh::Typle_t<>>::T,
                                        Tenh::Typle_t<int,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<int,float>,Tenh::Typle_t<int>>::T,
                                        Tenh::Typle_t<float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<int,float>,Tenh::Typle_t<int,float>>::T,
                                        Tenh::Typle_t<>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<int,float,double>,Tenh::Typle_t<>>::T,
                                        Tenh::Typle_t<int,float,double>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<int,float,double>,Tenh::Typle_t<int>>::T,
                                        Tenh::Typle_t<float,double>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<int,float,double>,Tenh::Typle_t<int,float>>::T,
                                        Tenh::Typle_t<double>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<>,Tenh::Typle_t<>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<>,Tenh::Typle_t<int>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_f<Tenh::Typle_t<>,Tenh::Typle_t<int,float>>::T,
                                        Tenh::Typle_t<>>::V, "error");


    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_e<Tenh::Typle_t<>>::Eval_f<Tenh::Typle_t<int>>::T,
                                        Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_e<Tenh::Typle_t<int>>::Eval_f<Tenh::Typle_t<int>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_e<Tenh::Typle_t<int,float>>::Eval_f<Tenh::Typle_t<int>>::T,
                                        Tenh::Typle_t<>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_e<Tenh::Typle_t<>>::Eval_f<Tenh::Typle_t<int,float>>::T,
                                        Tenh::Typle_t<int,float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_e<Tenh::Typle_t<int>>::Eval_f<Tenh::Typle_t<int,float>>::T,
                                        Tenh::Typle_t<float>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_e<Tenh::Typle_t<int,float>>::Eval_f<Tenh::Typle_t<int,float>>::T,
                                        Tenh::Typle_t<>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_e<Tenh::Typle_t<>>::Eval_f<Tenh::Typle_t<int,float,double>>::T,
                                        Tenh::Typle_t<int,float,double>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_e<Tenh::Typle_t<int>>::Eval_f<Tenh::Typle_t<int,float,double>>::T,
                                        Tenh::Typle_t<float,double>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::SetSubtraction_e<Tenh::Typle_t<int,float>>::Eval_f<Tenh::Typle_t<int,float,double>>::T,
                                        Tenh::Typle_t<double>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_EachTypleHasEqualLength (Context const &context)
{
    static_assert(Tenh::Hippo::EachTypleHasEqualLength_f<Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::Hippo::EachTypleHasEqualLength_f<Tenh::Typle_t<Tenh::Typle_t<>>>::V, "error");
    static_assert(Tenh::Hippo::EachTypleHasEqualLength_f<Tenh::Typle_t<Tenh::Typle_t<>,Tenh::Typle_t<>>>::V, "error");
    static_assert(Tenh::Hippo::EachTypleHasEqualLength_f<Tenh::Typle_t<Tenh::Typle_t<>,Tenh::Typle_t<>,Tenh::Typle_t<>>>::V, "error");

    static_assert(!Tenh::Hippo::EachTypleHasEqualLength_f<Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<>,Tenh::Typle_t<>>>::V, "error");
    static_assert(!Tenh::Hippo::EachTypleHasEqualLength_f<Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<int>,Tenh::Typle_t<>>>::V, "error");
    static_assert(Tenh::Hippo::EachTypleHasEqualLength_f<Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<int>,Tenh::Typle_t<int>>>::V, "error");

    static_assert(!Tenh::Hippo::EachTypleHasEqualLength_f<Tenh::Typle_t<Tenh::Typle_t<int,int>,Tenh::Typle_t<int>,Tenh::Typle_t<>>>::V, "error");
    static_assert(!Tenh::Hippo::EachTypleHasEqualLength_f<Tenh::Typle_t<Tenh::Typle_t<int,int>,Tenh::Typle_t<int,int,int>,Tenh::Typle_t<>>>::V, "error");
    static_assert(Tenh::Hippo::EachTypleHasEqualLength_f<Tenh::Typle_t<Tenh::Typle_t<int,int>,Tenh::Typle_t<int,int>,Tenh::Typle_t<int,int>>>::V, "error");


    static_assert(Tenh::Hippo::EachTypleHasEqualLength_e::Eval_f<Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::Hippo::EachTypleHasEqualLength_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<>>>::V, "error");
    static_assert(Tenh::Hippo::EachTypleHasEqualLength_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<>,Tenh::Typle_t<>>>::V, "error");
    static_assert(Tenh::Hippo::EachTypleHasEqualLength_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<>,Tenh::Typle_t<>,Tenh::Typle_t<>>>::V, "error");

    static_assert(!Tenh::Hippo::EachTypleHasEqualLength_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<>,Tenh::Typle_t<>>>::V, "error");
    static_assert(!Tenh::Hippo::EachTypleHasEqualLength_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<int>,Tenh::Typle_t<>>>::V, "error");
    static_assert(Tenh::Hippo::EachTypleHasEqualLength_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<int>,Tenh::Typle_t<int>>>::V, "error");

    static_assert(!Tenh::Hippo::EachTypleHasEqualLength_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<int,int>,Tenh::Typle_t<int>,Tenh::Typle_t<>>>::V, "error");
    static_assert(!Tenh::Hippo::EachTypleHasEqualLength_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<int,int>,Tenh::Typle_t<int,int,int>,Tenh::Typle_t<>>>::V, "error");
    static_assert(Tenh::Hippo::EachTypleHasEqualLength_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<int,int>,Tenh::Typle_t<int,int>,Tenh::Typle_t<int,int>>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_Zip (Context const &context)
{
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_f<Tenh::Typle_t<>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_f<Tenh::Typle_t<Tenh::Typle_t<>>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_f<Tenh::Typle_t<Tenh::Typle_t<>,Tenh::Typle_t<>>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_f<Tenh::Typle_t<Tenh::Typle_t<>,Tenh::Typle_t<>,Tenh::Typle_t<>>>::T,
                                        Tenh::Typle_t<>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_f<Tenh::Typle_t<Tenh::Typle_t<int>>>::T,
                                        Tenh::Typle_t<Tenh::Typle_t<int>>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_f<Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<char>>>::T,
                                        Tenh::Typle_t<Tenh::Typle_t<int,char>>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_f<Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<char>,Tenh::Typle_t<bool>>>::T,
                                        Tenh::Typle_t<Tenh::Typle_t<int,char,bool>>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_f<Tenh::Typle_t<Tenh::Typle_t<int,float>>>::T,
                                        Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<float>>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_f<Tenh::Typle_t<Tenh::Typle_t<int,float>,Tenh::Typle_t<char,Uint32>>>::T,
                                        Tenh::Typle_t<Tenh::Typle_t<int,char>,Tenh::Typle_t<float,Uint32>>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_f<Tenh::Typle_t<Tenh::Typle_t<int,float>,Tenh::Typle_t<char,Uint32>,Tenh::Typle_t<bool,double>>>::T,
                                        Tenh::Typle_t<Tenh::Typle_t<int,char,bool>,Tenh::Typle_t<float,Uint32,double>>>::V, "error");


    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_e::Eval_f<Tenh::Typle_t<>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<>>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<>,Tenh::Typle_t<>>>::T,
                                        Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<>,Tenh::Typle_t<>,Tenh::Typle_t<>>>::T,
                                        Tenh::Typle_t<>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<int>>>::T,
                                        Tenh::Typle_t<Tenh::Typle_t<int>>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<char>>>::T,
                                        Tenh::Typle_t<Tenh::Typle_t<int,char>>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<char>,Tenh::Typle_t<bool>>>::T,
                                        Tenh::Typle_t<Tenh::Typle_t<int,char,bool>>>::V, "error");

    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<int,float>>>::T,
                                        Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<float>>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<int,float>,Tenh::Typle_t<char,Uint32>>>::T,
                                        Tenh::Typle_t<Tenh::Typle_t<int,char>,Tenh::Typle_t<float,Uint32>>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Zip_e::Eval_f<Tenh::Typle_t<Tenh::Typle_t<int,float>,Tenh::Typle_t<char,Uint32>,Tenh::Typle_t<bool,double>>>::T,
                                        Tenh::Typle_t<Tenh::Typle_t<int,char,bool>,Tenh::Typle_t<float,Uint32,double>>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_Unzip (Context const &context)
{
    {
        typedef Tenh::Typle_t<> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }
    {
        typedef Tenh::Typle_t<Tenh::Typle_t<int>> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }
    {
        typedef Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<float>> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }
    {
        typedef Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<float>,Tenh::Typle_t<bool>> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }
    {
        typedef Tenh::Typle_t<Tenh::Typle_t<int,double>> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }
    {
        typedef Tenh::Typle_t<Tenh::Typle_t<int,double>,Tenh::Typle_t<float,char>> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }
    {
        typedef Tenh::Typle_t<Tenh::Typle_t<int,double>,Tenh::Typle_t<float,char>,Tenh::Typle_t<bool,Uint32>> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }


    {
        typedef Tenh::Typle_t<> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_e::Eval_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }
    {
        typedef Tenh::Typle_t<Tenh::Typle_t<int>> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_e::Eval_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }
    {
        typedef Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<float>> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_e::Eval_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }
    {
        typedef Tenh::Typle_t<Tenh::Typle_t<int>,Tenh::Typle_t<float>,Tenh::Typle_t<bool>> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_e::Eval_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }
    {
        typedef Tenh::Typle_t<Tenh::Typle_t<int,double>> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_e::Eval_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }
    {
        typedef Tenh::Typle_t<Tenh::Typle_t<int,double>,Tenh::Typle_t<float,char>> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_e::Eval_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }
    {
        typedef Tenh::Typle_t<Tenh::Typle_t<int,double>,Tenh::Typle_t<float,char>,Tenh::Typle_t<bool,Uint32>> Typles;
        static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::Unzip_e::Eval_f<Tenh::Hippo::Zip_f<Typles>::T>::T,Typles>::V, "error");
    }

    // there is currently no runtime test code here, but that could change
}

void test_UniformTypleOfLength (Context const &context)
{
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniformTypleOfLength_f<0,int>::T,Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniformTypleOfLength_f<1,int>::T,Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniformTypleOfLength_f<2,int>::T,Tenh::Typle_t<int,int>>::V, "error");
    static_assert(Tenh::TypesAreEqual_f<Tenh::Hippo::UniformTypleOfLength_f<3,int>::T,Tenh::Typle_t<int,int,int>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void test_TypleIsUniform (Context const &context)
{
    static_assert(Tenh::Hippo::TypleIsUniform_f<Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::Hippo::TypleIsUniform_f<Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::Hippo::TypleIsUniform_f<Tenh::Typle_t<int,int>>::V, "error");
    static_assert(Tenh::Hippo::TypleIsUniform_f<Tenh::Typle_t<int,int,int>>::V, "error");

    static_assert(!Tenh::Hippo::TypleIsUniform_f<Tenh::Typle_t<int,char>>::V, "error");
    static_assert(!Tenh::Hippo::TypleIsUniform_f<Tenh::Typle_t<int,int,char>>::V, "error");
    static_assert(!Tenh::Hippo::TypleIsUniform_f<Tenh::Typle_t<int,bool,char>>::V, "error");


    static_assert(Tenh::Hippo::TypleIsUniform_e::Eval_f<Tenh::Typle_t<>>::V, "error");
    static_assert(Tenh::Hippo::TypleIsUniform_e::Eval_f<Tenh::Typle_t<int>>::V, "error");
    static_assert(Tenh::Hippo::TypleIsUniform_e::Eval_f<Tenh::Typle_t<int,int>>::V, "error");
    static_assert(Tenh::Hippo::TypleIsUniform_e::Eval_f<Tenh::Typle_t<int,int,int>>::V, "error");

    static_assert(!Tenh::Hippo::TypleIsUniform_e::Eval_f<Tenh::Typle_t<int,char>>::V, "error");
    static_assert(!Tenh::Hippo::TypleIsUniform_e::Eval_f<Tenh::Typle_t<int,int,char>>::V, "error");
    static_assert(!Tenh::Hippo::TypleIsUniform_e::Eval_f<Tenh::Typle_t<int,bool,char>>::V, "error");

    // there is currently no runtime test code here, but that could change
}

void AddTests (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory("Typle_t");

    add_particular_tests_for_typle<Tenh::Typle_t<>,0>(dir);
    add_particular_tests_for_typle<Tenh::Typle_t<Sint32>,1>(dir);
    add_particular_tests_for_typle<Tenh::Typle_t<Sint32,Sint8>,2>(dir);
    add_particular_tests_for_typle<Tenh::Typle_t<Sint32,Sint8,double>,3>(dir);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_OnEach, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_NegationOfPredicate, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_And, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_Or, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_EachTypeSatisfies, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_Sum, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_Product, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_Min, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_Max, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_Element, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_Contains, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_IndexOfFirstOccurrence, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_TrailingTyple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_LeadingTyple, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_TypleRange, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_UniqueTypesIn, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_OccurrenceCount, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_ElementsHavingMultiplicity, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_ElementsOfTypleSatisfying, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_ContainsDuplicates, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_IsASubsetOf, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_AreEqualAsSets, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_HasNontrivialIntersectionAsSets, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_IntersectionAsSets, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_SetSubtraction, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_EachTypleHasEqualLength, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_Zip, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_Unzip, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_UniformTypleOfLength, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_TypleIsUniform, RESULT_NO_ERROR);

    // TODO: make and test value and type evaluators for each of these metafunctions
}

} // end of namespace Typle
} // end of namespace Test
