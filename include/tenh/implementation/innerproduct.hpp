// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/innerproduct.hpp by Victor Dods, created 2013/09/08
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_INNERPRODUCT_HPP_
#define TENH_IMPLEMENTATION_INNERPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/implementation/directsum.hpp"
#include "tenh/implementation/scalar2tensor.hpp"
#include "tenh/implementation/tensorproduct.hpp"
#include "tenh/interface/vector.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// forward declaration for the InnerProduct_f metafunction (should return
// an ImplementationOf_t type that uses ProceduralArray_t)
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
    typedef Scalar2TensorProductOfBasedVectorSpaces_c<DualOfBasedVectorSpace,DualOfBasedVectorSpace> Scalar2Tensor;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<Scalar2Tensor>::V,
                                 constant_component_generator_evaluator<Scalar_,DimensionOf_f<Scalar2Tensor>::V,1>,
                                 StandardInnerProduct> ComponentGenerator;
    InnerProduct_f();
public:
    typedef ImplementationOf_t<Scalar2Tensor,Scalar_,UseProceduralArray_t<ComponentGenerator> > T;
};

// ///////////////////////////////////////////////////////////////////////////
// induced inner product on TensorProductOfBasedVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////

// meta-function helper for induced inner products

template <typename FactorTyple_, typename InnerProductIdTyple_, typename Scalar_>
struct InnerProductOfEachInTyple_f
{
private:
    enum { STATIC_ASSERT_IN_ENUM(Length_f<FactorTyple_>::V == Length_f<InnerProductIdTyple_>::V, LENGTHS_MUST_BE_EQUAL) };
    InnerProductOfEachInTyple_f();
    typedef typename InnerProduct_f<typename Head_f<FactorTyple_>::T,
                                    typename Head_f<InnerProductIdTyple_>::T,
                                    Scalar_>::T HeadInnerProduct;
    typedef typename InnerProductOfEachInTyple_f<typename BodyTyple_f<FactorTyple_>::T,
                                                 typename BodyTyple_f<InnerProductIdTyple_>::T,
                                                 Scalar_>::T BodyInnerProductTyple;
public:
    typedef typename HeadBodyTyple_f<HeadInnerProduct,BodyInnerProductTyple>::T T;
};

template <typename Scalar_>
struct InnerProductOfEachInTyple_f<Typle_t<>,Typle_t<>,Scalar_>
{
    typedef Typle_t<> T;
private:
    InnerProductOfEachInTyple_f();
};

// induced inner product on TensorProductOfBasedVectorSpaces_c
template <typename FactorTyple_, typename InnerProductIdTyple_, typename Scalar_>
struct InnerProduct_f<TensorProductOfBasedVectorSpaces_c<FactorTyple_>,TensorProduct_c<InnerProductIdTyple_>,Scalar_>
{
private:
    typedef typename InnerProductOfEachInTyple_f<FactorTyple_,InnerProductIdTyple_,Scalar_>::T InnerProductTyple;
    InnerProduct_f();
public:
    typedef typename TensorProductOfProcedural2Tensors_f<InnerProductTyple>::T T;
};

// induced inner product on DirectSumOfBasedVectorSpaces_c
template <typename FactorTyple_, typename InnerProductIdTyple_, typename Scalar_>
struct InnerProduct_f<DirectSumOfBasedVectorSpaces_c<FactorTyple_>,DirectSum_c<InnerProductIdTyple_>,Scalar_>
{
private:
    typedef typename InnerProductOfEachInTyple_f<FactorTyple_,InnerProductIdTyple_,Scalar_>::T InnerProductTyple;
    InnerProduct_f();
public:
    typedef typename DirectSumOfProcedural2Tensors_f<InnerProductTyple>::T T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_INNERPRODUCT_HPP_
