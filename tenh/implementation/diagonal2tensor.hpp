// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/diagonal2tensor.hpp by Victor Dods, created 2013/08/20
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_DIAGONAL2TENSOR_HPP_
#define TENH_IMPLEMENTATION_DIAGONAL2TENSOR_HPP_

#include "tenh/core.hpp"

#include "tenh/array.hpp"
#include "tenh/conceptual/diagonalbased2tensorproduct.hpp"
#include "tenh/interface/embeddableastensor.hpp"

namespace Tenh {

template <typename Scalar, typename Space> struct ImplementationOf_t;

// Factor_ should be a BasedVectorSpace_c type
template <typename Scalar_, typename Factor1_, typename Factor2_>
struct ImplementationOf_t<Scalar_,Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> >
    :
    public EmbeddableAsTensor_i<ImplementationOf_t<Scalar_,Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> >,
                                Scalar_,
                                Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> >,
    // Array_t is privately inherited because it is an implementation detail
    private Array_t<Scalar_,Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>::DIM>
{
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsABasedVectorSpace_c<Factor1_>::V, MUST_BE_BASED_VECTOR_SPACE, FACTOR1),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsABasedVectorSpace_c<Factor2_>::V, MUST_BE_BASED_VECTOR_SPACE, FACTOR2)
    };

    typedef EmbeddableAsTensor_i<ImplementationOf_t<Scalar_,Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> >,
                                Scalar_,
                                Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> > Parent_EmbeddableAsTensor_i;
    typedef Array_t<Scalar_,Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>::DIM> Parent_Array_t;

    typedef typename Parent_EmbeddableAsTensor_i::Derived Derived;
    typedef typename Parent_EmbeddableAsTensor_i::Scalar Scalar;
    typedef typename Parent_EmbeddableAsTensor_i::BasedVectorSpace BasedVectorSpace;
    using Parent_EmbeddableAsTensor_i::DIM;
    typedef typename Parent_EmbeddableAsTensor_i::ComponentIndex ComponentIndex;

    typedef typename Parent_EmbeddableAsTensor_i::TensorProductOfBasedVectorSpaces TensorProductOfBasedVectorSpaces;
    typedef typename Parent_EmbeddableAsTensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_EmbeddableAsTensor_i::MultiIndex MultiIndex;
    using Parent_EmbeddableAsTensor_i::DEGREE;
    static Uint32 const ORDER = 2;
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> Diagonal2TensorProductOfBasedVectorSpaces;

    typedef typename DualOf_c<ImplementationOf_t>::T Dual; // relies on the template specialization below

    explicit ImplementationOf_t (WithoutInitialization const &w) : Parent_Array_t(w) { }
    // probably only useful for zero element (because this is basis-dependent), though
    // this would also give any scalar matrix, including the identity matrix (though you
    // would really want to TODO: implement Scalar2TensorProductOfBasedVectorSpaces_c)
    explicit ImplementationOf_t (Scalar const &fill_with) : Parent_Array_t(fill_with) { }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1) : Parent_Array_t(x0, x1) { }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1, Scalar const &x2) : Parent_Array_t(x0, x1, x2) { }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1, Scalar const &x2, Scalar const &x3) : Parent_Array_t(x0, x1, x2, x3) { }

    template <typename BundleIndexTypeList, typename BundledIndex>
    static MultiIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        STATIC_ASSERT(IsADimIndex_t<BundledIndex>::V, MUST_BE_DIM_INDEX);
        STATIC_ASSERT(BundleIndexTypeList::LENGTH == 2, LENGTH_MUST_BE_EXACTLY_2);
        typedef MultiIndex_t<BundleIndexTypeList> MultiIndex;
        typedef typename BundleIndexTypeList::HeadType Index1;
        typedef typename BundleIndexTypeList::BodyTypeList::HeadType Index2;
        STATIC_ASSERT(IsADimIndex_t<Index1>::V, MUST_BE_DIM_INDEX);
        STATIC_ASSERT(IsADimIndex_t<Index2>::V, MUST_BE_DIM_INDEX);
        STATIC_ASSERT(Index1::COMPONENT_COUNT == Factor1::DIM, DIMENSIONS_MUST_MATCH);
        STATIC_ASSERT(Index2::COMPONENT_COUNT == Factor2::DIM, DIMENSIONS_MUST_MATCH);
        Uint32 b_value = b.value();
        return MultiIndex(b_value, b_value, DONT_CHECK_RANGE);
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

    static bool component_is_immutable_zero (MultiIndex const &m) { return m.template el<0>().value() != m.template el<1>().value(); }
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return Scalar(1); }
    static ComponentIndex vector_index_of (MultiIndex const &m) { return ComponentIndex(m.template el<0>().value(), DONT_CHECK_RANGE); }

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + TypeStringOf_t<Scalar>::eval() + ',' + TypeStringOf_t<Diagonal2TensorProductOfBasedVectorSpaces>::eval() + '>';
    }

private:

    // this has no definition, and is designed to generate a compiler error if used (use the one accepting WithoutInitialization instead).
    ImplementationOf_t ();
};

template <typename Scalar, typename Factor1, typename Factor2>
struct DualOf_c<ImplementationOf_t<Scalar,Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > >
{
    typedef ImplementationOf_t<Scalar,typename DualOf_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> >::T> T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_DIAGONAL2TENSOR_HPP_
