// ///////////////////////////////////////////////////////////////////////////
// tenh/tensor2.hpp by Victor Dods, created 2013/03/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_TENSOR2_HPP_
#define TENH_TENSOR2_HPP_

#include "tenh/core.hpp"
#include "tenh/euclideanembedding.hpp"
#include "tenh/innerproduct.hpp"
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

template <typename Factor1, typename Factor2>
struct BasisOfTensor2_t
{
    static std::string type_as_string () { return "BasisOfTensor2_t<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + '>'; }
};

// general 2-tensor with no symmetries -- most general type of 2-tensor
// the Basis argument is available to override to e.g. StandardEuclideanBasis
// in the case of EuclideanEmbedding_t.  If you override Basis with something
// besides this default or StandardEuclideanBasis, you'll need to provide your
// own implementation of InnerProduct_t<Tensor2_t<Factor1,Factor2,Basis,Derived> >.
template <typename Factor1_, typename Factor2_, typename Basis_ = BasisOfTensor2_t<Factor1_,Factor2_>, typename Derived_ = NullType>
struct Tensor2_t
    :
    public Tensor_i<typename DerivedType_t<Derived_,Tensor2_t<Factor1_,Factor2_,Basis_,Derived_> >::T,
                    TypeList_t<Factor1_,TypeList_t<Factor2_> >,
                    DimensionOfTensorProduct_t<TypeList_t<Factor1_,TypeList_t<Factor2_> > >::V,
                    Basis_>,
    private Array_t<typename Factor1_::Scalar,
                    DimensionOfTensorProduct_t<TypeList_t<Factor1_,TypeList_t<Factor2_> > >::V,
                    typename DerivedType_t<Derived_,Tensor2_t<Factor1_,Factor2_,Basis_,Derived_> >::T>
    // privately inherited because the use of Array_t is an implementation detail
{
    enum { STATIC_ASSERT_IN_ENUM((Lvd::Meta::TypesAreEqual<typename Factor1_::Scalar,typename Factor2_::Scalar>::v), FACTOR_SCALAR_TYPES_ARE_EQUAL) };

    typedef Tensor_i<typename DerivedType_t<Derived_,Tensor2_t<Factor1_,Factor2_,Basis_,Derived_> >::T,
                     TypeList_t<Factor1_,TypeList_t<Factor2_> >,
                     DimensionOfTensorProduct_t<TypeList_t<Factor1_,TypeList_t<Factor2_> > >::V,
                     Basis_> Parent_Tensor_i;
    typedef Array_t<typename Factor1_::Scalar,
                    DimensionOfTensorProduct_t<TypeList_t<Factor1_,TypeList_t<Factor2_> > >::V,
                    typename DerivedType_t<Derived_,Tensor2_t<Factor1_,Factor2_,Basis_,Derived_> >::T> Parent_Array_t;
    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    static Uint32 const DIM = Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::Basis Basis;
    typedef Tensor2_t<typename Factor1_::WithStandardEuclideanBasis,
                      typename Factor2_::WithStandardEuclideanBasis,
                      StandardEuclideanBasis,
                      Derived_> WithStandardEuclideanBasis; // TEMP KLUDGE -- recursively convert to StandardEuclideanBasis
    typedef typename Parent_Tensor_i::Index Index;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::DEGREE;
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;
    static bool const IS_TENSOR2_T = true;

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
        return MultiIndex_t<BundleIndexTypeList>(Index1(row, DONT_CHECK_RANGE), Index2(col, DONT_CHECK_RANGE));
    }

    using Parent_Array_t::operator[];
    using Parent_Array_t::data_size_in_bytes;
    using Parent_Array_t::data_pointer;

