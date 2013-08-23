// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/vee.hpp by Ted Nitz, created 2013/08/19
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_VEE_HPP_
#define TENH_IMPLEMENTATION_VEE_HPP_

#include "tenh/core.hpp"

#include "tenh/array.hpp"
#include "tenh/mathutil.hpp"
#include "tenh/conceptual/symmetricpower.hpp"
#include "tenh/interface/embeddableastensor.hpp"

namespace Tenh {

template <typename Scalar, typename Space> struct ImplementationOf_t;

// Factor_ should be a BasedVectorSpace_c type
template <typename Scalar_, typename Factor_, Uint32 ORDER_>
struct ImplementationOf_t<Scalar_,SymmetricPowerOfBasedVectorSpaces_c<Factor_,ORDER_> >
    :
    public EmbeddableAsTensor_i<ImplementationOf_t<Scalar_,SymmetricPowerOfBasedVectorSpaces_c<Factor_,ORDER_> >,
                                Scalar_,
                                SymmetricPowerOfBasedVectorSpaces_c<Factor_,ORDER_> >,
    // Array_t is privately inherited because it is an implementation detail
    private Array_t<Scalar_,SymmetricPowerOfBasedVectorSpaces_c<Factor_,ORDER_>::DIM>
{
    enum { STATIC_ASSERT_IN_ENUM(HasBasedVectorSpaceStructure_f<Factor_>::V, MUST_BE_BASED_VECTOR_SPACE) };

    typedef EmbeddableAsTensor_i<ImplementationOf_t<Scalar_,SymmetricPowerOfBasedVectorSpaces_c<Factor_,ORDER_> >,
                                 Scalar_,
                                 SymmetricPowerOfBasedVectorSpaces_c<Factor_,ORDER_> > Parent_EmbeddableAsTensor_i;
    typedef Array_t<Scalar_,SymmetricPowerOfBasedVectorSpaces_c<Factor_,ORDER_>::DIM> Parent_Array_t;

    typedef typename Parent_EmbeddableAsTensor_i::Derived Derived;
    typedef typename Parent_EmbeddableAsTensor_i::Scalar Scalar;
    typedef typename Parent_EmbeddableAsTensor_i::BasedVectorSpace BasedVectorSpace;
    using Parent_EmbeddableAsTensor_i::DIM;
    typedef typename Parent_EmbeddableAsTensor_i::ComponentIndex ComponentIndex;

    typedef typename Parent_EmbeddableAsTensor_i::TensorProductOfBasedVectorSpaces TensorProductOfBasedVectorSpaces;
    typedef typename Parent_EmbeddableAsTensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_EmbeddableAsTensor_i::MultiIndex MultiIndex;
    using Parent_EmbeddableAsTensor_i::ORDER;
    static Uint32 const ORDER = ORDER_;
    typedef Factor_ Factor;
    typedef SymmetricPowerOfBasedVectorSpaces_c<Factor,ORDER> SymmetricPowerOfBasedVectorSpaces;

    typedef typename DualOf_f<ImplementationOf_t>::T Dual; // relies on the template specialization below

    explicit ImplementationOf_t (WithoutInitialization const &w) : Parent_Array_t(w) { }
    // probably only useful for zero element (because this is basis-dependent)
    explicit ImplementationOf_t (Scalar const &fill_with) : Parent_Array_t(fill_with) { }

    template <typename BundleIndexTypeList, typename BundledIndex>
    static MultiIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        //STATIC_ASSERT(IsADimIndex_f<BundledIndex>::V, MUST_BE_COMPONENT_INDEX);
        // this constructor breaks the vector index apart into a row-major multi-index
        return BundleIndexComputer_t<BundleIndexTypeList, BundledIndex, ORDER>::compute(b);
    }

    using Parent_Array_t::operator[];
    using Parent_Array_t::data_size_in_bytes;
    using Parent_Array_t::data_pointer;

    // TODO (maybe): Scalar &operator [] (MultiIndex const &) -- writeable access to components,
    // which would need to throw if the multi-index corresponds to an immutable zero.  there is
    // the complication that components with non-unit scale factors would need to be set with
    // a division by that scale factor.

