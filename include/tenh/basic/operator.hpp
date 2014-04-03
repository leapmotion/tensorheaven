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
template <typename Domain_, typename Codomain_, typename Scalar_ = float, typename UseArrayType_ = UseMemberArray_t<ComponentsAreConst::FALSE>>
struct Operator
    :
    public ExpressionOperand_i<Operator<Domain_,Codomain_,Scalar_,UseArrayType_>,2>,
    public ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<Typle_t<Codomain_,typename DualOf_f<Domain_>::T>>,
                              Scalar_,
                              UseArrayType_,
                              Operator<Domain_,Codomain_,Scalar_,UseArrayType_>>
{
private:
    typedef ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<Typle_t<Codomain_,typename DualOf_f<Domain_>::T>>,
                               Scalar_,
                               UseArrayType_,
                               Operator<Domain_,Codomain_,Scalar_,UseArrayType_>> Parent_Implementation;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<Codomain_,typename DualOf_f<Domain_>::T>> TensorProduct;
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
    template <typename... Types_>
    Operator (Tuple_t<Typle_t<Types_...>> const &x)
        :
        Parent_Implementation(x.as_member_array())
    {
        static_assert(IsUseMemberArray_f<UseArrayType_>::V, "List constructor on Operator requires UseMemberArray.");
    }

    // only use these if UsePreallocatedArray_t<...> is specified

    explicit Operator (Scalar_ *pointer_to_allocation, CheckPointer check_pointer = CheckPointer::TRUE)
        :
        Parent_Implementation(pointer_to_allocation, check_pointer)
    {
        static_assert(IsUsePreallocatedArray_f<UseArrayType_>::V, "Pointer to data constructor on Operator requires UsePreallocatedArray.");
    }
    template <typename T_>
    Operator (FillWith_t<T_> const &fill_with,
              Scalar_ *pointer_to_allocation, CheckPointer check_pointer = CheckPointer::TRUE)
        :
        Parent_Implementation(fill_with, pointer_to_allocation, check_pointer)
    {
        static_assert(IsUsePreallocatedArray_f<UseArrayType_>::V, "Pointer to data constructor on Operator requires UsePreallocatedArray.");
    }
    // this is the tuple-based constructor
    template <typename... Types_>
    Operator (Tuple_t<Typle_t<Types_...>> const &x,
              Scalar_ *pointer_to_allocation, CheckPointer check_pointer = CheckPointer::TRUE)
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

    template <typename ExpressionTemplate_, typename FreeDimIndexTyple_>
    void operator = (Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_> const &rhs)
    {
        static_assert(IsExpressionTemplate_f<ExpressionTemplate_>::V, "ExpressionTemplate_ must be an ExpressionTemplate_i");
        static_assert(TypesAreEqual_f<typename ExpressionTemplate_::FreeDimIndexTyple,FreeDimIndexTyple_>::V, "free indices must be the same");
        static_assert(Length_f<FreeDimIndexTyple_>::V == 2, "must have exactly 2 free indices");
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        (*this)(i*j) = rhs(i*j);
    }
    // TODO: a "no alias" version of operator= -- this probably requires adding a no_alias() method to this class.

    static std::string type_as_string (bool verbose)
    {
        return "Operator<" + type_string_of<Domain_>() + ','
                           + type_string_of<Codomain_>() + ','
                           + type_string_of<Scalar_>() + ','
                           + type_string_of<UseArrayType_>() + '>';
    }
};

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
struct DualOf_f<Operator<Domain_,Codomain_,Scalar_,UseArrayType_>>
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
    typedef typename UniformAbstractIndexTyple_f<2>::T AbstractIndexTyple;
    UniformlyIndexedExpressionTemplate_f();
public:
    typedef typename Op::template IndexedExpressionConstType_f<AbstractIndexTyple>::T T;
};

// template specialization for how to lhs-index an Operator for contraction
template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
struct LhsIndexedContractionExpressionTemplate_f<Operator<Domain_,Codomain_,Scalar_,UseArrayType_>,2>
{
private:
    typedef Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Op;
    typedef typename LhsOfContractionAbstractIndexTyple_f<2>::T AbstractIndexTyple;
    LhsIndexedContractionExpressionTemplate_f();
public:
    typedef typename Op::template IndexedExpressionConstType_f<AbstractIndexTyple>::T T;
};

// template specialization for how to rhs-index an Operator for contraction
template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
struct RhsIndexedContractionExpressionTemplate_f<Operator<Domain_,Codomain_,Scalar_,UseArrayType_>,2>
{
private:
    typedef Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Op;
    typedef typename RhsOfContractionAbstractIndexTyple_f<2>::T AbstractIndexTyple;
    RhsIndexedContractionExpressionTemplate_f();
public:
    typedef typename Op::template IndexedExpressionConstType_f<AbstractIndexTyple>::T T;
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
    // TODO: write an expression template for outer product -- essentially taking a tuple of Vector_i types
    // and bundling each of their indices into a single index.
}

// TODO: figure out the return type (yugh).
// this is the outer product
template <typename BasedVectorSpace_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTyple_,
          typename FreeDimIndexTyple_,
          typename UsedDimIndexTyple_>
XYZ operator % (Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &lhs,
                ExpressionTemplate_OuterProduct_t<Derived_,Scalar_,FreeFactorTyple_,FreeDimIndexTyple_,UsedDimIndexTyple_> const &rhs)
{
    // TODO: return a new outer product expression template
}

// TODO: figure out the return type (yugh).
// this is the outer product
template <typename BasedVectorSpace_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename FreeFactorTyple_,
          typename FreeDimIndexTyple_,
          typename UsedDimIndexTyple_>
XYZ operator % (ExpressionTemplate_OuterProduct_t<Derived_,Scalar_,FreeFactorTyple_,FreeDimIndexTyple_,UsedDimIndexTyple_> const &lhs,
                Operator<Domain_,Codomain_,Scalar_,UseArrayType_> const &rhs)
{
    // TODO: return a new outer product expression template
}

// TODO: figure out the return type (yugh).
// this is the outer product
template <typename Scalar_,
          typename Lhs_Derived_,
          typename Lhs_FreeFactorTyple_,
          typename Lhs_FreeDimIndexTyple_,
          typename Lhs_UsedDimIndexTyple_,
          typename Rhs_Derived_,
          typename Rhs_FreeFactorTyple_,
          typename Rhs_FreeDimIndexTyple_,
          typename Rhs_UsedDimIndexTyple_>
XYZ operator % (ExpressionTemplate_OuterProduct_t<Lhs_Derived_,Scalar_,Lhs_FreeFactorTyple_,Lhs_FreeDimIndexTyple_,Lhs_UsedDimIndexTyple_> const &lhs,
                ExpressionTemplate_OuterProduct_t<Rhs_Derived_,Scalar_,Rhs_FreeFactorTyple_,Rhs_FreeDimIndexTyple_,Rhs_UsedDimIndexTyple_> const &rhs)
{
    // TODO: return a new outer product expression template
}
*/
} // end of namespace Tenh

#endif // TENH_BASIC_OPERATOR_HPP_
