// ///////////////////////////////////////////////////////////////////////////
// test_abstractindex.cpp by Victor Dods, created 2013/10/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_abstractindex.hpp"

#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/indexrenamer.hpp"
#include "tenh/meta/typetuple.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace AbstractIndex {

template <typename AbstractIndex_, Tenh::AbstractIndexSymbol EXPECTED_SYMBOL_>
void test_SymbolOf_f (Context const &context)
{
    Tenh::AbstractIndexSymbol actual_symbol = Tenh::SymbolOf_f<AbstractIndex_>::V;
    assert_eq(actual_symbol, EXPECTED_SYMBOL_);
}

void test_BaseAbstractIndexRenamer_e (Context const &context)
{
    typedef Tenh::AbstractIndex_c<'i'> i;
    typedef Tenh::AbstractIndex_c<'j'> j;
    typedef Tenh::AbstractIndex_c<'k'> k;
    typedef Tenh::AbstractIndex_c<'P'> P;
    typedef Tenh::TypeTuple_f<i,j,P>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<j,P,k>::T CodomainIndexTypeList;
    typedef Tenh::BaseAbstractIndexRenamer_e<DomainIndexTypeList,CodomainIndexTypeList> BaseAbstractIndexRenamer;
    assert((Tenh::TypesAreEqual_f<BaseAbstractIndexRenamer::Eval_f<i>::T,j>::V));
    assert((Tenh::TypesAreEqual_f<BaseAbstractIndexRenamer::Eval_f<j>::T,P>::V));
    assert((Tenh::TypesAreEqual_f<BaseAbstractIndexRenamer::Eval_f<P>::T,k>::V));
}

void test_IndexRenamer_e (Context const &context)
{
    typedef Tenh::AbstractIndex_c<'i'> i;
    typedef Tenh::AbstractIndex_c<'j'> j;
    typedef Tenh::AbstractIndex_c<'k'> k;
    typedef Tenh::AbstractIndex_c<'P'> P;
    typedef Tenh::TypeTuple_f<i,j,P>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<j,P,k>::T CodomainIndexTypeList;
    typedef Tenh::IndexRenamer_e<DomainIndexTypeList,CodomainIndexTypeList> IndexRenamer;

    assert((Tenh::TypesAreEqual_f<IndexRenamer::Eval_f<i>::T,j>::V));
    assert((Tenh::TypesAreEqual_f<IndexRenamer::Eval_f<j>::T,P>::V));
    assert((Tenh::TypesAreEqual_f<IndexRenamer::Eval_f<P>::T,k>::V));

    static Tenh::AbstractIndexSymbol const OFFSET = 'k'; // this is the max of {'j','k','P'}
    assert((Tenh::TypesAreEqual_f<IndexRenamer::Eval_f<Tenh::AbstractIndex_c<'a'> >::T,
                                  Tenh::AbstractIndex_c<OFFSET+'a'> >::V));
    assert((Tenh::TypesAreEqual_f<IndexRenamer::Eval_f<Tenh::AbstractIndex_c<'k'> >::T,
                                  Tenh::AbstractIndex_c<OFFSET+'k'> >::V));
    assert((Tenh::TypesAreEqual_f<IndexRenamer::Eval_f<Tenh::AbstractIndex_c<12345> >::T,
                                  Tenh::AbstractIndex_c<OFFSET+12345> >::V));
}

void test_induced_TypeList_IndexRenamer_e (Context const &context)
{
    typedef Tenh::AbstractIndex_c<'i'> i;
    typedef Tenh::AbstractIndex_c<'j'> j;
    typedef Tenh::AbstractIndex_c<'k'> k;
    typedef Tenh::AbstractIndex_c<'P'> P;
    typedef Tenh::TypeTuple_f<i,j,P>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<j,P,k>::T CodomainIndexTypeList;
    typedef Tenh::IndexRenamer_e<DomainIndexTypeList,CodomainIndexTypeList> IndexRenamer;

    assert((Tenh::TypesAreEqual_f<IndexRenamer::Eval_f<i>::T,j>::V));
    assert((Tenh::TypesAreEqual_f<IndexRenamer::Eval_f<j>::T,P>::V));
    assert((Tenh::TypesAreEqual_f<IndexRenamer::Eval_f<P>::T,k>::V));

    static Tenh::AbstractIndexSymbol const OFFSET = 'k'; // this is the max of {'j','k','P'}

    typedef Tenh::TypeTuple_f<Tenh::AbstractIndex_c<'i'>,
                              Tenh::AbstractIndex_c<'j'>,
                              Tenh::AbstractIndex_c<'i'>,
                              Tenh::AbstractIndex_c<'k'>,
                              Tenh::AbstractIndex_c<'P'> >::T TypeList;
    typedef IndexRenamer::Eval_f<TypeList>::T MappedTypeList;
    typedef Tenh::TypeTuple_f<Tenh::AbstractIndex_c<'j'>,
                              Tenh::AbstractIndex_c<'P'>,
                              Tenh::AbstractIndex_c<'j'>,
                              Tenh::AbstractIndex_c<OFFSET+'k'>,
                              Tenh::AbstractIndex_c<'k'> >::T ExpectedTypeList;
    assert((Tenh::TypesAreEqual_f<MappedTypeList,ExpectedTypeList>::V));
}

void AddTests (Directory *parent)
{
    Directory *abstractindex_dir = new Directory("abstractindex", parent);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(abstractindex_dir, "test_SymbolOf_f<AbstractIndex_c<'i'>>", test_SymbolOf_f<Tenh::AbstractIndex_c<'i'>,'i'>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(abstractindex_dir, "test_SymbolOf_f<AbstractIndex_c<12345>>", test_SymbolOf_f<Tenh::AbstractIndex_c<12345>,12345>, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(abstractindex_dir, test_BaseAbstractIndexRenamer_e, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(abstractindex_dir, test_IndexRenamer_e, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(abstractindex_dir, test_induced_TypeList_IndexRenamer_e, RESULT_NO_ERROR);
}

} // end of namespace AbstractIndex
} // end of namespace Test
