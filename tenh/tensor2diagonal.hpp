// ///////////////////////////////////////////////////////////////////////////
// tenh/tensor2diagonal.hpp by Victor Dods, created 2013/03/30
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_TENSOR2DIAGONAL_HPP_
#define TENH_TENSOR2DIAGONAL_HPP_

#include <ostream>

#include "tenh/core.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/interface/tensor.hpp"
#include "tenh/meta/typetuple.hpp"
#include "tenh/vector.hpp"

namespace Tenh {

// diagonal 2-tensor
template <typename Factor1_, typename Factor2_, typename Derived_ = NullType>
struct Tensor2Diagonal_t : public Vector_t<typename Factor1_::Scalar,
                                           ((Factor1_::DIM < Factor2_::DIM) ? Factor1_::DIM : Factor2_::DIM),
                                           typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                  Tensor2Diagonal_t<Factor1_,Factor2_,Derived_>,
                                                                  Derived_>::T>,
                           public Tensor_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                  Tensor2Diagonal_t<Factor1_,Factor2_,Derived_>,
                                                                  Derived_>::T,
                                           TypeList_t<Factor1_,TypeList_t<Factor2_> > >
{
    enum { FACTOR_SCALAR_TYPES_ARE_EQUAL = 
        Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename Factor1_::Scalar,typename Factor2_::Scalar>::v>::v };

    typedef Vector_t<typename Factor1_::Scalar,
                     (Factor1_::DIM < Factor2_::DIM ? Factor1_::DIM : Factor2_::DIM),
                     typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                            Tensor2Diagonal_t<Factor1_,Factor2_,Derived_>,
                                            Derived_>::T> Parent_Vector_t;
    typedef Tensor_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                     Tensor2Diagonal_t<Factor1_,Factor2_,Derived_>,
                                                                     Derived_>::T,
                     TypeList_t<Factor1_,TypeList_t<Factor2_> > > Parent_Tensor_i;
    typedef typename Parent_Vector_t::Scalar Scalar;
    using Parent_Vector_t::DIM;
    typedef typename Parent_Vector_t::Derived Derived;
    typedef typename Parent_Vector_t::Index Index;
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::DEGREE;

    Tensor2Diagonal_t (WithoutInitialization const &w) : Parent_Vector_t(w) { }
    Tensor2Diagonal_t (Scalar fill_with) : Parent_Vector_t(fill_with) { }

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

    using Parent_Vector_t::operator[];

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

        if (!Factor1::component_corresponds_to_memory_location(i1)
            || !Factor2::component_corresponds_to_memory_location(i2))
            return Scalar(0);

        MultiIndex m(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2));
        if (!component_corresponds_to_memory_location(m))
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
        if (i1.is_at_end() || i2.is_at_end())
            throw std::invalid_argument("index/indices out of range");

        if (!Factor1::component_corresponds_to_memory_location(i1)
            || !Factor2::component_corresponds_to_memory_location(i2))
            throw std::invalid_argument("this tensor component is not writable");

        MultiIndex m(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2));
        if (!component_corresponds_to_memory_location(m))
            throw std::invalid_argument("this tensor component is not writable");

        // write to the component, but divide through by the total scale factor for the component.
        operator[](vector_index_of(m)) = s / (Factor1::scalar_factor_for_component(i1) 
                                              * Factor2::scalar_factor_for_component(i2));
    }
    using Parent_Vector_t::component_corresponds_to_memory_location;
    using Parent_Vector_t::scalar_factor_for_component;
    using Parent_Vector_t::vector_index_of;
    // the diagonal is not stored in memory -- all components are zero.
    static bool component_corresponds_to_memory_location (MultiIndex const &m)
    {
        // only diagonal elements correspond to memory locations
        return m.template el<0>().value() == m.template el<1>().value(); 
    }
    
    // the diagonal components have a scale factor of 1, all others have 0
    static Scalar scalar_factor_for_component (MultiIndex const &m)
    {
        if (m.template el<0>().value() == m.template el<1>().value()) // diagonal component
            return Scalar(1);
        else // off diagonal
            return Scalar(0); // not actually used, but is here for completeness
    }
    
    // this should return iff component_corresponds_to_memory_location(c) returns true.
    static Index vector_index_of (CompoundIndex const &c)
    {
        return Index::range_unchecked(rowcol_index_to_contiguous_index(m.template el<0>().value(),
                                                                       m.template el<1>().value()));
    }

    static std::string type_as_string ()
    {
        // TODO: return Derived's type_as_string value?
//         if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
//             return "Tensor2Diagonal_t<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + '>';
//         else
//             return Derived::type_as_string();
        // for now, just return this type string
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

template <typename Factor1, typename Factor2>
std::ostream &operator << (std::ostream &out, Tensor2Diagonal_t<Factor1,Factor2> const &t)
{
    typedef Tensor2Diagonal_t<Factor1,Factor2> Tensor2Diagonal;

    if (Tensor2Diagonal::DIM == 0)
        return out << "[]";

    typename Tensor2Diagonal::MultiIndex m;
    out << '\n';
    for (typename Factor1::Index i; i.is_not_at_end(); ++i)
    {
        out << '[';
        for (typename Factor2::Index j; j.is_not_at_end(); ++j)
        {
            out << t[m] << '\t';
            ++m;
        }
        out << "]\n";
    }
    return out;
}

} // end of namespace Tenh

#endif // TENH_TENSOR2DIAGONAL_HPP_
