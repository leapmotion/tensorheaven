// ///////////////////////////////////////////////////////////////////////////
// test_abstractindex.cpp by Victor Dods, created 2013/10/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_abstractindex.hpp"

#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/reindex.hpp"

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

void test_BaseAbstractReindex_e (Context const &context)
{
    typedef Tenh::AbstractIndex_c<'i'> i;
    typedef Tenh::AbstractIndex_c<'j'> j;
    typedef Tenh::AbstractIndex_c<'k'> k;
    typedef Tenh::AbstractIndex_c<'P'> P;
    typedef Tenh::Typle_t<i,j,P> DomainIndexTyple;
    typedef Tenh::Typle_t<j,P,k> CodomainIndexTyple;
    typedef Tenh::AbstractIndexMap_e<DomainIndexTyple,CodomainIndexTyple> AbstractIndexMap;
    assert((Tenh::TypesAreEqual_f<AbstractIndexMap::Eval_f<i>::T,j>::V));
    assert((Tenh::TypesAreEqual_f<AbstractIndexMap::Eval_f<j>::T,P>::V));
    assert((Tenh::TypesAreEqual_f<AbstractIndexMap::Eval_f<P>::T,k>::V));
}

void test_Reindex_e (Context const &context)
{
    typedef Tenh::AbstractIndex_c<'i'> i;
    typedef Tenh::AbstractIndex_c<'j'> j;
    typedef Tenh::AbstractIndex_c<'k'> k;
    typedef Tenh::AbstractIndex_c<'P'> P;
    typedef Tenh::Typle_t<i,j,P> DomainIndexTyple;
    typedef Tenh::Typle_t<j,P,k> CodomainIndexTyple;
    typedef Tenh::Reindex_e<DomainIndexTyple,CodomainIndexTyple> Reindex;

    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<i>::T,j>::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<j>::T,P>::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<P>::T,k>::V));

    static Tenh::AbstractIndexSymbol const OFFSET = 'k'; // this is the max of {'j','k','P'}
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<Tenh::AbstractIndex_c<'a'> >::T,
                                  Tenh::AbstractIndex_c<OFFSET+'a'> >::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<Tenh::AbstractIndex_c<'k'> >::T,
                                  Tenh::AbstractIndex_c<OFFSET+'k'> >::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<Tenh::AbstractIndex_c<12345> >::T,
                                  Tenh::AbstractIndex_c<OFFSET+12345> >::V));
}

void test_induced_Typle_Reindex_e (Context const &context)
{
    typedef Tenh::AbstractIndex_c<'i'> i;
    typedef Tenh::AbstractIndex_c<'j'> j;
    typedef Tenh::AbstractIndex_c<'k'> k;
    typedef Tenh::AbstractIndex_c<'P'> P;
    typedef Tenh::Typle_t<i,j,P> DomainIndexTyple;
    typedef Tenh::Typle_t<j,P,k> CodomainIndexTyple;
    typedef Tenh::Reindex_e<DomainIndexTyple,CodomainIndexTyple> Reindex;

    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<i>::T,j>::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<j>::T,P>::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<P>::T,k>::V));

    static Tenh::AbstractIndexSymbol const OFFSET = 'k'; // this is the max of {'j','k','P'}

    typedef Tenh::Typle_t<Tenh::AbstractIndex_c<'i'>,
                          Tenh::AbstractIndex_c<'j'>,
                          Tenh::AbstractIndex_c<'i'>,
                          Tenh::AbstractIndex_c<'k'>,
                          Tenh::AbstractIndex_c<'P'>> Typle;
    typedef Reindex::Eval_f<Typle>::T MappedTyple;
    typedef Tenh::Typle_t<Tenh::AbstractIndex_c<'j'>,
                          Tenh::AbstractIndex_c<'P'>,
                          Tenh::AbstractIndex_c<'j'>,
                          Tenh::AbstractIndex_c<OFFSET+'k'>,
                          Tenh::AbstractIndex_c<'k'>> ExpectedTyple;
    assert((Tenh::TypesAreEqual_f<MappedTyple,ExpectedTyple>::V));
}

