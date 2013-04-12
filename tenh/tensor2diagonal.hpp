// ///////////////////////////////////////////////////////////////////////////
// tenh/tensor2diagonal.hpp by Victor Dods, created 2013/03/30
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_TENSOR2DIAGONAL_HPP_
#define TENH_TENSOR2DIAGONAL_HPP_

#include <ostream>

#include "tenh/core.hpp"
#include "tenh/interface/tensor.hpp"
#include "tenh/meta/typetuple.hpp"
#include "tenh/vector.hpp"

namespace Tenh {

// diagonal 2-tensor (its off-diagonal entries are zero)
template <typename Factor1_, typename Factor2_, typename Derived_ = NullType>
struct Tensor2Diagonal_t : public Tensor_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                  Tensor2Diagonal_t<Factor1_,Factor2_,Derived_>,
                                                                  Derived_>::T,
                                           TypeList_t<Factor1_,TypeList_t<Factor2_> >,
                                           ((Factor1_::DIM < Factor2_::DIM) ? Factor1_::DIM : Factor2_::DIM)>,
                           private Array_t<typename Factor1_::Scalar,
                                           ((Factor1_::DIM < Factor2_::DIM) ? Factor1_::DIM : Factor2_::DIM),
                                           typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                  Tensor2Diagonal_t<Factor1_,Factor2_,Derived_>,
                                                                  Derived_>::T>
                           // privately inherited because the use of Array_t is an implementation detail
{
    enum { FACTOR_SCALAR_TYPES_ARE_EQUAL =
        STATIC_ASSERT_AS_RVALUE((Lvd::Meta::TypesAreEqual<typename Factor1_::Scalar,typename Factor2_::Scalar>::v), FACTOR_SCALAR_TYPES_ARE_EQUAL) };

    typedef Tensor_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                     Tensor2Diagonal_t<Factor1_,Factor2_,Derived_>,
                                                                     Derived_>::T,
                     TypeList_t<Factor1_,TypeList_t<Factor2_> >,
                     ((Factor1_::DIM < Factor2_::DIM) ? Factor1_::DIM : Factor2_::DIM)> Parent_Tensor_i;
    typedef Array_t<typename Factor1_::Scalar,
                    ((Factor1_::DIM < Factor2_::DIM) ? Factor1_::DIM : Factor2_::DIM),
                    typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                            Tensor2Diagonal_t<Factor1_,Factor2_,Derived_>,
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

    Tensor2Diagonal_t (WithoutInitialization const &w) : Parent_Array_t(w) { }
    Tensor2Diagonal_t (Scalar fill_with) : Parent_Array_t(fill_with) { }
    Tensor2Diagonal_t (Scalar const &x0, Scalar const &x1) : Parent_Array_t(x0, x1) { }
    Tensor2Diagonal_t (Scalar const &x0, Scalar const &x1, Scalar const &x2) : Parent_Array_t(x0, x1, x2) { }
    Tensor2Diagonal_t (Scalar const &x0, Scalar const &x1, Scalar const &x2, Scalar const &x3) : Parent_Array_t(x0, x1, x2, x3) { }

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

    using Parent_Array_t::operator[];
    using Parent_Array_t::data_size_in_bytes;
    using Parent_Array_t::data_pointer;

    // using two indices in a Tensor2Diagonal_t is breaking apart the Index type and using it
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
        assert(i1.is_not_at_end() && i2.is_not_at_end() && "you used Index_t(x, DONT_CHECK_RANGE) inappropriately");

        if (Factor1::component_is_immutable_zero(i1) || Factor2::component_is_immutable_zero(i2))
            return Scalar(0);

        MultiIndex m(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2));
        if (component_is_immutable_zero(m))
            return Scalar(0);

        return Factor1::scalar_factor_for_component(i1) *
               Factor2::scalar_factor_for_component(i2) *
               operator[](vector_index_of(m));
    }

    // write 2-tensor components -- will throw if a component doesn't correspond to a memory location
    // Index1 could be Factor1::Index or Factor1::MultiIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::MultiIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    void set_component (Index1 const &i1, Index2 const &i2, Scalar s)
    {
        assert(i1.is_not_at_end() && i2.is_not_at_end() && "you used Index_t(x, DONT_CHECK_RANGE) inappropriately");

        if (Factor1::component_is_immutable_zero(i1) || Factor2::component_is_immutable_zero(i2))
            throw std::invalid_argument("this tensor component is not writable");

        MultiIndex m(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2));
        if (component_is_immutable_zero(m))
            throw std::invalid_argument("this tensor component is not writable");

        // write to the component, but divide through by the total scale factor for the component.
        operator[](vector_index_of(m))
            = s / (Factor1::scalar_factor_for_component(i1) * Factor2::scalar_factor_for_component(i2));
    }
    using Parent_Tensor_i::component_is_immutable_zero;
    using Parent_Tensor_i::scalar_factor_for_component;
    using Parent_Tensor_i::vector_index_of;
    // the diagonal is not stored in memory -- all components are zero.
    static bool component_is_immutable_zero (MultiIndex const &m)
    {
        // only diagonal elements correspond to memory locations
        return m.template el<0>().value() != m.template el<1>().value();
    }

    // the diagonal components have a scale factor of 1, all others have 0
    static Scalar scalar_factor_for_component (MultiIndex const &m)
    {
        if (m.template el<0>().value() == m.template el<1>().value()) // diagonal component
            return Scalar(1);
        else // off diagonal
            return Scalar(0); // not actually used, but is here for completeness
    }

    // this should return iff !component_is_immutable_zero(c) and otherwise throw.
    static Index vector_index_of (MultiIndex const &m)
    {
        return Index(rowcol_index_to_contiguous_index(m.template el<0>().value(),
                                                      m.template el<1>().value()),
                     DONT_CHECK_RANGE);
    }

    static std::string type_as_string ()
    {
        if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
            return "Tensor2Diagonal_t<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + '>';
        else
            return "Tensor2Diagonal_t<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + ','
                    + TypeStringOf_t<Derived>::eval() + '>';
    }

private:

    // functions between the indexing schemes -- multi-index is (row,col) with row == col and vector index is contiguous.
    static Uint32 rowcol_index_to_contiguous_index (Uint32 row, Uint32 col)
    {
        if (row != col)
            throw std::invalid_argument("row must be equal to col");
        else
            return row;
    }
    static void contiguous_index_to_rowcol_index (Uint32 i, Uint32 &row, Uint32 &col)
    {
        row = i*(Factor2::DIM+1);
        col = row;
    }
};

} // end of namespace Tenh

#endif // TENH_TENSOR2DIAGONAL_HPP_
