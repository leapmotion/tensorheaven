// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/tensor.hpp by Victor Dods, created 2013/08/10
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_TENSOR_HPP_
#define TENH_IMPLEMENTATION_TENSOR_HPP_

#include "tenh/core.hpp"

#include "tenh/array.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/interface/tensor.hpp"

namespace Tenh {

template <typename Concept_, typename Scalar_> struct ImplementationOf_t;

template <typename FactorTypeList_, typename Scalar_>
struct ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_>
    :
    public Tensor_i<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_>,
                    Scalar_,
                    TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >,
    // Array_t is privately inherited because it is an implementation detail
    private Array_t<Scalar_,UniqueVectorSpaceStructureOf_f<TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >::T::DIMENSION>
{
    typedef Tensor_i<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_>,
                     Scalar_,
                     TensorProductOfBasedVectorSpaces_c<FactorTypeList_> > Parent_Tensor_i;
    typedef Array_t<Scalar_,UniqueVectorSpaceStructureOf_f<TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >::T::DIMENSION> Parent_Array_t;

    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    typedef typename Parent_Tensor_i::BasedVectorSpace BasedVectorSpace;
    using Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::ComponentIndex ComponentIndex;

    typedef typename Parent_Tensor_i::TensorProductOfBasedVectorSpaces TensorProductOfBasedVectorSpaces;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::ORDER;
    using Parent_Tensor_i::IS_TENSOR_I;

    typedef typename DualOf_f<ImplementationOf_t>::T Dual; // relies on the template specialization below

    explicit ImplementationOf_t (WithoutInitialization const &w) : Parent_Array_t(w) { }
    // probably only useful for zero element (because this is basis-dependent)
    explicit ImplementationOf_t (Scalar const &fill_with) : Parent_Array_t(fill_with) { }
    // TODO: decide if there should be constructors that take a number of initializing Scalar arguments

    template <typename BundleIndexTypeList, typename BundledIndex>
    static MultiIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        STATIC_ASSERT(IsDimIndex_f<BundledIndex>::V, MUST_BE_COMPONENT_INDEX);
        // this constructor breaks the vector index apart into a row-major multi-index
        return MultiIndex_t<BundleIndexTypeList>(b);
    }

    using Parent_Array_t::operator[];
    using Parent_Array_t::allocation_size_in_bytes;
    using Parent_Array_t::pointer_to_allocation;

    template <typename OtherIndexTypeList>
    Scalar operator [] (MultiIndex_t<OtherIndexTypeList> const &m) const
    {
        STATIC_ASSERT(IsTypeList_f<OtherIndexTypeList>::V, MUST_BE_TYPELIST);
        typedef MultiIndex_t<OtherIndexTypeList> OtherMultiIndex;
        STATIC_ASSERT((OtherMultiIndex::LENGTH == MultiIndex::LENGTH), MUST_HAVE_EQUAL_LENGTHS);
        //std::cout << OtherMultiIndex::LENGTH << ", " << MultiIndex::LENGTH << '\n';
        assert(m.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        // NOTE: this construction is unnecessary to the code, but IS necessary to the compile-time type checking
        // the compiler should optimize it out anyway.
        MultiIndex x(m);
        // m.value() is what does the multi-index-to-vector-index computation
        return operator[](ComponentIndex(m.value(), DONT_CHECK_RANGE));
    }
    template <typename OtherIndexTypeList>
    Scalar &operator [] (MultiIndex_t<OtherIndexTypeList> const &m)
    {
        STATIC_ASSERT(IsTypeList_f<OtherIndexTypeList>::V, MUST_BE_TYPELIST);
        typedef MultiIndex_t<OtherIndexTypeList> OtherMultiIndex;
        STATIC_ASSERT((OtherMultiIndex::LENGTH == MultiIndex::LENGTH), MUST_HAVE_EQUAL_LENGTHS);
        //std::cout << OtherMultiIndex::LENGTH << ", " << MultiIndex::LENGTH << '\n';
        assert(m.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        // NOTE: this construction is unnecessary to the code, but IS necessary to the compile-time type checking
        // the compiler should optimize it out anyway.
        MultiIndex x(m);
        // m.value() is what does the multi-index-to-vector-index computation
        return operator[](ComponentIndex(m.value(), DONT_CHECK_RANGE));
    }

    // these are what provide indexed expressions -- via expression templates
    using Parent_Tensor_i::operator();

/*
    // write 2-tensor components -- will throw if a component doesn't correspond to a memory location
    // Index1 could be Factor1::Index or Factor1::MultiIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::MultiIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    void set_component (Index1 const &i1, Index2 const &i2, Scalar s)
    {
        assert(i1.is_not_at_end() && i2.is_not_at_end() && "you used Index_t(x, DONT_RANGE_CHECK) inappropriately");

        if (Factor1::component_is_immutable_zero(i1) || Factor2::component_is_immutable_zero(i2))
        {
            throw std::invalid_argument("this tensor component is not writable");
        }

        MultiIndex m(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2));
        // write to the component, but divide through by the total scale factor for the component.
        operator[](vector_index_of(m)) = s / (Factor1::scalar_factor_for_component(i1) * Factor2::scalar_factor_for_component(i2));
    }
    */
    using Parent_Tensor_i::component_is_immutable_zero;
    using Parent_Tensor_i::scalar_factor_for_component;
    using Parent_Tensor_i::vector_index_of;
    // // all components are stored in memory (in the array m), and have scalar factor 1
    // // TODO: this should probably go into the conceptual layer
    // static bool component_is_immutable_zero (MultiIndex const &m) { return false; }
    // static Scalar scalar_factor_for_component (MultiIndex const &m) { return Scalar(1); }
    // static ComponentIndex vector_index_of (MultiIndex const &m) { return ComponentIndex(m.value(), DONT_CHECK_RANGE); }

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + TypeStringOf_t<TensorProductOfBasedVectorSpaces>::eval() + ',' + TypeStringOf_t<Scalar>::eval() + '>';
    }

private:

    // this has no definition, and is designed to generate a compiler error if used (use the one accepting WithoutInitialization instead).
    ImplementationOf_t ();
};

template <typename FactorTypeList, typename Scalar>
struct DualOf_f<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList>,Scalar> >
{
    typedef ImplementationOf_t<typename DualOf_f<TensorProductOfBasedVectorSpaces_c<FactorTypeList> >::T,Scalar> T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_TENSOR_HPP_
