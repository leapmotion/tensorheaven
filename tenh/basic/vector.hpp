// ///////////////////////////////////////////////////////////////////////////
// tenh/basic/vector.hpp by Victor Dods, created 2013/10/01
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_BASIC_VECTOR_HPP_
#define TENH_BASIC_VECTOR_HPP_

#include "tenh/core.hpp"

#include "tenh/expressiontemplate_reindex.hpp"
#include "tenh/implementation/diagonal2tensor.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/vee.hpp"
#include "tenh/implementation/wedge.hpp"
#include "tenh/reindex.hpp"

namespace Tenh {

template <typename BasedVectorSpace_, typename Scalar_ = float, typename UseArrayType_ = UseMemberArray>
struct Vector
    :
    public ImplementationOf_t<BasedVectorSpace_,Scalar_,UseArrayType_>
{
private:
    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_,UseArrayType_> Parent_Implementation;
public:
    typedef typename Parent_Implementation::ComponentIndex ComponentIndex;
    typedef typename DualOf_f<Vector>::T Dual; // relies on the template specialization below

    explicit Vector (WithoutInitialization const &w) : Parent_Implementation(w) { }

    // only use these if UseMemberArray is specified

    // probably only useful for zero element (because this is basis-dependent)
    explicit Vector (Scalar_ const &fill_with)
        :
        Parent_Implementation(fill_with)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemberArray);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Vector (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x)
        :
        Parent_Implementation(x.as_member_array())
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemberArray);
    }

    // only use these if UsePreallocatedArray is specified

    explicit Vector (Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }
    Vector (Scalar_ const &fill_with,
            Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(fill_with, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Vector (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x,
            Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(x, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }

    // only use this if UseImmutableArray_t<...> is specified or if the vector space is 0-dimensional
    Vector ()
        :
        Parent_Implementation(WithoutInitialization()) // sort of meaningless constructor
    {
        STATIC_ASSERT(IsUseImmutableArray_f<UseArrayType_>::V || DimensionOf_f<BasedVectorSpace_>::V == 0,
                      MUST_BE_USE_IMMUTABLE_ARRAY_OR_BE_ZERO_DIMENSIONAL);
    }

    template <typename Derived_,
              typename FreeFactorTypeList_,
              typename FreeDimIndexTypeList_,
              typename UsedDimIndexTypeList_>
    void operator = (ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &rhs)
    {
        STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 1, LENGTH_MUST_BE_EXACTLY_1);
        typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex;
        typedef AbstractIndex_c<RhsDimIndex::SYMBOL> IndexToRename;
        typedef AbstractIndex_c<'i'> I;
        typedef TypeList_t<IndexToRename> DomainAbstractIndexTypeList;
        typedef TypeList_t<I> CodomainAbstractIndexTypeList;
        I i;
        (*this)(i) = reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(rhs.as_derived());
    }
    // TODO: a "no alias" version of operator= -- this probably requires adding a no_alias() method to this class.

    static std::string type_as_string ()
    {
        return "Vector<" + type_string_of<BasedVectorSpace_>() + ','
                         + type_string_of<Scalar_>() + ','
                         + type_string_of<UseArrayType_>() + '>';
    }
};

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
struct DualOf_f<Vector<BasedVectorSpace_,Scalar_,UseArrayType_> >
{
    typedef Vector<typename DualOf_f<BasedVectorSpace_>::T,Scalar_,typename DualOf_f<UseArrayType_>::T> T;
};

template <typename BasedVectorSpace_, typename Scalar_, typename Lhs_UseArrayType_, typename Rhs_UseArrayType_>
ExpressionTemplate_Addition_t<
    typename Vector<BasedVectorSpace_,Scalar_,Lhs_UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    typename Vector<BasedVectorSpace_,Scalar_,Rhs_UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    '+'>
    operator + (Vector<BasedVectorSpace_,Scalar_,Lhs_UseArrayType_> const &lhs,
                Vector<BasedVectorSpace_,Scalar_,Rhs_UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    return lhs(i) + rhs(i);
}

template <typename BasedVectorSpace_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
ExpressionTemplate_Addition_t<
    typename Vector<BasedVectorSpace_,Scalar_,UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    Derived_,
    '+'>
    operator + (Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &lhs,
                ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &rhs)
{
    STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 1, LENGTH_MUST_BE_EXACTLY_1);
    typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex;
    typedef AbstractIndex_c<RhsDimIndex::SYMBOL> IndexToRename;
    typedef AbstractIndex_c<'i'> I;
    typedef TypeList_t<IndexToRename> DomainAbstractIndexTypeList;
    typedef TypeList_t<I> CodomainAbstractIndexTypeList;
    I i;
    return lhs(i) + reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(rhs.as_derived());
}

