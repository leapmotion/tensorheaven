// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/wedge.hpp by Victor Dods, created 2013/08/15
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_WEDGE_HPP_
#define TENH_IMPLEMENTATION_WEDGE_HPP_

#include "tenh/core.hpp"

#include "tenh/mathutil.hpp"
#include "tenh/conceptual/exteriorpower.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/implementation/implementationof.hpp"
#include "tenh/interface/embeddableastensor.hpp"

namespace Tenh {

// Factor_ should be a BasedVectorSpace_c type
template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
struct ImplementationOf_t<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>
    :
    public EmbeddableAsTensor_i<ImplementationOf_t<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>,
                                Scalar_,
                                ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,
                                ComponentsAreImmutable_f<UseArrayType_>::V>,
    // privately inherited because it is an implementation detail
    private ArrayStorage_f<Scalar_,
                           UniqueVectorSpaceStructureOf_f<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_> >::T::DIMENSION,
                           UseArrayType_,
                           ImplementationOf_t<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_> >::T
{
    enum { STATIC_ASSERT_IN_ENUM(HasBasedVectorSpaceStructure_f<Factor_>::V, MUST_BE_BASED_VECTOR_SPACE) };

    typedef EmbeddableAsTensor_i<ImplementationOf_t<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>,
                                 Scalar_,
                                 ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,
                                 ComponentsAreImmutable_f<UseArrayType_>::V> Parent_EmbeddableAsTensor_i;
    typedef typename ArrayStorage_f<Scalar_,
                                    UniqueVectorSpaceStructureOf_f<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_> >::T::DIMENSION,
                                    UseArrayType_,
                                    ImplementationOf_t<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_> >::T Parent_Array_i;

    typedef typename Parent_EmbeddableAsTensor_i::Derived Derived;
    typedef typename Parent_EmbeddableAsTensor_i::Scalar Scalar;
    typedef typename Parent_EmbeddableAsTensor_i::BasedVectorSpace BasedVectorSpace;
    using Parent_EmbeddableAsTensor_i::DIM;
    typedef typename Parent_EmbeddableAsTensor_i::ComponentIndex ComponentIndex;

    typedef typename Parent_EmbeddableAsTensor_i::TensorProductOfBasedVectorSpaces TensorProductOfBasedVectorSpaces;
    typedef typename Parent_EmbeddableAsTensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_EmbeddableAsTensor_i::MultiIndex MultiIndex;
    static Uint32 const ORDER = ORDER_;
    typedef Factor_ Factor;
    typedef ExteriorPowerOfBasedVectorSpace_c<ORDER,Factor> ExteriorPowerOfBasedVectorSpace;

    using Parent_Array_i::COMPONENTS_ARE_IMMUTABLE;
    typedef typename Parent_Array_i::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_Array_i::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;

    typedef typename DualOf_f<ImplementationOf_t>::T Dual; // relies on the template specialization below

    explicit ImplementationOf_t (WithoutInitialization const &w) : Parent_Array_i(w) { }

    // only use these if UseMemberArray is specified

    // probably only useful for zero element (because this is basis-dependent)
    explicit ImplementationOf_t (Scalar const &fill_with)
        :
        Parent_Array_i(fill_with)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemberArray);
    }

    // only use these if UsePreallocatedArray is specified

    explicit ImplementationOf_t (Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }
    ImplementationOf_t (Scalar const &fill_with, 
                        Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(fill_with, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }

    template <typename BundleIndexTypeList, typename BundledIndex>
    static MultiIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        //STATIC_ASSERT(IsDimIndex_f<BundledIndex>::V, MUST_BE_COMPONENT_INDEX);
        // this constructor breaks the vector index apart into a row-major multi-index
        return BundleIndexComputer_t<BundleIndexTypeList, BundledIndex, ORDER>::compute(b);
    }

    using Parent_Array_i::as_derived;
    using Parent_Array_i::operator[];
    using Parent_Array_i::allocation_size_in_bytes;
    using Parent_Array_i::pointer_to_allocation;

    // TODO (maybe): Scalar &operator [] (MultiIndex const &) -- writeable access to components,
    // which would need to throw if the multi-index corresponds to an immutable zero.  there is
    // the complication that components with non-unit scale factors would need to be set with
    // a division by that scale factor.

    // these are what provide indexed expressions -- via expression templates
    using Parent_EmbeddableAsTensor_i::operator();

    static bool component_is_immutable_zero (MultiIndex const &m) { return MultiIndexMultiplicity_t<MultiIndex>::eval(sorted<typename MultiIndex::IndexTypeList,std::greater<Uint32> >(m)) != 1; }
    // TODO: This needs to be (-1)^sign(m)/ORDER!.
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return SignComputer_t<MultiIndex>::compute(m)/Scalar(Factorial_t<ORDER>::V); }
    static ComponentIndex vector_index_of (MultiIndex const &m)
    {
        MultiIndex n = sorted<typename MultiIndex::IndexTypeList,std::greater<Uint32> >(m);
        return ComponentIndex(VectorIndexComputer_t<MultiIndex>::compute(n), CHECK_RANGE);
    }

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + TypeStringOf_t<ExteriorPowerOfBasedVectorSpace>::eval() + ',' 
                                     + TypeStringOf_t<Scalar>::eval() + ','
                                     + TypeStringOf_t<UseArrayType_>::eval() + '>';
    }

