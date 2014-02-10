// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/expressiontemplate.hpp by Victor Dods, created 2013/04/05
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_EXPRESSIONTEMPLATE_HPP_
#define TENH_INTERFACE_EXPRESSIONTEMPLATE_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/conceptual/linearembedding.hpp"
#include "tenh/dimindex.hpp"
#include "tenh/multiindex.hpp"
#include "tenh/print_multiindexable.hpp"

namespace Tenh {

// ////////////////////////////////////////////////////////////////////////////
// compile-time interface for expression templates
// ////////////////////////////////////////////////////////////////////////////

// TEMP: until the indexed expressions are flexible enough to safely do
// head/body recursion (e.g. in tensor product of procedural 2-tensors)
enum class CheckFactorTypes : bool { TRUE = true, FALSE = false };

inline std::ostream &operator << (std::ostream &out, CheckFactorTypes check_factor_types)
{
    return out << "CheckFactorTypes::" << (bool(check_factor_types) ? "TRUE" : "FALSE");
}

template <typename Operand, typename BundleAbstractIndexTyple, typename ResultingFactorType, typename ResultingAbstractIndexType, CheckFactorTypes CHECK_FACTOR_TYPES_>
struct ExpressionTemplate_IndexBundle_t;

template <typename Operand, typename SourceIndexType, typename SplitIndexTyple>
struct ExpressionTemplate_IndexSplit_t;

template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
struct ExpressionTemplate_IndexSplitToIndex_t;

template <typename Operand_,
          typename SourceAbstractIndexType_,
          typename EmbeddingCodomain_,
          typename EmbeddedAbstractIndexType_,
          typename EmbeddingId_>
struct ExpressionTemplate_IndexEmbed_t;

template <typename Operand_,
          typename SourceAbstractIndexType_,
          typename CoembeddingCodomain_,
          typename CoembeddedAbstractIndexType_,
          typename EmbeddingId_>
struct ExpressionTemplate_IndexCoembed_t;

template <typename Operand>
struct ExpressionTemplate_Eval_t;

// this is essentially a compile-time interface, requiring:
// - a Derived type (should be the type of the thing that ultimately inherits this)
// - a Scalar type (should be the scalar type of the expression template's tensor operand)
// - a FreeDimIndexTyple type (the free indices of this expression template)
// - a UsedDimIndexTyple type (the indices that have been used further down the AST)
// requires also particular methods:
//   operator Scalar () const // conversion to scalar -- always declared, but should
//                            // only compile if the conversion is well-defined (e.g. no free indices)
//   Scalar operator [] (MultiIndex const &) const // accessor using MultiIndex_t<FreeDimIndexTyple>
//   bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const // used in checking for aliasing
template <typename Derived_,
          typename Scalar_,
          typename FreeFactorTyple_,
          typename FreeDimIndexTyple_,
          typename UsedDimIndexTyple_>
struct ExpressionTemplate_i // _i is for "compile-time interface"
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!TypesAreEqual_f<Derived_,NullType>::V), DERIVED_MUST_NOT_BE_NULL_TYPE),
        STATIC_ASSERT_IN_ENUM__UNIQUE((EachTypeSatisfies_f<FreeDimIndexTyple_,IsDimIndex_e>::V), MUST_BE_TYPLE_OF_DIM_INDEX_TYPES, FREEDIMINDEXTYPLE),
        STATIC_ASSERT_IN_ENUM__UNIQUE((EachTypeSatisfies_f<UsedDimIndexTyple_,IsDimIndex_e>::V), MUST_BE_TYPLE_OF_DIM_INDEX_TYPES, USEDDIMINDEXTYPLE)
    };

    typedef Derived_ Derived;
    // these typedefs make the Derived-specified typedefs available at the baseclass level,
    typedef Scalar_ Scalar;
    typedef FreeFactorTyple_ FreeFactorTyple;
    typedef FreeDimIndexTyple_ FreeDimIndexTyple;
    typedef UsedDimIndexTyple_ UsedDimIndexTyple;
    typedef MultiIndex_t<FreeDimIndexTyple> MultiIndex;

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

    template <CheckFactorTypes CHECK_FACTOR_TYPES_,
              typename ResultingFactorType,
              typename ResultingAbstractIndexType,
              typename... AbstractIndexTypes_>
    struct BundleReturnType_f
    {
        typedef ExpressionTemplate_IndexBundle_t<Derived,
                                                 Typle_t<AbstractIndexTypes_...>,
                                                 ResultingFactorType,
                                                 ResultingAbstractIndexType,
                                                 CHECK_FACTOR_TYPES_> T;
    };

    // method for "bundling" separate abstract indices into a single abstract index
    // of a more specific type (e.g. a 2-tensor, a fully symmetric 3-tensor, etc)
    // (m(j*i)*a(j*k)*m(k*l)).bundle(i*l,Q) -- bundle i,l into Q
    template <typename ResultingFactorType,
              typename ResultingAbstractIndexType,
              typename... AbstractIndexTypes_>
    typename BundleReturnType_f<CheckFactorTypes::TRUE,
                                ResultingFactorType,
                                ResultingAbstractIndexType,
                                AbstractIndexTypes_...>::T
        bundle (Typle_t<AbstractIndexTypes_...> const &,
                ResultingFactorType const &,
                ResultingAbstractIndexType const &) const
    {
        typedef Typle_t<AbstractIndexTypes_...> AbstractIndexTyple;
        // make sure that ResultingAbstractIndexType actually is one
        STATIC_ASSERT(IsAbstractIndex_f<ResultingAbstractIndexType>::V, MUST_BE_ABSTRACT_INDEX);
        // make sure that the index typle actually contains AbstractIndex_c types
        STATIC_ASSERT((EachTypeSatisfies_f<AbstractIndexTyple,IsAbstractIndex_e>::V), MUST_BE_TYPLE_OF_ABSTRACT_INDEX_TYPES);
        // make sure that ResultingFactorType is the correct conceptual type
        // TODO: there is probably a stronger type check (a type which is embeddable into a tensor space)
        STATIC_ASSERT(HasBasedVectorSpaceStructure_f<ResultingFactorType>::V, MUST_BE_BASED_VECTOR_SPACE);
        return typename BundleReturnType_f<CheckFactorTypes::TRUE,
                                           ResultingFactorType,
                                           ResultingAbstractIndexType,
                                           AbstractIndexTypes_...>::T(as_derived());
    }

    // method for "bundling" separate abstract indices into a single abstract index
    // of a more specific type (e.g. a 2-tensor, a fully symmetric 3-tensor, etc)
    // (m(j*i)*a(j*k)*m(k*l)).bundle(i*l,Q) -- bundle i,l into Q
    // NOTE: this is a temporary hack to allow bundles that are technically type
    // unsafe, but that function.  this is necessary until the indexing paradigm
    // is expressable enough to do head/body recursion with type correctness.
    template <typename ResultingFactorType,
              typename ResultingAbstractIndexType,
              typename... AbstractIndexTypes_>
    typename BundleReturnType_f<CheckFactorTypes::FALSE,
                                ResultingFactorType,
                                ResultingAbstractIndexType,
                                AbstractIndexTypes_...>::T
        bundle_with_no_type_check (Typle_t<AbstractIndexTypes_...> const &,
                                   ResultingFactorType const &,
                                   ResultingAbstractIndexType const &) const
    {
        typedef Typle_t<AbstractIndexTypes_...> AbstractIndexTyple;
        // make sure that ResultingAbstractIndexType actually is one
        STATIC_ASSERT(IsAbstractIndex_f<ResultingAbstractIndexType>::V, MUST_BE_ABSTRACT_INDEX);
        // make sure that the index typle actually contains AbstractIndex_c types
        STATIC_ASSERT((EachTypeSatisfies_f<AbstractIndexTyple,IsAbstractIndex_e>::V), MUST_BE_TYPLE_OF_ABSTRACT_INDEX_TYPES);
        // make sure that ResultingFactorType is the correct conceptual type
        // TODO: there is probably a stronger type check (a type which is embeddable into a tensor space)
        STATIC_ASSERT(HasBasedVectorSpaceStructure_f<ResultingFactorType>::V, MUST_BE_BASED_VECTOR_SPACE);
        return typename BundleReturnType_f<CheckFactorTypes::FALSE,
                                           ResultingFactorType,
                                           ResultingAbstractIndexType,
                                           AbstractIndexTypes_...>::T(as_derived());
    }

    template <typename SourceAbstractIndexType,typename... AbstractIndexTypes_>
    struct SplitReturnType_f
    {
        typedef ExpressionTemplate_IndexSplit_t<Derived,
                                                SourceAbstractIndexType,
                                                Typle_t<AbstractIndexTypes_...>> T;
    };

    // method for "splitting" the index of something that is "embeddable
    // as tensor" into a separate indices (a multiindex).  for example:
    //   a(P*Q).split(P,i*j).split(Q,k*l)
    // splits the tensor indices P and Q into the pairs i,j and k,l
    // so that the expression now has the four free indices i,j,k,l.
    // essentially this embeds the indexed factor into a particular
    // tensor product, and forgets the symmetries of the indexed factor.
    template <typename SourceAbstractIndexType,
              typename... AbstractIndexTypes_>
    typename SplitReturnType_f<SourceAbstractIndexType,AbstractIndexTypes_...>::T
        split (SourceAbstractIndexType const &,
               Typle_t<AbstractIndexTypes_...> const &) const
    {
        typedef Typle_t<AbstractIndexTypes_...> AbstractIndexTyple;
        // make sure that SourceAbstractIndexType actually is one
        STATIC_ASSERT(IsAbstractIndex_f<SourceAbstractIndexType>::V, MUST_BE_ABSTRACT_INDEX);
        // make sure that the index typle actually contains AbstractIndex_c types
        STATIC_ASSERT((EachTypeSatisfies_f<AbstractIndexTyple,IsAbstractIndex_e>::V), MUST_BE_TYPLE_OF_ABSTRACT_INDEX_TYPES);
        return typename SplitReturnType_f<SourceAbstractIndexType,
                                          AbstractIndexTypes_...>::T(as_derived());
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
    ExpressionTemplate_IndexSplitToIndex_t<Derived,SourceAbstractIndexType,AbstractIndex_c<SPLIT_ABSTRACT_INDEX_SYMBOL_>>
        split (SourceAbstractIndexType const &,
               AbstractIndex_c<SPLIT_ABSTRACT_INDEX_SYMBOL_> const &) const
    {
        // make sure that SourceAbstractIndexType actually is one
        STATIC_ASSERT(IsAbstractIndex_f<SourceAbstractIndexType>::V, MUST_BE_ABSTRACT_INDEX);
        return ExpressionTemplate_IndexSplitToIndex_t<Derived,SourceAbstractIndexType,AbstractIndex_c<SPLIT_ABSTRACT_INDEX_SYMBOL_>>(as_derived());
    }

    // metafunction for determining the return type of the embed method.
    template <typename SourceAbstractIndexType_,
              typename EmbeddingCodomain_,
              AbstractIndexSymbol EMBEDDED_ABSTRACT_INDEX_SYMBOL_,
              typename EmbeddingId_>
    struct EmbedReturnType_f
    {
        typedef ExpressionTemplate_IndexEmbed_t<Derived,
                                                SourceAbstractIndexType_,
                                                EmbeddingCodomain_,
                                                AbstractIndex_c<EMBEDDED_ABSTRACT_INDEX_SYMBOL_>,
                                                EmbeddingId_> T;
    };
    // method for "embedding" the factor for a given index into another
    // space, such as embedding a diagonal 2-tensor into the corresponding
    // space of tensors -- though this is done entirely with vector indices.
    // for example:
    //   diag(i).embed(i,TensorProduct(),j)
    // embeds the diagonal 2-tensor into the corresponding 2-tensor product
    // defined by the type TensorProduct, having index j.
    template <typename EmbeddingId_,
              typename SourceAbstractIndexType_,
              typename EmbeddingCodomain_,
              AbstractIndexSymbol EMBEDDED_ABSTRACT_INDEX_SYMBOL_>
    typename EmbedReturnType_f<SourceAbstractIndexType_,
                               EmbeddingCodomain_,
                               EMBEDDED_ABSTRACT_INDEX_SYMBOL_,
                               EmbeddingId_>::T
        embed_using (SourceAbstractIndexType_ const &,
                     EmbeddingCodomain_ const &,
                     AbstractIndex_c<EMBEDDED_ABSTRACT_INDEX_SYMBOL_> const &) const
    {
        // make sure that SourceAbstractIndexType_ actually is one
        STATIC_ASSERT(IsAbstractIndex_f<SourceAbstractIndexType_>::V, MUST_BE_ABSTRACT_INDEX);
        return typename EmbedReturnType_f<SourceAbstractIndexType_,
                                          EmbeddingCodomain_,
                                          EMBEDDED_ABSTRACT_INDEX_SYMBOL_,
                                          EmbeddingId_>::T(as_derived());
    }
    // convenience method equivalent to embed_using<NaturalEmbedding>, e.g. for symmetric powers, etc.
    template <typename SourceAbstractIndexType_,
              typename EmbeddingCodomain_,
              AbstractIndexSymbol EMBEDDED_ABSTRACT_INDEX_SYMBOL_>
    typename EmbedReturnType_f<SourceAbstractIndexType_,
                               EmbeddingCodomain_,
                               EMBEDDED_ABSTRACT_INDEX_SYMBOL_,
                               NaturalEmbedding>::T
        embed (SourceAbstractIndexType_ const &,
               EmbeddingCodomain_ const &,
               AbstractIndex_c<EMBEDDED_ABSTRACT_INDEX_SYMBOL_> const &) const
    {
        // make sure that SourceAbstractIndexType_ actually is one
        STATIC_ASSERT(IsAbstractIndex_f<SourceAbstractIndexType_>::V, MUST_BE_ABSTRACT_INDEX);
        return typename EmbedReturnType_f<SourceAbstractIndexType_,
                                          EmbeddingCodomain_,
                                          EMBEDDED_ABSTRACT_INDEX_SYMBOL_,
                                          NaturalEmbedding>::T(as_derived());
    }

    // metafunction for determining the return type of the coembed method.
    template <typename SourceAbstractIndexType_,
              typename CoembeddingCodomain_,
              AbstractIndexSymbol COEMBEDDED_ABSTRACT_INDEX_SYMBOL_,
              typename EmbeddingId_>
    struct CoembedReturnType_f
    {
        typedef ExpressionTemplate_IndexCoembed_t<Derived,
                                                  SourceAbstractIndexType_,
                                                  CoembeddingCodomain_,
                                                  AbstractIndex_c<COEMBEDDED_ABSTRACT_INDEX_SYMBOL_>,
                                                  EmbeddingId_> T;
    };
    // method for "coembedding" the factor for a given index into another
    // space (essentially pulling from the larger space into the smaller)
    // which is an operation that is adjoint to embedding.  coembed is defined
    // by the property
    //   A(i).embed(i,UpstairsSpace(),j)*B(j) - A(i)*B(j).coembed(j,DownStairsSpace(),i) == 0
    template <typename EmbeddingId_,
              typename SourceAbstractIndexType_,
              typename CoembeddingCodomain_,
              AbstractIndexSymbol COEMBEDDED_ABSTRACT_INDEX_SYMBOL_>
    typename CoembedReturnType_f<SourceAbstractIndexType_,
                                 CoembeddingCodomain_,
                                 COEMBEDDED_ABSTRACT_INDEX_SYMBOL_,
                                 EmbeddingId_>::T
        coembed_using (SourceAbstractIndexType_ const &,
                       CoembeddingCodomain_ const &,
                       AbstractIndex_c<COEMBEDDED_ABSTRACT_INDEX_SYMBOL_> const &) const
    {
        // make sure that SourceAbstractIndexType_ actually is one
        STATIC_ASSERT(IsAbstractIndex_f<SourceAbstractIndexType_>::V, MUST_BE_ABSTRACT_INDEX);
        return typename CoembedReturnType_f<SourceAbstractIndexType_,
                                            CoembeddingCodomain_,
                                            COEMBEDDED_ABSTRACT_INDEX_SYMBOL_,
                                            EmbeddingId_>::T(as_derived());
    }
    // convenience method equivalent to coembed_using<NaturalEmbedding>, e.g. for symmetric powers, etc.
    template <typename SourceAbstractIndexType_,
              typename CoembeddingCodomain_,
              AbstractIndexSymbol COEMBEDDED_ABSTRACT_INDEX_SYMBOL_>
    typename CoembedReturnType_f<SourceAbstractIndexType_,
                                 CoembeddingCodomain_,
                                 COEMBEDDED_ABSTRACT_INDEX_SYMBOL_,
                                 NaturalEmbedding>::T
        coembed (SourceAbstractIndexType_ const &,
                 CoembeddingCodomain_ const &,
                 AbstractIndex_c<COEMBEDDED_ABSTRACT_INDEX_SYMBOL_> const &) const
    {
        // make sure that SourceAbstractIndexType_ actually is one
        STATIC_ASSERT(IsAbstractIndex_f<SourceAbstractIndexType_>::V, MUST_BE_ABSTRACT_INDEX);
        return typename CoembedReturnType_f<SourceAbstractIndexType_,
                                            CoembeddingCodomain_,
                                            COEMBEDDED_ABSTRACT_INDEX_SYMBOL_,
                                            NaturalEmbedding>::T(as_derived());
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
                                       + type_string_of<FreeFactorTyple_>() + ','
                                       + type_string_of<FreeDimIndexTyple_>() + ','
                                       + type_string_of<UsedDimIndexTyple_>() + '>';
    }
};

