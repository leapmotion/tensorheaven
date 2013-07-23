// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/tensorproduct.hpp by Victor Dods, created 2013/07/22
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_TENSORPRODUCT_HPP_
#define TENH_CONCEPTUAL_TENSORPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/dual.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/meta/typelist.hpp"

namespace Tenh {

/*
TensorProductOf_c<list of vector spaces> -- instantiates TensorProductOfVectorSpaces_c, is a VectorSpace_c
TensorProductOf_c<list of bases> -- instantiates TensorProductOfBases_c, is a Basis_c
TensorProductOf_c<list of based vector spaces> -- inherits TensorProductOfBasedVectorSpaces_c, is a BasedTensorProduct_c
BasedTensorProduct_c<TensorProductOfVectorSpaces,Basis> -- 
*/

template <typename TypeList>
struct DualsOfTypeList_t
{
    typedef TypeList_t<typename DualOf_c<typename TypeList::HeadType>::T,
                       typename DualsOfTypeList_t<typename TypeList::BodyTypeList>::T> T;
};

template <>
struct DualsOfTypeList_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};

// generic tensor product of formal symbols (e.g. identifiers, builtin C++ types, etc)
template <typename FactorTypeList_>
struct TensorProduct_c
{
    typedef FactorTypeList_ FactorTypeList;
    static Uint32 const ORDER = FactorTypeList::LENGTH;
    typedef typename DualOf_c<TensorProduct_c>::T Dual;
    static bool const IS_TENSOR_PRODUCT = true;

    static std::string type_as_string ()
    {
        return "TensorProduct_c<" + TypeStringOf_t<FactorTypeList>::eval() + '>';
    }
};

template <typename FactorTypeList>
struct DualOf_c<TensorProduct_c<FactorTypeList> >
{
    typedef TensorProduct_c<typename DualsOfTypeList_t<FactorTypeList>::T> T;
};

/*
template <typename VectorSpaceTypeList>
struct ProductOfDimensions_t
{
    enum { STATIC_ASSERT_IN_ENUM(VectorSpaceTypeList::HeadType::IS_VECTOR_SPACE, MUST_BE_VECTOR_SPACE) };

    static Uint32 const V = VectorSpaceTypeList::HeadType::DIM * 
                            ProductOfDimensions_t<typename VectorSpaceTypeList::BodyTypeList>::V;
};

template <>
struct ProductOfDimensions_t<EmptyTypeList>
{
    static Uint32 const V = 1; // a 0-tensor is the scalar field by convention
};

template <typename TypeList>
struct IdsOfTypeList_t
{
    typedef TypeList_t<typename TypeList::HeadType::Id,
                       typename IdsOfTypeList_t<typename TypeList::BodyTypeList>::T> T;
};

template <>
struct IdsOfTypeList_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};

template <>
struct DualOfVectorSpaceTypeList_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};

template <typename VectorSpaceTypeList_>
struct TensorProductOfVectorSpaces_c
    : 
    public TensorProduct_c<VectorSpaceTypeList_>,
    public VectorSpace_c<typename VectorSpaceTypeList_::HeadType::Field,
                         ProductOfDimensions_t<VectorSpaceTypeList_>::V,
                         TensorProduct_c<typename IdsOfTypeList_t<VectorSpaceTypeList_>::T> >
{
    typedef TensorProduct_c<VectorSpaceTypeList_> Parent_TensorProduct;
    typedef VectorSpace_c<typename VectorSpaceTypeList_::HeadType::Field,
                          ProductOfDimensions_t<VectorSpaceTypeList_>::V,
                          TensorProduct_c<typename IdsOfTypeList_t<VectorSpaceTypeList_>::T> > Parent_VectorSpace;

    // TODO: static assert that all factor types are vector spaces having the same base field
    //enum { STATIC_ASSERT_IN_ENUM(Field_::IS_FIELD, MUST_BE_FIELD) };

    typedef typename Parent_TensorProduct::FactorTypeList FactorTypeList;
    using Parent_TensorProduct::ORDER;
    using Parent_TensorProduct::IS_TENSOR_PRODUCT;
    typedef typename Parent_VectorSpace::Field Field;
    using Parent_VectorSpace::DIM;
    typedef typename Parent_VectorSpace::Id Id;
    //typedef typename Parent_VectorSpace::Dual Dual; // TODO: real dual space
    using Parent_VectorSpace::IS_VECTOR_SPACE;
    static bool const IS_TENSOR_PRODUCT_OF_VECTOR_SPACES = true;

    static std::string type_as_string ()
    {
        return "TensorProductOfVectorSpaces_c<" + TypeStringOf_t<FactorTypeList>::eval() + '>';
    }
};

template <typename VectorSpaceTypeList>
struct DualOf_c<TensorProductOfVectorSpaces_c<VectorSpaceTypeList> >
{
    typedef Basis_c<typename DualOf_c<Id>::T> T; TODO real code
};
*/
/*
template <typename FactorTypeList_>
struct TensorProductOfBases_c : public Basis_c<
{
    // TODO: static assert that all factor types are bases
    //enum { STATIC_ASSERT_IN_ENUM(Field_::IS_FIELD, MUST_BE_FIELD) };

    typedef FactorTypeList_ FactorTypeList;
    static Uint32 const ORDER = FactorTypeList::LENGTH;
    static bool const IS_TENSOR_PRODUCT_OF_BASES = true;

    static std::string type_as_string ()
    {
        return "TensorProductOfBases_c<" + TypeStringOf_t<FactorTypeList>::eval() + '>';
    }
};

template <typename TensorProduct_, typename BasisOfTensorProduct_>
struct BasedTensorProduct_c
{
    enum
    {
        STATIC_ASSERT_IN_ENUM(TensorProduct_::IS_TENSOR_PRODUCT, MUST_BE_TENSOR_PRODUCT),
        STATIC_ASSERT_IN_ENUM(TensorProduct_::IS_BASIS, MUST_BE_BASIS)
    };
};

template <typename FactorTypeList_>
struct TensorProductOfBasedVectorSpaces_c
{
    // TODO: static assert that all factor types are based vector spaces
    //enum { STATIC_ASSERT_IN_ENUM(Field_::IS_FIELD, MUST_BE_FIELD) };

    typedef FactorTypeList_ FactorTypeList;
    static Uint32 const ORDER = FactorTypeList::LENGTH;
    static bool const IS_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES = true;

    static std::string type_as_string ()
    {
        return "TensorProductOfBasedVectorSpaces_c<" + TypeStringOf_t<FactorTypeList>::eval() + '>';
    }
};

// NOTES: the most "basic" type of tensor product with basis is a pair (TensorProduct, BasisOfSaidTensorProduct).
// tensor products of based vector spaces inherit this type, where TensorProduct is the tensor product of the
// vector spaces, and BasisOfSaidTensorProduct is given by the tensor product of the bases of the vector spaces

template <typename FactorTypeList>
struct TensorProductOf_c
{
    enum { STATIC_ASSERT_IN_ENUM((FactorTypeList::LENGTH > 0), FACTOR_TYPE_LIST_MUST_BE_NONEMPTY)}

    typedef typename Meta::If<(Meta::TypesAreEqual<typename FactorTypeList::Head,)
};
*/
} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_TENSORPRODUCT_HPP_
