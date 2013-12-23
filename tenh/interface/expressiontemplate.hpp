// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/expressiontemplate.hpp by Victor Dods, created 2013/04/05
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_EXPRESSIONTEMPLATE_HPP_
#define TENH_INTERFACE_EXPRESSIONTEMPLATE_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/dimindex.hpp"
#include "tenh/meta/typelist.hpp"
#include "tenh/multiindex.hpp"
#include "tenh/print_multiindexable.hpp"

namespace Tenh {

// ////////////////////////////////////////////////////////////////////////////
// compile-time interface for expression templates
// ////////////////////////////////////////////////////////////////////////////

template <typename Operand, typename BundleAbstractIndexTypeList, typename ResultingFactorType, typename ResultingAbstractIndexType>
struct ExpressionTemplate_IndexBundle_t;

template <typename Operand, typename SourceIndexType, typename SplitIndexTypeList>
struct ExpressionTemplate_IndexSplit_t;

template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
struct ExpressionTemplate_IndexSplitToIndex_t;

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
//   bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const // used in checking for aliasing
template <typename Derived_,
          typename Scalar_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
struct ExpressionTemplate_i // _i is for "compile-time interface"
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!TypesAreEqual_f<Derived_,NullType>::V), DERIVED_MUST_NOT_BE_NULL_TYPE),
        STATIC_ASSERT_IN_ENUM__UNIQUE((EachTypeSatisfies_f<FreeDimIndexTypeList_,IsDimIndex_p>::V), MUST_BE_TYPELIST_OF_DIM_INDEX_TYPES, FREEDIMINDEXTYPELIST),
        STATIC_ASSERT_IN_ENUM__UNIQUE((EachTypeSatisfies_f<UsedDimIndexTypeList_,IsDimIndex_p>::V), MUST_BE_TYPELIST_OF_DIM_INDEX_TYPES, USEDDIMINDEXTYPELIST)
    };

    typedef Derived_ Derived;
    // these typedefs make the Derived-specified typedefs available at the baseclass level,
    typedef Scalar_ Scalar;
    typedef FreeFactorTypeList_ FreeFactorTypeList;
    typedef FreeDimIndexTypeList_ FreeDimIndexTypeList;
    typedef UsedDimIndexTypeList_ UsedDimIndexTypeList;
    typedef MultiIndex_t<FreeDimIndexTypeList> MultiIndex;

    // TODO: some consistency checks on the various types

    // compile-time interface methods
    operator Scalar () const { return as_derived().operator Scalar(); }
    Scalar operator [] (MultiIndex const &m) const { return as_derived().operator[](m); }
    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const { return as_derived().overlaps_memory_range(ptr, range); }

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

    template <typename AbstractIndexHeadType,
              typename AbstractIndexBodyTypeList,
              typename ResultingFactorType,
              typename ResultingAbstractIndexType>
    struct BundleReturnType_f
    {
        typedef ExpressionTemplate_IndexBundle_t<Derived,
                                                 TypeList_t<AbstractIndexHeadType,
                                                            AbstractIndexBodyTypeList>,
                                                 ResultingFactorType,
                                                 ResultingAbstractIndexType> T;
    };

    // method for "bundling" separate abstract indices into a single abstract index
    // of a more specific type (e.g. a 2-tensor, a fully symmetric 3-tensor, etc)
    // (m(j*i)*a(j*k)*m(k*l)).bundle(i*l,Q) -- bundle i,l into Q
    template <typename AbstractIndexHeadType,
              typename AbstractIndexBodyTypeList,
              typename ResultingFactorType,
              typename ResultingAbstractIndexType>
    typename BundleReturnType_f<AbstractIndexHeadType,
                                AbstractIndexBodyTypeList,
                                ResultingFactorType,
                                ResultingAbstractIndexType>::T
        bundle (TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> const &,
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
        return typename BundleReturnType_f<AbstractIndexHeadType,
                                           AbstractIndexBodyTypeList,
                                           ResultingFactorType,
                                           ResultingAbstractIndexType>::T(as_derived());
    }

    template <typename SourceAbstractIndexType,
              typename AbstractIndexHeadType,
              typename AbstractIndexBodyTypeList>
    struct SplitReturnType_f
    {
        typedef ExpressionTemplate_IndexSplit_t<Derived,
                                                SourceAbstractIndexType,
                                                TypeList_t<AbstractIndexHeadType,
                                                           AbstractIndexBodyTypeList> > T;
    };

    // method for "splitting" the index of something that is "embeddable
    // as tensor" into a separate indices (a multiindex).  for example:
    //   a(P*Q).split(P,i*j).split(Q,k*l)
    // splits the tensor indices P and Q into the pairs i,j and k,l
    // so that the expression now has the four free indices i,j,k,l.
    // essentially this embeds the indexed factor into a particular
    // tensor product, and forgets the symmetries of the indexed factor.
    template <typename SourceAbstractIndexType,
              typename AbstractIndexHeadType,
              typename AbstractIndexBodyTypeList>
    typename SplitReturnType_f<SourceAbstractIndexType,
                               AbstractIndexHeadType,
                               AbstractIndexBodyTypeList>::T
        split (SourceAbstractIndexType const &,
               TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> const &) const
    {
        // make sure that SourceAbstractIndexType actually is one
        STATIC_ASSERT(IsAbstractIndex_f<SourceAbstractIndexType>::V, MUST_BE_ABSTRACT_INDEX);
        // make sure that the index type list actually contains AbstractIndex_c types
        STATIC_ASSERT((EachTypeSatisfies_f<TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList>, IsAbstractIndex_p>::V), MUST_BE_TYPELIST_OF_ABSTRACT_INDEX_TYPES);
        return typename SplitReturnType_f<SourceAbstractIndexType,
                                          AbstractIndexHeadType,
                                          AbstractIndexBodyTypeList>::T(as_derived());
    }
    // method for "splitting" the index of something that is "embeddable
    // as tensor" into the vector index of the tensor product.  for example,
    // if a is an element of the kth symmetric power of vector space X, then
    //   a(P).split(P,Q)
    // is an element of the kth [nonsymmetric] tensor power of vector space X,
    // indexed by the single index Q.  essentially this embeds the indexed
    // factor into a particular tensor product, and forgets the symmetries of
    // the indexed factor.
    // NOTE: this method will be deprecated when the "embed" feature exists.
    template <typename SourceAbstractIndexType, AbstractIndexSymbol SPLIT_ABSTRACT_INDEX_SYMBOL_>
    ExpressionTemplate_IndexSplitToIndex_t<Derived,SourceAbstractIndexType,AbstractIndex_c<SPLIT_ABSTRACT_INDEX_SYMBOL_> >
        split (SourceAbstractIndexType const &,
               AbstractIndex_c<SPLIT_ABSTRACT_INDEX_SYMBOL_> const &) const
    {
        // make sure that SourceAbstractIndexType actually is one
        STATIC_ASSERT(IsAbstractIndex_f<SourceAbstractIndexType>::V, MUST_BE_ABSTRACT_INDEX);
        return ExpressionTemplate_IndexSplitToIndex_t<Derived,SourceAbstractIndexType,AbstractIndex_c<SPLIT_ABSTRACT_INDEX_SYMBOL_> >(as_derived());
    }
    // method for doing an intermediate evaluation of an expression template to avoid aliasing
    ExpressionTemplate_Eval_t<Derived> eval () const
    {
        return ExpressionTemplate_Eval_t<Derived>(as_derived());
    }

    static std::string type_as_string ()
    {
        return "ExpressionTemplate_i<" + type_string_of<Derived_>() + ','
                                       + type_string_of<Scalar_>() + ','
                                       + type_string_of<FreeFactorTypeList_>() + ','
                                       + type_string_of<FreeDimIndexTypeList_>() + ','
                                       + type_string_of<UsedDimIndexTypeList_>() + '>';
    }
};

