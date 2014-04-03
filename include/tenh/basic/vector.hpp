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

template <typename BasedVectorSpace_, typename Scalar_ = float, typename UseArrayType_ = UseMemberArray_t<ComponentsAreConst::FALSE>>
struct Vector
    :
    public ExpressionOperand_i<Vector<BasedVectorSpace_,Scalar_,UseArrayType_>,1>,
    public ImplementationOf_t<BasedVectorSpace_,Scalar_,UseArrayType_,Vector<BasedVectorSpace_,Scalar_,UseArrayType_>>
{
private:
    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_,UseArrayType_,Vector<BasedVectorSpace_,Scalar_,UseArrayType_>> Parent_Implementation;
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
        static_assert(IsUseMemberArray_f<UseArrayType_>::V, "The fill with constructor on Vector can only be called if the Vector is UseMemberArray.");
    }
    // this is the tuple-based constructor
    template <typename... Types_>
    Vector (Tuple_t<Typle_t<Types_...>> const &x)
        :
        Parent_Implementation(x.as_member_array())
    {
        static_assert(IsUseMemberArray_f<UseArrayType_>::V, "The List constructor on Vector can only be called if the Vector is UseMemberArray.");
    }

    // only use these if UsePreallocatedArray_t<...> is specified

    explicit Vector (Scalar_ *pointer_to_allocation, CheckPointer check_pointer = CheckPointer::TRUE)
        :
        Parent_Implementation(pointer_to_allocation, check_pointer)
    {
        static_assert(IsUsePreallocatedArray_f<UseArrayType_>::V, "The pointer to allocation constructor on Vector can only be called if the Vector is UsePreallocatedArray");
    }
    template <typename T_>
    Vector (FillWith_t<T_> const &fill_with,
            Scalar_ *pointer_to_allocation, CheckPointer check_pointer = CheckPointer::TRUE)
        :
        Parent_Implementation(fill_with, pointer_to_allocation, check_pointer)
    {
        static_assert(IsUsePreallocatedArray_f<UseArrayType_>::V, "The pointer to allocation constructor on Vector can only be called if the Vector is UsePreallocatedArray");
    }
    // this is the tuple-based constructor
    template <typename... Types_>
    Vector (Tuple_t<Typle_t<Types_...>> const &x,
            Scalar_ *pointer_to_allocation, CheckPointer check_pointer = CheckPointer::TRUE)
        :
        Parent_Implementation(x, pointer_to_allocation, check_pointer)
    {
        static_assert(IsUsePreallocatedArray_f<UseArrayType_>::V, "The pointer to allocation constructor on Vector can only be called if the Vector is UsePreallocatedArray");
    }

    // only use this if UseProceduralArray_t<...> is specified or if the vector space is 0-dimensional
    Vector ()
        :
        Parent_Implementation(WithoutInitialization()) // sort of meaningless constructor
    {
        static_assert(IsUseProceduralArray_f<UseArrayType_>::V || DimensionOf_f<BasedVectorSpace_>::V == 0,
                      "To construct a Vector without arguments it must be procedural or dimension zero.");
    }

    template <typename ExpressionTemplate_, typename FreeDimIndexTyple_>
    void operator = (Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_> const &rhs)
    {
        static_assert(IsExpressionTemplate_f<ExpressionTemplate_>::V, "ExpressionTemplate_ must be an ExpressionTemplate_i");
        static_assert(TypesAreEqual_f<typename ExpressionTemplate_::FreeDimIndexTyple,FreeDimIndexTyple_>::V, "free indices must be the same");
        static_assert(Length_f<FreeDimIndexTyple_>::V == 1, "must have exactly 1 free indices");
        AbstractIndex_c<'i'> i;
        (*this)(i) = rhs(i);
    }
    // TODO: a "no alias" version of operator= -- this probably requires adding a no_alias() method to this class.

    static std::string type_as_string (bool verbose)
    {
        return "Vector<" + type_string_of<BasedVectorSpace_>() + ','
                         + type_string_of<Scalar_>() + ','
                         + type_string_of<UseArrayType_>() + '>';
    }
};

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
struct DualOf_f<Vector<BasedVectorSpace_,Scalar_,UseArrayType_>>
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
    typedef typename UniformAbstractIndexTyple_f<1>::T AbstractIndexTyple;
    static AbstractIndexSymbol const SYMBOL = SymbolOf_f<typename Head_f<AbstractIndexTyple>::T>::V;
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
    typedef typename LhsOfContractionAbstractIndexTyple_f<1>::T AbstractIndexTyple;
    static AbstractIndexSymbol const SYMBOL = SymbolOf_f<typename Head_f<AbstractIndexTyple>::T>::V;
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
    typedef typename RhsOfContractionAbstractIndexTyple_f<1>::T AbstractIndexTyple;
    static AbstractIndexSymbol const SYMBOL = SymbolOf_f<typename Head_f<AbstractIndexTyple>::T>::V;
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
XYZ operator % (Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &lhs,
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
                Vector<BasedVectorSpace_,Scalar_,UseArrayType_> const &rhs)
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

#endif // TENH_BASIC_VECTOR_HPP_
