// ///////////////////////////////////////////////////////////////////////////
// test_dimindex.cpp by Victor Dods, created 2013/10/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_dimindex.hpp"

#include "tenh/dimindex.hpp"
#include "tenh/reindex.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace DimIndex {

void test_Reindex_e (Context const &context)
{
    typedef Tenh::AbstractIndex_c<'i'> i;
    typedef Tenh::AbstractIndex_c<'j'> j;
    typedef Tenh::AbstractIndex_c<'k'> k;
    typedef Tenh::AbstractIndex_c<'P'> P;
    typedef Tenh::Typle_t<i,j,P> DomainIndexTyple;
    typedef Tenh::Typle_t<j,P,k> CodomainIndexTyple;
    typedef Tenh::Reindex_e<DomainIndexTyple,CodomainIndexTyple> Reindex;

    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<Tenh::DimIndex_t<'i',42> >::T,Tenh::DimIndex_t<'j',42> >::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<Tenh::DimIndex_t<'j',8> >::T,Tenh::DimIndex_t<'P',8> >::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<Tenh::DimIndex_t<'P',3> >::T,Tenh::DimIndex_t<'k',3> >::V));

    static Tenh::AbstractIndexSymbol const OFFSET = 'k'; // this is the max of {'j','k','P'}
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<Tenh::DimIndex_t<'a',123> >::T,
                                  Tenh::DimIndex_t<OFFSET+'a',123> >::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<Tenh::DimIndex_t<'k',666> >::T,
                                  Tenh::DimIndex_t<OFFSET+'k',666> >::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<Tenh::DimIndex_t<12345,67890> >::T,
                                  Tenh::DimIndex_t<OFFSET+12345,67890> >::V));
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

    typedef Tenh::Typle_t<Tenh::DimIndex_t<'i',10>,
                          Tenh::DimIndex_t<'j',20>,
                          Tenh::DimIndex_t<'i',30>,
                          Tenh::DimIndex_t<'k',40>,
                          Tenh::DimIndex_t<'P',50>> Typle;
    typedef Reindex::Eval_f<Typle>::T MappedTyple;
    typedef Tenh::Typle_t<Tenh::DimIndex_t<'j',10>,
                          Tenh::DimIndex_t<'P',20>,
                          Tenh::DimIndex_t<'j',30>,
                          Tenh::DimIndex_t<OFFSET+'k',40>,
                          Tenh::DimIndex_t<'k',50>> ExpectedTyple;
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

    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<Tenh::DimIndex_t<'i',42> >::T,Tenh::DimIndex_t<'j',42> >::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<Tenh::DimIndex_t<'j',8> >::T,Tenh::DimIndex_t<'P',8> >::V));
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<Tenh::DimIndex_t<'P',3> >::T,Tenh::DimIndex_t<'k',3> >::V));

    static Tenh::AbstractIndexSymbol const OFFSET = 'k'; // this is the max of {'j','k','P'}

    // ensure that the reindexed<...>() function works on DimIndex_t types, using a
    // hacky, but effective, way to compare them (via type_as_string).
    assert_eq((Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(Tenh::DimIndex_t<'i',10>())).type_as_string(),
              (Tenh::DimIndex_t<'j',10>().type_as_string())); // expected value
    assert_eq((Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(Tenh::DimIndex_t<'j',20>())).type_as_string(),
              (Tenh::DimIndex_t<'P',20>().type_as_string())); // expected value
    assert_eq((Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(Tenh::DimIndex_t<'k',30>())).type_as_string(),
              (Tenh::DimIndex_t<OFFSET+'k',30>().type_as_string())); // expected value
    assert_eq((Tenh::reindexed<DomainIndexTyple,CodomainIndexTyple>(Tenh::DimIndex_t<'P',40>())).type_as_string(),
              (Tenh::DimIndex_t<'k',40>().type_as_string())); // expected value
}

void AddTests (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory("dimindex");

    LVD_ADD_TEST_CASE_FUNCTION(dir, test_Reindex_e, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_induced_Typle_Reindex_e, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_reindexed, RESULT_NO_ERROR);
}

} // end of namespace DimIndex
} // end of namespace Test
