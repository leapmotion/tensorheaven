// ///////////////////////////////////////////////////////////////////////////
// tenh/basic/vector.hpp by Victor Dods, created 2013/10/01
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_BASIC_VECTOR_HPP_
#define TENH_BASIC_VECTOR_HPP_

#include "tenh/core.hpp"

#include "tenh/basic/expressionoperand.hpp"
#include "tenh/basic/overloads.hpp"
#include "tenh/basic/reindexable.hpp"
#include "tenh/expressiontemplate_reindex.hpp"
#include "tenh/implementation/diagonal2tensor.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/vee.hpp"
#include "tenh/implementation/wedge.hpp"
#include "tenh/reindex.hpp"

namespace Tenh {

template <typename BasedVectorSpace_, typename Scalar_ = float, typename UseArrayType_ = UseMemberArray_t<COMPONENTS_ARE_NONCONST> >
struct Vector
    :
    public ExpressionOperand_i<Vector<BasedVectorSpace_,Scalar_,UseArrayType_>,1>,
    public ImplementationOf_t<BasedVectorSpace_,Scalar_,UseArrayType_,Vector<BasedVectorSpace_,Scalar_,UseArrayType_> >
{
private:
    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_,UseArrayType_,Vector<BasedVectorSpace_,Scalar_,UseArrayType_> > Parent_Implementation;
public:
    typedef Scalar_ Scalar;
    typedef typename Parent_Implementation::ComponentIndex ComponentIndex;
    using Parent_Implementation::DIM;
    typedef typename DualOf_f<Vector>::T Dual; // relies on the template specialization below

    explicit Vector (WithoutInitialization const &w) : Parent_Implementation(w) { }

    // TODO: copy constructor and operator= (for appropriate UseArrayType_)

    // only use these if UseMemberArray_t<...> is specified

    // probably only useful for zero element (because this is basis-dependent)
    template <typename T_>
    explicit Vector (FillWith_t<T_> const &fill_with)
        :
        Parent_Implementation(fill_with)
    {
        STATIC_ASSERT(IsUseMemberArray_f<UseArrayType_>::V, MUST_BE_USE_MEMBER_ARRAY);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Vector (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x)
        :
        Parent_Implementation(x.as_member_array())
    {
        STATIC_ASSERT(IsUseMemberArray_f<UseArrayType_>::V, MUST_BE_USE_MEMBER_ARRAY);
    }

    // only use these if UsePreallocatedArray_t<...> is specified

    explicit Vector (Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(IsUsePreallocatedArray_f<UseArrayType_>::V, MUST_BE_USE_PREALLOCATED_ARRAY);
    }
    template <typename T_>
    Vector (FillWith_t<T_> const &fill_with,
            Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(fill_with, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(IsUsePreallocatedArray_f<UseArrayType_>::V, MUST_BE_USE_PREALLOCATED_ARRAY);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Vector (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x,
            Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(x, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(IsUsePreallocatedArray_f<UseArrayType_>::V, MUST_BE_USE_PREALLOCATED_ARRAY);
    }

    // only use this if UseProceduralArray_t<...> is specified or if the vector space is 0-dimensional
    Vector ()
        :
        Parent_Implementation(WithoutInitialization()) // sort of meaningless constructor
    {
        STATIC_ASSERT(IsUseProceduralArray_f<UseArrayType_>::V || DimensionOf_f<BasedVectorSpace_>::V == 0,
                      MUST_BE_USE_PROCEDURAL_ARRAY_OR_BE_ZERO_DIMENSIONAL);
    }

    template <typename ExpressionTemplate_, typename FreeDimIndexTypeList_>
    void operator = (Reindexable_t<ExpressionTemplate_,FreeDimIndexTypeList_> const &rhs)
    {
        STATIC_ASSERT(IsExpressionTemplate_f<ExpressionTemplate_>::V, MUST_BE_EXPRESSION_TEMPLATE);
        STATIC_ASSERT_TYPES_ARE_EQUAL(typename ExpressionTemplate_::FreeDimIndexTypeList,FreeDimIndexTypeList_);
        STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 1, LENGTH_MUST_BE_EXACTLY_1);
        AbstractIndex_c<'i'> i;
        (*this)(i) = rhs(i);
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
private:
    DualOf_f();
};

// template specialization for how to uniformly index a Vector (e.g. for addition/subtraction/etc)
template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
struct UniformlyIndexedExpressionTemplate_f<Vector<BasedVectorSpace_,Scalar_,UseArrayType_>,1>
{
private:
    typedef Vector<BasedVectorSpace_,Scalar_,UseArrayType_> Vec;
    typedef typename UniformAbstractIndexTypeList_f<1>::T AbstractIndexTypeList;
    static AbstractIndexSymbol const SYMBOL = SymbolOf_f<typename Head_f<AbstractIndexTypeList>::T>::V;
    UniformlyIndexedExpressionTemplate_f();
public:
    typedef typename Vec::template IndexedExpressionConstType_f<SYMBOL>::T T;
};

// template specialization for how to lhs-index a Vector for contraction
template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
struct LhsIndexedContractionExpressionTemplate_f<Vector<BasedVectorSpace_,Scalar_,UseArrayType_>,1>
{
private:
    typedef Vector<BasedVectorSpace_,Scalar_,UseArrayType_> Vec;
    typedef typename LhsOfContractionAbstractIndexTypeList_f<1>::T AbstractIndexTypeList;
    static AbstractIndexSymbol const SYMBOL = SymbolOf_f<typename Head_f<AbstractIndexTypeList>::T>::V;
    LhsIndexedContractionExpressionTemplate_f();
public:
    typedef typename Vec::template IndexedExpressionConstType_f<SYMBOL>::T T;
};

// template specialization for how to rhs-index a Vector for contraction
template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
struct RhsIndexedContractionExpressionTemplate_f<Vector<BasedVectorSpace_,Scalar_,UseArrayType_>,1>
{
private:
    typedef Vector<BasedVectorSpace_,Scalar_,UseArrayType_> Vec;
    typedef typename RhsOfContractionAbstractIndexTypeList_f<1>::T AbstractIndexTypeList;
    static AbstractIndexSymbol const SYMBOL = SymbolOf_f<typename Head_f<AbstractIndexTypeList>::T>::V;
    RhsIndexedContractionExpressionTemplate_f();
public:
    typedef typename Vec::template IndexedExpressionConstType_f<SYMBOL>::T T;
};

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
