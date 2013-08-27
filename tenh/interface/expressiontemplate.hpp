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
#include "tenh/print_multiindexable.hpp"

namespace Tenh {

// ////////////////////////////////////////////////////////////////////////////
// compile-time interface for expression templates
// ////////////////////////////////////////////////////////////////////////////

template <typename Operand, typename BundleAbstractIndexTypeList, typename ResultingFactorType, typename ResultingAbstractIndexType>
struct ExpressionTemplate_IndexBundle_t;

template <typename Operand, typename SourceIndexType, typename SplitIndexTypeList>
struct ExpressionTemplate_IndexSplit_t;

template <typename Operand>
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
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
struct ExpressionTemplate_i // _i is for "compile-time interface"
{
    enum
    {
        //DERIVED_MUST_NOT_BE_NULL_TYPE = Lvd::Meta::Assert<!Lvd::Meta::TypesAreEqual<Derived_,NullType>::v>::v
        STATIC_ASSERT_IN_ENUM((!Lvd::Meta::TypesAreEqual<Derived_,NullType>::v), DERIVED_MUST_NOT_BE_NULL_TYPE),
        STATIC_ASSERT_IN_ENUM__UNIQUE(EachTypeIsADimIndex_f<FreeDimIndexTypeList_>::V, MUST_BE_TYPELIST_OF_DIM_INDEX_TYPES, FREEDIMINDEXTYPELIST),
        STATIC_ASSERT_IN_ENUM__UNIQUE(EachTypeIsADimIndex_f<UsedDimIndexTypeList_>::V, MUST_BE_TYPELIST_OF_DIM_INDEX_TYPES, USEDDIMINDEXTYPELIST)
    };

    typedef Derived_ Derived;
    // these typedefs make the Derived-specified typedefs available at the baseclass level,
    typedef Scalar_ Scalar;
    typedef FreeFactorTypeList_ FreeFactorTypeList;
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
    template <typename AbstractIndexHeadType, typename AbstractIndexBodyTypeList, typename ResultingFactorType, typename ResultingAbstractIndexType>
    ExpressionTemplate_IndexBundle_t<Derived,TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList>,ResultingFactorType,ResultingAbstractIndexType> bundle (
        TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> const &,
        ResultingFactorType const &,
        ResultingAbstractIndexType const &) const
    {
        // make sure that ResultingAbstractIndexType actually is one
        STATIC_ASSERT(IsAbstractIndex_f<ResultingAbstractIndexType>::V, MUST_BE_ABSTRACT_INDEX);
        // make sure that the index type list actually contains AbstractIndex_c types
        STATIC_ASSERT((EachTypeSatisfies_f<TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList>, IsAbstractIndex_p>::V), MUST_BE_TYPELIST_OF_ABSTRACT_INDEX_TYPES);
        // make sure that ResultingFactorType is the correct conceptual type
        // TODO: there is probably a stronger type check (a type which is embeddable into a tensor space)
        STATIC_ASSERT(HasBasedVectorSpaceStructure_f<ResultingFactorType>::V, MUST_BE_BASED_VECTOR_SPACE);
        return ExpressionTemplate_IndexBundle_t<Derived,TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList>,ResultingFactorType,ResultingAbstractIndexType>(as_derived());
    }
    // method for "splitting" a tensor index into a separate indices.
    // a(P|Q).split(P,i|j).split(Q,k|l) -- split the tensor indices P and Q into the pairs i,j and k,l
    // so that the expression now has the four free indices i,j,k,l.
    template <typename SourceAbstractIndexType, typename AbstractIndexHeadType, typename AbstractIndexBodyTypeList>
    ExpressionTemplate_IndexSplit_t<Derived,SourceAbstractIndexType,TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> > split (
        SourceAbstractIndexType const &,
        TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> const &) const
    {
        // make sure that SourceAbstractIndexType actually is one
        STATIC_ASSERT(IsAbstractIndex_f<SourceAbstractIndexType>::V, MUST_BE_ABSTRACT_INDEX);
        // make sure that the index type list actually contains AbstractIndex_c types
        STATIC_ASSERT((EachTypeSatisfies_f<TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList>, IsAbstractIndex_p>::V), MUST_BE_TYPELIST_OF_ABSTRACT_INDEX_TYPES);
        return ExpressionTemplate_IndexSplit_t<Derived,SourceAbstractIndexType,TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> >(as_derived());
    }
    // method for doing an intermediate evaluation of an expression template to avoid aliasing
    ExpressionTemplate_Eval_t<Derived> eval () const
    {
        return ExpressionTemplate_Eval_t<Derived>(as_derived());
    }
};

// will print any order expression template in a nice-looking justified way.  if the order is greater
// than 1, this will print newlines, notably including the first character.
template <typename Derived,
          typename Scalar,
          typename FreeFactorTypeList,
          typename FreeDimIndexTypeList,
          typename UsedDimIndexTypeList>
std::ostream &operator << (std::ostream &out, ExpressionTemplate_i<Derived,Scalar,FreeFactorTypeList,FreeDimIndexTypeList,UsedDimIndexTypeList> const &e)
{
    typedef ExpressionTemplate_i<Derived,Scalar,FreeFactorTypeList,FreeDimIndexTypeList,UsedDimIndexTypeList> ExpressionTemplate;
    print_multiindexable(out, e, FreeDimIndexTypeList());
    typedef typename AbstractIndicesOfDimIndexTypeList_t<FreeDimIndexTypeList>::T AbstractIndexTypeList;
    // print the abstract index symbols
    if (AbstractIndexTypeList::LENGTH > 0)
        out << '(' << symbol_string_of_abstract_index_type_list(AbstractIndexTypeList()) << ')';
    return out;
}

} // end of namespace Tenh

#endif // TENH_INTERFACE_EXPRESSIONTEMPLATE_HPP_
