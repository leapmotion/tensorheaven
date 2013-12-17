// ///////////////////////////////////////////////////////////////////////////
// test_expressiontemplate_reindex.cpp by Victor Dods, created 2013/10/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_expressiontemplate_reindex.hpp"

#include "tenh/expressiontemplate_reindex.hpp"
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
namespace ExpressionTemplate_Reindex {

void vector_based_IndexedObject_t (Context const &context)
{
    static Uint32 const DIM = 3;
    typedef int DummyId;
    typedef float Scalar;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,DummyId>,Tenh::Basis_c<DummyId> > BasedVectorSpace;
    typedef Tenh::ImplementationOf_t<BasedVectorSpace,Scalar> Vector;

    // create an ExpressionTemplate_IndexedObject_t
    typedef Tenh::AbstractIndex_c<'j'> J;
    typedef Tenh::AbstractIndex_c<'k'> K;

    // set up the abstract index mapping
    typedef Tenh::TypeTuple_f<J>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<K>::T CodomainIndexTypeList;
    typedef Tenh::Reindex_e<DomainIndexTypeList,CodomainIndexTypeList> Reindex;
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<J>::T,K>::V));

    // ensure that the indices are mapped properly, including ones that aren't used in the expression
    {
        typedef Vector::IndexedExpressionConstType_f<'j'>::T IndexedExpression;
        typedef Vector::IndexedExpressionConstType_f<'k'>::T ExpectedIndexedExpression;
        typedef Reindex::Eval_f<IndexedExpression>::T ActualIndexedExpression;
        assert((Tenh::TypesAreEqual_f<ActualIndexedExpression,ExpectedIndexedExpression>::V));
    }
    {
        typedef Vector::IndexedExpressionConstType_f<'k'>::T IndexedExpression;
        typedef Vector::IndexedExpressionConstType_f<'k'+'k'>::T ExpectedIndexedExpression;
        typedef Reindex::Eval_f<IndexedExpression>::T ActualIndexedExpression;
        assert((Tenh::TypesAreEqual_f<ActualIndexedExpression,ExpectedIndexedExpression>::V));
    }
    {
        typedef Vector::IndexedExpressionConstType_f<'r'>::T IndexedExpression;
        typedef Vector::IndexedExpressionConstType_f<'k'+'r'>::T ExpectedIndexedExpression;
        typedef Reindex::Eval_f<IndexedExpression>::T ActualIndexedExpression;
        assert((Tenh::TypesAreEqual_f<ActualIndexedExpression,ExpectedIndexedExpression>::V));
    }

    // test reindexing an actual instance of an indexed expression -- compare the types only,
    // not the contents of the expression template AST (this is just too much trouble).
    {
        J j;
        K k;
        typedef Tenh::AbstractIndex_c<'p'> P;
        P p;
        Vector v(Tenh::fill_with(0));
        // hacky, but effective, way of ensuring the reindexed expression template is what
        // it's supposed to be (via type_as_string).  what's being tested is essentially
        // the commutativity of the operations of creating an ExpressionTemplate_IndexedObject_t
        // and reindexing.
        assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(v(j))).type_as_string(),
                  v(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(j)).type_as_string()); // expected value
        assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(v(k))).type_as_string(),
                  v(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(k)).type_as_string()); // expected value
        assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(v(p))).type_as_string(),
                  v(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(p)).type_as_string()); // expected value
    }
}

