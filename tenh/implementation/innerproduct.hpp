// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/innerproduct.hpp by Victor Dods, created 2013/09/08
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_INNERPRODUCT_HPP_
#define TENH_IMPLEMENTATION_INNERPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/implementation/vee.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// forward declaration for the InnerProduct_f metafunction (should return
// an ImplementationOf_t type that uses ImmutableArray_t)
// ///////////////////////////////////////////////////////////////////////////

template <typename BasedVectorSpace_, typename InnerProductId_, typename Scalar_> struct InnerProduct_f;

// ///////////////////////////////////////////////////////////////////////////
// standard inner product
// ///////////////////////////////////////////////////////////////////////////

// corresponds to the identity matrix
struct StandardInnerProduct { static std::string type_as_string () { return "StandardInnerProduct"; } };

namespace ComponentGeneratorEvaluator {

// this would be better defined as a private static method in the specialization of InnerProduct_f below,
// but g++ complains about that being invalid in a const expression (whereas clang++ is fine with it)
template <typename EmbeddableInTensorProductOfBasedVectorSpaces_, typename Scalar_>
Scalar_ standard_inner_product (ComponentIndex_t<DimensionOf_f<EmbeddableInTensorProductOfBasedVectorSpaces_>::V> const &i)
{
    typedef ComponentIndex_t<DimensionOf_f<EmbeddableInTensorProductOfBasedVectorSpaces_>::V> ComponentIndex;
    // for using bundle_index_map -- the use of UsePreallocatedArray is somewhat arbitrary,
    // since only the static method bundle_index_map will be used.
    typedef ImplementationOf_t<EmbeddableInTensorProductOfBasedVectorSpaces_,Scalar_,UsePreallocatedArray> BootstrappingImplementation;
    typedef typename BootstrappingImplementation::MultiIndex::IndexTypeList IndexTypeList;
    typedef MultiIndex_t<IndexTypeList> MultiIndex;
    MultiIndex m(BootstrappingImplementation::template bundle_index_map<IndexTypeList,ComponentIndex>(i));
    return m.template el<0>() == m.template el<1>() ? Scalar_(1) : Scalar_(0);
}

} // end of namespace ComponentGeneratorEvaluator

// template specialization for inner product on Euclidean space
template <typename VectorSpace_, typename Scalar_>
struct InnerProduct_f<BasedVectorSpace_c<VectorSpace_,StandardBasis>,StandardInnerProduct,Scalar_>
{
private:
    typedef BasedVectorSpace_c<VectorSpace_,StandardBasis> BasedVectorSpace;
    typedef typename DualOf_f<BasedVectorSpace>::T DualOfBasedVectorSpace;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOfBasedVectorSpace> SymmetricPower;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<SymmetricPower>::V,
                                 ComponentGeneratorEvaluator::standard_inner_product<SymmetricPower,Scalar_>,
                                 StandardInnerProduct> ComponentGenerator;
public:
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

// ///////////////////////////////////////////////////////////////////////////
// induced inner product on TensorProductOfBasedVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////

namespace ComponentGeneratorEvaluator {

// the fact that this is a global function (instead of a static method inside
// the below InnerProduct_f specialization) is a workaround for GCC not accepting
// a static method as a template param (which works in clang).
template <typename SymmetricPower_, typename FactorTypeList_, typename InnerProductIdTypeList_, typename Scalar_>
Scalar_ inner_product_induced_on_tensor_product (ComponentIndex_t<DimensionOf_f<SymmetricPower_>::V> const &i)
{
    typedef ComponentIndex_t<DimensionOf_f<SymmetricPower_>::V> ComponentIndex;
    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList_> TensorProductOfBasedVectorSpaces;
    typedef typename InnerProduct_f<typename FactorTypeList_::HeadType,
                                    typename InnerProductIdTypeList_::HeadType,
                                    Scalar_>::T HeadInnerProduct;
    HeadInnerProduct head_inner_product;
    typedef typename InnerProduct_f<TensorProductOfBasedVectorSpaces_c<typename FactorTypeList_::BodyTypeList>,
                                    TensorProduct_c<typename InnerProductIdTypeList_::BodyTypeList>,
                                    Scalar_>::T BodyInnerProduct;
    BodyInnerProduct body_inner_product;
    AbstractIndex_c<'a'> a;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'B'> B;
    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'Q'> Q;
    AbstractIndex_c<'r'> r;
    AbstractIndex_c<'s'> s;
    AbstractIndex_c<'T'> T;
    return (head_inner_product(a).split(a,j|k) * body_inner_product(B).split(B,P|Q))
            .bundle(j|P,TensorProductOfBasedVectorSpaces(),r)
            .bundle(k|Q,TensorProductOfBasedVectorSpaces(),s)
            .bundle(r|s,SymmetricPower_(),T)[MultiIndex_t<TypeList_t<ComponentIndex> >(i)];
}

} // end of namespace ComponentGeneratorEvaluator

template <typename FactorTypeList_, typename InnerProductIdTypeList_, typename Scalar_>
struct InnerProduct_f<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,TensorProduct_c<InnerProductIdTypeList_>,Scalar_>
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(FactorTypeList_::LENGTH == InnerProductIdTypeList_::LENGTH, LENGTHS_MUST_BE_EQUAL),
        STATIC_ASSERT_IN_ENUM((FactorTypeList_::LENGTH > 0), ORDER_MUST_BE_POSITIVE) // TODO: allow ORDER_ == 0
    };
    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList_> TensorProductOfBasedVectorSpaces;
    typedef typename DualOf_f<TensorProductOfBasedVectorSpaces>::T DualOfTensorProductOfBasedVectorSpaces;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOfTensorProductOfBasedVectorSpaces> SymmetricPower;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<SymmetricPower>::V,
                                 ComponentGeneratorEvaluator::inner_product_induced_on_tensor_product<SymmetricPower,
                                                                                                      FactorTypeList_,
                                                                                                      InnerProductIdTypeList_,
                                                                                                      Scalar_>,
                                 TensorProduct_c<InnerProductIdTypeList_> > ComponentGenerator;
