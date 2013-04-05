// ///////////////////////////////////////////////////////////////////////////
// tenh/tensor2.hpp by Victor Dods, created 2013/03/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_TENSOR2_HPP_
#define TENH_TENSOR2_HPP_

#include <ostream>

#include "tenh/core.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/tensor.hpp"
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

// general 2-tensor with no symmetries -- most general type of 2-tensor
template <typename Factor1_, typename Factor2_, typename Derived_ = NullType>
struct Tensor2_t : public Vector_t<typename Factor1_::Scalar,
                                   Factor1_::DIM*Factor2_::DIM,
                                   typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                          Tensor2_t<Factor1_,Factor2_,Derived_>,
                                                          Derived_>::T>,
                   public Tensor_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                          Tensor2_t<Factor1_,Factor2_,Derived_>,
                                                          Derived_>::T,
                                   TypeList_t<Factor1_,TypeList_t<Factor2_> > >
{
    enum { FACTOR_SCALAR_TYPES_ARE_EQUAL 
        = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename Factor1_::Scalar,typename Factor2_::Scalar>::v>::v };

    typedef Vector_t<typename Factor1_::Scalar,
                     Factor1_::DIM*Factor2_::DIM,
                     typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                            Tensor2_t<Factor1_,Factor2_,Derived_>,
                                            Derived_>::T> Parent_Vector_t;
    typedef Tensor_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                     Tensor2_t<Factor1_,Factor2_,Derived_>,
                                                                     Derived_>::T,
                                            TypeList_t<Factor1_,TypeList_t<Factor2_> > > Parent_Tensor_i;
    typedef typename Parent_Vector_t::Scalar Scalar;
    using Parent_Vector_t::DIM;
    typedef typename Parent_Vector_t::Derived Derived;
    typedef typename Parent_Vector_t::Index Index;
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;
    typedef typename TypeTuple_t<Factor1,Factor2>::T FactorTypeList;
    typedef typename TypeTuple_t<typename Factor1::Index,typename Factor2::Index>::T FactorIndexTypeList;
    typedef CompoundIndex_t<FactorIndexTypeList> CompoundIndex;

    Tensor2_t (WithoutInitialization const &w) : Parent_Vector_t(w) { }
    Tensor2_t (Scalar fill_with) : Parent_Vector_t(fill_with) { }

    template <typename BundleIndexTypeList, typename BundledIndex>
    static CompoundIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        typedef typename BundleIndexTypeList::template El_t<0>::T Index1;
        typedef typename BundleIndexTypeList::template El_t<1>::T Index2;
        // this is just to check that there is a valid conversion to the requested CompoundIndex type.
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
        return CompoundIndex_t<BundleIndexTypeList>(Index1(row), Index2(col));
    }

    // type conversion operator for canonical coercion to the Factor1 or Factor2 factor type when the
    // tensor is Factor1 \otimes OneDimVectorSpace  or  OneDimVectorSpace \otimes Factor2.