void tensor_based_IndexedObject_t (Context const &context)
{
    static Uint32 const DIM = 3;
    typedef int DummyId;
    typedef float Scalar;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,DummyId>,Tenh::Basis_c<DummyId> > BasedVectorSpace;
    typedef Tenh::TypeTuple_f<BasedVectorSpace,Tenh::DualOf_f<BasedVectorSpace>::T>::T FactorTypeList;
    typedef Tenh::TensorProductOfBasedVectorSpaces_c<FactorTypeList> TensorProduct;
    typedef Tenh::ImplementationOf_t<TensorProduct,Scalar> Tensor;

    typedef Tenh::AbstractIndex_c<'j'> J;
    typedef Tenh::AbstractIndex_c<'k'> K;

    // set up the abstract index mapping
    typedef Tenh::TypeTuple_f<J>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<K>::T CodomainIndexTypeList;
    typedef Tenh::Reindex_e<DomainIndexTypeList,CodomainIndexTypeList> Reindex;
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<J>::T,K>::V));

    // ensure that the indices are mapped properly, including ones that aren't used in the expression
    {
        typedef Tensor::IndexedExpressionConstType_f<Tenh::TypeTuple_f<J,K>::T>::T IndexedExpression;
        typedef Tensor::IndexedExpressionConstType_f<Tenh::TypeTuple_f<K,Tenh::AbstractIndex_c<'k'+'k'> >::T>::T ExpectedIndexedExpression;
        typedef Reindex::Eval_f<IndexedExpression>::T ActualIndexedExpression;
        assert((Tenh::TypesAreEqual_f<ActualIndexedExpression,ExpectedIndexedExpression>::V));
    }
    {
        typedef Tensor::IndexedExpressionConstType_f<Tenh::TypeTuple_f<J,J>::T>::T IndexedExpression;
        typedef Tensor::IndexedExpressionConstType_f<Tenh::TypeTuple_f<K,K>::T>::T ExpectedIndexedExpression;
        typedef Reindex::Eval_f<IndexedExpression>::T ActualIndexedExpression;
        assert((Tenh::TypesAreEqual_f<ActualIndexedExpression,ExpectedIndexedExpression>::V));
    }
    {
        typedef Tensor::IndexedExpressionConstType_f<Tenh::TypeTuple_f<Tenh::AbstractIndex_c<'a'>,Tenh::AbstractIndex_c<'b'> >::T>::T IndexedExpression;
        typedef Tensor::IndexedExpressionConstType_f<Tenh::TypeTuple_f<Tenh::AbstractIndex_c<'k'+'a'>,Tenh::AbstractIndex_c<'k'+'b'> >::T>::T ExpectedIndexedExpression;
        typedef Reindex::Eval_f<IndexedExpression>::T ActualIndexedExpression;
        assert((Tenh::TypesAreEqual_f<ActualIndexedExpression,ExpectedIndexedExpression>::V));
    }

    // test reindexing an actual instance of an indexed expression -- compare the types only,
    // not the contents of the expression template AST (this is just too much trouble).
    {
        J j;
        K k;
        typedef Tenh::AbstractIndex_c<'p'> P;
        P p;
        Tensor t(Tenh::fill_with(0));
        // hacky, but effective, way of ensuring the reindexed expression template is what it's supposed to be.
        assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(t(j*k))).type_as_string(),
                  t(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(j*k)).type_as_string()); // expected value
        assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(t(k*p))).type_as_string(),
                  t(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(k*p)).type_as_string()); // expected value

        // single-indexed expressions
        assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(t(j))).type_as_string(),
                  t(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(j)).type_as_string()); // expected value
        assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(t(k))).type_as_string(),
                  t(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(k)).type_as_string()); // expected value
        assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(t(p))).type_as_string(),
                  t(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(p)).type_as_string()); // expected value
    }
}

void Addition_t (Context const &context)
{
    static Uint32 const DIM = 3;
    typedef int DummyId;
    typedef float Scalar;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,DummyId>,Tenh::Basis_c<DummyId> > BasedVectorSpace;
    typedef Tenh::ImplementationOf_t<BasedVectorSpace,Scalar> Vector;

    typedef Tenh::AbstractIndex_c<'i'> I;
    typedef Tenh::AbstractIndex_c<'j'> J;
    typedef Tenh::AbstractIndex_c<'k'> K;

    // set up the abstract index mapping
    typedef Tenh::TypeTuple_f<I>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<J>::T CodomainIndexTypeList;
    typedef Tenh::Reindex_e<DomainIndexTypeList,CodomainIndexTypeList> Reindex;
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<I>::T,J>::V));

    Vector x(Tenh::fill_with(3));
    Vector y(Tenh::fill_with(4));

    I i;
    J j;
    K k;
    // hacky, but effective, way of ensuring the reindexed expression template is what it's supposed to be.
    // verify that the operations of index-expression-addition and reindexing commute.
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(i) + y(i))).type_as_string(),
              (Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(i)) +
               Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(y(i))).type_as_string()); // expected value
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(j) + y(j))).type_as_string(),
              (Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(j)) +
               Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(y(j))).type_as_string()); // expected value
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(k) + y(k))).type_as_string(),
              (Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(k)) +
               Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(y(k))).type_as_string()); // expected value
}

