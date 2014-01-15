// ///////////////////////////////////////////////////////////////////////////
// tenh/basic/operator.hpp by Victor Dods, created 2013/10/04
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_BASIC_OPERATOR_HPP_
#define TENH_BASIC_OPERATOR_HPP_

#include "tenh/core.hpp"

#include "tenh/basic/expressionoperand.hpp"
#include "tenh/basic/overloads.hpp"
#include "tenh/basic/reindexable.hpp"
#include "tenh/basic/vector.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/expressiontemplate_reindex.hpp"
#include "tenh/implementation/tensor.hpp"

namespace Tenh {

// Domain_ and Codomain_ must be based vector spaces.
template <typename Domain_, typename Codomain_, typename Scalar_ = float, typename UseArrayType_ = UseMemberArray_t<COMPONENTS_ARE_NONCONST> >
struct Operator
    :
    public ExpressionOperand_i<Operator<Domain_,Codomain_,Scalar_,UseArrayType_>,2>,
    public ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Codomain_,
                                                                 TypeList_t<typename DualOf_f<Domain_>::T> > >,
                              Scalar_,
                              UseArrayType_,
                              Operator<Domain_,Codomain_,Scalar_,UseArrayType_> >
{
private:
    typedef ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Codomain_,
                                                                  TypeList_t<typename DualOf_f<Domain_>::T> > >,
                               Scalar_,
                               UseArrayType_,
                               Operator<Domain_,Codomain_,Scalar_,UseArrayType_> > Parent_Implementation;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<Codomain_,
                                               TypeList_t<typename DualOf_f<Domain_>::T> > > TensorProduct;
public:
    typedef Scalar_ Scalar;
    typedef typename Parent_Implementation::ComponentIndex ComponentIndex;
    typedef typename Parent_Implementation::MultiIndex MultiIndex;
    using Parent_Implementation::DIM;
    typedef typename DualOf_f<Operator>::T Dual; // relies on the template specialization below

    explicit Operator (WithoutInitialization const &w) : Parent_Implementation(w) { }
    Operator (Operator const &op) : Parent_Implementation(op) { }

    // TODO: copy constructor and operator= (for appropriate UseArrayType_)

    // only use these if UseMemberArray_t<...> is specified

    // probably only useful for zero element (because this is basis-dependent)
    template <typename T_>
    explicit Operator (FillWith_t<T_> const &fill_with)
        :
        Parent_Implementation(fill_with)
    {
        static_assert(IsUseMemberArray_f<UseArrayType_>::V, "Fill with constructor on Operator requires UseMemberArray.");
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Operator (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x)
        :
        Parent_Implementation(x.as_member_array())
    {
        static_assert(IsUseMemberArray_f<UseArrayType_>::V, "List constructor on Operator requires UseMemberArray.");
    }

    // only use these if UsePreallocatedArray_t<...> is specified

    explicit Operator (Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(pointer_to_allocation, check_pointer)
    {
        static_assert(IsUsePreallocatedArray_f<UseArrayType_>::V, "Pointer to data constructor on Operator requires UsePreallocatedArray.");
    }
    template <typename T_>
    Operator (FillWith_t<T_> const &fill_with,
              Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(fill_with, pointer_to_allocation, check_pointer)
    {
        static_assert(IsUsePreallocatedArray_f<UseArrayType_>::V, "Pointer to data constructor on Operator requires UsePreallocatedArray.");
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Operator (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x,
              Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(x, pointer_to_allocation, check_pointer)
    {
        static_assert(IsUsePreallocatedArray_f<UseArrayType_>::V, "Pointer to data constructor on Operator requires UsePreallocatedArray.");
    }

    // only use this if UseProceduralArray_t<...> is specified or if the vector space is 0-dimensional
    Operator ()
        :
        Parent_Implementation(WithoutInitialization()) // sort of meaningless constructor
    {
        static_assert(IsUseProceduralArray_f<UseArrayType_>::V || DimensionOf_f<TensorProduct>::V == 0,
                      "Constructing an Operator without arguments requires the dimension be 0, or the data to be procedurally generated.");
    }

    template <typename ExpressionTemplate_, typename FreeDimIndexTypeList_>
    void operator = (Reindexable_t<ExpressionTemplate_,FreeDimIndexTypeList_> const &rhs)
    {
        static_assert(IsExpressionTemplate_f<ExpressionTemplate_>::V, "The Reindexable on the right hand side of Operator's operator= must be an ExpressionTemplate");
        static_assert(TypesAreEqual_f<typename ExpressionTemplate_::FreeDimIndexTypeList,FreeDimIndexTypeList_>::V, "The FreeDimIndexTypeLists on either side of operator= must match.");
        static_assert(Length_f<FreeDimIndexTypeList_>::V == 2, "An expression template must have exactly two free indices to assign it to an Operator.");
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        (*this)(i*j) = rhs(i*j);
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
private:
    DualOf_f();
};

// template specialization for how to uniformly index an Operator (e.g. for addition/subtraction/etc)
template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
struct UniformlyIndexedExpressionTemplate_f<Operator<Domain_,Codomain_,Scalar_,UseArrayType_>,2>
{
private:
    typedef Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Op;
    typedef typename UniformAbstractIndexTypeList_f<2>::T AbstractIndexTypeList;
    UniformlyIndexedExpressionTemplate_f();
public:
    typedef typename Op::template IndexedExpressionConstType_f<AbstractIndexTypeList>::T T;
};

// template specialization for how to lhs-index an Operator for contraction
template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
struct LhsIndexedContractionExpressionTemplate_f<Operator<Domain_,Codomain_,Scalar_,UseArrayType_>,2>
{
private:
    typedef Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Op;
    typedef typename LhsOfContractionAbstractIndexTypeList_f<2>::T AbstractIndexTypeList;
    LhsIndexedContractionExpressionTemplate_f();
public:
    typedef typename Op::template IndexedExpressionConstType_f<AbstractIndexTypeList>::T T;
};

// template specialization for how to rhs-index an Operator for contraction
template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
struct RhsIndexedContractionExpressionTemplate_f<Operator<Domain_,Codomain_,Scalar_,UseArrayType_>,2>
{
private:
    typedef Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Op;
    typedef typename RhsOfContractionAbstractIndexTypeList_f<2>::T AbstractIndexTypeList;
    RhsIndexedContractionExpressionTemplate_f();
public:
    typedef typename Op::template IndexedExpressionConstType_f<AbstractIndexTypeList>::T T;
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