template <typename BasedVectorSpace_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
ExpressionTemplate_Addition_t<
    Derived_,
    typename Vector<BasedVectorSpace_,Scalar_,UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    '+'>
    operator + (ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &lhs,
                Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &rhs)
{
    STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 1, LENGTH_MUST_BE_EXACTLY_1);
    typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex;
    typedef AbstractIndex_c<RhsDimIndex::SYMBOL> IndexToRename;
    typedef AbstractIndex_c<'i'> I;
    typedef TypeList_t<IndexToRename> DomainAbstractIndexTypeList;
    typedef TypeList_t<I> CodomainAbstractIndexTypeList;
    I i;
    return reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(lhs.as_derived()) + rhs(i);
}

// the "ExpressionTemplate_i + ExpressionTemplate_i" situation is already handled
// by the existing expression template code.

template <typename BasedVectorSpace_, typename Scalar_, typename Lhs_UseArrayType_, typename Rhs_UseArrayType_>
ExpressionTemplate_Addition_t<
    typename Vector<BasedVectorSpace_,Scalar_,Lhs_UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    typename Vector<BasedVectorSpace_,Scalar_,Rhs_UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    '-'>
    operator - (Vector<BasedVectorSpace_,Scalar_,Lhs_UseArrayType_> const &lhs,
                Vector<BasedVectorSpace_,Scalar_,Rhs_UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    return lhs(i) - rhs(i);
}

template <typename BasedVectorSpace_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
ExpressionTemplate_Addition_t<
    typename Vector<BasedVectorSpace_,Scalar_,UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    Derived_,
    '-'>
    operator - (Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &lhs,
                ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &rhs)
{
    STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 1, LENGTH_MUST_BE_EXACTLY_1);
    typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex;
    typedef AbstractIndex_c<RhsDimIndex::SYMBOL> IndexToRename;
    typedef AbstractIndex_c<'i'> I;
    typedef TypeList_t<IndexToRename> DomainAbstractIndexTypeList;
    typedef TypeList_t<I> CodomainAbstractIndexTypeList;
    I i;
    return lhs(i) - reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(rhs.as_derived());
}

template <typename BasedVectorSpace_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
ExpressionTemplate_Addition_t<
    Derived_,
    typename Vector<BasedVectorSpace_,Scalar_,UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    '-'>
    operator - (ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &lhs,
                Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &rhs)
{
    STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 1, LENGTH_MUST_BE_EXACTLY_1);
    typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex;
    typedef AbstractIndex_c<RhsDimIndex::SYMBOL> IndexToRename;
    typedef AbstractIndex_c<'i'> I;
    typedef TypeList_t<IndexToRename> DomainAbstractIndexTypeList;
    typedef TypeList_t<I> CodomainAbstractIndexTypeList;
    I i;
    return reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(lhs.as_derived()) - rhs(i);
}

// the "ExpressionTemplate_i - ExpressionTemplate_i" situation is already handled
// by the existing expression template code.

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
ExpressionTemplate_ScalarMultiplication_t<
    typename Vector<BasedVectorSpace_,Scalar_,UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    Scalar_,
    '*'>
    operator * (Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &lhs,
                Scalar_ const &rhs)
{
    AbstractIndex_c<'i'> i;
    return lhs(i) * rhs;
}

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
ExpressionTemplate_ScalarMultiplication_t<
    typename Vector<BasedVectorSpace_,Scalar_,UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    Scalar_,
    '*'>
    operator * (Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &lhs,
                int rhs)
{
    AbstractIndex_c<'i'> i;
    return lhs(i) * rhs;
}

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
ExpressionTemplate_ScalarMultiplication_t<
    typename Vector<BasedVectorSpace_,Scalar_,UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    Scalar_,
    '*'>
    operator * (Scalar_ const &lhs,
                Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    return lhs * rhs(i);
}

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
ExpressionTemplate_ScalarMultiplication_t<
    typename Vector<BasedVectorSpace_,Scalar_,UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    Scalar_,
    '*'>
    operator * (int lhs,
                Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    return lhs * rhs(i);
}

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_, typename ScalarOperand_>
ExpressionTemplate_ScalarMultiplication_t<
    typename Vector<BasedVectorSpace_,Scalar_,UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    Scalar_,
    '/'>
    operator / (Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &lhs,
                ScalarOperand_ const &rhs)
{
    AbstractIndex_c<'i'> i;
    return lhs(i) / rhs;
}

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
ExpressionTemplate_ScalarMultiplication_t<
    typename Vector<BasedVectorSpace_,Scalar_,UseArrayType_>::template IndexedExpressionConstType_f<'i'>::T,
    Scalar_,
    '*'>
    operator - (Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &v)
{
    AbstractIndex_c<'i'> i;
    return -v(i);
}