void ScalarMultiplication_t (Context const &context)
{
    static Uint32 const DIM = 3;
    typedef int DummyId;
    typedef float Scalar;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,DummyId>,Tenh::Basis_c<DummyId> > BasedVectorSpace;
    typedef Tenh::ImplementationOf_t<BasedVectorSpace,Scalar> Vector;

    typedef Tenh::AbstractIndex_c<'i'> I;
    typedef Tenh::AbstractIndex_c<'j'> J;
    typedef Tenh::AbstractIndex_c<'k'> K;

    // set up the abstract index mapping
    typedef Tenh::TypeTuple_f<I>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<J>::T CodomainIndexTypeList;
    typedef Tenh::Reindex_e<DomainIndexTypeList,CodomainIndexTypeList> Reindex;
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<I>::T,J>::V));

    Vector x(Tenh::fill_with(3));
    Vector y(Tenh::fill_with(4));

    I i;
    J j;
    K k;
    // hacky, but effective, way of ensuring the reindexed expression template is what it's supposed to be.
    // verify that the operations of index-expression-scalar-multiplication and reindexing commute.
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(i) * Scalar(3))).type_as_string(),
              (Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(i)) * Scalar(3)).type_as_string()); // expected value
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(j) * Scalar(3))).type_as_string(),
              (Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(j)) * Scalar(3)).type_as_string()); // expected value
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(k) * Scalar(3))).type_as_string(),
              (Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(k)) * Scalar(3)).type_as_string()); // expected value
}

void Multiplication_t (Context const &context)
{
    static Uint32 const DIM = 3;
    typedef int DummyId;
    typedef float Scalar;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,DummyId>,Tenh::Basis_c<DummyId> > BasedVectorSpace;
    typedef Tenh::ImplementationOf_t<BasedVectorSpace,Scalar> Vector;

    typedef Tenh::AbstractIndex_c<'i'> I;
    typedef Tenh::AbstractIndex_c<'j'> J;
    typedef Tenh::AbstractIndex_c<'k'> K;

    // set up the abstract index mapping
    typedef Tenh::TypeTuple_f<I>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<J>::T CodomainIndexTypeList;
    typedef Tenh::Reindex_e<DomainIndexTypeList,CodomainIndexTypeList> Reindex;
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<I>::T,J>::V));

    Vector x(Tenh::fill_with(3));
    Vector y(Tenh::fill_with(4));

    I i;
    J j;
    K k;
    // hacky, but effective, way of ensuring the reindexed expression template is what it's supposed to be.
    // verify that the operations of index-expression-multiplication and reindexing commute.
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(i) * y(i))).type_as_string(),
              (Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(i)) *
               Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(y(i))).type_as_string()); // expected value
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(j) * y(j))).type_as_string(),
              (Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(j)) *
               Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(y(j))).type_as_string()); // expected value
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(k) * y(k))).type_as_string(),
              (Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(k)) *
               Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(y(k))).type_as_string()); // expected value

    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(i) * y(j))).type_as_string(),
              (Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(i)) *
               Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(y(j))).type_as_string()); // expected value
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(j) * y(k))).type_as_string(),
              (Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(j)) *
               Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(y(k))).type_as_string()); // expected value
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(k) * y(i))).type_as_string(),
              (Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(x(k)) *
               Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(y(i))).type_as_string()); // expected value
}

