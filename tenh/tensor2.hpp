// ///////////////////////////////////////////////////////////////////////////
// tenh/tensor2.hpp by Victor Dods, created 2013/03/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_TENSOR2_HPP_
#define TENH_TENSOR2_HPP_

#include "tenh/core.hpp"
#include "tenh/expression_templates.hpp"
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
                   public Array_t<typename Factor1_::Scalar,DimensionOfTensorProduct_t<TypeList_t<Factor1_,TypeList_t<Factor2_> > >::V>
{
    enum { FACTOR_SCALAR_TYPES_ARE_EQUAL 
        = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename Factor1_::Scalar,typename Factor2_::Scalar>::v>::v };

    typedef Tensor_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                     Tensor2_t<Factor1_,Factor2_,Derived_>,
                                                                     Derived_>::T,
                                            TypeList_t<Factor1_,TypeList_t<Factor2_> >,
                                            DimensionOfTensorProduct_t<TypeList_t<Factor1_,TypeList_t<Factor2_> > >::V> Parent_Tensor_i;
    typedef Array_t<typename Factor1_::Scalar,DimensionOfTensorProduct_t<TypeList_t<Factor1_,TypeList_t<Factor2_> > >::V> Parent_Array_t;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    using Parent_Tensor_i::DIM;
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

    using Parent_Array_t::component_access_without_range_check;
    // for access to particular components -- have to NOT do "using Parent_Tensor_i::operator[]" because of ambiguous overload
    using Parent_Tensor_i::Parent_Vector_i::operator[];

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
        // NOTE: this construction is unnecessary to the code, but IS necessary to the compile-time type checking
        // the compiler should optimize it out anyway.
        MultiIndex x(m);
//         typename Factor1::Index(m.template el<0>());
//         typename Factor2::Index(m.template el<1>());
        if (m.is_at_end())
        {
            throw std::invalid_argument("index out of range");
        }
        else
        {
            return this->component_access_without_range_check(m.value());
        }
    }

    // these are what provide indexed expressions -- via expression templates
    using Parent_Tensor_i::operator();
    
    // access 2-tensor components
    // Index1 could be Factor1::Index or Factor1::MultiIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::MultiIndex (checked by its use in the other functions)
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
                   this->component_access_without_range_check(
                        vector_index_of(MultiIndex(Factor1::vector_index_of(i1), 
                                                   Factor2::vector_index_of(i2))).value());
    }
    // write 2-tensor components -- will throw if a component doesn't correspond to a memory location
    // Index1 could be Factor1::Index or Factor1::MultiIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::MultiIndex (checked by its use in the other functions)
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

        MultiIndex m(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2));
        // write to the component, but divide through by the total scale factor for the component.
        this->component_access_without_range_check(vector_index_of(m).value()) 
            = s / (Factor1::scalar_factor_for_component(i1) * Factor2::scalar_factor_for_component(i2));
    }
    using Parent_Tensor_i::component_corresponds_to_memory_location;
    using Parent_Tensor_i::scalar_factor_for_component;
    using Parent_Tensor_i::vector_index_of;
    // all components are stored in memory (in the array m), and have scalar factor 1
    static bool component_corresponds_to_memory_location (MultiIndex const &m) { return true; }
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return Scalar(1); }
    static Index vector_index_of (MultiIndex const &m) { return Index::range_unchecked(m.value()); }

    using Parent_Array_t::data_size_in_bytes;
    using Parent_Array_t::data_pointer;

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
    
    using Parent_Array_t::operator[]; // this should not be publicly accessible
};

} // end of namespace Tenh

#endif // TENH_TENSOR2_HPP_
