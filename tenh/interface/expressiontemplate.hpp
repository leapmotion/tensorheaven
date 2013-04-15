// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/expressiontemplate.hpp by Victor Dods, created 2013/04/05
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_EXPRESSIONTEMPLATE_HPP_
#define TENH_INTERFACE_EXPRESSIONTEMPLATE_HPP_

#include "tenh/core.hpp"
// #include "tenh/expression_templates_utility.hpp"
#include "tenh/index.hpp"

namespace Tenh {

// ////////////////////////////////////////////////////////////////////////////
// compile-time interface for expression templates
// ////////////////////////////////////////////////////////////////////////////

template <typename Operand, typename BundleIndexTypeList, typename ResultingIndexType>
struct ExpressionTemplate_IndexBundle_t;

template <typename Operand, typename SourceIndexType, typename SplitIndexTypeList>
struct ExpressionTemplate_IndexSplit_t;

template <typename Operand, typename FreeIndexTypeList_>
struct ExpressionTemplate_Eval_t;

// this is essentially a compile-time interface, requiring:
// - a Derived type (should be the type of the thing that ultimately inherits this)
// - a Scalar type (should be the scalar type of the expression template's tensor operand)
// - a FreeIndexTypeList type (the free indices of this expression template)
// - a UsedIndexTypeList type (the indices that have been used further down the AST)
// requires also particular methods:
//   operator Scalar () const // conversion to scalar -- always declared, but should
//                            // only compile if the conversion is well-defined (e.g. no free indices)
//   Scalar operator [] (MultiIndex const &) const // accessor using MultiIndex_t<FreeIndexTypeList>
//   template <typename OtherTensor> bool uses_tensor (OtherTensor const &) const // used in checking for aliasing
template <typename Derived_, typename Scalar_, typename FreeIndexTypeList_, typename UsedIndexTypeList_>
struct ExpressionTemplate_i // _i is for "compile-time interface"
{
    enum { DERIVED_MUST_NOT_BE_NULL_TYPE = Lvd::Meta::Assert<!Lvd::Meta::TypesAreEqual<Derived_,NullType>::v>::v };

    typedef Derived_ Derived;
    // these typedefs make the Derived-specified typedefs available at the baseclass level,
    typedef Scalar_ Scalar;
    typedef FreeIndexTypeList_ FreeIndexTypeList;
    typedef UsedIndexTypeList_ UsedIndexTypeList;
    typedef MultiIndex_t<FreeIndexTypeList> MultiIndex;
    static bool const IS_EXPRESSION_TEMPLATE = true;

    // TODO: some consistency checks on the various types

    // compile-time interface methods
    operator Scalar () const { return as_derived().operator Scalar(); }
    Scalar operator [] (MultiIndex const &m) const { return as_derived().operator[](m); }
    template <typename OtherTensor> bool uses_tensor (OtherTensor const &t) const { return as_derived().template uses_tensor<OtherTensor>(t); }

    // for accessing this as the Derived type
    Derived const &as_derived () const { return *static_cast<Derived const *>(this); }
    Derived &as_derived () { return *static_cast<Derived *>(this); }

