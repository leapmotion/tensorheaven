// ///////////////////////////////////////////////////////////////////////////
// test_expressiontemplate_indexrenamer.cpp by Victor Dods, created 2013/10/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_expressiontemplate_indexrenamer.hpp"

#include "tenh/expressiontemplate_indexrenamer.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/vector.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace ExpressionTemplate_IndexRenamer {

void test_vector_based_IndexedObject_t (Context const &context)
{
    static Uint32 const DIM = 3;
    typedef int DummyId;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,DummyId>,Tenh::Basis_c<DummyId> > BasedVectorSpace;
    typedef Tenh::ImplementationOf_t<BasedVectorSpace,float> Vector;

    // create an ExpressionTemplate_IndexedObject_t
    typedef Tenh::AbstractIndex_c<'j'> j;
    typedef Tenh::AbstractIndex_c<'k'> k;

    // set up the abstract index mapping
    typedef Tenh::TypeTuple_f<j>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<k>::T CodomainIndexTypeList;
    typedef Tenh::IndexRenamer_e<DomainIndexTypeList,CodomainIndexTypeList> IndexRenamer;
    assert((Tenh::TypesAreEqual_f<IndexRenamer::Eval_f<j>::T,k>::V));

    // ensure that the indices are mapped properly, including ones that aren't used in the expression
    {
        typedef Vector::IndexedExpressionConstType_f<'j'>::T IndexedExpression;
        typedef Vector::IndexedExpressionConstType_f<'k'>::T ExpectedIndexedExpression;
        typedef IndexRenamer::Eval_f<IndexedExpression>::T ActualIndexedExpression;
        assert((Tenh::TypesAreEqual_f<ActualIndexedExpression,ExpectedIndexedExpression>::V));
    }
    {
        typedef Vector::IndexedExpressionConstType_f<'k'>::T IndexedExpression;
        typedef Vector::IndexedExpressionConstType_f<'k'+'k'>::T ExpectedIndexedExpression;
        typedef IndexRenamer::Eval_f<IndexedExpression>::T ActualIndexedExpression;
        assert((Tenh::TypesAreEqual_f<ActualIndexedExpression,ExpectedIndexedExpression>::V));
    }
    {
        typedef Vector::IndexedExpressionConstType_f<'r'>::T IndexedExpression;
        typedef Vector::IndexedExpressionConstType_f<'k'+'r'>::T ExpectedIndexedExpression;
        typedef IndexRenamer::Eval_f<IndexedExpression>::T ActualIndexedExpression;
        assert((Tenh::TypesAreEqual_f<ActualIndexedExpression,ExpectedIndexedExpression>::V));
    }
}

void test_tensor_based_IndexedObject_t (Context const &context)
{
    static Uint32 const DIM = 3;
    typedef int DummyId;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,DummyId>,Tenh::Basis_c<DummyId> > BasedVectorSpace;
    typedef Tenh::TypeTuple_f<BasedVectorSpace,Tenh::DualOf_f<BasedVectorSpace>::T>::T FactorTypeList;
    typedef Tenh::TensorProductOfBasedVectorSpaces_c<FactorTypeList> TensorProduct;
    typedef Tenh::ImplementationOf_t<TensorProduct,float> Tensor;

    // create an ExpressionTemplate_IndexedObject_t
    typedef Tenh::AbstractIndex_c<'j'> j;
    typedef Tenh::AbstractIndex_c<'k'> k;

    // set up the abstract index mapping
    typedef Tenh::TypeTuple_f<j>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<k>::T CodomainIndexTypeList;
    typedef Tenh::IndexRenamer_e<DomainIndexTypeList,CodomainIndexTypeList> IndexRenamer;
    assert((Tenh::TypesAreEqual_f<IndexRenamer::Eval_f<j>::T,k>::V));

    // ensure that the indices are mapped properly, including ones that aren't used in the expression
    {
        typedef Tensor::IndexedExpressionConstType_f<Tenh::TypeTuple_f<j,k>::T>::T IndexedExpression;
        typedef Tensor::IndexedExpressionConstType_f<Tenh::TypeTuple_f<k,Tenh::AbstractIndex_c<'k'+'k'> >::T>::T ExpectedIndexedExpression;
        typedef IndexRenamer::Eval_f<IndexedExpression>::T ActualIndexedExpression;
        assert((Tenh::TypesAreEqual_f<ActualIndexedExpression,ExpectedIndexedExpression>::V));
    }
    {
        typedef Tensor::IndexedExpressionConstType_f<Tenh::TypeTuple_f<j,j>::T>::T IndexedExpression;
        typedef Tensor::IndexedExpressionConstType_f<Tenh::TypeTuple_f<k,k>::T>::T ExpectedIndexedExpression;
        typedef IndexRenamer::Eval_f<IndexedExpression>::T ActualIndexedExpression;
        assert((Tenh::TypesAreEqual_f<ActualIndexedExpression,ExpectedIndexedExpression>::V));
    }
    {
        typedef Tensor::IndexedExpressionConstType_f<Tenh::TypeTuple_f<Tenh::AbstractIndex_c<'a'>,Tenh::AbstractIndex_c<'b'> >::T>::T IndexedExpression;
        typedef Tensor::IndexedExpressionConstType_f<Tenh::TypeTuple_f<Tenh::AbstractIndex_c<'k'+'a'>,Tenh::AbstractIndex_c<'k'+'b'> >::T>::T ExpectedIndexedExpression;
        typedef IndexRenamer::Eval_f<IndexedExpression>::T ActualIndexedExpression;
        assert((Tenh::TypesAreEqual_f<ActualIndexedExpression,ExpectedIndexedExpression>::V));
    }
}

void AddTests (Directory *parent)
{
    Directory *dir = new Directory("expressiontemplate_indexrenamer", parent);

    LVD_ADD_TEST_CASE_FUNCTION(dir, test_vector_based_IndexedObject_t, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, test_tensor_based_IndexedObject_t, RESULT_NO_ERROR);
}

} // end of namespace ExpressionTemplate_IndexRenamer
} // end of namespace Test
