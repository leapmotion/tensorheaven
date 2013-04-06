// ///////////////////////////////////////////////////////////////////////////
// tenh/tensor2symmetric.hpp by Victor Dods, created 2013/03/20
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_TENSOR2SYMMETRIC_HPP_
#define TENH_TENSOR2SYMMETRIC_HPP_

#include <cmath> // TEMP until rowcol/contiguous index map is implemented as a lookup table
#include <ostream>

#include "tenh/core.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/interface/tensor.hpp"
#include "tenh/naturalpairing.hpp"
#include "tenh/meta/typetuple.hpp"
#include "tenh/vector.hpp"

namespace Tenh {

// symmetric 2-tensor (it is equal to its transpose)
template <typename Factor1_, typename Factor2_ = Factor1_, typename Derived_ = NullType>
struct Tensor2Symmetric_t : public Vector_t<typename Factor1_::Scalar,
                                            ((Factor1_::DIM+1)*Factor1_::DIM)/2,
                                            typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                   Tensor2Symmetric_t<Factor1_,Factor2_,Derived_>,
                                                                   Derived_>::T>,
                            public Tensor_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                   Tensor2Symmetric_t<Factor1_,Factor2_,Derived_>,
                                                                   Derived_>::T,
                                            TypeList_t<Factor1_,TypeList_t<Factor2_> > >
{
    enum { FACTOR1_AND_FACTOR2_MUST_BE_IDENTICAL = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<Factor1_,Factor2_>::v>::v };

    typedef Vector_t<typename Factor1_::Scalar,
                     ((Factor1_::DIM+1)*Factor1_::DIM)/2,
                     typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                            Tensor2Symmetric_t<Factor1_,Factor2_,Derived_>,
                                            Derived_>::T> Parent_Vector_t;
    typedef Tensor_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                     Tensor2Symmetric_t<Factor1_,Factor2_,Derived_>,
                                                                     Derived_>::T,
                     TypeList_t<Factor1_,TypeList_t<Factor2_> > > Parent_Tensor_i;
    typedef typename Parent_Vector_t::Scalar Scalar;
    using Parent_Vector_t::DIM;
    typedef typename Parent_Vector_t::Derived Derived;
    typedef typename Parent_Vector_t::Index Index;
    typedef Factor1_ Factor;
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::DEGREE;
    static Uint32 const DIAGONAL_COMPONENT_COUNT = Factor::DIM;
    static Uint32 const STRICTLY_LOWER_TRIANGULAR_COMPONENT_COUNT = ((Factor::DIM-1)*Factor::DIM)/2;

    Tensor2Symmetric_t (WithoutInitialization const &w) : Parent_Vector_t(w) { }
    Tensor2Symmetric_t (Scalar fill_with) : Parent_Vector_t(fill_with) { }

    template <typename BundleIndexTypeList, typename BundledIndex>
    static MultiIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        typedef typename BundleIndexTypeList::template El_t<0>::T Index1;
        typedef typename BundleIndexTypeList::template El_t<1>::T Index2;
        // this is just to check that there is a valid conversion to the requested MultiIndex_t type.
        // it doesn't actually produce any side-effects, and should be optimized out.
        {
            Lvd::Meta::Assert<BundleIndexTypeList::LENGTH == 2>();
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

    // TODO: because Factor1 and Factor2 are identical, it doesn't make sense to
    // have a type coercion to either one unless they are 1-dimensional, in which case
    // it can be a type coercion to the Scalar type.

    // TODO: because the diagonal is indexed contiguously last, there is an easy type coercion to Tensor2Diagonal_t

    // dumb, but the compiler wouldn't inherit implicitly, and won't parse a "using" statement
    Scalar const &operator [] (Index const &i) const { return Parent_Vector_t::operator[](i); }
    // dumb, but the compiler wouldn't inherit implicitly, and won't parse a "using" statement
    Scalar &operator [] (Index const &i) { return Parent_Vector_t::operator[](i); }

    // using two indices in a Tensor2Symmetric_t is breaking apart the Index type and using it
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
    using Parent_Vector_t::operator();
    using Parent_Tensor_i::operator();

    // access 2-tensor components
    // Index1 could be Factor1::Index or Factor1::MultiIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::MultiIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    Scalar component (Index1 const &i1, Index2 const &i2) const
    {
        if (i1.is_at_end() || i2.is_at_end())
            throw std::invalid_argument("index/indices out of range");

        if (!Factor1::component_corresponds_to_memory_location(i1) || !Factor2::component_corresponds_to_memory_location(i2))
            return Scalar(0);

        MultiIndex m(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2));
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
        if (i1.is_at_end() || i2.is_at_end())
            throw std::invalid_argument("index/indices out of range");

        if (!Factor1::component_corresponds_to_memory_location(i1) || !Factor2::component_corresponds_to_memory_location(i2))
            throw std::invalid_argument("this tensor component is not writable");

        MultiIndex m(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2));
        // write to the component, but divide through by the total scale factor for the component.
        operator[](vector_index_of(m)) = s / (Factor1::scalar_factor_for_component(i1) * Factor2::scalar_factor_for_component(i2));
    }
    using Parent_Vector_t::component_corresponds_to_memory_location;
    using Parent_Vector_t::scalar_factor_for_component;
    using Parent_Vector_t::vector_index_of;
    // all components correspond exactly to a memory location
    static bool component_corresponds_to_memory_location (MultiIndex const &m) { return true; }
    // all components have a scale factor of 1
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return Scalar(1); }
    // this should return iff component_corresponds_to_memory_location(m) returns true.
    static Index vector_index_of (MultiIndex const &m)
    {
        return Index::range_unchecked(rowcol_index_to_contiguous_index(m.template el<0>().value(), m.template el<1>().value()));
    }

    static std::string type_as_string ()
    {
        if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
            return "Tensor2Symmetric_t<" + TypeStringOf_t<Factor>::eval() + '>';
        else
            return "Tensor2Symmetric_t<" + TypeStringOf_t<Factor>::eval() + ',' + TypeStringOf_t<Derived>::eval() + '>';
    }

