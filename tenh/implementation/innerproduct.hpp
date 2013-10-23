// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/innerproduct.hpp by Victor Dods, created 2013/09/08
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_INNERPRODUCT_HPP_
#define TENH_IMPLEMENTATION_INNERPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/implementation/tensorproduct.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// forward declaration for the InnerProduct_f metafunction (should return
// an ImplementationOf_t type that uses ImmutableArray_t)
// ///////////////////////////////////////////////////////////////////////////

template <typename BasedVectorSpace_, typename InnerProductId_, typename Scalar_> struct InnerProduct_f;

// ///////////////////////////////////////////////////////////////////////////
// standard inner product on a based vector space having an orthonormal basis
// ///////////////////////////////////////////////////////////////////////////

// corresponds to the identity matrix
struct StandardInnerProduct { static std::string type_as_string () { return "StandardInnerProduct"; } };

// template specialization for standard inner product on a based vector space having orthonormal basis
template <typename VectorSpace_, typename OrthonormalBasisId_, typename Scalar_>
struct InnerProduct_f<BasedVectorSpace_c<VectorSpace_,OrthonormalBasis_c<OrthonormalBasisId_> >,StandardInnerProduct,Scalar_>
{
private:
    typedef BasedVectorSpace_c<VectorSpace_,OrthonormalBasis_c<OrthonormalBasisId_> > BasedVectorSpace;
    typedef typename DualOf_f<BasedVectorSpace>::T DualOfBasedVectorSpace;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedVectorSpace,DualOfBasedVectorSpace> Diagonal2Tensor;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<Diagonal2Tensor>::V,
                                 constant_component_generator_evaluator<Scalar_,DimensionOf_f<Diagonal2Tensor>::V,1>,
                                 StandardInnerProduct> ComponentGenerator;
    InnerProduct_f();
public:
    typedef ImplementationOf_t<Diagonal2Tensor,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

// ///////////////////////////////////////////////////////////////////////////
// induced inner product on TensorProductOfBasedVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////

// meta-function helper for induced inner products

template <typename FactorTypeList_, typename InnerProductIdTypeList_, typename Scalar_>
struct InnerProductOfEachInTypeList_f
{
private:
    enum { STATIC_ASSERT_IN_ENUM(FactorTypeList_::LENGTH == InnerProductIdTypeList_::LENGTH, LENGTHS_MUST_BE_EQUAL) };
    InnerProductOfEachInTypeList_f();
public:
    typedef TypeList_t<typename InnerProduct_f<typename FactorTypeList_::HeadType,
                                               typename InnerProductIdTypeList_::HeadType,
                                               Scalar_>::T,
                       typename InnerProductOfEachInTypeList_f<typename FactorTypeList_::BodyTypeList,
                                                               typename InnerProductIdTypeList_::BodyTypeList,
                                                               Scalar_>::T> T;
};

template <typename Scalar_>
struct InnerProductOfEachInTypeList_f<EmptyTypeList,EmptyTypeList,Scalar_>
{
    typedef EmptyTypeList T;
private:
    InnerProductOfEachInTypeList_f();
};

// induced inner product on TensorProductOfBasedVectorSpaces_c
template <typename FactorTypeList_, typename InnerProductIdTypeList_, typename Scalar_>
struct InnerProduct_f<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,TensorProduct_c<InnerProductIdTypeList_>,Scalar_>
{
private:
    typedef typename InnerProductOfEachInTypeList_f<FactorTypeList_,InnerProductIdTypeList_,Scalar_>::T InnerProductTypeList;
    InnerProduct_f();
public:
    typedef typename TensorProductOfImmutable2Tensors_f<InnerProductTypeList>::T T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_INNERPRODUCT_HPP_