    // Index1 could be Factor1::Index or Factor1::MultiIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::MultiIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    Scalar operator [] (MultiIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &m) const
    {
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
        std::string basis_string;
        if (!Lvd::Meta::TypesAreEqual<Basis,BasisOfTensor2_t<Factor1,Factor2> >())
            basis_string = ',' + TypeStringOf_t<Basis>::eval();

        std::string derived_string;
        if (!Lvd::Meta::TypesAreEqual<Derived_,NullType>())
            derived_string = ',' + TypeStringOf_t<Derived>::eval();

        return "Tensor2_t<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + basis_string + derived_string + '>';
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

    friend struct InnerProduct_t<Tensor2_t,Basis>;
};

// template specialization for the inner product in this particular coordinatization of Tensor2_t
// (specified by BasisOfTensor2_t<Factor1,Factor2>).
template <typename Factor1, typename Factor2, typename Derived>
struct InnerProduct_t<Tensor2_t<Factor1,Factor2,BasisOfTensor2_t<Factor1,Factor2>,Derived>,BasisOfTensor2_t<Factor1,Factor2> >
{
    typedef Tensor2_t<Factor1,Factor2,BasisOfTensor2_t<Factor1,Factor2>,Derived> Tensor2;
    typedef typename Tensor2::Scalar Scalar;
    typedef typename Tensor2::Index Index;