void IndexBundle_t (Context const &context)
{
    static Uint32 const DIM = 3;
    typedef int DummyId;
    typedef float Scalar;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,DummyId>,Tenh::Basis_c<DummyId> > BasedVectorSpace;
    typedef Tenh::TypeTuple_f<BasedVectorSpace,BasedVectorSpace,BasedVectorSpace>::T FactorTypeList;
    typedef Tenh::TensorProductOfBasedVectorSpaces_c<FactorTypeList> TensorProduct;
    typedef Tenh::ImplementationOf_t<TensorProduct,Scalar> Tensor;

    typedef Tenh::AbstractIndex_c<'i'> I;
    typedef Tenh::AbstractIndex_c<'j'> J;
    typedef Tenh::AbstractIndex_c<'k'> K;
    typedef Tenh::AbstractIndex_c<'l'> L;

    // set up the abstract index mapping
    typedef Tenh::TypeTuple_f<I>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<J>::T CodomainIndexTypeList;
    typedef Tenh::Reindex_e<DomainIndexTypeList,CodomainIndexTypeList> Reindex;
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<I>::T,J>::V));

    Tensor t(Tenh::fill_with(3));

    I i;
    J j;
    K k;
    L l;
    // hacky, but effective, way of ensuring the reindexed expression template is what it's supposed to be.
    // verify that the operations of index-expression-bundle and reindexing commute.
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(t(i*j*k).bundle(i*j*k,TensorProduct(),l)).type_as_string()),
              (t(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(i*j*k))
               .bundle(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(i*j*k),
                       TensorProduct(),
                       Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(l))).type_as_string()); // expected value
}

void IndexSplit_t (Context const &context)
{
    static Uint32 const DIM = 3;
    typedef int DummyId;
    typedef float Scalar;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,DummyId>,Tenh::Basis_c<DummyId> > BasedVectorSpace;
    typedef Tenh::TypeTuple_f<BasedVectorSpace,BasedVectorSpace,BasedVectorSpace>::T FactorTypeList;
    typedef Tenh::TensorProductOfBasedVectorSpaces_c<FactorTypeList> TensorProduct;
    typedef Tenh::ImplementationOf_t<TensorProduct,Scalar> Tensor;

    typedef Tenh::AbstractIndex_c<'i'> I;
    typedef Tenh::AbstractIndex_c<'j'> J;
    typedef Tenh::AbstractIndex_c<'k'> K;
    typedef Tenh::AbstractIndex_c<'l'> L;

    // set up the abstract index mapping
    typedef Tenh::TypeTuple_f<I>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<J>::T CodomainIndexTypeList;
    typedef Tenh::Reindex_e<DomainIndexTypeList,CodomainIndexTypeList> Reindex;
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<I>::T,J>::V));

    Tensor t(Tenh::fill_with(3));

    I i;
    J j;
    K k;
    L l;
    // hacky, but effective, way of ensuring the reindexed expression template is what it's supposed to be.
    // verify that the operations of index-expression-split and reindexing commute.
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(t(i).split(i,j*k*l)).type_as_string()),
              (t(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(i))
               .split(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(i),
                      Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(j*k*l))).type_as_string()); // expected value
}

void IndexSplitToIndex_t (Context const &context)
{
    static Uint32 const DIM = 3;
    typedef int DummyId;
    typedef float Scalar;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,DummyId>,Tenh::Basis_c<DummyId> > BasedVectorSpace;
    typedef Tenh::TypeTuple_f<BasedVectorSpace,BasedVectorSpace,BasedVectorSpace>::T FactorTypeList;
    typedef Tenh::TensorProductOfBasedVectorSpaces_c<FactorTypeList> TensorProduct;
    typedef Tenh::ImplementationOf_t<TensorProduct,Scalar> Tensor;

    typedef Tenh::AbstractIndex_c<'i'> I;
    typedef Tenh::AbstractIndex_c<'j'> J;
    typedef Tenh::AbstractIndex_c<'k'> K;
    typedef Tenh::AbstractIndex_c<'l'> L;

    // set up the abstract index mapping
    typedef Tenh::TypeTuple_f<I>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<J>::T CodomainIndexTypeList;
    typedef Tenh::Reindex_e<DomainIndexTypeList,CodomainIndexTypeList> Reindex;
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<I>::T,J>::V));

    Tensor t(Tenh::fill_with(3));

    I i;
    J j;
    K k;
    L l;
    // hacky, but effective, way of ensuring the reindexed expression template is what it's supposed to be.
    // verify that the operations of index-expression-split-to-index and reindexing commute.
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(t(i).split(i,j)).type_as_string()),
              (t(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(i))
               .split(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(i),
                      Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(j))).type_as_string()); // expected value
}