public:
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

// base case for order-1 tensor products.

namespace ComponentGeneratorEvaluator {

template <typename EmbeddableInTensorProductOfBasedVectorSpaces_, typename BasedVectorSpace_, typename InnerProductId_, typename Scalar_>
Scalar_ inner_product_induced_on_1st_tensor_power (ComponentIndex_t<DimensionOf_f<EmbeddableInTensorProductOfBasedVectorSpaces_>::V> const &i)
{
    typedef SymmetricPowerOfBasedVectorSpace_c<2,BasedVectorSpace_> SymmetricPower;
    STATIC_ASSERT(DimensionOf_f<SymmetricPower>::V == DimensionOf_f<EmbeddableInTensorProductOfBasedVectorSpaces_>::V,
                  DIMENSIONS_MUST_MATCH);
    typedef typename InnerProduct_f<BasedVectorSpace_,InnerProductId_,Scalar_>::T HeadInnerProduct;
    HeadInnerProduct head_inner_product;
    return head_inner_product[i];
}

} // end of namespace ComponentGeneratorEvaluator

// template specialization for ORDER_ == 1 (which is really just the original inner product)
// TODO: do specialization for ORDER_ == 0 (but this would depend on the basis being Standard
// (i.e. the 0th tensor power is the field itself and 1 is the basis vector)).
template <typename BasedVectorSpace_, typename InnerProductId_, typename Scalar_>
struct InnerProduct_f<TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedVectorSpace_> >,TensorProduct_c<TypeList_t<InnerProductId_> >,Scalar_>
{
private:
    typedef typename TensorPowerOfBasedVectorSpace_f<1,BasedVectorSpace_>::T TensorProductOfBasedVectorSpaces;
    typedef typename DualOf_f<TensorProductOfBasedVectorSpaces>::T DualOfTensorProductOfBasedVectorSpaces;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOfTensorProductOfBasedVectorSpaces> SymmetricPower;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<SymmetricPower>::V,
                                 ComponentGeneratorEvaluator::inner_product_induced_on_1st_tensor_power<SymmetricPower,
                                                                                                        BasedVectorSpace_,
                                                                                                        InnerProductId_,
                                                                                                        Scalar_>,
                                 typename TensorPower_f<1,InnerProductId_>::T> ComponentGenerator;
public:
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_INNERPRODUCT_HPP_
