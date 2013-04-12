// ///////////////////////////////////////////////////////////////////////////
// tenh/tensor2antisymmetric.hpp by Victor Dods, created 2013/03/20
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_TENSOR2ANTISYMMETRIC_HPP_
#define TENH_TENSOR2ANTISYMMETRIC_HPP_

#include <cmath>
#include <ostream>

#include "tenh/core.hpp"
#include "tenh/interface/tensor.hpp"
#include "tenh/naturalpairing.hpp"
#include "tenh/meta/typetuple.hpp"
#include "tenh/vector.hpp"

namespace Tenh {

// antisymmetric 2-tensor (its transpose is equal to its negative)
template <typename Factor1_, typename Factor2_ = Factor1_, typename Derived_ = NullType>
struct Tensor2Antisymmetric_t : public Tensor_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                       Tensor2Antisymmetric_t<Factor1_,Factor2_,Derived_>,
                                                                       Derived_>::T,
                                                TypeList_t<Factor1_,TypeList_t<Factor2_> >,
                                                ((Factor1_::DIM-1)*Factor1_::DIM)/2>,
                                private Array_t<typename Factor1_::Scalar,
                                                ((Factor1_::DIM-1)*Factor1_::DIM)/2,
                                                typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                       Tensor2Antisymmetric_t<Factor1_,Factor2_,Derived_>,
                                                                       Derived_>::T>
                                // privately inherited because the use of Array_t is an implementation detail
{
    enum { FACTOR1_AND_FACTOR2_MUST_BE_IDENTICAL = STATIC_ASSERT_AS_RVALUE((Lvd::Meta::TypesAreEqual<Factor1_,Factor2_>::v), FACTOR1_AND_FACTOR2_MUST_BE_IDENTICAL) };

    typedef Tensor_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                     Tensor2Antisymmetric_t<Factor1_,Factor2_,Derived_>,
                                                                     Derived_>::T,
                     TypeList_t<Factor1_,TypeList_t<Factor2_> >,
                     ((Factor1_::DIM-1)*Factor1_::DIM)/2> Parent_Tensor_i;
    typedef Array_t<typename Factor1_::Scalar,
                    ((Factor1_::DIM-1)*Factor1_::DIM)/2,
                    typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                            Tensor2Antisymmetric_t<Factor1_,Factor2_,Derived_>,
                                            Derived_>::T> Parent_Array_t;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    static Uint32 const DIM = Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Index Index;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex; // TODO: rename to TensorMultiIndex (?)
    using Parent_Tensor_i::DEGREE;
    typedef Factor1_ Factor;
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;
    static Uint32 const STRICTLY_LOWER_TRIANGULAR_COMPONENT_COUNT = ((Factor::DIM-1)*Factor::DIM)/2;

    Tensor2Antisymmetric_t (WithoutInitialization const &w) : Parent_Array_t(w) { }
    Tensor2Antisymmetric_t (Scalar fill_with) : Parent_Array_t(fill_with) { }

    template <typename BundleIndexTypeList, typename BundledIndex>
    static MultiIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        typedef typename BundleIndexTypeList::template El_t<0>::T Index1;
        typedef typename BundleIndexTypeList::template El_t<1>::T Index2;
        // this is just to check that there is a valid conversion to the requested MultiIndex_t type.
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
    // these are what provide indexed expressions -- via expression templates

    // TODO: because Factor1 and Factor2 are identical, it doesn't make sense to
    // have a type coercion to either one unless they are 1-dimensional, in which case
    // it can be a type coercion to the Scalar type.  however, an antisymmetric
    // 2-tensor on a 1-dimensional space is always zero.

    // TODO: because the diagonal is always zero, there is an easy type coercion to Tensor2Diagonal_t

    using Parent_Array_t::operator[];
    using Parent_Array_t::data_size_in_bytes;
    using Parent_Array_t::data_pointer;

    // using two indices in a Tensor2Antisymmetric_t is breaking apart the Index type and using it
    // as a general tensor -- this is where the fancy indexing scheme happens.
    // Index1 could be Factor1::Index or Factor1::MultiIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::MultiIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    Scalar operator [] (MultiIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &m) const
    {
//         // NOTE: these constructions are unnecessary to the code, but ARE necessary to the compile-time type checking
//         // the compiler should optimize it out anyway.
//         typename Factor1::Index(m.template el<0>());
//         typename Factor2::Index(m.template el<1>());
        return component(m.template el<0>(), m.template el<1>());
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
            return Scalar(0);

        MultiIndex m(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2));
        if (component_is_immutable_zero(m))
            return Scalar(0);

        return Factor1::scalar_factor_for_component(i1) *
               Factor2::scalar_factor_for_component(i2) *
               scalar_factor_for_component(m) *
               operator[](vector_index_of(m));
    }
    // write 2-tensor components -- will throw if a component doesn't correspond to a memory location
    // Index1 could be Factor1::Index or Factor1::MultiIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::MultiIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    void set_component (Index1 const &i1, Index2 const &i2, Scalar s)
    {
        assert(i1.is_not_at_end() && i2.is_not_at_end() && "you used Index_t(x, DONT_RANGE_CHECK) inappropriately");

        if (Factor1::component_is_immutable_zero(i1) || Factor2::component_is_immutable_zero(i2))
            throw std::invalid_argument("this tensor component is not writable");

        MultiIndex m(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2));
        if (component_is_immutable_zero(m))
            throw std::invalid_argument("this tensor component is not writable");

