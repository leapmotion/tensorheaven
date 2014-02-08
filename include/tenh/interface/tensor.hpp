// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/tensor.hpp by Victor Dods, created 2013/04/05
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_TENSOR_HPP_
#define TENH_INTERFACE_TENSOR_HPP_

#include <ostream>

#include "tenh/core.hpp"

#include "tenh/componentindex.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/interface/embeddableastensor.hpp"
#include "tenh/print_multiindexable.hpp"

namespace Tenh {

// compile-time interface for a non-symmetric tensor product class.  TensorProductOfBasedVectorSpaces_
// should be a TensorProductOfBasedVectorSpaces_c type.
template <typename Derived_,
          typename Scalar_,
          typename TensorProductOfBasedVectorSpaces_,
          ComponentQualifier COMPONENT_QUALIFIER_>
struct Tensor_i
    :
    public EmbeddableAsTensor_i<Derived_,Scalar_,TensorProductOfBasedVectorSpaces_,COMPONENT_QUALIFIER_>
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!TypesAreEqual_f<Derived_,NullType>::V), DERIVED_MUST_NOT_BE_NULL_TYPE),
        STATIC_ASSERT_IN_ENUM(IS_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES_UNIQUELY(TensorProductOfBasedVectorSpaces_), MUST_BE_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES)
    };

    typedef EmbeddableAsTensor_i<Derived_,Scalar_,TensorProductOfBasedVectorSpaces_,COMPONENT_QUALIFIER_> Parent_EmbeddableAsTensor_i;
    typedef typename Parent_EmbeddableAsTensor_i::Derived Derived;
    typedef typename Parent_EmbeddableAsTensor_i::Scalar Scalar;
    using Parent_EmbeddableAsTensor_i::DIM;
    typedef typename Parent_EmbeddableAsTensor_i::ComponentIndex ComponentIndex;
    using Parent_EmbeddableAsTensor_i::COMPONENT_QUALIFIER;
    typedef typename Parent_EmbeddableAsTensor_i::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_EmbeddableAsTensor_i::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;
    typedef typename Parent_EmbeddableAsTensor_i::QualifiedComponent QualifiedComponent;

    typedef TensorProductOfBasedVectorSpaces_ TensorProductOfBasedVectorSpaces;
    typedef typename FactorTypleOf_f<TensorProductOfBasedVectorSpaces>::T FactorTyple;
    typedef MultiIndex_t<typename FactorComponentIndexTyple_f<FactorTyple>::T> MultiIndex;
    // this is not the "fully expanded" order, but the number of [what you could think of
    // as "parenthesized"] factors that formed this tensor product type.
    static Uint32 const ORDER = OrderOf_f<TensorProductOfBasedVectorSpaces_>::V;
    static bool const IS_TENSOR_I = true; // TODO: deprecate this in favor of IsATensor_i<...>

    // TODO: could put canonical as_factorX conversions here

    static Uint32 order () { return ORDER; }

    using Parent_EmbeddableAsTensor_i::dim;
    using Parent_EmbeddableAsTensor_i::as_derived;
    using Parent_EmbeddableAsTensor_i::operator[];
    // multi-index component access which is a frontend for the vector-index component access
    template <typename OtherIndexTyple_>
    ComponentAccessConstReturnType operator [] (MultiIndex_t<OtherIndexTyple_> const &m) const
    {
        static_assert(IsTyple_f<OtherIndexTyple_>::V, "OtherIndexTyple_ must be a Typle_t");
        typedef MultiIndex_t<OtherIndexTyple_> OtherMultiIndex;
        STATIC_ASSERT((OtherMultiIndex::LENGTH == MultiIndex::LENGTH), MUST_HAVE_EQUAL_LENGTHS);
        //std::cout << OtherMultiIndex::LENGTH << ", " << MultiIndex::LENGTH << '\n';
        assert(m.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        // NOTE: this construction is unnecessary to the code, but IS necessary to the 
        // compile-time type checking -- the compiler should optimize it out anyway.
        MultiIndex x(m);
        // m.value() is what does the multi-index-to-vector-index computation
        return Parent_EmbeddableAsTensor_i::operator[](ComponentIndex(m.value(), CheckRange::FALSE));
    }
    // multi-index component access which is a frontend for the vector-index component access
    template <typename OtherIndexTyple_>
    ComponentAccessNonConstReturnType operator [] (MultiIndex_t<OtherIndexTyple_> const &m)
    {
        static_assert(IsTyple_f<OtherIndexTyple_>::V, "OtherIndexTyple_ must be a Typle_t");
        typedef MultiIndex_t<OtherIndexTyple_> OtherMultiIndex;
        STATIC_ASSERT((OtherMultiIndex::LENGTH == MultiIndex::LENGTH), MUST_HAVE_EQUAL_LENGTHS);
        //std::cout << OtherMultiIndex::LENGTH << ", " << MultiIndex::LENGTH << '\n';
        assert(m.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        // NOTE: this construction is unnecessary to the code, but IS necessary to the 
        // compile-time type checking -- the compiler should optimize it out anyway.
        MultiIndex x(m);
        // m.value() is what does the multi-index-to-vector-index computation
        return Parent_EmbeddableAsTensor_i::operator[](ComponentIndex(m.value(), CheckRange::FALSE));
    }

    using Parent_EmbeddableAsTensor_i::operator();

    // Because the return type for "operator () (...) const" is an abomination, use this helper.
    // This metafunction also serves to allow a 1-multiindex (i.e. a typle of abstract indices
    // having 1 element) to create vector-indexed expressions.
    template <typename AbstractIndexTyple_>
    struct IndexedExpressionConstType_f;
    // Because the return type for "operator () (...) const" is an abomination, use this helper.
    // This metafunction also serves to allow a 1-multiindex (i.e. a typle of abstract indices
    // having 1 element) to create vector-indexed expressions.
    template <typename AbstractIndexTyple_>
    struct IndexedExpressionNonConstType_f;

    // specialization for indexed expressions using 1-multiindices, effectively indexing this
    // Tensor_i as a Vector_i
    template <typename AbstractIndexHeadType_>
    typename IndexedExpressionConstType_f<Typle_t<AbstractIndexHeadType_>>::T
        operator () (Typle_t<AbstractIndexHeadType_> const &) const
    {
        STATIC_ASSERT(IsAbstractIndex_f<AbstractIndexHeadType_>::V, MUST_BE_ABSTRACT_INDEX);
        return typename IndexedExpressionConstType_f<Typle_t<AbstractIndexHeadType_>>::T(as_derived());
    }
    // specialization for indexed expressions using 1-multiindices, effectively indexing this
    // Tensor_i as a Vector_i
    template <typename AbstractIndexHeadType_>
    typename IndexedExpressionNonConstType_f<Typle_t<AbstractIndexHeadType_>>::T
        operator () (Typle_t<AbstractIndexHeadType_> const &)
    {
        STATIC_ASSERT(IsAbstractIndex_f<AbstractIndexHeadType_>::V, MUST_BE_ABSTRACT_INDEX);
        return typename IndexedExpressionNonConstType_f<Typle_t<AbstractIndexHeadType_>>::T(as_derived());
    }

    // the two separate head/body template arguments are necessary to disambiguate this method
    // from one that takes a single index (i.e. the index-by-vector-index one).
    template <typename... AbstractIndexTypes_>
    typename IndexedExpressionConstType_f<Typle_t<AbstractIndexTypes_...>>::T
        operator () (Typle_t<AbstractIndexTypes_...> const &) const
    {
        typedef Typle_t<AbstractIndexTypes_...> AbstractIndexTyple;
        STATIC_ASSERT((EachTypeSatisfies_f<AbstractIndexTyple,IsAbstractIndex_e>::V), EACH_TYPE_MUST_BE_ABSTRACT_INDEX);
        STATIC_ASSERT((Length_f<AbstractIndexTyple>::V == ORDER), ARGUMENT_LENGTH_MUST_EQUAL_ORDER);
        return typename IndexedExpressionConstType_f<AbstractIndexTyple>::T(as_derived());
    }
    template <typename... AbstractIndexTypes_>
    typename IndexedExpressionNonConstType_f<Typle_t<AbstractIndexTypes_...>>::T
        operator () (Typle_t<AbstractIndexTypes_...> const &)
    {
        typedef Typle_t<AbstractIndexTypes_...> AbstractIndexTyple;
        STATIC_ASSERT((EachTypeSatisfies_f<AbstractIndexTyple,IsAbstractIndex_e>::V), EACH_TYPE_MUST_BE_ABSTRACT_INDEX);
        STATIC_ASSERT((Length_f<AbstractIndexTyple>::V == ORDER), ARGUMENT_LENGTH_MUST_EQUAL_ORDER);
        return typename IndexedExpressionNonConstType_f<AbstractIndexTyple>::T(as_derived());
    }

    static bool component_is_procedural_zero (MultiIndex const &) { return false; }
    static Scalar scalar_factor_for_component (MultiIndex const &) { return Scalar(1); }
    static ComponentIndex vector_index_of (MultiIndex const &m) { return ComponentIndex(m.value(), CheckRange::FALSE); }

    static std::string type_as_string ()
    {
        return "Tensor_i<" + type_string_of<Derived>() + ','
                           + type_string_of<Scalar>() + ','
                           + type_string_of<TensorProductOfBasedVectorSpaces>() + ','
                           + component_qualifier_as_string(COMPONENT_QUALIFIER_) + '>';
    }
};

// will print any order tensor in a nice-looking justified way.  if the order is greater
// than 1, this will print newlines, notably including the first character.
template <typename Derived_, typename Scalar_, typename TensorProductOfBasedVectorSpaces_, ComponentQualifier COMPONENT_QUALIFIER_>
std::ostream &operator << (std::ostream &out, Tensor_i<Derived_,Scalar_,TensorProductOfBasedVectorSpaces_,COMPONENT_QUALIFIER_> const &t)
{
    typedef Tensor_i<Derived_,Scalar_,TensorProductOfBasedVectorSpaces_,COMPONENT_QUALIFIER_> Tensor;
    typedef typename Tensor::MultiIndex::IndexTyple IndexTyple;
    print_multiindexable(out, t, IndexTyple());
    return out;
}

// the general definition indexes this Tensor_i as a tensor (using a multiindex)
template <typename Derived_, typename Scalar_, typename TensorProductOfBasedVectorSpaces_, ComponentQualifier COMPONENT_QUALIFIER_>
template <typename AbstractIndexTyple_>
struct Tensor_i<Derived_,Scalar_,TensorProductOfBasedVectorSpaces_,COMPONENT_QUALIFIER_>
       ::IndexedExpressionConstType_f
{
    typedef ExpressionTemplate_IndexedObject_t<
                Derived, // have to use Derived instead of Tensor_i, so that the return-a-reference operator[] is used
                FactorTyple,
                typename DimIndexTypleOf_f<FactorTyple,AbstractIndexTyple_>::T,
                typename SummedIndexTyple_f<typename DimIndexTypleOf_f<FactorTyple,AbstractIndexTyple_>::T>::T,
                ForceConst::TRUE,
                CheckForAliasing::TRUE> T;
};

// the special definition (for 1-multiindices) indexes this Tensor_i as a Vector_i
template <typename Derived_, typename Scalar_, typename TensorProductOfBasedVectorSpaces_, ComponentQualifier COMPONENT_QUALIFIER_>
template <typename AbstractIndexHeadType_>
struct Tensor_i<Derived_,Scalar_,TensorProductOfBasedVectorSpaces_,COMPONENT_QUALIFIER_>
       ::IndexedExpressionConstType_f<Typle_t<AbstractIndexHeadType_>>
{
    typedef typename Parent_EmbeddableAsTensor_i::template IndexedExpressionConstType_f<SymbolOf_f<AbstractIndexHeadType_>::V>::T T;
};

// the general definition indexes this Tensor_i as a tensor (using a multiindex)
template <typename Derived_, typename Scalar_, typename TensorProductOfBasedVectorSpaces_, ComponentQualifier COMPONENT_QUALIFIER_>
template <typename AbstractIndexTyple_>
struct Tensor_i<Derived_,Scalar_,TensorProductOfBasedVectorSpaces_,COMPONENT_QUALIFIER_>
       ::IndexedExpressionNonConstType_f
{
    typedef ExpressionTemplate_IndexedObject_t<
                Derived, // have to use Derived instead of Tensor_i, so that the return-a-reference operator[] is used
                FactorTyple,
                typename DimIndexTypleOf_f<FactorTyple,AbstractIndexTyple_>::T,
                typename SummedIndexTyple_f<typename DimIndexTypleOf_f<FactorTyple,AbstractIndexTyple_>::T>::T,
                ForceConst::FALSE,
                CheckForAliasing::TRUE> T;
};

// the special definition (for 1-multiindices) indexes this Tensor_i as a Vector_i
template <typename Derived_, typename Scalar_, typename TensorProductOfBasedVectorSpaces_, ComponentQualifier COMPONENT_QUALIFIER_>
template <typename AbstractIndexHeadType_>
struct Tensor_i<Derived_,Scalar_,TensorProductOfBasedVectorSpaces_,COMPONENT_QUALIFIER_>
       ::IndexedExpressionNonConstType_f<Typle_t<AbstractIndexHeadType_>>
{
    typedef typename Parent_EmbeddableAsTensor_i::template IndexedExpressionNonConstType_f<SymbolOf_f<AbstractIndexHeadType_>::V>::T T;
};

} // end of namespace Tenh

#endif // TENH_INTERFACE_TENSOR_HPP_