//     template <typename Factor>
//     operator Factor const & () const
//     {
//         Lvd::Meta::Assert<(Lvd::Meta::TypesAreEqual<Factor,Factor1>::v && Factor2::DIM == 1) || (Lvd::Meta::TypesAreEqual<Factor,Factor2>::v && Factor1::DIM == 1)>();
//         return *reinterpret_cast<Factor const *>(&Parent_Vector_t::m[0]);
//     }
//     operator Factor1 const & () const
//     {
//         Lvd::Meta::Assert<(Factor2::DIM == 1)>();
//         return *reinterpret_cast<Factor1 const *>(&Parent_Vector_t::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
//     }
    Factor1 const &as_factor1 () const
    {
        Lvd::Meta::Assert<(Factor2::DIM == 1)>();
        return *reinterpret_cast<Factor1 *>(&Parent_Vector_t::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
    }
    // this could be implemented as "operator Factor1 & ()" but it would be bad to make implicit casts that can be used to change the value of this.
    Factor1 &as_factor1 ()
    {
        Lvd::Meta::Assert<(Factor2::DIM == 1)>();
        return *reinterpret_cast<Factor1 *>(&Parent_Vector_t::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
    }
    // type conversion operator for canonical coercion to the Factor2 factor type when the
    // tensor is a tensor product of a 1-dimensional vector space with Factor2.
//     operator Factor2 const & () const
//     {
//         Lvd::Meta::Assert<(Factor1::DIM == 1)>();
//         return *reinterpret_cast<Factor2 const *>(&Parent_Vector_t::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
//     }
    Factor2 const &as_factor2 () const
    {
        Lvd::Meta::Assert<(Factor1::DIM == 1)>();
        return *reinterpret_cast<Factor2 *>(&Parent_Vector_t::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
    }
    // this could be implemented as "operator Factor2 & ()" but it would be bad to make implicit casts that can be used to change the value of this.
    Factor2 &as_factor2 ()
    {
        Lvd::Meta::Assert<(Factor1::DIM == 1)>();
        return *reinterpret_cast<Factor2 *>(&Parent_Vector_t::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
    }

    using Parent_Vector_t::operator[];

    // Index1 could be Factor1::Index or Factor1::CompoundIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::CompoundIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    Scalar operator [] (CompoundIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &c) const
    {
//         // NOTE: this construction is unnecessary to the code, but IS necessary to the compile-time type checking
//         // the compiler should optimize it out anyway.
//         typename Factor1::Index(c.template el<0>());
//         typename Factor2::Index(c.template el<1>());
        return component(c.template el<0>(), c.template el<1>());
    }
    template <typename Index1, typename Index2>
    Scalar &operator [] (CompoundIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &c)
    {
        // NOTE: this construction is unnecessary to the code, but IS necessary to the compile-time type checking
        // the compiler should optimize it out anyway.
        CompoundIndex x(c);
//         typename Factor1::Index(c.template el<0>());
//         typename Factor2::Index(c.template el<1>());
        if (c.is_at_end())
        {
            throw std::invalid_argument("index out of range");
        }
        else
        {
            return this->component_access_without_range_check(c.value());
        }
    }

    // these are what provide indexed expressions -- via expression templates
    using Parent_Vector_t::operator();
    using Parent_Tensor_i::operator();
    
    // access 2-tensor components
    // Index1 could be Factor1::Index or Factor1::CompoundIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::CompoundIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    Scalar component (Index1 const &i1, Index2 const &i2) const
    {
        if (i1.is_at_end() || i2.is_at_end())
        {
            throw std::invalid_argument("index/indices out of range");
        }

        if (!Factor1::component_corresponds_to_memory_location(i1) 
            || !Factor2::component_corresponds_to_memory_location(i2))
        {
            return Scalar(0);
        }
        else
        {
            return Factor1::scalar_factor_for_component(i1) *
                   Factor2::scalar_factor_for_component(i2) *
                   operator[](vector_index_of(CompoundIndex(Factor1::vector_index_of(i1), 
                                                            Factor2::vector_index_of(i2))));
        }
    }
    // write 2-tensor components -- will throw if a component doesn't correspond to a memory location
    // Index1 could be Factor1::Index or Factor1::CompoundIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::CompoundIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    void set_component (Index1 const &i1, Index2 const &i2, Scalar s)
    {
        if (i1.is_at_end() || i2.is_at_end())
        {
            throw std::invalid_argument("index/indices out of range");
        }

        if (!Factor1::component_corresponds_to_memory_location(i1) 
            || !Factor2::component_corresponds_to_memory_location(i2))
        {
            throw std::invalid_argument("this tensor component is not writable");
        }

        CompoundIndex c(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2));
        // write to the component, but divide through by the total scale factor for the component.
        operator[](vector_index_of(c)) = s / (Factor1::scalar_factor_for_component(i1) 
                                              * Factor2::scalar_factor_for_component(i2));
    }
    using Parent_Vector_t::component_corresponds_to_memory_location;
    using Parent_Vector_t::scalar_factor_for_component;
    using Parent_Vector_t::vector_index_of;
    // all components are stored in memory (in the array m), and have scalar factor 1
    static bool component_corresponds_to_memory_location (CompoundIndex const &c) { return true; }
    static Scalar scalar_factor_for_component (CompoundIndex const &c) { return Scalar(1); }
    static Index vector_index_of (CompoundIndex const &c) { return Index::range_unchecked(c.value()); }

    static std::string type_as_string ()
    {
        // TODO: return Derived's type_as_string value?
//         if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
//             return "Tensor2_t<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + '>';
//         else
//             return Derived::type_as_string();
        // for now, just return this type string
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

    // functions between the indexing schemes -- compound index is (row,col) with row > col and vector index is contiguous.
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

template <typename Factor1, typename Factor2, typename Derived>
std::ostream &operator << (std::ostream &out, Tensor2_t<Factor1,Factor2,Derived> const &t)
{
    typedef Tensor2_t<Factor1,Factor2,Derived> Tensor2;

    if (Tensor2::DIM == 0)
    {
        return out << "[]";
    }

    typename Tensor2::CompoundIndex c;
    out << '\n';
    for (typename Factor1::Index i; i.is_not_at_end(); ++i)
    {
        out << '[';
        for (typename Factor2::Index j; j.is_not_at_end(); ++j)
        {
            out << t[c] << '\t';
            ++c;
        }
        out << "]\n";
    }
    return out;
}

} // end of namespace Tenh

#endif // TENH_TENSOR2_HPP_
