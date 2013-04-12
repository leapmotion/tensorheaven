// ///////////////////////////////////////////////////////////////////////////
// tenh/tensor2.hpp by Victor Dods, created 2013/03/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_TENSOR2_HPP_
#define TENH_TENSOR2_HPP_

#include "tenh/core.hpp"
#include "tenh/interface/tensor.hpp"
#include "tenh/meta/typetuple.hpp"
#include "tenh/vector.hpp"

namespace Tenh {

/*
notes on further 2-tensor design
--------------------------------
certain tensors with symmetries have components which do not correspond to a memory location,
which are understood to be zero (they must be zero in order for the relevant type to represent
a vector space).  any tensor product having such a factor will inherit this property, and will
have to determine which components correspond to memory locations and which are understood
to be zero.


*/

template <typename FactorTypeList>
struct DimensionOfTensorProduct_t
{
    static Uint32 const V = FactorTypeList::HeadType::DIM * DimensionOfTensorProduct_t<typename FactorTypeList::BodyTypeList>::V;
};

template <>
struct DimensionOfTensorProduct_t<EmptyTypeList>
{
    static Uint32 const V = 1;
};

// general 2-tensor with no symmetries -- most general type of 2-tensor
template <typename Factor1_, typename Factor2_, typename Derived_ = NullType>
struct Tensor2_t : public Tensor_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                          Tensor2_t<Factor1_,Factor2_,Derived_>,
                                                          Derived_>::T,
                                   TypeList_t<Factor1_,TypeList_t<Factor2_> >,
                                   DimensionOfTensorProduct_t<TypeList_t<Factor1_,TypeList_t<Factor2_> > >::V>,
                   private Array_t<typename Factor1_::Scalar,
                                   DimensionOfTensorProduct_t<TypeList_t<Factor1_,TypeList_t<Factor2_> > >::V,
                                   typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                          Tensor2_t<Factor1_,Factor2_,Derived_>,
                                                          Derived_>::T>
                   // privately inherited because the use of Array_t is an implementation detail
{
    enum { FACTOR_SCALAR_TYPES_ARE_EQUAL
        = STATIC_ASSERT_AS_RVALUE((Lvd::Meta::TypesAreEqual<typename Factor1_::Scalar,typename Factor2_::Scalar>::v), FACTOR_SCALAR_TYPES_ARE_EQUAL) };

    typedef Tensor_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                     Tensor2_t<Factor1_,Factor2_,Derived_>,
                                                                     Derived_>::T,
                                            TypeList_t<Factor1_,TypeList_t<Factor2_> >,
                                            DimensionOfTensorProduct_t<TypeList_t<Factor1_,TypeList_t<Factor2_> > >::V> Parent_Tensor_i;
    typedef Array_t<typename Factor1_::Scalar,
                    DimensionOfTensorProduct_t<TypeList_t<Factor1_,TypeList_t<Factor2_> > >::V,
                    typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                            Tensor2_t<Factor1_,Factor2_,Derived_>,
                                            Derived_>::T> Parent_Array_t;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    static Uint32 const DIM = Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Index Index;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::DEGREE;
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;

    Tensor2_t (WithoutInitialization const &w) : Parent_Array_t(w) { }
    Tensor2_t (Scalar fill_with) : Parent_Array_t(fill_with) { }

    template <typename BundleIndexTypeList, typename BundledIndex>
    static MultiIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        typedef typename BundleIndexTypeList::template El_t<0>::T Index1;
        typedef typename BundleIndexTypeList::template El_t<1>::T Index2;
        // this is just to check that there is a valid conversion to the requested MultiIndex type.
        // it doesn't actually produce any side-effects, and should be optimized out.
        {
            STATIC_ASSERT((BundleIndexTypeList::LENGTH == 2), CAN_ONLY_BUNDLE_TWO_INDICES);
            Index1 i1;
            Index2 i2;
            typename Factor1::Index f1(i1);
            typename Factor2::Index f2(i2);
            // check that the parameter BundleIndex type is compatible with Index
            Index i(b);
        }

        Uint32 row;
        Uint32 col;
        contiguous_index_to_rowcol_index(b.value(), row, col);
        return MultiIndex_t<BundleIndexTypeList>(Index1(row), Index2(col));
    }

    using Parent_Array_t::operator[];
    using Parent_Array_t::data_size_in_bytes;
    using Parent_Array_t::data_pointer;

    // Index1 could be Factor1::Index or Factor1::MultiIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::MultiIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    Scalar operator [] (MultiIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &m) const
    {
//         // NOTE: this construction is unnecessary to the code, but IS necessary to the compile-time type checking
//         // the compiler should optimize it out anyway.
//         typename Factor1::Index(m.template el<0>());
//         typename Factor2::Index(m.template el<1>());
        return component(m.template el<0>(), m.template el<1>());
    }
    template <typename Index1, typename Index2>
    Scalar &operator [] (MultiIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &m)
    {
        assert(m.is_not_at_end() && "you used Index_t(x, DONT_RANGE_CHECK) inappropriately");
        // NOTE: this construction is unnecessary to the code, but IS necessary to the compile-time type checking
        // the compiler should optimize it out anyway.
        MultiIndex x(m);
        return operator[](Index(m.value(), DONT_CHECK_RANGE));
    }

    // these are what provide indexed expressions -- via expression templates
    using Parent_Tensor_i::operator();

