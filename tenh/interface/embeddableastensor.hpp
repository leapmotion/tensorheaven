// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/embeddableastensor.hpp by Victor Dods, created 2013/08/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_EMBEDDABLEASTENSOR_HPP_
#define TENH_INTERFACE_EMBEDDABLEASTENSOR_HPP_

#include "tenh/core.hpp"

#include <ostream>

#include "tenh/componentindex.hpp"
#include "tenh/conceptual/embeddableintensorproduct.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/interface/vector.hpp"

namespace Tenh {

// compile-time interface for a symmetric tensor type -- e.g. exterior/symmetric product.
// EmbeddableAsTensor_ should be a EmbeddableAsTensor_c type.
// TODO: technically, this should be LinearlyEmbeddableAsTensor_i
template <typename Derived_, typename Scalar_, typename EmbeddableInTensorProductOfBasedVectorSpaces_>
struct EmbeddableAsTensor_i : public Vector_i<Derived_,Scalar_,EmbeddableInTensorProductOfBasedVectorSpaces_>
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!Lvd::Meta::TypesAreEqual<Derived_,NullType>::v), DERIVED_MUST_NOT_BE_NULL_TYPE),
        //STATIC_ASSERT_IN_ENUM((FactorTypeList_::LENGTH > 0), MUST_BE_NONEMPTY) // NOTE: deprecate this, since 0-order tensors should be allowed
        STATIC_ASSERT_IN_ENUM(IsEmbeddableInTensorProductOfBasedVectorSpaces_c<EmbeddableInTensorProductOfBasedVectorSpaces_>::V, MUST_BE_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES)
    };

    typedef Vector_i<Derived_,Scalar_,EmbeddableInTensorProductOfBasedVectorSpaces_> Parent_Vector_i;
    typedef typename Parent_Vector_i::Derived Derived;
    typedef typename Parent_Vector_i::Scalar Scalar;
    typedef typename Parent_Vector_i::BasedVectorSpace BasedVectorSpace;
    using Parent_Vector_i::DIM; // does this make any sense?  e.g. simple tensors aren't a linear space
    typedef typename Parent_Vector_i::ComponentIndex ComponentIndex;

    typedef EmbeddableInTensorProductOfBasedVectorSpaces_ EmbeddableInTensorProductOfBasedVectorSpaces;
    typedef typename EmbeddableInTensorProductOfBasedVectorSpaces::TensorProductOfBasedVectorSpaces TensorProductOfBasedVectorSpaces;
    typedef typename TensorProductOfBasedVectorSpaces::FactorTypeList FactorTypeList;
    typedef MultiIndex_t<typename FactorComponentIndexTypeList_t<FactorTypeList>::T> MultiIndex;
    // this is not the "fully expanded" degree, but the number of [what you could think of
    // as "parenthesized"] factors that formed this tensor product type.
    static Uint32 const DEGREE = FactorTypeList::LENGTH;
    // static bool const IS_EMBEDDABLE_AS_TENSOR_I = true; // TODO: deprecate this in favor of IsEmbeddableAsTensor_i<...>

    static Uint32 degree () { return DEGREE; }

    using Parent_Vector_i::dim;
    using Parent_Vector_i::as_derived;
    using Parent_Vector_i::operator[];
    // you have to split before multi-index accessing

    using Parent_Vector_i::operator();
    // this provides the "split" operation without needing an intermediate temporary index,
    // since this object will be frequently split.
    // TODO: could the C++11 infer the return type?  this return type is annoying
    template <typename AbstractIndexHeadType, typename AbstractIndexBodyTypeList>
    ExpressionTemplate_IndexSplit_t<ExpressionTemplate_IndexedObject_t<Parent_Vector_i,
                                                                       TypeList_t<BasedVectorSpace>,
                                                                       TypeList_t<DimIndex_t<'~',DIM> >,
                                                                       EmptyTypeList,
                                                                       FORCE_CONST,
                                                                       CHECK_FOR_ALIASING>,
                                    AbstractIndex_c<'~'>,
                                    TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> > split (
        TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> const &abstract_multiindex) const
    {
        // make sure that the index type list actually contains AbstractIndex_c types
        STATIC_ASSERT((EachTypeIsAnAbstractIndex_c<TypeList_t<AbstractIndexHeadType,AbstractIndexBodyTypeList> >::V), MUST_BE_TYPELIST_OF_ABSTRACT_INDEX_TYPES);
        AbstractIndex_c<'~'> dummy_index;
        return operator()(dummy_index).split(dummy_index, abstract_multiindex);
    }
    // can't directly multi-index this object -- use split instead.

    static bool component_is_immutable_zero (MultiIndex const &m) { return as_derived().component_is_immutable_zero(m); }
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return as_derived().scalar_factor_for_component(m); }
    static ComponentIndex vector_index_of (MultiIndex const &m) { return as_derived().vector_index_of(m); }

    static std::string type_as_string ()
    {
        return "EmbeddableAsTensor_i<" + TypeStringOf_t<Derived>::eval() + ','
                                       + TypeStringOf_t<Scalar>::eval() + ','
                                       + TypeStringOf_t<EmbeddableInTensorProductOfBasedVectorSpaces>::eval() + '>';
    }
};

template <typename Derived, typename Scalar, typename EmbeddableInTensorProductOfBasedVectorSpaces>
std::ostream &operator << (std::ostream &out, EmbeddableAsTensor_i<Derived,Scalar,EmbeddableInTensorProductOfBasedVectorSpaces> const &e)
{
    return out << *static_cast<Vector_i<Derived,Scalar,EmbeddableInTensorProductOfBasedVectorSpaces> const *>(&e);
}

} // end of namespace Tenh

#endif // TENH_INTERFACE_EMBEDDABLEASTENSOR_HPP_