private:

    // this has no definition, and is designed to generate a compiler error if used (use the one accepting WithoutInitialization instead).
    ImplementationOf_t ();

    template <typename BundleIndexTypeList, typename BundledIndex, Uint32 ORD>
    struct BundleIndexComputer_t;

    template<typename T, typename I = int>
    struct VectorIndexComputer_t;

    template<typename T, typename I = int>
    struct SignComputer_t;
};

template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
template <typename BundleIndexTypeList, typename BundledIndex, Uint32 ORD>
struct ImplementationOf_t<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::BundleIndexComputer_t
{
    static MultiIndex_t<BundleIndexTypeList> compute (BundledIndex const &b)
    {
        return MultiIndex_t<BundleIndexTypeList>(typename BundleIndexTypeList::HeadType(index_of_greatest_triangular_number_less_than(b.value(),ORD), CHECK_RANGE), BundleIndexComputer_t<typename BundleIndexTypeList::BodyTypeList, BundledIndex, ORD-1>::compute(BundledIndex(b.value() - binomial_coefficient(index_of_greatest_triangular_number_less_than(b.value(),ORD), ORD), CHECK_RANGE)));
    }
};

template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
template <typename FactorType, typename BundledIndex, Uint32 ORD>
struct ImplementationOf_t<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::BundleIndexComputer_t<TypeList_t<FactorType>, BundledIndex, ORD>
{
    static MultiIndex_t<TypeList_t<FactorType> > compute (BundledIndex const &b)
    {
        return MultiIndex_t<TypeList_t<FactorType> >(FactorType(index_of_greatest_triangular_number_less_than(b.value(),ORD), CHECK_RANGE));
    }
};

template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
template <typename T, typename I>
struct ImplementationOf_t<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::VectorIndexComputer_t
{
    static Uint32 compute (T const &m)
    {
        return VectorIndexComputer_t<typename T::BodyMultiIndex>::compute(m.body()) + binomial_coefficient(m.head().value(),T::LENGTH);
    }
};

template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
template <typename I>
struct ImplementationOf_t<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::VectorIndexComputer_t<MultiIndex_t<EmptyTypeList>, I>
{
    static Uint32 compute (MultiIndex_t<EmptyTypeList> const &m)
    {
        return 0;
    }
};

template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
template <typename T, typename I>
struct ImplementationOf_t<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::SignComputer_t
{
    static Scalar_ compute (T const &m)
    {
        Scalar_ sign(1);
        for (Uint32 i = 0; i < T::LENGTH; ++i)
        {
            if (m.head().value() > m.index(i).value())
            {
                sign *= Scalar_(-1);
            }
        }

        return SignComputer_t<typename T::BodyMultiIndex>::compute(m.body()) * sign;
    }
};

template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
template <typename I>
struct ImplementationOf_t<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::SignComputer_t<MultiIndex_t<EmptyTypeList>, I>
{
    static Scalar_ compute (MultiIndex_t<EmptyTypeList> const &m)
    {
        return Scalar_(1);
    }
};

template <Uint32 ORDER, typename Factor, typename Scalar, typename UseArrayType_>
struct DualOf_f<ImplementationOf_t<ExteriorPowerOfBasedVectorSpace_c<ORDER,Factor>,Scalar,UseArrayType_> >
{
    typedef ImplementationOf_t<typename DualOf_f<ExteriorPowerOfBasedVectorSpace_c<ORDER,Factor> >::T,Scalar,typename DualOf_f<UseArrayType_>::T> T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_WEDGE_HPP_