void Eval_t (Context const &context)
{
    static Uint32 const DIM = 3;
    typedef int DummyId;
    typedef float Scalar;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,DummyId>,Tenh::Basis_c<DummyId> > BasedVectorSpace;
    typedef Tenh::TypeTuple_f<BasedVectorSpace,BasedVectorSpace,BasedVectorSpace>::T FactorTypeList;
    typedef Tenh::TensorProductOfBasedVectorSpaces_c<FactorTypeList> TensorProduct;
    typedef Tenh::ImplementationOf_t<TensorProduct,Scalar> Tensor;

    typedef Tenh::AbstractIndex_c<'i'> I;
    typedef Tenh::AbstractIndex_c<'j'> J;
    typedef Tenh::AbstractIndex_c<'k'> K;
    typedef Tenh::AbstractIndex_c<'l'> L;

    // set up the abstract index mapping
    typedef Tenh::TypeTuple_f<I>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<J>::T CodomainIndexTypeList;
    typedef Tenh::Reindex_e<DomainIndexTypeList,CodomainIndexTypeList> Reindex;
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<I>::T,J>::V));

    Tensor t(Tenh::fill_with(3));

    I i;
    J j;
    K k;
    L l;
    // hacky, but effective, way of ensuring the reindexed expression template is what it's supposed to be.
    // verify that the operations of index-expression-eval and reindexing commute.
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(t(i).eval()).type_as_string()),
              (t(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(i)).eval().type_as_string())); // expected value
}

void fancy_expression (Context const &context)
{
    static Uint32 const DIM = 3;
    typedef int DummyId;
    typedef float Scalar;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,DummyId>,Tenh::Basis_c<DummyId> > BasedVectorSpace;
    typedef Tenh::TypeTuple_f<BasedVectorSpace,BasedVectorSpace,BasedVectorSpace>::T FactorTypeList;
    typedef Tenh::TensorProductOfBasedVectorSpaces_c<FactorTypeList> TensorProduct;
    typedef Tenh::ImplementationOf_t<BasedVectorSpace,Scalar> Vector;
    typedef Tenh::ImplementationOf_t<TensorProduct,Scalar> Tensor;

    typedef Tenh::AbstractIndex_c<'i'> I;
    typedef Tenh::AbstractIndex_c<'j'> J;
    typedef Tenh::AbstractIndex_c<'k'> K;
    typedef Tenh::AbstractIndex_c<'l'> L;

    // set up the abstract index mapping
    typedef Tenh::TypeTuple_f<I>::T DomainIndexTypeList;
    typedef Tenh::TypeTuple_f<J>::T CodomainIndexTypeList;
    typedef Tenh::Reindex_e<DomainIndexTypeList,CodomainIndexTypeList> Reindex;
    assert((Tenh::TypesAreEqual_f<Reindex::Eval_f<I>::T,J>::V));

    Tensor t(Tenh::fill_with(3));
    Tenh::DualOf_f<Tensor>::T a(Tenh::fill_with(5));
    Vector x(Tenh::fill_with(1));
    Tenh::DualOf_f<Vector>::T c(Tenh::fill_with(8));

    I i;
    J j;
    K k;
    L l;
    // hacky, but effective, way of ensuring the reindexed expression template is what it's supposed to be.
    // verify that the operations of composing this fancy expression and reindexing commute.
    assert_eq((Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(Scalar(3) * t(i*j*k)*a(j*k*l) + x(i)*c(l))).type_as_string(),
              (Scalar(3) * t(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(i*j*k))
                         * a(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(j*k*l))
                         + x(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(i))
                         * c(Tenh::reindexed<DomainIndexTypeList,CodomainIndexTypeList>(l))).type_as_string()); // expected value
}

void AddTests (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory("expressiontemplate_reindex");

    LVD_ADD_TEST_CASE_FUNCTION(dir, vector_based_IndexedObject_t, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, tensor_based_IndexedObject_t, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, Addition_t, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, ScalarMultiplication_t, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, Multiplication_t, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, IndexBundle_t, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, IndexSplit_t, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, IndexSplitToIndex_t, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, Eval_t, RESULT_NO_ERROR);
    LVD_ADD_TEST_CASE_FUNCTION(dir, fancy_expression, RESULT_NO_ERROR);
}

} // end of namespace ExpressionTemplate_Reindex
} // end of namespace Test