        // write to the component, but divide through by the total scale factor for the component.
        operator[](vector_index_of(m))
            = s / (Factor1::scalar_factor_for_component(i1) * Factor2::scalar_factor_for_component(i2) * scalar_factor_for_component(m));
    }
    using Parent_Tensor_i::component_is_immutable_zero;
    using Parent_Tensor_i::scalar_factor_for_component;
    using Parent_Tensor_i::vector_index_of;
    // the diagonal is not stored in memory -- all components are zero.
    static bool component_is_immutable_zero (MultiIndex const &m)
    {
        return m.template el<0>() == m.template el<1>(); // off diagonal elements correspond to memory locations
    }
    // the diagonal is zero, the upper triangular components have a scalar factor of -1, and 1 otherwise.
    static Scalar scalar_factor_for_component (MultiIndex const &m)
    {
        if (m.template el<0>() == m.template el<1>()) // diagonal component
            return Scalar(0); // not actually used, but there for completeness
        else if (m.template el<0>() < m.template el<1>()) // upper triangular component
            return Scalar(-1);
        else // lower triangular component
            return Scalar(1);
    }
    // this should return iff !component_is_immutable_zero(m) and otherwise throw
    static Index vector_index_of (MultiIndex const &m)
    {
        return Index(rowcol_index_to_contiguous_index(m.template el<0>().value(), m.template el<1>().value()), DONT_CHECK_RANGE);
    }

    static std::string type_as_string ()
    {
        if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
            return "Tensor2Antisymmetric_t<" + TypeStringOf_t<Factor>::eval() + '>';
        else
            return "Tensor2Antisymmetric_t<" + TypeStringOf_t<Factor>::eval() + ',' + TypeStringOf_t<Derived>::eval() + '>';
    }

private:

    // functions between the indexing schemes -- multi-index is (row,col) with row > col and vector index is contiguous.
    static Uint32 rowcol_index_to_contiguous_index (Uint32 row, Uint32 col)
    {
        // strict lower triangle elements are stored contiguously in row-major order:
        // [ . . . ... ]
        // [ 0 . . ... ]
        // [ 1 2 . ... ]
        // [ 3 4 5 ... ]
        // ...
        // the index of the first element in row R is the (R-1)th triangular number, so the mapping
        // (row,col) to contiguous index i is i := r*(r-1)/2 + c.
        if (row == col)
            throw std::invalid_argument("there is no contiguous index corresponding to a diagonal component index");
        if (row < col)
            std::swap(row, col);
        return row*(row-1)/2 + col;
    }
    static void contiguous_index_to_rowcol_index (Uint32 i, Uint32 &row, Uint32 &col)
    {
        // given i, row = floor((1 + sqrt(1+8*i))/2) and col = i - r*(r-1)/2.
        // TODO: implement as lookup table
        row = Uint32(std::floor(0.5f + std::sqrt(0.25f + 2.0f*i)));
        col = i - row*(row-1)/2;
    }
};

// template specialization for the natural pairing in this particular coordinatization of Tensor2Antisymmetric_t
template <typename Factor1, typename Factor2, typename Derived>
struct NaturalPairing_t<Tensor2Antisymmetric_t<Factor1,Factor2,Derived> >
{
    typedef Tensor2Antisymmetric_t<Factor1,Factor2,Derived> Tensor2Antisymmetric;
    typedef typename Tensor2Antisymmetric::Scalar Scalar;
    typedef typename Tensor2Antisymmetric::Index Index;

    static Scalar component (Index const &i)
    {
        return Scalar(2); // the off-diagonal components occur twice (in the component matrix)
    }
};