    // these are what provide indexed expressions -- via expression templates
    using Parent_EmbeddableAsTensor_i::operator();

    static bool component_is_immutable_zero (MultiIndex const &m) { return false; }
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return Scalar(MultiIndexMultiplicity_t<MultiIndex>::eval(sorted<typename MultiIndex::IndexTypeList,std::greater<Uint32> >(m)))/Scalar(Factorial_t<ORDER>::V); }
    static ComponentIndex vector_index_of (MultiIndex const &m)
    {
        MultiIndex n = sorted<typename MultiIndex::IndexTypeList,std::greater<Uint32> >(m);
        return ComponentIndex(VectorIndexComputer_t<MultiIndex>::compute(n), CHECK_RANGE);
    }

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + TypeStringOf_t<Scalar>::eval() + ',' + TypeStringOf_t<SymmetricPowerOfBasedVectorSpaces>::eval() + '>';
    }

private:

    // this has no definition, and is designed to generate a compiler error if used (use the one accepting WithoutInitialization instead).
    ImplementationOf_t ();

    template <typename BundleIndexTypeList, typename BundledIndex, Uint32 ORD>
    struct BundleIndexComputer_t;

    template<typename T, typename I = int>
    struct VectorIndexComputer_t;
};

template <typename Scalar_, typename Factor_, Uint32 ORDER_>
template <typename BundleIndexTypeList, typename BundledIndex, Uint32 ORD>
struct ImplementationOf_t<Scalar_,SymmetricPowerOfBasedVectorSpaces_c<Factor_,ORDER_> >::BundleIndexComputer_t
{
    static MultiIndex_t<BundleIndexTypeList> compute (BundledIndex const &b)
    {
        return MultiIndex_t<BundleIndexTypeList>(typename BundleIndexTypeList::HeadType(index_of_greatest_triangular_number_less_than(b.value(),ORD) - ORD + 1, CHECK_RANGE), BundleIndexComputer_t<typename BundleIndexTypeList::BodyTypeList, BundledIndex, ORD-1>::compute(BundledIndex(b.value() - binomial_coefficient(index_of_greatest_triangular_number_less_than(b.value(),ORD), ORD), CHECK_RANGE)));
    }
};

template <typename Scalar_, typename Factor_, Uint32 ORDER_>
template <typename FactorType, typename BundledIndex, Uint32 ORD>
struct ImplementationOf_t<Scalar_,SymmetricPowerOfBasedVectorSpaces_c<Factor_,ORDER_> >::BundleIndexComputer_t<TypeList_t<FactorType>, BundledIndex, ORD>
{
    static MultiIndex_t<TypeList_t<FactorType> > compute (BundledIndex const &b)
    {
        return MultiIndex_t<TypeList_t<FactorType> >(FactorType(index_of_greatest_triangular_number_less_than(b.value(),ORD) - ORD + 1, CHECK_RANGE));
    }
};

template <typename Scalar_, typename Factor_, Uint32 ORDER_>
template <typename T, typename I>
struct ImplementationOf_t<Scalar_,SymmetricPowerOfBasedVectorSpaces_c<Factor_,ORDER_> >::VectorIndexComputer_t
{
    static Uint32 compute (T const &m)
    {
        return VectorIndexComputer_t<typename T::BodyMultiIndex>::compute(m.body()) + binomial_coefficient(m.head().value() + T::LENGTH - 1,T::LENGTH);
    }
};

template <typename Scalar_, typename Factor_, Uint32 ORDER_>
template <typename I>
struct ImplementationOf_t<Scalar_,SymmetricPowerOfBasedVectorSpaces_c<Factor_,ORDER_> >::VectorIndexComputer_t<MultiIndex_t<EmptyTypeList>, I>
{
    static Uint32 compute (MultiIndex_t<EmptyTypeList> const &m)
    {
        return 0;
    }
};

template <typename Scalar, typename Factor, Uint32 ORDER>
struct DualOf_f<ImplementationOf_t<Scalar,SymmetricPowerOfBasedVectorSpaces_c<Factor,ORDER> > >
{
    typedef ImplementationOf_t<Scalar,typename DualOf_f<SymmetricPowerOfBasedVectorSpaces_c<Factor,ORDER> >::T> T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_VEE_HPP_
