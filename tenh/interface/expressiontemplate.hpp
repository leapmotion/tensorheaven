// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/expressiontemplate.hpp by Victor Dods, created 2013/04/05
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_EXPRESSIONTEMPLATE_HPP_
#define TENH_INTERFACE_EXPRESSIONTEMPLATE_HPP_

#include "tenh/core.hpp"

#include "tenh/dimindex.hpp"
#include "tenh/multiindex.hpp"
#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/meta/typelist.hpp"

namespace Tenh {

// ////////////////////////////////////////////////////////////////////////////
// compile-time interface for expression templates
// ////////////////////////////////////////////////////////////////////////////

template <typename Operand, typename BundleIndexTypeList, typename ResultingIndexType>
struct ExpressionTemplate_IndexBundle_t;

template <typename Operand, typename SourceIndexType, typename SplitIndexTypeList>
struct ExpressionTemplate_IndexSplit_t;

template <typename Operand, typename FreeDimIndexTypeList_>
struct ExpressionTemplate_Eval_t;

// this is essentially a compile-time interface, requiring:
// - a Derived type (should be the type of the thing that ultimately inherits this)
// - a Scalar type (should be the scalar type of the expression template's tensor operand)
// - a FreeDimIndexTypeList type (the free indices of this expression template)
// - a UsedDimIndexTypeList type (the indices that have been used further down the AST)
// requires also particular methods:
//   operator Scalar () const // conversion to scalar -- always declared, but should
//                            // only compile if the conversion is well-defined (e.g. no free indices)
//   Scalar operator [] (MultiIndex const &) const // accessor using MultiIndex_t<FreeDimIndexTypeList>
//   template <typename OtherTensor> bool uses_tensor (OtherTensor const &) const // used in checking for aliasing
template <typename Derived_, 
          typename Scalar_,
          typename FactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
struct ExpressionTemplate_i // _i is for "compile-time interface"
{
    enum
    {
        //DERIVED_MUST_NOT_BE_NULL_TYPE = Lvd::Meta::Assert<!Lvd::Meta::TypesAreEqual<Derived_,NullType>::v>::v
        STATIC_ASSERT_IN_ENUM((!Lvd::Meta::TypesAreEqual<Derived_,NullType>::v), DERIVED_MUST_NOT_BE_NULL_TYPE),
        STATIC_ASSERT_IN_ENUM__UNIQUE(EachTypeIsADimIndex_t<FreeDimIndexTypeList_>::V, MUST_BE_TYPELIST_OF_DIM_INDEX_TYPES, FREEDIMINDEXTYPELIST),
        STATIC_ASSERT_IN_ENUM__UNIQUE(EachTypeIsADimIndex_t<UsedDimIndexTypeList_>::V, MUST_BE_TYPELIST_OF_DIM_INDEX_TYPES, USEDDIMINDEXTYPELIST)
    };

    typedef Derived_ Derived;
    // these typedefs make the Derived-specified typedefs available at the baseclass level,
    typedef Scalar_ Scalar;
    typedef FactorTypeList_ FactorTypeList;
    typedef FreeDimIndexTypeList_ FreeDimIndexTypeList;
    typedef UsedDimIndexTypeList_ UsedDimIndexTypeList;
    typedef MultiIndex_t<FreeDimIndexTypeList> MultiIndex;
    static bool const IS_EXPRESSION_TEMPLATE_I = true; // TODO: make IsAnExpressionTemplate_i<...>

    // TODO: some consistency checks on the various types

    // compile-time interface methods
    operator Scalar () const { return as_derived().operator Scalar(); }
    Scalar operator [] (MultiIndex const &m) const { return as_derived().operator[](m); }
    template <typename OtherTensor> bool uses_tensor (OtherTensor const &t) const { return as_derived().template uses_tensor<OtherTensor>(t); }

    // for accessing this as the Derived type
    Derived const &as_derived () const { return *static_cast<Derived const *>(this); }
    Derived &as_derived () { return *static_cast<Derived *>(this); }

    // convenience methods for squared_norm and norm
    // requires InnerProduct_t to be implemented on all free-indexed types
    // NOTE: will not currently work for complex types
    // NOTE: you must include tenh/expressiontemplate_eval.hpp for the definition of this method
    typename AssociatedFloatingPointType_t<Scalar>::T squared_norm () const;
    // requires InnerProduct_t<Derived> to be implemented
    // NOTE: will not currently work for complex types
    // NOTE: you must include tenh/expressiontemplate_eval.hpp for the definition of this method
    typename AssociatedFloatingPointType_t<Scalar>::T norm () const; // definition is in expressiontemplate_eval.hpp