    static Scalar component (Index const &i)
    {
        Uint32 row;
        Uint32 col;
        Tensor2::contiguous_index_to_rowcol_index(i.value(), row, col);
        return InnerProduct_t<Factor1,typename Factor1::Basis>::component(typename Factor1::Index(row, DONT_CHECK_RANGE)) *
               InnerProduct_t<Factor2,typename Factor2::Basis>::component(typename Factor2::Index(col, DONT_CHECK_RANGE));
    }
};

// template <typename Factor1, typename Factor2, typename Derived>
// struct InnerProductInverse_t<Tensor2_t<Factor1,Factor2,BasisOfTensor2_t<Factor1,Factor2>,Derived>,BasisOfTensor2_t<Factor1,Factor2> >
// {
//     typedef Tensor2_t<Factor1,Factor2,BasisOfTensor2_t<Factor1,Factor2>,Derived> Tensor2;
//     typedef typename Tensor2::Scalar Scalar;
//     typedef typename Tensor2::Index Index;
//
//     static Scalar component (Index const &i)
//     {
//         Uint32 row;
//         Uint32 col;
//         Tensor2::contiguous_index_to_rowcol_index(i.value(), row, col);
//         return InnerProductInverse_t<Factor1,typename Factor1::Basis>::component(typename Factor1::Index(row, DONT_CHECK_RANGE)) *
//                InnerProductInverse_t<Factor2,typename Factor2::Basis>::component(typename Factor2::Index(col, DONT_CHECK_RANGE));
//     }
// };

template <typename TensorFactor1_, typename TensorFactor2_, typename TensorDerived>
struct EuclideanEmbedding_t<Tensor2_t<TensorFactor1_,
                                      TensorFactor2_,
                                      BasisOfTensor2_t<TensorFactor1_,TensorFactor2_>,
                                      TensorDerived> >
    :
    public EuclideanEmbedding_Parent_Tensor_i<Tensor2_t<TensorFactor1_,
                                                        TensorFactor2_,
                                                        BasisOfTensor2_t<TensorFactor1_,TensorFactor2_>,
                                                        TensorDerived> >::T
{
    typedef typename EuclideanEmbedding_Parent_Tensor_i<Tensor2_t<TensorFactor1_,
                                                                  TensorFactor2_,
                                                                  BasisOfTensor2_t<TensorFactor1_,TensorFactor2_>,
                                                                  TensorDerived> >::T Parent_Tensor_i;
    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    using Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::Basis Basis;
    typedef typename Parent_Tensor_i::Index Index;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::DEGREE;
    typedef TensorFactor1_ TensorFactor1;
    typedef TensorFactor2_ TensorFactor2;
    typedef Tensor2_t<TensorFactor1,
                      TensorFactor2,
                      BasisOfTensor2_t<TensorFactor1,TensorFactor2>,
                      TensorDerived> Tensor2;

    Scalar operator [] (MultiIndex const &m) const
    {
        EuclideanEmbedding_t<TensorFactor1> e1;
        EuclideanEmbedding_t<TensorFactor2> e2;
        TypedIndex_t<typename TensorFactor1::WithStandardEuclideanBasis,'i'> i;
        TypedIndex_t<TensorFactor1,'j'> j;
        TypedIndex_t<typename TensorFactor2::WithStandardEuclideanBasis,'k'> k;
        TypedIndex_t<TensorFactor2,'l'> l;
        TypedIndex_t<typename Tensor2::WithStandardEuclideanBasis,'p'> p;
        TypedIndex_t<Tensor2,'q'> q;
        return ((e1(i|j)*e2(k|l)).bundle(i|k,p).bundle(j|l,q))[m];
    }

    // NOTE: these may be unnecessary/undesired, because this type does not represent a vector space
//     using Parent_Tensor_i::component_is_immutable_zero;
//     using Parent_Tensor_i::scalar_factor_for_component;
//     using Parent_Tensor_i::vector_index_of;
    static std::string type_as_string ()
    {
        return "EuclideanEmbedding_t<" + TypeStringOf_t<Tensor2>::eval() + '>';
    }
};

template <typename TensorFactor1_, typename TensorFactor2_, typename TensorDerived>
struct EuclideanEmbeddingInverse_t<Tensor2_t<TensorFactor1_,
                                             TensorFactor2_,
                                             BasisOfTensor2_t<TensorFactor1_,TensorFactor2_>,
                                             TensorDerived> >
    :
    public EuclideanEmbeddingInverse_Parent_Tensor_i<Tensor2_t<TensorFactor1_,
                                                               TensorFactor2_,
                                                               BasisOfTensor2_t<TensorFactor1_,TensorFactor2_>,
                                                               TensorDerived> >::T
{
    typedef typename EuclideanEmbeddingInverse_Parent_Tensor_i<Tensor2_t<TensorFactor1_,
                                                                         TensorFactor2_,
                                                                         BasisOfTensor2_t<TensorFactor1_,TensorFactor2_>,
                                                                         TensorDerived> >::T Parent_Tensor_i;
    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    using Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::Basis Basis;
    typedef typename Parent_Tensor_i::Index Index;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::DEGREE;
    typedef TensorFactor1_ TensorFactor1;
    typedef TensorFactor2_ TensorFactor2;
    typedef Tensor2_t<TensorFactor1,
                      TensorFactor2,
                      BasisOfTensor2_t<TensorFactor1,TensorFactor2>,
                      TensorDerived> Tensor2;

    Scalar operator [] (MultiIndex const &m) const
    {
        EuclideanEmbeddingInverse_t<TensorFactor1> e1_inv;
        EuclideanEmbeddingInverse_t<TensorFactor2> e2_inv;
        TypedIndex_t<TensorFactor1,'i'> i;
        TypedIndex_t<typename TensorFactor1::WithStandardEuclideanBasis,'j'> j;
        TypedIndex_t<TensorFactor2,'k'> k;
        TypedIndex_t<typename TensorFactor2::WithStandardEuclideanBasis,'l'> l;
        TypedIndex_t<Tensor2,'p'> p;
        TypedIndex_t<typename Tensor2::WithStandardEuclideanBasis,'q'> q;
        return ((e1_inv(i|j)*e2_inv(k|l)).bundle(i|k,p).bundle(j|l,q))[m];
    }

    // NOTE: these may be unnecessary/undesired, because this type does not represent a vector space
//     using Parent_Tensor_i::component_is_immutable_zero;
//     using Parent_Tensor_i::scalar_factor_for_component;
//     using Parent_Tensor_i::vector_index_of;
    static std::string type_as_string ()
    {
        return "EuclideanEmbeddingInverse_t<" + TypeStringOf_t<Tensor2>::eval() + '>';
    }
};

} // end of namespace Tenh

#endif // TENH_TENSOR2_HPP_