template <typename T_> struct IsExpressionTemplate_f { static bool const V = false; };
template <typename Derived_,
          typename Scalar_,
          typename FreeFactorTyple_,
          typename FreeDimIndexTyple_,
          typename UsedDimIndexTyple_>
struct IsExpressionTemplate_f<ExpressionTemplate_i<Derived_,
                                                   Scalar_,
                                                   FreeFactorTyple_,
                                                   FreeDimIndexTyple_,
                                                   UsedDimIndexTyple_> >
{
    static bool const V = true;
};

MAKE_1_ARY_VALUE_EVALUATOR(IsExpressionTemplate, bool);

// will print any order expression template in a nice-looking justified way.  if the order is greater
// than 1, this will print newlines, notably including the first character.
template <typename Derived_,
          typename Scalar_,
          typename FreeFactorTyple_,
          typename FreeDimIndexTyple_,
          typename UsedDimIndexTyple_>
std::ostream &operator << (std::ostream &out, ExpressionTemplate_i<Derived_,Scalar_,FreeFactorTyple_,FreeDimIndexTyple_,UsedDimIndexTyple_> const &e)
{
    print_multiindexable(out, e, FreeDimIndexTyple_());
    typedef typename AbstractIndicesOfDimIndexTyple_f<FreeDimIndexTyple_>::T AbstractIndexTyple;
    // print the abstract index symbols
    if (Length_f<AbstractIndexTyple>::V > 0)
        out << '(' << symbol_string_of_abstract_index_typle(AbstractIndexTyple()) << ')';
    return out;
}

} // end of namespace Tenh

#endif // TENH_INTERFACE_EXPRESSIONTEMPLATE_HPP_
