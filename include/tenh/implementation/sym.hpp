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

// NOTE: with embed/coembed, this may be no longer useful.

// this would be better defined as a private static method in Sym_f below, but g++
// complains about that being invalid in a const expression (whereas clang++ is fine with it)
template <Uint32 ORDER_, typename Factor_, Uint32 COMPONENT_COUNT_, typename Scalar_>
Scalar_ sym (ComponentIndex_t<COMPONENT_COUNT_> const &i)
{
    typedef ComponentIndex_t<COMPONENT_COUNT_> ComponentIndex;
    typedef SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_> SymmetricPower;
    typedef typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T TensorPower;
    typedef Typle_t<SymmetricPower,typename DualOf_f<TensorPower>::T> FactorTyple;
    typedef TensorProductOfBasedVectorSpaces_c<FactorTyple> Projection;
    static_assert(DimensionOf_f<Projection>::V == COMPONENT_COUNT_, "dimensions must match");

    // for using bundle_index_map -- the use of UsePreallocatedArray_t<ComponentsAreConst::FALSE> is somewhat arbitrary,
    // since only the static method bundle_index_map will be used.
    typedef ImplementationOf_t<Projection,Scalar_,UsePreallocatedArray_t<ComponentsAreConst::FALSE>> BootstrappingImplementation;
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UsePreallocatedArray_t<ComponentsAreConst::FALSE>> Factor0BootstrappingImplementation;
    typedef ImplementationOf_t<TensorPower,Scalar_,UsePreallocatedArray_t<ComponentsAreConst::FALSE>> Factor1BootstrappingImplementation;

    typedef typename BootstrappingImplementation::MultiIndex MultiIndex;
    typedef typename MultiIndex::IndexTyple IndexTyple;
    static_assert(Length_f<IndexTyple>::V == 2, "must have exactly 2 indices");
    MultiIndex m(BootstrappingImplementation::template bundle_index_map<IndexTyple,ComponentIndex>(i));

    typedef typename Factor1BootstrappingImplementation::MultiIndex::IndexTyple Factor1IndexTyple;
    static_assert(Length_f<Factor1IndexTyple>::V == ORDER_, "length of Factor1IndexTyple must be exactly ORDER_");
    typedef MultiIndex_t<Factor1IndexTyple> TensorPowerMultiIndex;
    typedef typename Factor1BootstrappingImplementation::ComponentIndex TensorPowerComponentIndex;
    TensorPowerMultiIndex m1(Factor1BootstrappingImplementation::template bundle_index_map<Factor1IndexTyple,TensorPowerComponentIndex>(m.template el<1>()));

    // vector_index_of sorts the multiindex and returns the vector index -- if this
    // matches the symmetric vector index, then return 1; otherwise 0.
    return (m.template el<0>() == Factor0BootstrappingImplementation::vector_index_of(m1)) ? Scalar_(1) : Scalar_(0);
}

} // end of namespace ComponentGeneratorEvaluator

template <Uint32 ORDER_, typename Factor_>
struct SymId_t
{
    static std::string type_as_string (bool verbose) { return "Sym<" + FORMAT(ORDER_) + ',' + type_string_of<Factor_>() + '>'; }
};

// template specialization for standard inner product on a based vector space having orthonormal basis
template <Uint32 ORDER_, typename Factor_, typename Scalar_>
struct Sym_f
{
private:
    typedef SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_> SymmetricPower;
    typedef typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T TensorPower;
    typedef Typle_t<SymmetricPower,typename DualOf_f<TensorPower>::T> FactorTyple;
    typedef TensorProductOfBasedVectorSpaces_c<FactorTyple> Projection;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<Projection>::V,
                                 ComponentGeneratorEvaluator::sym<ORDER_,Factor_,DimensionOf_f<Projection>::V,Scalar_>,
                                 SymId_t<ORDER_,Factor_>> ComponentGenerator;
    Sym_f();
public:
    typedef ImplementationOf_t<Projection,Scalar_,UseProceduralArray_t<ComponentGenerator>> T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_SYM_HPP_
