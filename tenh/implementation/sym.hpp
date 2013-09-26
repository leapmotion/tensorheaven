// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/sym.hpp by Victor Dods, created 2013/09/26
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_SYM_HPP_
#define TENH_IMPLEMENTATION_SYM_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/symmetricpower.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/vee.hpp"

namespace Tenh {

namespace ComponentGeneratorEvaluator {

// this would be better defined as a private static method in the specialization of InnerProduct_f below,
// but g++ complains about that being invalid in a const expression (whereas clang++ is fine with it)
template <Uint32 ORDER_, typename Factor_, Uint32 COMPONENT_COUNT_, typename Scalar_>
Scalar_ sym (ComponentIndex_t<COMPONENT_COUNT_> const &i)
{
    typedef ComponentIndex_t<COMPONENT_COUNT_> ComponentIndex;
    typedef SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_> SymmetricPower;
    typedef typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T TensorPower;
    typedef TypeList_t<SymmetricPower,
            TypeList_t<typename DualOf_f<TensorPower>::T> > FactorTypeList;
    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList> Projection;
    STATIC_ASSERT(DimensionOf_f<Projection>::V == COMPONENT_COUNT_, DIMENSIONS_MUST_MATCH);

    // for using bundle_index_map -- the use of UsePreallocatedArray is somewhat arbitrary,
    // since only the static method bundle_index_map will be used.
    typedef ImplementationOf_t<Projection,Scalar_,UsePreallocatedArray> BootstrappingImplementation;
    typedef ImplementationOf_t<TensorPower,Scalar_,UsePreallocatedArray> Factor1BootstrappingImplementation;
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UsePreallocatedArray> Factor0BootstrappingImplementation;

    typedef typename BootstrappingImplementation::MultiIndex MultiIndex;
    typedef typename MultiIndex::IndexTypeList IndexTypeList;
    STATIC_ASSERT(IndexTypeList::LENGTH == 2, LENGTH_MUST_BE_EXACTLY_2);
    MultiIndex m(BootstrappingImplementation::template bundle_index_map<IndexTypeList,ComponentIndex>(i));

    typedef typename Factor1BootstrappingImplementation::MultiIndex::IndexTypeList Factor1IndexTypeList;
    STATIC_ASSERT(Factor1IndexTypeList::LENGTH == ORDER_, LENGTHS_MUST_BE_EQUAL);
    typedef MultiIndex_t<Factor1IndexTypeList> TensorPowerMultiIndex;
    typedef typename Factor1BootstrappingImplementation::ComponentIndex TensorPowerComponentIndex;
    TensorPowerMultiIndex m1(Factor1BootstrappingImplementation::template bundle_index_map<Factor1IndexTypeList,TensorPowerComponentIndex>(m.template el<1>()));

    // vector_index_of sorts the multiindex and returns the vector index -- if this
    // matches the symmetric vector index, then return 1; otherwise 0.
    return (m.template el<0>() == Factor0BootstrappingImplementation::vector_index_of(m1)) ? Scalar_(1) : Scalar_(0);
}

} // end of namespace ComponentGeneratorEvaluator

template <Uint32 ORDER_, typename Factor_>
struct SymId_t
{
    static std::string type_as_string () { return "Sym<" + AS_STRING(ORDER_) + ',' + TypeStringOf_t<Factor_>::eval() + '>'; }
};

// template specialization for standard inner product on a based vector space having orthonormal basis
template <Uint32 ORDER_, typename Factor_, typename Scalar_>
struct Sym_f
{
private:
    typedef SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_> SymmetricPower;
    typedef typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T TensorPower;
    typedef TypeList_t<SymmetricPower,
            TypeList_t<typename DualOf_f<TensorPower>::T> > FactorTypeList;
    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList> Projection;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<Projection>::V,
                                 ComponentGeneratorEvaluator::sym<ORDER_,Factor_,DimensionOf_f<Projection>::V,Scalar_>,
                                 SymId_t<ORDER_,Factor_> > ComponentGenerator;
public:
    typedef ImplementationOf_t<Projection,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_SYM_HPP_
