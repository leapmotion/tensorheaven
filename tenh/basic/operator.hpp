// ///////////////////////////////////////////////////////////////////////////
// tenh/basic/operator.hpp by Victor Dods, created 2013/10/04
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_BASIC_OPERATOR_HPP_
#define TENH_BASIC_OPERATOR_HPP_

#include "tenh/core.hpp"

#include "tenh/basic/expressionoperand.hpp"
#include "tenh/basic/reindexable.hpp"
#include "tenh/basic/vector.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/expressiontemplate_reindex.hpp"
#include "tenh/implementation/tensor.hpp"

namespace Tenh {

// Domain_ and Codomain_ must be based vector spaces.
template <typename Domain_, typename Codomain_, typename Scalar_ = float, typename UseArrayType_ = UseMemberArray>
struct Operator
    :
    public ExpressionOperand_i<Operator<Domain_,Codomain_,Scalar_,UseArrayType_>,2>,
    public ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Codomain_,
                                                                 TypeList_t<typename DualOf_f<Domain_>::T> > >,
                              Scalar_,
                              UseArrayType_>
{
private:
    typedef ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Codomain_,
                                                                  TypeList_t<typename DualOf_f<Domain_>::T> > >,
                               Scalar_,
                               UseArrayType_> Parent_Implementation;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<Codomain_,
                                               TypeList_t<typename DualOf_f<Domain_>::T> > > TensorProduct;
public:
    typedef typename Parent_Implementation::ComponentIndex ComponentIndex;
    typedef typename Parent_Implementation::MultiIndex MultiIndex;
    typedef typename DualOf_f<Operator>::T Dual; // relies on the template specialization below

    explicit Operator (WithoutInitialization const &w) : Parent_Implementation(w) { }
    Operator (Operator const &op) : Parent_Implementation(op) { }

    // TODO: copy constructor and operator= (for appropriate UseArrayType_)

    // only use these if UseMemberArray is specified

    // probably only useful for zero element (because this is basis-dependent)
    explicit Operator (Scalar_ const &fill_with)
        :
        Parent_Implementation(fill_with)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemberArray);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Operator (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x)
        :
        Parent_Implementation(x.as_member_array())
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemberArray);
    }

    // only use these if UsePreallocatedArray is specified

    explicit Operator (Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }
    Operator (Scalar_ const &fill_with,
              Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(fill_with, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Operator (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x,
              Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(x, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }

    // only use this if UseImmutableArray_t<...> is specified or if the vector space is 0-dimensional
    Operator ()
        :
        Parent_Implementation(WithoutInitialization()) // sort of meaningless constructor
    {
        STATIC_ASSERT(IsUseImmutableArray_f<UseArrayType_>::V || DimensionOf_f<TensorProduct>::V == 0,
                      MUST_BE_USE_IMMUTABLE_ARRAY_OR_BE_ZERO_DIMENSIONAL);
    }

    template <typename ExpressionTemplate_, typename FreeDimIndexTypeList_>
    void operator = (Reindexable_t<ExpressionTemplate_,FreeDimIndexTypeList_> const &rhs)
    {
        STATIC_ASSERT(IsExpressionTemplate_f<ExpressionTemplate_>::V, MUST_BE_EXPRESSION_TEMPLATE);
        STATIC_ASSERT_TYPES_ARE_EQUAL(typename ExpressionTemplate_::FreeDimIndexTypeList,FreeDimIndexTypeList_);
        STATIC_ASSERT(Length_f<FreeDimIndexTypeList_>::V == 2, LENGTH_MUST_BE_EXACTLY_2);
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        (*this)(i|j) = rhs(i|j);
    }
    // TODO: a "no alias" version of operator= -- this probably requires adding a no_alias() method to this class.

    static std::string type_as_string ()
    {
        return "Operator<" + type_string_of<Domain_>() + ','
                           + type_string_of<Codomain_>() + ','
                           + type_string_of<Scalar_>() + ','
                           + type_string_of<UseArrayType_>() + '>';
    }
};

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
struct DualOf_f<Operator<Domain_,Codomain_,Scalar_,UseArrayType_> >
{
    // the "dual" of a linear operator is its natural adjoint; if
    // A : V --> W, then A* : W* --> V*, where * denotes the dual functor.
    typedef Operator<typename DualOf_f<Codomain_>::T,typename DualOf_f<Domain_>::T,Scalar_,typename DualOf_f<UseArrayType_>::T> T;
};
/*

// TODO: figure out the return type (yugh).
// this is the outer product
template <typename Lhs_BasedVectorSpace_,
          typename Rhs_BasedVectorSpace_,
          typename Scalar_,
          typename Lhs_UseArrayType_,
          typename Rhs_UseArrayType_>
XYZ operator % (Operator<Lhs_BasedVectorSpace_,Scalar_,Lhs_UseArrayType_> const &lhs,
                Operator<Rhs_BasedVectorSpace_,Scalar_,Rhs_UseArrayType_> const &rhs)
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
XYZ operator % (Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &lhs,
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
                Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &rhs)
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

#endif // TENH_BASIC_OPERATOR_HPP_