    // method for "bundling" separate abstract indices into a single abstract index
    // of a more specific type (e.g. a 2-tensor, a fully symmetric 3-tensor, etc)
    // (m(j|i)*a(j|k)*m(k|l)).bundle(i|l,Q) -- bundle i,l into Q
    template <typename AbstractIndexHeadType, typename AbstractIndexBodyTypeList, typename ResultingAbstractIndexType>
    ExpressionTemplate_IndexBundle_t<Derived,TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList>,ResultingAbstractIndexType> bundle (
        TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> const &,
        ResultingAbstractIndexType const &) const
    {
        // make sure that the index type list actually contains AbstractIndex_c types
        STATIC_ASSERT((EachTypeIsAnAbstractIndex_c<TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> >::V), EACH_TYPE_IS_ABSTRACT_INDEX);
        return ExpressionTemplate_IndexBundle_t<Derived,TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList>,ResultingAbstractIndexType>(as_derived());
    }
    // method for "splitting" a tensor index into a separate indices.
    // a(P|Q).split(P,i|j).split(Q,k|l) -- split the tensor indices P and Q into the pairs i,j and k,l
    // so that the expression now has the four free indices i,j,k,l.
    template <typename SourceAbstractIndexType, typename AbstractIndexHeadType, typename AbstractIndexBodyTypeList>
    ExpressionTemplate_IndexSplit_t<Derived,SourceAbstractIndexType,TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> > split (
        SourceAbstractIndexType const &,
        TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> const &) const
    {
        // make sure that the index type list actually contains AbstractIndex_c types
        STATIC_ASSERT((EachTypeIsAnAbstractIndex_c<TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> >::V), EACH_TYPE_IS_ABSTRACT_INDEX);
        return ExpressionTemplate_IndexSplit_t<Derived,SourceAbstractIndexType,TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> >(as_derived());
    }
    // method for doing an intermediate evaluation of an expression template to avoid aliasing
    ExpressionTemplate_Eval_t<Derived,FreeDimIndexTypeList> eval () const
    {
        return ExpressionTemplate_Eval_t<Derived,FreeDimIndexTypeList>(as_derived());
    }
};

// it's just more convenient to write specializations for particular numbers of free indices

// specialization for 1 index
template <typename Derived, typename Scalar, typename FactorTypeList, typename FreeDimIndex1, typename UsedDimIndexTypeList>
std::ostream &operator << (std::ostream &out, ExpressionTemplate_i<Derived,Scalar,FactorTypeList,TypeList_t<FreeDimIndex1>,UsedDimIndexTypeList> const &e)
{
    // determine the max size that a component takes up
    Uint32 max_component_width = 0;
    for (FreeDimIndex1 i; i.is_not_at_end(); ++i)
    {
        std::ostringstream sout;
        sout << e[i];
        if (sout.str().length() > max_component_width)
            max_component_width = sout.str().length();
    }

    out << "[ ";
    for (FreeDimIndex1 i; i.is_not_at_end(); ++i)
    {
        out.setf(std::ios_base::right);
        out.width(max_component_width);
        out << e[i] << ' ';
    }
    return out << "](" << FreeDimIndex1::SYMBOL << ')';
}

// specialization for 2 indices
template <typename Derived, typename Scalar, typename FactorTypeList, typename FreeDimIndex1, typename FreeDimIndex2, typename UsedDimIndexTypeList>
std::ostream &operator << (std::ostream &out, ExpressionTemplate_i<Derived,Scalar,FactorTypeList,TypeList_t<FreeDimIndex1,TypeList_t<FreeDimIndex2> >,UsedDimIndexTypeList> const &e)
{
    // determine the max size that a component takes up
    Uint32 max_component_width = 0;
    for (typename Derived::MultiIndex m; m.is_not_at_end(); ++m)
    {
        std::ostringstream sout;
        sout << e[m];
        if (sout.str().length() > max_component_width)
            max_component_width = sout.str().length();
    }

    typename Derived::MultiIndex m;
    if (m.is_at_end())
        return out << "\n[[ ]]\n";

    out << "\n[";
    for (FreeDimIndex1 i; i.is_not_at_end(); ++i)
    {
        if (i.value() != 0)
            out << ' ';
        out << "[ ";
        for (FreeDimIndex2 j; j.is_not_at_end(); ++j)
        {
            out.setf(std::ios_base::right);
            out.width(max_component_width);
            out << e[m] << ' ';
            ++m;
        }
        out << ']';
        FreeDimIndex1 next(i);
        ++next;
        if (next.is_not_at_end())
            out << '\n';
    }
    return out << "](" << FreeDimIndex1::SYMBOL << ',' << FreeDimIndex2::SYMBOL << ")\n";
}

// specialization for 3 indices
template <typename Derived, typename Scalar, typename FactorTypeList, typename FreeDimIndex1, typename FreeDimIndex2, typename FreeDimIndex3, typename UsedDimIndexTypeList>
std::ostream &operator << (std::ostream &out, ExpressionTemplate_i<Derived,Scalar,FactorTypeList,TypeList_t<FreeDimIndex1,TypeList_t<FreeDimIndex2,TypeList_t<FreeDimIndex3> > >,UsedDimIndexTypeList> const &e)
{
    // determine the max size that a component takes up
    Uint32 max_component_width = 0;
    for (typename Derived::MultiIndex m; m.is_not_at_end(); ++m)
    {
        std::ostringstream sout;
        sout << e[m];
        if (sout.str().length() > max_component_width)
            max_component_width = sout.str().length();
    }

    typename Derived::MultiIndex m;
    if (m.is_at_end())
        return out << "\n[[[ ]]]\n";

    out << "\n[";
    for (FreeDimIndex1 i; i.is_not_at_end(); ++i)
    {
        if (i.value() != 0)
            out << ' ';
        out << '[';
        for (FreeDimIndex2 j; j.is_not_at_end(); ++j)
        {
            if (j.value() != 0)
                out << "  ";
            out << "[ ";
            for (FreeDimIndex3 k; k.is_not_at_end(); ++k)
            {
                out.setf(std::ios_base::right);
                out.width(max_component_width);
                out << e[m] << ' ';
                ++m;
            }
            out << ']';
            FreeDimIndex2 next(j);
            ++next;
            if (next.is_not_at_end())
                out << '\n';
        }
        out << ']';
        FreeDimIndex1 next(i);
        ++next;
        if (next.is_not_at_end())
            out << "\n\n";
    }
    return out << "](" << FreeDimIndex1::SYMBOL << ',' << FreeDimIndex2::SYMBOL << ',' << FreeDimIndex3::SYMBOL << ")\n";
}

} // end of namespace Tenh

#endif // TENH_INTERFACE_EXPRESSIONTEMPLATE_HPP_