template <typename T_> struct IsExpressionTemplate_f { static bool const V = false; };
template <typename Derived_,
          typename Scalar_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
struct IsExpressionTemplate_f<ExpressionTemplate_i<Derived_,
                                                   Scalar_,
                                                   FreeFactorTypeList_,
                                                   FreeDimIndexTypeList_,
                                                   UsedDimIndexTypeList_> >
{
    static bool const V = true;
};

// will print any order expression template in a nice-looking justified way.  if the order is greater
// than 1, this will print newlines, notably including the first character.
template <typename Derived_,
          typename Scalar_,
          typename FreeFactorTypeList_,
          typename FreeDimIndexTypeList_,
          typename UsedDimIndexTypeList_>
std::ostream &operator << (std::ostream &out, ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTypeList_,FreeDimIndexTypeList_,UsedDimIndexTypeList_> const &e)
{
    print_multiindexable(out, e, FreeDimIndexTypeList_());
    typedef typename AbstractIndicesOfDimIndexTypeList_t<FreeDimIndexTypeList_>::T AbstractIndexTypeList;
    // print the abstract index symbols
    if (AbstractIndexTypeList::LENGTH > 0)
        out << '(' << symbol_string_of_abstract_index_type_list(AbstractIndexTypeList()) << ')';
    return out;
}

} // end of namespace Tenh

#endif // TENH_INTERFACE_EXPRESSIONTEMPLATE_HPP_
