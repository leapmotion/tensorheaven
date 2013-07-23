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
    typedef typename DualOf_c<TensorProduct_c>::T Dual; // relies on the template specialization below

    static std::string type_as_string ()
    {
        return "TensorProduct_c<" + TypeStringOf_t<FactorTypeList>::eval() + '>';
    }
};

template <typename T> struct IsATensorProduct_t { static bool const V = false; };
template <typename FactorTypeList> struct IsATensorProduct_t<TensorProduct_c<FactorTypeList> > { static bool const V = true; };

template <typename FactorTypeList>
struct DualOf_c<TensorProduct_c<FactorTypeList> >
{
    typedef TensorProduct_c<typename DualsOfTypeList_t<FactorTypeList>::T> T;
};

template <typename FactorTypeList>
struct AllFactorsAreVectorSpaces_t
{
    static bool const V = IsAVectorSpace_t<typename FactorTypeList::HeadType>::V && 
                          AllFactorsAreVectorSpaces_t<typename FactorTypeList::BodyTypeList>::V;
};

template <>
struct AllFactorsAreVectorSpaces_t<EmptyTypeList>
{
    static bool const V = true;
};

template <typename FactorTypeList>
struct AllFactorsAreBases_t
{
    static bool const V = IsABasis_t<typename FactorTypeList::HeadType>::V && 
                          AllFactorsAreBases_t<typename FactorTypeList::BodyTypeList>::V;
};

template <>
struct AllFactorsAreBases_t<EmptyTypeList>
{
    static bool const V = true;
};

template <typename FactorTypeList>
struct AllFactorsHaveTheSameField_t
{
    typedef typename FactorTypeList::HeadType HeadType;
    typedef typename FactorTypeList::BodyTypeList BodyTypeList;
    static bool const V = Lvd::Meta::TypesAreEqual<typename HeadType::Field,typename BodyTypeList::HeadType::Field>::v &&
                          AllFactorsHaveTheSameField_t<BodyTypeList>::V;
};

template <typename HeadType>
struct AllFactorsHaveTheSameField_t<TypeList_t<HeadType> >
{
    static bool const V = true;
};

template <>
struct AllFactorsHaveTheSameField_t<EmptyTypeList>
{
    static bool const V = true;
};

template <typename FactorTypeList>
struct ProductOfDimensions_t
{
    enum { STATIC_ASSERT_IN_ENUM(IsAVectorSpace_t<typename FactorTypeList::HeadType>::V, MUST_BE_VECTOR_SPACE) };

    static Uint32 const V = FactorTypeList::HeadType::DIM * 
                            ProductOfDimensions_t<typename FactorTypeList::BodyTypeList>::V;
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

// FactorTypeList_ must be a TypeList_t of VectorSpace_c types
template <typename FactorTypeList_>
struct TensorProductOfVectorSpaces_c
    : 
    public TensorProduct_c<FactorTypeList_>,
    public VectorSpace_c<typename FactorTypeList_::HeadType::Field,
                         ProductOfDimensions_t<FactorTypeList_>::V,
                         TensorProduct_c<typename IdsOfTypeList_t<FactorTypeList_>::T> >
{
    typedef TensorProduct_c<FactorTypeList_> Parent_TensorProduct;
    typedef VectorSpace_c<typename FactorTypeList_::HeadType::Field,
                          ProductOfDimensions_t<FactorTypeList_>::V,
                          TensorProduct_c<typename IdsOfTypeList_t<FactorTypeList_>::T> > Parent_VectorSpace;

    enum
    {
        STATIC_ASSERT_IN_ENUM(AllFactorsAreVectorSpaces_t<FactorTypeList_>::V, ALL_FACTORS_MUST_BE_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(AllFactorsHaveTheSameField_t<FactorTypeList_>::V, ALL_FACTORS_MUST_HAVE_SAME_FIELD)
    };

    typedef typename Parent_TensorProduct::FactorTypeList FactorTypeList;
    using Parent_TensorProduct::ORDER;
    typedef typename Parent_VectorSpace::Field Field;
    using Parent_VectorSpace::DIM;
    typedef typename Parent_VectorSpace::Id Id;
    typedef typename DualOf_c<TensorProductOfVectorSpaces_c>::T Dual; // relies on the template specialization below

    static std::string type_as_string ()
    {
        return "TensorProductOfVectorSpaces_c<" + TypeStringOf_t<FactorTypeList>::eval() + '>';
    }
};

template <typename T> struct IsATensorProductOfVectorSpaces_t { static bool const V = false; };
template <typename FactorTypeList> struct IsATensorProductOfVectorSpaces_t<TensorProductOfVectorSpaces_c<FactorTypeList> > { static bool const V = true; };

template <typename FactorTypeList>
struct DualOf_c<TensorProductOfVectorSpaces_c<FactorTypeList> >
{
    typedef TensorProductOfVectorSpaces_c<typename DualsOfTypeList_t<FactorTypeList>::T> T;
};




// FactorTypeList_ must be a TypeList_t of Basis_c types
template <typename FactorTypeList_>
struct TensorProductOfBases_c
    : 
    public TensorProduct_c<FactorTypeList_>,
    public Basis_c<TensorProduct_c<typename IdsOfTypeList_t<FactorTypeList_>::T> >
{
    typedef TensorProduct_c<FactorTypeList_> Parent_TensorProduct;
    typedef Basis_c<TensorProduct_c<typename IdsOfTypeList_t<FactorTypeList_>::T> > Parent_Basis;

    enum
    {
        STATIC_ASSERT_IN_ENUM(AllFactorsAreBases_t<FactorTypeList_>::V, ALL_FACTORS_MUST_BE_BASES)
    };

    typedef typename Parent_TensorProduct::FactorTypeList FactorTypeList;
    using Parent_TensorProduct::ORDER;
    typedef typename Parent_Basis::Id Id;
    typedef typename DualOf_c<TensorProductOfBases_c>::T Dual; // relies on the template specialization below

    static std::string type_as_string ()
    {
        return "TensorProductOfBases_c<" + TypeStringOf_t<FactorTypeList>::eval() + '>';
    }
};

template <typename T> struct IsATensorProductOfBases_t { static bool const V = false; };
template <typename FactorTypeList> struct IsATensorProductOfBases_t<TensorProductOfBases_c<FactorTypeList> > { static bool const V = true; };

template <typename FactorTypeList>
struct DualOf_c<TensorProductOfBases_c<FactorTypeList> >
{
    typedef TensorProductOfBases_c<typename DualsOfTypeList_t<FactorTypeList>::T> T;
};

/*
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