/*
// NOTE: while this is a tensor, it isn't a tensor space, and so it technically shouldn't be used as a factor
// type in a tensor product.  this is essentially a constant value -- it has only const accessors and can't be written to.
// because the (inner product) is essentially scalar multiplication by 2, it follows that multiplication by sqrt(2)
// gives the isometric embedding
template <typename Scalar_, Uint32 DIM_, typename VectorDerived_, typename Tensor2AntisymmetricDerived_>
struct EuclideanEmbedding_t<Tensor2Antisymmetric_t<Vector_t<Scalar_,DIM_,VectorDerived_>,
                                                   Vector_t<Scalar_,DIM_,VectorDerived_>,
                                                   Tensor2AntisymmetricDerived_> >
    :
    public Tensor_i<EuclideanEmbedding_t<Tensor2Antisymmetric_t<Vector_t<Scalar_,DIM_,VectorDerived_>,
                                                                Vector_t<Scalar_,DIM_,VectorDerived_>,
                                                                Tensor2AntisymmetricDerived_> >,
                    TypeList_t<Tensor2Antisymmetric_t<Vector_t<Scalar_,DIM_,VectorDerived_>,
                                                      Vector_t<Scalar_,DIM_,VectorDerived_>,
                                                      Tensor2AntisymmetricDerived_>,
                               TypeList_t<Tensor2Antisymmetric_t<Vector_t<Scalar_,DIM_,VectorDerived_>,
                                                                 Vector_t<Scalar_,DIM_,VectorDerived_>,
                                                                 Tensor2AntisymmetricDerived_> > >,
                    >,
                    Tensor2Antisymmetric_t<Factor1,Factor2,Derived_>::DIM*Tensor2Antisymmetric_t<Factor1,Factor2,Derived_>::DIM>
{
    typedef Tensor_i<EuclideanEmbedding_t<Tensor2Antisymmetric_t<Factor1,Factor2,Derived_> >,
                     TypeList_t<Tensor2Antisymmetric_t<Factor1,Factor2,Derived_>,TypeList_t<Tensor2Antisymmetric_t<Factor1,Factor2,Derived_> > >,
                     Tensor2Antisymmetric_t<Factor1,Factor2,Derived_>::DIM*Tensor2Antisymmetric_t<Factor1,Factor2,Derived_>::DIM> Parent_Tensor_i;
    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    using Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::Index Index;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::DEGREE;
    typedef Tensor2Antisymmetric_t<Factor1,Factor2,Derived_> Tensor2Antisymmetric; // this is the vector type that's being embedded

    Scalar operator [] (MultiIndex const &m) const
    {
        //return m.template el<0>() == m.template el<1>() ? Scalar(M_SQRT2) : Scalar(0);
    }

    // this SHOULD be inconvenient and ugly to call.  it should be used ONLY when you know for certain that 0 <= i < DIM
    Scalar component_access_without_range_check (Uint32 i) const
    {
        Uint32 row = i / Tensor2Antisymmetric::DIM;
        Uint32 col = i % Tensor2Antisymmetric::DIM;
        return row == col ? Scalar(M_SQRT2) : Scalar(0);
    }
};



template <typename Factor1, typename Factor2, typename Derived_>
struct EuclideanEmbedding_t<Tensor2Antisymmetric_t<Factor1,Factor2,Derived_> >
    :
    public Tensor_i<EuclideanEmbedding_t<Tensor2Antisymmetric_t<Factor1,Factor2,Derived_> >,
                    TypeList_t<Tensor2Antisymmetric_t<Factor1,Factor2,Derived_>,TypeList_t<Tensor2Antisymmetric_t<Factor1,Factor2,Derived_> > >,
                    Tensor2Antisymmetric_t<Factor1,Factor2,Derived_>::DIM*Tensor2Antisymmetric_t<Factor1,Factor2,Derived_>::DIM>
{
    typedef Tensor_i<EuclideanEmbedding_t<Tensor2Antisymmetric_t<Factor1,Factor2,Derived_> >,
                     TypeList_t<Tensor2Antisymmetric_t<Factor1,Factor2,Derived_>,TypeList_t<Tensor2Antisymmetric_t<Factor1,Factor2,Derived_> > >,
                     Tensor2Antisymmetric_t<Factor1,Factor2,Derived_>::DIM*Tensor2Antisymmetric_t<Factor1,Factor2,Derived_>::DIM> Parent_Tensor_i;
    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    using Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::Index Index;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::DEGREE;
    typedef Tensor2Antisymmetric_t<Factor1,Factor2,Derived_> Tensor2Antisymmetric; // this is the vector type that's being embedded

    Scalar operator [] (MultiIndex const &m) const
    {
        //return m.template el<0>() == m.template el<1>() ? Scalar(M_SQRT2) : Scalar(0);
    }

    // this SHOULD be inconvenient and ugly to call.  it should be used ONLY when you know for certain that 0 <= i < DIM
    Scalar component_access_without_range_check (Uint32 i) const
    {
        Uint32 row = i / Tensor2Antisymmetric::DIM;
        Uint32 col = i % Tensor2Antisymmetric::DIM;
        return row == col ? Scalar(M_SQRT2) : Scalar(0);
    }
};
*/
} // end of namespace Tenh

#endif // TENH_TENSOR2ANTISYMMETRIC_HPP_