// this only allows natural pairings of vectors with covectors (via the existing expression template code)
template <typename Lhs_BasedVectorSpace_, typename Rhs_BasedVectorSpace_, typename Scalar_, typename Lhs_UseArrayType_, typename Rhs_UseArrayType_>
Scalar_ operator * (Vector<Lhs_BasedVectorSpace_,Scalar_,Lhs_UseArrayType_> const &lhs,
                    Vector<Rhs_BasedVectorSpace_,Scalar_,Rhs_UseArrayType_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    return lhs(i) * rhs(i);
}

// this only allows natural pairings of vectors with covectors (via the existing expression template code)
template <typename BasedVectorSpace_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
Scalar_ operator * (Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &lhs,
                    ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &rhs)
{
    STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 1, LENGTH_MUST_BE_EXACTLY_1);
    typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex;
    typedef AbstractIndex_c<RhsDimIndex::SYMBOL> IndexToRename;
    typedef AbstractIndex_c<'i'> I;
    typedef TypeList_t<IndexToRename> DomainAbstractIndexTypeList;
    typedef TypeList_t<I> CodomainAbstractIndexTypeList;
    I i;
    return lhs(i) * reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(rhs.as_derived());
}

// this only allows natural pairings of vectors with covectors (via the existing expression template code)
template <typename BasedVectorSpace_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
Scalar_ operator * (ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &lhs,
                    Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &rhs)
{
    STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 1, LENGTH_MUST_BE_EXACTLY_1);
    typedef typename Head_f<FreeDimIndexTypeList_>::T RhsDimIndex;
    typedef AbstractIndex_c<RhsDimIndex::SYMBOL> IndexToRename;
    typedef AbstractIndex_c<'i'> I;
    typedef TypeList_t<IndexToRename> DomainAbstractIndexTypeList;
    typedef TypeList_t<I> CodomainAbstractIndexTypeList;
    I i;
    return reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(lhs.as_derived()) * rhs(i);
}

// the "ExpressionTemplate_i * ExpressionTemplate_i" situation is already handled
// by the existing expression template code.

/*

// TODO: figure out the return type (yugh).
// this is the outer product
template <typename Lhs_BasedVectorSpace_,
          typename Rhs_BasedVectorSpace_,
          typename Scalar_,
          typename Lhs_UseArrayType_,
          typename Rhs_UseArrayType_>
XYZ operator % (Vector<Lhs_BasedVectorSpace_,Scalar_,Lhs_UseArrayType_> const &lhs,
                Vector<Rhs_BasedVectorSpace_,Scalar_,Rhs_UseArrayType_> const &rhs)
{
    // TODO: write an expression template for outer product -- essentially taking a list of Vector_i types
    // and bundling each of their indices into a single index.
}

// TODO: figure out the return type (yugh).
// this is the outer product
template <typename BasedVectorSpace_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
XYZ operator % (Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &lhs,
                ExpressionTemplate_OuterProduct_t<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &rhs)
{
    // TODO: return a new outer product expression template
}

// TODO: figure out the return type (yugh).
// this is the outer product
template <typename BasedVectorSpace_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
XYZ operator % (ExpressionTemplate_OuterProduct_t<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &lhs,
                Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &rhs)
{
    // TODO: return a new outer product expression template
}

// TODO: figure out the return type (yugh).
// this is the outer product
template <typename Scalar_,
          typename Lhs_Derived_,
          typename Lhs_FreeFactorTypeList_,
          typename Lhs_FreeDimIndexTypeList_,
          typename Lhs_UsedDimIndexTypeList_,
          typename Rhs_Derived_,
          typename Rhs_FreeFactorTypeList_,
          typename Rhs_FreeDimIndexTypeList_,
          typename Rhs_UsedDimIndexTypeList_>
XYZ operator % (ExpressionTemplate_OuterProduct_t<Lhs_Derived_,Scalar_,Lhs_FreeFactorTypeList_,Lhs_FreeDimIndexTypeList_,Lhs_UsedDimIndexTypeList_> const &lhs,
                ExpressionTemplate_OuterProduct_t<Rhs_Derived_,Scalar_,Rhs_FreeFactorTypeList_,Rhs_FreeDimIndexTypeList_,Rhs_UsedDimIndexTypeList_> const &rhs)
{
    // TODO: return a new outer product expression template
}
*/
} // end of namespace Tenh

#endif // TENH_BASIC_VECTOR_HPP_