void test_reindexed (Context const &context)
{
    typedef Tenh::AbstractIndex_c<'i'> i;
    typedef Tenh::AbstractIndex_c<'j'> j;
    typedef Tenh::AbstractIndex_c<'k'> k;
    typedef Tenh::AbstractIndex_c<'P'> P;
    typedef Tenh::Typle_t<i,j,P> DomainIndexTyple;
    typedef Tenh::Typle_t<j,P,k> CodomainIndexTyple;
    typedef Tenh::Reindex_e<DomainIndexTyple,CodomainIndexTyple> Reindex;

    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<i>::T,j>::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<j>::T,P>::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<P>::T,k>::V));

    static Tenh::AbstractIndexSymbol const OFFSET = 'k'; // this is the max of {'j','k','P'}

    // ensure that the reindexed<...>() function works on AbstractIndex_c types, using a
    // hacky, but effective, way to compare them (via type_as_string).
    assert_eq(Tenh::type_string_of(Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(Tenh::AbstractIndex_c<'i'>())),
              Tenh::type_string_of(Tenh::AbstractIndex_c<'j'>())); // expected value
    assert_eq(Tenh::type_string_of(Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(Tenh::AbstractIndex_c<'j'>())),
              Tenh::type_string_of(Tenh::AbstractIndex_c<'P'>())); // expected value
    assert_eq(Tenh::type_string_of(Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(Tenh::AbstractIndex_c<'k'>())),
              Tenh::type_string_of(Tenh::AbstractIndex_c<OFFSET+'k'>())); // expected value
    assert_eq(Tenh::type_string_of(Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(Tenh::AbstractIndex_c<'P'>())),
              Tenh::type_string_of(Tenh::AbstractIndex_c<'k'>())); // expected value
}

void test_reindexed_AbstractIndexTyple (Context const &context)
{
    typedef Tenh::AbstractIndex_c<'i'> I;
    typedef Tenh::AbstractIndex_c<'j'> J;
    typedef Tenh::AbstractIndex_c<'k'> K;
    typedef Tenh::AbstractIndex_c<'p'> P;
    typedef Tenh::Typle_t<I,J,P> DomainIndexTyple;
    typedef Tenh::Typle_t<J,P,K> CodomainIndexTyple;
    typedef Tenh::Reindex_e<DomainIndexTyple,CodomainIndexTyple> Reindex;

    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<I>::T,J>::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<J>::T,P>::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<P>::T,K>::V));

    // static Tenh::AbstractIndexSymbol const OFFSET = 'k'; // this is the max of {'j','k','p'}

    I i;
    J j;
    K k;
    P p;
    // ensure that the reindexed<...>() function works on AbstractIndex_c types, using a
    // hacky, but effective, way to compare them (via type_as_string).
    // verify that abstract index concatenation and reindexing commute.
    assert_eq(Tenh::type_string_of(Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(i*j)),
              Tenh::type_string_of(Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(i)*
                                   Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(j))); // expected value
    assert_eq(Tenh::type_string_of(Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(j*j*p*i)),
              Tenh::type_string_of(Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(j)*
                                   Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(j)*
                                   Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(p)*
                                   Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(i))); // expected value
    assert_eq(Tenh::type_string_of(Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(k*p*i)),
              Tenh::type_string_of(Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(k)*
                                   Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(p)*
                                   Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(i))); // expected value
}

void AddTests (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory("abstractindex");

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "test_SymbolOf_f<AbstractIndex_c<'i'>>", test_SymbolOf_f<Tenh::AbstractIndex_c<'i'>,'i'>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "test_SymbolOf_f<AbstractIndex_c<12345>>", test_SymbolOf_f<Tenh::AbstractIndex_c<12345>,12345>, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_BaseAbstractReindex_e, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_Reindex_e, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_induced_Typle_Reindex_e, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_reindexed, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_reindexed_AbstractIndexTyple, RESULT_NO_ERROR);
}

} // end of namespace AbstractIndex
} // end of namespace Test
