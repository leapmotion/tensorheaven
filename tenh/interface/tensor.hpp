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
#include "tenh/interface/vector.hpp"
#include "tenh/print_multiindexable.hpp"

namespace Tenh {

template <typename FactorTypeList>
struct FactorComponentIndexTypeList_t
{
    typedef TypeList_t<ComponentIndex_t<AS_VECTOR_SPACE(typename FactorTypeList::HeadType)::DIMENSION>,
                       typename FactorComponentIndexTypeList_t<typename FactorTypeList::BodyTypeList>::T> T;
};

template <typename HeadType>
struct FactorComponentIndexTypeList_t<TypeList_t<HeadType> >
{
    typedef TypeList_t<ComponentIndex_t<AS_VECTOR_SPACE(HeadType)::DIMENSION> > T;
};

template <>
struct FactorComponentIndexTypeList_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};

// compile-time interface for a non-symmetric tensor product class.  TensorProductOfBasedVectorSpaces_
// should be a TensorProductOfBasedVectorSpaces_c type.
template <typename Derived_, typename Scalar_, typename TensorProductOfBasedVectorSpaces_>
struct Tensor_i : public Vector_i<Derived_,Scalar_,TensorProductOfBasedVectorSpaces_>
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!Lvd::Meta::TypesAreEqual<Derived_,NullType>::v), DERIVED_MUST_NOT_BE_NULL_TYPE),
        //STATIC_ASSERT_IN_ENUM((FactorTypeList_::LENGTH > 0), MUST_BE_NONEMPTY) // TODO: deprecate this, since 0-order tensors should be allowed
        STATIC_ASSERT_IN_ENUM(IS_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES_UNIQUELY(TensorProductOfBasedVectorSpaces_), MUST_BE_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES)
    };

    typedef Vector_i<Derived_,Scalar_,TensorProductOfBasedVectorSpaces_> Parent_Vector_i;
    typedef typename Parent_Vector_i::Derived Derived;
    typedef typename Parent_Vector_i::Scalar Scalar;
    typedef typename Parent_Vector_i::BasedVectorSpace BasedVectorSpace;
    using Parent_Vector_i::DIM;
    typedef typename Parent_Vector_i::ComponentIndex ComponentIndex;

    typedef TensorProductOfBasedVectorSpaces_ TensorProductOfBasedVectorSpaces;
    typedef typename TensorProductOfBasedVectorSpaces::FactorTypeList FactorTypeList;
    typedef MultiIndex_t<typename FactorComponentIndexTypeList_t<FactorTypeList>::T> MultiIndex;
    // this is not the "fully expanded" order, but the number of [what you could think of
    // as "parenthesized"] factors that formed this tensor product type.
    static Uint32 const ORDER = AS_TENSOR_PRODUCT(TensorProductOfBasedVectorSpaces_)::ORDER;
    static bool const IS_TENSOR_I = true; // TODO: deprecate this in favor of IsATensor_i<...>

    // TODO: could put canonical as_factorX conversions here

    static Uint32 order () { return ORDER; }

    using Parent_Vector_i::dim;
    using Parent_Vector_i::as_derived;
    using Parent_Vector_i::operator[];
    Scalar operator [] (MultiIndex const &m) const { return as_derived().Derived::operator[](m); }
    Scalar operator [] (MultiIndex const &m) { return as_derived().Derived::operator[](m); }

    using Parent_Vector_i::operator();
    // the two separate head/body template arguments are necessary to disambiguate this method
    // from one that takes a single index (i.e. the index-by-vector-index one).
    template <typename AbstractIndexTypeListHeadType, typename AbstractIndexTypeListBodyTypeList>
    ExpressionTemplate_IndexedObject_t<
        Derived, // have to use Derived instead of Tensor_i, so that the return-a-reference operator[] is used
        FactorTypeList,
        typename DimIndexTypeListOf_t<FactorTypeList,
                                      TypeList_t<AbstractIndexTypeListHeadType,
                                                 AbstractIndexTypeListBodyTypeList> >::T,
        typename SummedIndexTypeList_t<typename DimIndexTypeListOf_t<FactorTypeList,
                                                                     TypeList_t<AbstractIndexTypeListHeadType,
                                                                                AbstractIndexTypeListBodyTypeList> >::T>::T,
        FORCE_CONST,
        CHECK_FOR_ALIASING> operator () (TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> const &) const
    {
        typedef TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> ArgumentAbstractIndexTypeList;
        STATIC_ASSERT(EachTypeIsAnAbstractIndex_f<ArgumentAbstractIndexTypeList>::V, EACH_TYPE_MUST_BE_ABSTRACT_INDEX);
        STATIC_ASSERT((ArgumentAbstractIndexTypeList::LENGTH == ORDER), ARGUMENT_LENGTH_MUST_EQUAL_ORDER);
        return ExpressionTemplate_IndexedObject_t<
            Derived, // have to use Derived instead of Tensor_i, so that the return-a-reference operator[] is used
            FactorTypeList,
            typename DimIndexTypeListOf_t<FactorTypeList,
                                          TypeList_t<AbstractIndexTypeListHeadType,
                                                     AbstractIndexTypeListBodyTypeList> >::T,
            typename SummedIndexTypeList_t<typename DimIndexTypeListOf_t<FactorTypeList,
                                                                         TypeList_t<AbstractIndexTypeListHeadType,
                                                                                    AbstractIndexTypeListBodyTypeList> >::T>::T,
            FORCE_CONST,
            CHECK_FOR_ALIASING>(as_derived());
    }
    template <typename AbstractIndexTypeListHeadType, typename AbstractIndexTypeListBodyTypeList>
    ExpressionTemplate_IndexedObject_t<
        Derived, // have to use Derived instead of Tensor_i, so that the return-a-reference operator[] is used
        FactorTypeList,
        typename DimIndexTypeListOf_t<FactorTypeList,
                                      TypeList_t<AbstractIndexTypeListHeadType,
                                                 AbstractIndexTypeListBodyTypeList> >::T,
        typename SummedIndexTypeList_t<typename DimIndexTypeListOf_t<FactorTypeList,
                                                                     TypeList_t<AbstractIndexTypeListHeadType,
                                                                                AbstractIndexTypeListBodyTypeList> >::T>::T,
        DONT_FORCE_CONST,
        CHECK_FOR_ALIASING> operator () (TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> const &)
    {
        typedef TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> ArgumentAbstractIndexTypeList;
        STATIC_ASSERT(EachTypeIsAnAbstractIndex_f<ArgumentAbstractIndexTypeList>::V, EACH_TYPE_MUST_BE_ABSTRACT_INDEX);
        STATIC_ASSERT((ArgumentAbstractIndexTypeList::LENGTH == ORDER), ARGUMENT_LENGTH_MUST_EQUAL_ORDER);
        return ExpressionTemplate_IndexedObject_t<
            Derived, // have to use Derived instead of Tensor_i, so that the return-a-reference operator[] is used
            FactorTypeList,
            typename DimIndexTypeListOf_t<FactorTypeList,
                                          TypeList_t<AbstractIndexTypeListHeadType,
                                                     AbstractIndexTypeListBodyTypeList> >::T,
            typename SummedIndexTypeList_t<typename DimIndexTypeListOf_t<FactorTypeList,
                                                                         TypeList_t<AbstractIndexTypeListHeadType,
                                                                                    AbstractIndexTypeListBodyTypeList> >::T>::T,
            DONT_FORCE_CONST,
            CHECK_FOR_ALIASING>(as_derived());
    }

    static bool component_is_immutable_zero (MultiIndex const &m) { return false; }
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return Scalar(1); }
    static ComponentIndex vector_index_of (MultiIndex const &m) { return ComponentIndex(m.value(), DONT_CHECK_RANGE); }

    static std::string type_as_string ()
    {
        return "Tensor_i<" + TypeStringOf_t<Derived>::eval() + ','
                           + TypeStringOf_t<Scalar>::eval() + ','
                           + TypeStringOf_t<TensorProductOfBasedVectorSpaces>::eval() + '>';
    }
};

// will print any order tensor in a nice-looking justified way.  if the order is greater
// than 1, this will print newlines, notably including the first character.
template <typename Derived, typename Scalar, typename TensorProductOfBasedVectorSpaces>
std::ostream &operator << (std::ostream &out, Tensor_i<Derived,Scalar,TensorProductOfBasedVectorSpaces> const &t)
{
    typedef Tensor_i<Derived,Scalar,TensorProductOfBasedVectorSpaces> Tensor;
    typedef typename Tensor::MultiIndex::IndexTypeList IndexTypeList;
    print_multiindexable(out, t, IndexTypeList());
    return out;
}

} // end of namespace Tenh

#endif // TENH_INTERFACE_TENSOR_HPP_
