// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/innerproduct.hpp by Victor Dods, created 2013/09/08
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_INNERPRODUCT_HPP_
#define TENH_IMPLEMENTATION_INNERPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/implementation/vee.hpp"

namespace Tenh {

template <typename BasedVectorSpace_, typename InnerProductId_, typename Scalar_> struct InnerProduct_f;

// corresponds to the identity matrix
struct StandardInnerProduct { static std::string type_as_string () { return "StandardInnerProduct"; } };

// template specialization for inner product on Euclidean space
template <typename VectorSpace_, typename Scalar_>
struct InnerProduct_f<BasedVectorSpace_c<VectorSpace_,StandardBasis>,StandardInnerProduct,Scalar_>
{
private:
    typedef BasedVectorSpace_c<VectorSpace_,StandardBasis> BasedVectorSpace;
    typedef typename DualOf_f<BasedVectorSpace>::T DualOfBasedVectorSpace;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOfBasedVectorSpace> SymmetricPower;
    // for using bundle_index_map
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UsePreallocatedArray> BootstrappingImplementation;
    typedef ComponentIndex_t<AS_VECTOR_SPACE(SymmetricPower)::DIMENSION> ComponentIndex;
    static Scalar_ evaluator (ComponentIndex const &i)
    {
        typedef typename BootstrappingImplementation::MultiIndex::IndexTypeList IndexTypeList;
        typedef MultiIndex_t<IndexTypeList> MultiIndex;
        MultiIndex m(BootstrappingImplementation::template bundle_index_map<IndexTypeList,ComponentIndex>(i));
        return m.template el<0>() == m.template el<1>() ? Scalar_(1) : Scalar_(0);
    }
    typedef ComponentGenerator_t<Scalar_,AS_VECTOR_SPACE(SymmetricPower)::DIMENSION,evaluator,StandardInnerProduct> ComponentGenerator;
public:
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_INNERPRODUCT_HPP_