    // method for "bundling" two separate indices into a single 2-tensor index
    // (m(j|i)*a(j|k)*m(k|l)).bundle(i|l,Q) -- bundle i,l into Q
    template <typename IndexHeadType, typename IndexBodyTypeList, typename ResultingIndexType>
    ExpressionTemplate_IndexBundle_t<Derived,TypeList_t<IndexHeadType,IndexBodyTypeList>,ResultingIndexType> bundle (
        TypeList_t<IndexHeadType,IndexBodyTypeList> const &,
        ResultingIndexType const &) const
    {
        // make sure that the index type list actually contains TypedIndex_t types
        AssertThatEachTypeIsATypedIndex_t<TypeList_t<IndexHeadType,IndexBodyTypeList> >();
        return ExpressionTemplate_IndexBundle_t<Derived,TypeList_t<IndexHeadType,IndexBodyTypeList>,ResultingIndexType>(as_derived());
    }
    // method for "splitting" a tensor index into a separate indices.
    // a(P|Q).split(P,i|j).split(Q,k|l) -- split the tensor indices P and Q into the pairs i,j and k,l
    // so that the expression now has the four free indices i,j,k,l.
    template <typename SourceIndexType, typename IndexHeadType, typename IndexBodyTypeList>
    ExpressionTemplate_IndexSplit_t<Derived,SourceIndexType,TypeList_t<IndexHeadType,IndexBodyTypeList> > split (
        SourceIndexType const &,
        TypeList_t<IndexHeadType,IndexBodyTypeList> const &) const
    {
        // make sure that the index type list actually contains TypedIndex_t types
        AssertThatEachTypeIsATypedIndex_t<TypeList_t<IndexHeadType,IndexBodyTypeList> >();
        return ExpressionTemplate_IndexSplit_t<Derived,SourceIndexType,TypeList_t<IndexHeadType,IndexBodyTypeList> >(as_derived());
    }
    // method for doing an intermediate evaluation of an expression template to avoid aliasing
    ExpressionTemplate_Eval_t<Derived,FreeIndexTypeList> eval () const
    {
        return ExpressionTemplate_Eval_t<Derived,FreeIndexTypeList>(as_derived());
    }
};

// it's just more convenient to write specializations for particular numbers of free indices

// specialization for 1 index
template <typename Derived, typename Scalar, typename FreeIndex1, typename UsedIndexTypeList>
std::ostream &operator << (std::ostream &out, ExpressionTemplate_i<Derived,Scalar,TypeList_t<FreeIndex1>,UsedIndexTypeList> const &e)
{
    // determine the max size that a component takes up
    Uint32 max_component_width = 0;
    for (FreeIndex1 i; i.is_not_at_end(); ++i)
    {
        std::ostringstream sout;
        sout << e[i];
        if (sout.str().length() > max_component_width)
            max_component_width = sout.str().length();
    }

    out << "[ ";
    for (FreeIndex1 i; i.is_not_at_end(); ++i)
    {
        out.setf(std::ios_base::right);
        out.width(max_component_width);
        out << e[i] << ' ';
    }
    return out << "](" << FreeIndex1::SYMBOL << ')';
}

// specialization for 2 indices
template <typename Derived, typename Scalar, typename FreeIndex1, typename FreeIndex2, typename UsedIndexTypeList>
std::ostream &operator << (std::ostream &out, ExpressionTemplate_i<Derived,Scalar,TypeList_t<FreeIndex1,TypeList_t<FreeIndex2> >,UsedIndexTypeList> const &e)
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
    for (FreeIndex1 i; i.is_not_at_end(); ++i)
    {
        if (i.value() != 0)
            out << ' ';
        out << "[ ";
        for (FreeIndex2 j; j.is_not_at_end(); ++j)
        {
            out.setf(std::ios_base::right);
            out.width(max_component_width);
            out << e[m] << ' ';
            ++m;
        }
        out << ']';
        FreeIndex1 next(i);
        ++next;
        if (next.is_not_at_end())
            out << '\n';
    }
    return out << "](" << FreeIndex1::SYMBOL << ',' << FreeIndex2::SYMBOL << ")\n";
}

// specialization for 3 indices
template <typename Derived, typename Scalar, typename FreeIndex1, typename FreeIndex2, typename FreeIndex3, typename UsedIndexTypeList>
std::ostream &operator << (std::ostream &out, ExpressionTemplate_i<Derived,Scalar,TypeList_t<FreeIndex1,TypeList_t<FreeIndex2,TypeList_t<FreeIndex3> > >,UsedIndexTypeList> const &e)
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
    for (FreeIndex1 i; i.is_not_at_end(); ++i)
    {
        if (i.value() != 0)
            out << ' ';
        out << '[';
        for (FreeIndex2 j; j.is_not_at_end(); ++j)
        {
            if (j.value() != 0)
                out << "  ";
            out << "[ ";
            for (FreeIndex3 k; k.is_not_at_end(); ++k)
            {
                out.setf(std::ios_base::right);
                out.width(max_component_width);
                out << e[m] << ' ';
                ++m;
            }
            out << ']';
            FreeIndex2 next(j);
            ++next;
            if (next.is_not_at_end())
                out << '\n';
        }
        out << ']';
        FreeIndex1 next(i);
        ++next;
        if (next.is_not_at_end())
            out << "\n\n";
    }
    return out << "](" << FreeIndex1::SYMBOL << ',' << FreeIndex2::SYMBOL << ',' << FreeIndex3::SYMBOL << ")\n";
}

} // end of namespace Tenh

#endif // TENH_INTERFACE_EXPRESSIONTEMPLATE_HPP_