    // access 2-tensor components
    // Index1 could be Factor1::Index or Factor1::MultiIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::MultiIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    Scalar component (Index1 const &i1, Index2 const &i2) const
    {
        assert(i1.is_not_at_end() && i2.is_not_at_end() && "you used Index_t(x, DONT_RANGE_CHECK) inappropriately");

        if (Factor1::component_is_immutable_zero(i1) || Factor2::component_is_immutable_zero(i2))
        {
            return Scalar(0);
        }
        else
        {
            return Factor1::scalar_factor_for_component(i1) *
                   Factor2::scalar_factor_for_component(i2) *
                   operator[](vector_index_of(MultiIndex(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2))));
        }
    }
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
    using Parent_Tensor_i::component_is_immutable_zero;
    using Parent_Tensor_i::scalar_factor_for_component;
    using Parent_Tensor_i::vector_index_of;
    // all components are stored in memory (in the array m), and have scalar factor 1
    static bool component_is_immutable_zero (MultiIndex const &m) { return false; }
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return Scalar(1); }
    static Index vector_index_of (MultiIndex const &m) { return Index(m.value(), DONT_CHECK_RANGE); }

    static std::string type_as_string ()
    {
        if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
        {
            return "Tensor2_t<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + '>';
        }
        else
        {
            return "Tensor2_t<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + ','
                   + TypeStringOf_t<Derived>::eval() + '>';
        }
    }

private:

    // functions between the indexing schemes -- multi-index is (row,col) with row > col and vector index is contiguous.
    static Uint32 rowcol_index_to_contiguous_index (Uint32 row, Uint32 col)
    {
        return Factor2::DIM*row + col;
    }
    static void contiguous_index_to_rowcol_index (Uint32 i, Uint32 &row, Uint32 &col)
    {
        row = i / Factor2::DIM;
        col = i % Factor2::DIM;
    }
};

/*
template <typename Scalar_, Uint32 DIM_, typename Derived_ = NullType> // don't worry about type ID for now
struct Vector_t;
*/
/*
template <typename Factor1Scalar, Uint32 Factor1DIM, typename Factor1Derived,
          typename Factor2Scalar, Uint32 Factor2DIM, typename Factor2Derived,
          typename Tensor2Derived>
struct EuclideanEmbedding_t : public typename EuclideanEmbedding_Parent_Tensor_i<Tensor2_t<Vector_t<Factor1Scalar,Factor1DIM,Factor1Derived>,
                                                                                           Vector_t<Factor2Scalar,Factor2DIM,Factor2Derived>,
                                                                                           Tensor2Derived> >::T
{
    typedef typename EuclideanEmbedding_Parent_Tensor_i<Tensor2_t<Vector_t<Factor1Scalar,Factor1DIM,Factor1Derived>,
                                                                  Vector_t<Factor2Scalar,Factor2DIM,Factor2Derived>,
                                                                  Tensor2Derived> >::T Parent_Tensor_i;
    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    using Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::Index Index;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::DEGREE;

    Scalar operator [] (MultiIndex const &m) const { return m.template el<0>() == m.template el<1>() ? Scalar(1) : Scalar(0); }

    // this SHOULD be inconvenient and ugly to call.  it should be used ONLY when you know for certain that 0 <= i < DIM
    Scalar component_access_without_range_check (Uint32 i) const
    {
        Uint32 row = i / Vector::DIM;
        Uint32 col = i % Vector::DIM;
        return row == col ? Scalar(1) : Scalar(0);
    }

    // NOTE: these may be unnecessary/undesired, because this type does not represent a vector space
//     using Parent_Tensor_i::component_is_immutable_zero;
//     using Parent_Tensor_i::scalar_factor_for_component;
//     using Parent_Tensor_i::vector_index_of;
};
*/

// NOTE: while this is a tensor, it isn't a tensor space, and so it technically shouldn't be used as a factor
// type in a tensor product.  this is essentially a constant value -- it has only const accessors and can't be written to.
/*
template <typename Factor1, typename Factor2, typename Tensor2Derived>
struct EuclideanEmbedding_t : public EuclideanEmbedding_Parent_Tensor_i<Tensor2_t<Factor1,Factor2,Tensor2Derived> >::T
{
    typedef typename EuclideanEmbedding_Parent_Tensor_i<Tensor2_t<Factor1,Factor2,Tensor2Derived> >::T Parent_Tensor_i;
    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    using Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::Index Index;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::DEGREE;

    Scalar operator [] (MultiIndex const &m) const
    {
        if (m.is_at_end())
            throw std::invalid_argument("index out of range");

        if (m_euclidean_embedding_factor1.component_is_immutable_zero(m.template el<0>()) ||
            m_euclidean_embedding_factor2.component_is_immutable_zero(m.template el<1>()))
        {
            return Scalar(0);
        }


        return m_euclidean_embedding_factor1.component_access_without_range_check(m.template el<0>().value());
    }

    // this SHOULD be inconvenient and ugly to call.  it should be used ONLY when you know for certain that 0 <= i < DIM
    Scalar component_access_without_range_check (Uint32 i) const
    {
        Uint32 row = i / Vector::DIM;
        Uint32 col = i % Vector::DIM;
        return row == col ? Scalar(1) : Scalar(0);
    }

    // NOTE: these may be unnecessary/undesired, because this type does not represent a vector space
//     using Parent_Tensor_i::component_is_immutable_zero;
//     using Parent_Tensor_i::scalar_factor_for_component;
//     using Parent_Tensor_i::vector_index_of;

private:

    // NOTE: this is sort of temporary
    EuclideanEmbedding_t<Factor1> m_euclidean_embedding_factor1;
    EuclideanEmbedding_t<Factor2> m_euclidean_embedding_factor2;
};
*/

} // end of namespace Tenh

#endif // TENH_TENSOR2_HPP_