private:

    // functions between the indexing schemes -- multi-index is (row,col) with row >= col and vector index is contiguous.
    static Uint32 rowcol_index_to_contiguous_index (Uint32 row, Uint32 col)
    {
        // strict lower triangle elements are stored contiguously in row-major order:
        // [ d0  .  . ... ]
        // [  0 d1  . ... ]
        // [  1  2 d2 ... ]
        // [  3  4  5 ... ]
        // ...
        // the index of the first element in row R is the (R-1)th triangular number, so the mapping
        // (row,col) to contiguous index i is i := r*(r-1)/2 + c.
        // the diagonal is indexed contiguously after the strictly lower triangular components.
        if (row == col)
            return STRICTLY_LOWER_TRIANGULAR_COMPONENT_COUNT + row;

        if (row < col)
            std::swap(row, col);//throw std::invalid_argument("row must be greater than col");
        // now it is the same as antisymmetric indexing
        return row*(row-1)/2 + col;
    }
    static void contiguous_index_to_rowcol_index (Uint32 i, Uint32 &row, Uint32 &col)
    {
        // TODO: implement as lookup table
        if (i >= STRICTLY_LOWER_TRIANGULAR_COMPONENT_COUNT)
        {
            row = i - STRICTLY_LOWER_TRIANGULAR_COMPONENT_COUNT;
            col = row;
        }
        else // same as antisymmetric indexing
        {
            // given i, row = floor((1 + sqrt(1+8*i))/2) and col = i - r*(r-1)/2.
            row = Uint32(std::floor(0.5f + std::sqrt(0.25f + 2.0f*i)));
            col = i - row*(row-1)/2;
        }
    }
};

// template specialization for the natural pairing in this particular coordinatization of Tensor2Symmetric_t
template <typename Factor>
struct NaturalPairing_t<Tensor2Symmetric_t<Factor> >
{
    typedef Tensor2Symmetric_t<Factor> Tensor2Symmetric;
    typedef typename Tensor2Symmetric::Scalar Scalar;
    typedef typename Tensor2Symmetric::Index Index;

    static Scalar component (Index const &i)
    {
        if (i.value() < Tensor2Symmetric::DIAGONAL_COMPONENT_COUNT)
            return Scalar(1); // the diagonal components occur only once (in the component matrix)
        else
            return Scalar(2); // but the off-diagonal components occur twice (in the component matrix)
    }
};

} // end of namespace Tenh

#endif // TENH_TENSOR2SYMMETRIC_HPP_
