#include <cassert>
#include <iostream>

#include "tenh/core.hpp"

#include "tenh/array.hpp"
#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/diagonalbased2tensorproduct.hpp"
#include "tenh/conceptual/symmetricpower.hpp"
#include "tenh/conceptual/tensorpower.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/conceptual/dual.hpp"
#include "tenh/interface/tensor.hpp"
#include "tenh/interface/vector.hpp"

struct X
{
    static std::string type_as_string () { return "X"; }
};

struct Y
{
    static std::string type_as_string () { return "Y"; }
};

struct Z
{
    static std::string type_as_string () { return "Z"; }
};

namespace Tenh {

template <typename Scalar, typename Space> struct ImplementationOf_t;

template <typename Scalar_, typename VectorSpace_, typename Basis_>
struct ImplementationOf_t<Scalar_,BasedVectorSpace_c<VectorSpace_,Basis_> >
    :
    public Vector_i<ImplementationOf_t<Scalar_,BasedVectorSpace_c<VectorSpace_,Basis_> >,
                    Scalar_,
                    BasedVectorSpace_c<VectorSpace_,Basis_> >,
    // Array_t is privately inherited because it is an implementation detail
    private Array_t<Scalar_,VectorSpace_::DIM>
{
    typedef Vector_i<ImplementationOf_t<Scalar_,BasedVectorSpace_c<VectorSpace_,Basis_> >,
                     Scalar_,
                     BasedVectorSpace_c<VectorSpace_,Basis_> > Parent_Vector_i;
    typedef Array_t<Scalar_,VectorSpace_::DIM> Parent_Array_t;

    typedef typename Parent_Vector_i::Derived Derived;
    typedef typename Parent_Vector_i::Scalar Scalar;
    typedef typename Parent_Vector_i::BasedVectorSpace BasedVectorSpace;
    using Parent_Vector_i::DIM;
    typedef typename Parent_Vector_i::Basis Basis;
    typedef typename Parent_Vector_i::ComponentIndex ComponentIndex;
    typedef typename Parent_Vector_i::MultiIndex MultiIndex;

    typedef typename DualOf_c<ImplementationOf_t>::T Dual; // relies on the template specialization below

    explicit ImplementationOf_t (WithoutInitialization const &w) : Parent_Array_t(w) { }
    explicit ImplementationOf_t (Scalar fill_with) : Parent_Array_t(fill_with) { }
    ImplementationOf_t (Scalar x0, Scalar x1) : Parent_Array_t(x0, x1) { }
    ImplementationOf_t (Scalar x0, Scalar x1, Scalar x2) : Parent_Array_t(x0, x1, x2) { }
    ImplementationOf_t (Scalar x0, Scalar x1, Scalar x2, Scalar x3) : Parent_Array_t(x0, x1, x2, x3) { }

    using Parent_Vector_i::as_derived;
    using Parent_Array_t::operator[];
    using Parent_Array_t::data_size_in_bytes;
    using Parent_Array_t::data_pointer;

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + TypeStringOf_t<Scalar>::eval() + ',' + TypeStringOf_t<BasedVectorSpace>::eval() + '>';
    }

private:

    // this has no definition, and is designed to generate a compiler error if used (use the one accepting WithoutInitialization instead).
    // TODO: may need to make this public to allow 0-dimensional vectors, adding a static assert to check that it's actually 0-dimensional
    // and not being used improperly.
    ImplementationOf_t ();
};

template <typename Scalar, typename VectorSpace, typename Basis>
struct DualOf_c<ImplementationOf_t<Scalar,BasedVectorSpace_c<VectorSpace,Basis> > >
{
    typedef ImplementationOf_t<Scalar,typename BasedVectorSpace_c<VectorSpace,Basis>::Dual> T;
};




template <typename Scalar_, typename FactorTypeList_>
struct ImplementationOf_t<Scalar_,TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >
    :
    public Tensor_i<ImplementationOf_t<Scalar_,TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >,
                    Scalar_,
                    TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >,
    // Array_t is privately inherited because it is an implementation detail
    private Array_t<Scalar_,TensorProductOfBasedVectorSpaces_c<FactorTypeList_>::DIM>
{
    typedef Tensor_i<ImplementationOf_t<Scalar_,TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >,
                     Scalar_,
                     TensorProductOfBasedVectorSpaces_c<FactorTypeList_> > Parent_Tensor_i;
    typedef Array_t<Scalar_,TensorProductOfBasedVectorSpaces_c<FactorTypeList_>::DIM> Parent_Array_t;

    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    typedef typename Parent_Tensor_i::BasedVectorSpace BasedVectorSpace;
    using Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::Basis Basis;
    typedef typename Parent_Tensor_i::ComponentIndex ComponentIndex;

    typedef typename Parent_Tensor_i::TensorProductOfBasedVectorSpaces TensorProductOfBasedVectorSpaces;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::DEGREE;
    using Parent_Tensor_i::IS_TENSOR_I;    

    typedef typename DualOf_c<ImplementationOf_t>::T Dual; // relies on the template specialization below

    ImplementationOf_t (WithoutInitialization const &w) : Parent_Array_t(w) { }
    ImplementationOf_t (Scalar fill_with) : Parent_Array_t(fill_with) { }

    // TODO: do later
    // template <typename BundleIndexTypeList, typename BundledIndex>
    // static MultiIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    // {
    //     typedef typename BundleIndexTypeList::template El_t<0>::T Index1;
    //     typedef typename BundleIndexTypeList::template El_t<1>::T Index2;
    //     // this is just to check that there is a valid conversion to the requested MultiIndex type.
    //     // it doesn't actually produce any side-effects, and should be optimized out.
    //     {
    //         STATIC_ASSERT((BundleIndexTypeList::LENGTH == 2), CAN_ONLY_BUNDLE_TWO_INDICES);
    //         Index1 i1;
    //         Index2 i2;
    //         typename Factor1::Index f1(i1);
    //         typename Factor2::Index f2(i2);
    //         // check that the parameter BundleIndex type is compatible with Index
    //         Index i(b);
    //     }

    //     Uint32 row;
    //     Uint32 col;
    //     contiguous_index_to_rowcol_index(b.value(), row, col);
    //     return MultiIndex_t<BundleIndexTypeList>(Index1(row, DONT_CHECK_RANGE), Index2(col, DONT_CHECK_RANGE));
    // }

    using Parent_Array_t::operator[];
    using Parent_Array_t::data_size_in_bytes;
    using Parent_Array_t::data_pointer;

    // // Index1 could be Factor1::Index or Factor1::MultiIndex (checked by its use in the other functions)
    // // Index2 could be Factor2::Index or Factor2::MultiIndex (checked by its use in the other functions)
    // template <typename Index1, typename Index2>
    // Scalar operator [] (MultiIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &m) const
    // {
    //     return component(m.template el<0>(), m.template el<1>());
    // }
    // template <typename Index1, typename Index2>
    // Scalar &operator [] (MultiIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &m)
    // {
    //     assert(m.is_not_at_end() && "you used Index_t(x, DONT_RANGE_CHECK) inappropriately");
    //     // NOTE: this construction is unnecessary to the code, but IS necessary to the compile-time type checking
    //     // the compiler should optimize it out anyway.
    //     MultiIndex x(m);
    //     return operator[](Index(m.value(), DONT_CHECK_RANGE));
    // }
    template <typename OtherIndexTypeList>
    Scalar operator [] (MultiIndex_t<OtherIndexTypeList> const &m) const
    {
        STATIC_ASSERT(IsATypeList_t<OtherIndexTypeList>::V, MUST_BE_TYPELIST);
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
        STATIC_ASSERT(IsATypeList_t<OtherIndexTypeList>::V, MUST_BE_TYPELIST);
        typedef MultiIndex_t<OtherIndexTypeList> OtherMultiIndex;
        STATIC_ASSERT((OtherMultiIndex::LENGTH == MultiIndex::LENGTH), MUST_HAVE_EQUAL_LENGTHS);
        //std::cout << OtherMultiIndex::LENGTH << ", " << MultiIndex::LENGTH << '\n';
        assert(m.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        // NOTE: this construction is unnecessary to the code, but IS necessary to the compile-time type checking
        // the compiler should optimize it out anyway.
        MultiIndex x(m);
        // m.value() is what does the multi-index-to-vector-index computation
        return operator[](ComponentIndex(m.value(), DONT_CHECK_RANGE));
        // std::cout << TypeStringOf_t<OtherMultiIndex>::eval() << ", " << TypeStringOf_t<MultiIndex>::eval() << '\n';
        // return Scalar(0);
    }

    // these are what provide indexed expressions -- via expression templates
    using Parent_Tensor_i::operator();

/*
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
    */
    using Parent_Tensor_i::component_is_immutable_zero;
    using Parent_Tensor_i::scalar_factor_for_component;
    using Parent_Tensor_i::vector_index_of;
    // all components are stored in memory (in the array m), and have scalar factor 1
    static bool component_is_immutable_zero (MultiIndex const &m) { return false; }
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return Scalar(1); }
    static ComponentIndex vector_index_of (MultiIndex const &m) { return ComponentIndex(m.value(), DONT_CHECK_RANGE); }

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + TypeStringOf_t<Scalar>::eval() + ',' + TypeStringOf_t<TensorProductOfBasedVectorSpaces>::eval() + '>';
    }

private:

/*
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
*/
    // this has no definition, and is designed to generate a compiler error if used (use the one accepting WithoutInitialization instead).
    ImplementationOf_t ();
};

template <typename Scalar, typename FactorTypeList>
struct DualOf_c<ImplementationOf_t<Scalar,TensorProductOfBasedVectorSpaces_c<FactorTypeList> > >
{
    typedef ImplementationOf_t<Scalar,typename TensorProductOfBasedVectorSpaces_c<FactorTypeList>::Dual> T;
};


} // end of namespace Tenh



using namespace Tenh;

int main (int argc, char **argv)
{
    {
        typedef Basis_c<X> Basis;
        typedef Basis::Dual DualBasis;
        typedef DualBasis::Dual DualDualBasis;
        std::cout << "Basis = "  << TypeStringOf_t<Basis>::eval() << '\n'
                  << "DualBasis = " << TypeStringOf_t<DualBasis>::eval()  << '\n'
                  << "DualOf_c<Basis>::T = " << TypeStringOf_t<DualOf_c<Basis>::T>::eval() << '\n'
                  << "DualDualBasis = " << TypeStringOf_t<DualDualBasis>::eval() << '\n' << '\n';
        // make sure the basis is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<Basis,DualDualBasis>::v));
        // make sure that Basis::Dual and DualOf_c<Basis>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualBasis,DualOf_c<Basis>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,3,X> VectorSpace;
        typedef VectorSpace::Dual DualVectorSpace;
        typedef DualVectorSpace::Dual DualDualVectorSpace;
        std::cout << "VectorSpace = "  << TypeStringOf_t<VectorSpace>::eval() << '\n'
                  << "DualVectorSpace = " << TypeStringOf_t<DualVectorSpace>::eval() << '\n' 
                  << "DualOf_c<VectorSpace>::T = " << TypeStringOf_t<DualOf_c<VectorSpace>::T>::eval() << '\n'
                  << "DualDualVectorSpace = " << TypeStringOf_t<DualDualVectorSpace>::eval() << '\n' << '\n';
        // make sure the vector space is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<VectorSpace,DualDualVectorSpace>::v));
        // make sure that VectorSpace::Dual and DualOf_c<VectorSpace>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualVectorSpace,DualOf_c<VectorSpace>::T>::v));
        // make sure VectorSpace is actually a VectorSpace
        assert(IsAVectorSpace_c<VectorSpace>::V);
    }

    {
        typedef VectorSpace_c<RealField,3,X> VectorSpace;
        typedef Basis_c<Y> Basis;
        typedef BasedVectorSpace_c<VectorSpace,Basis> BasedVectorSpace;
        // BasedVectorSpace::Index i;
        typedef BasedVectorSpace::Dual DualBasedVectorSpace;
        typedef DualBasedVectorSpace::Dual DualDualBasedVectorSpace;
        std::cout << "BasedVectorSpace = "  << TypeStringOf_t<BasedVectorSpace>::eval() << '\n'
                  // << "BasedVectorSpace::Index = " << TypeStringOf_t<BasedVectorSpace::Index>::eval() << '\n'  
                  // << "BasedVectorSpace::Index::Dual = " << TypeStringOf_t<BasedVectorSpace::Index::Dual>::eval() << '\n'  
                  // << "BasedVectorSpace::Dual::Index = " << TypeStringOf_t<BasedVectorSpace::Dual::Index>::eval() << '\n'  
                  << "DualBasedVectorSpace = " << TypeStringOf_t<DualBasedVectorSpace>::eval() << '\n' 
                  << "DualOf_c<BasedVectorSpace>::T = " << TypeStringOf_t<DualOf_c<BasedVectorSpace>::T>::eval() << '\n'
                  << "DualDualBasedVectorSpace = " << TypeStringOf_t<DualDualBasedVectorSpace>::eval() << '\n' << '\n';
        // make sure the vector space is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<BasedVectorSpace,DualDualBasedVectorSpace>::v));
        // make sure that BasedVectorSpace::Dual and DualOf_c<BasedVectorSpace>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualBasedVectorSpace,DualOf_c<BasedVectorSpace>::T>::v));
        // make sure that BasedVectorSpace::Dual::Index and BasedVectorSpace::Dual::Index are the same
        // assert((Lvd::Meta::TypesAreEqual<BasedVectorSpace::Dual::Index,BasedVectorSpace::Index::Dual>::v));
        // make sure BasedVectorSpace is actually a VectorSpace_c and a BasedVectorSpace_c
        assert(IsAVectorSpace_c<BasedVectorSpace>::V);
        assert(IsABasedVectorSpace_c<BasedVectorSpace>::V);
    }

    {
        typedef TypeList_t<X,TypeList_t<int,TypeList_t<float> > > TypeList;
        typedef TensorProduct_c<TypeList> TensorProduct;
        typedef TensorProduct::Dual DualTensorProduct;
        typedef DualTensorProduct::Dual DualDualTensorProduct;
        std::cout << "TensorProduct = "  << TypeStringOf_t<TensorProduct>::eval() << '\n'
                  << "DualTensorProduct = " << TypeStringOf_t<DualTensorProduct>::eval() << '\n' 
                  << "DualOf_c<TensorProduct>::T = " << TypeStringOf_t<DualOf_c<TensorProduct>::T>::eval() << '\n'
                  << "DualDualTensorProduct = " << TypeStringOf_t<DualDualTensorProduct>::eval() << '\n' << '\n';
        // make sure the tensor product is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<TensorProduct,DualDualTensorProduct>::v));
        // make sure that TensorProduct::Dual and DualOf_c<TensorProduct>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualTensorProduct,DualOf_c<TensorProduct>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,3,X> X3;
        typedef VectorSpace_c<RealField,4,Y> Y4;
        typedef VectorSpace_c<RealField,5,Z> Z5;
        typedef TensorProductOfVectorSpaces_c<TypeList_t<X3,TypeList_t<Y4,TypeList_t<Z5> > > > T;
        typedef T::Dual DualT;
        typedef DualT::Dual DualDualT;
        std::cout << "TensorProductOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualTensorProductOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_c<TensorProductOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                  << "DualDualTensorProductOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor product of vector spaces is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that T::Dual and DualOf_c<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
    }

    {
        typedef Basis_c<X> BX;
        typedef Basis_c<Y> BY;
        typedef Basis_c<Z> BZ;
        typedef TensorProductOfBases_c<TypeList_t<BX,TypeList_t<BY,TypeList_t<BZ> > > > T;
        typedef T::Dual DualT;
        typedef DualT::Dual DualDualT;
        std::cout << "TensorProductOfBases = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualTensorProductOfBases = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_c<TensorProductOfBases>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                  << "DualDualTensorProductOfBases = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor product of bases is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that T::Dual and DualOf_c<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,3,X> X3;
        typedef VectorSpace_c<RealField,4,Y> Y4;
        typedef VectorSpace_c<RealField,5,Z> Z5;
        typedef Basis_c<X> BX;
        typedef Basis_c<Y> BY;
        typedef Basis_c<Z> BZ;
        typedef BasedVectorSpace_c<X3,BX> BasedX;
        typedef BasedVectorSpace_c<Y4,BY> BasedY;
        typedef BasedVectorSpace_c<Z5,BZ> BasedZ;
        assert(IsABasedVectorSpace_c<BasedX>::V);
        assert(IsABasedVectorSpace_c<BasedY>::V);
        assert(IsABasedVectorSpace_c<BasedZ>::V);

        typedef TensorProductOfVectorSpaces_c<TypeList_t<X3,TypeList_t<Y4,TypeList_t<Z5> > > > TVS;
        assert(IsAVectorSpace_c<TVS>::V);
        assert(IsATensorProduct_c<TVS>::V);
        assert(IsATensorProductOfVectorSpaces_c<TVS>::V);

        typedef TensorProductOfBases_c<TypeList_t<BX,TypeList_t<BY,TypeList_t<BZ> > > > TB;
        assert(IsABasis_c<TB>::V);
        assert(IsATensorProduct_c<TB>::V);

        {
            typedef BasedTensorProductOfVectorSpaces_c<TVS,TB> T;
            assert(IsATensorProduct_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsATensorProductOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorProductOfVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "BasedTensorProductOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualBasedTensorProductOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<BasedTensorProductOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualBasedTensorProductOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the based tensor product of vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }

        {
            typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX,TypeList_t<BasedY,TypeList_t<BasedZ> > > > T;
            assert(IsATensorProduct_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsATensorProductOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorProductOfVectorSpaces_c<T>::V);
            assert(IsATensorProductOfBasedVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "TensorProductOfBasedVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualTensorProductOfBasedVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<TensorProductOfBasedVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualTensorProductOfBasedVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the tensor product of based vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }

        {
            typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedX,BasedY> T;
            assert(IsATensorProduct_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsATensorProductOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorProductOfVectorSpaces_c<T>::V);
            assert(IsATensorProductOfBasedVectorSpaces_c<T>::V);
            assert(IsADiagonal2TensorProductOfBasedVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "Diagonal2TensorProductOfBasedVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualDiagonal2TensorProductOfBasedVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<Diagonal2TensorProductOfBasedVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualDiagonal2TensorProductOfBasedVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the diagonal 2 tensor product of based vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }
    }

    ////////////////////////////////

    {
        typedef TensorPower_c<X,3> TensorPower;
        typedef TensorPower::Dual DualTensorPower;
        typedef DualTensorPower::Dual DualDualTensorPower;
        std::cout << "TensorPower = "  << TypeStringOf_t<TensorPower>::eval() << '\n'
                  << "DualTensorPower = " << TypeStringOf_t<DualTensorPower>::eval() << '\n' 
                  << "DualOf_c<TensorPower>::T = " << TypeStringOf_t<DualOf_c<TensorPower>::T>::eval() << '\n'
                  << "DualDualTensorPower = " << TypeStringOf_t<DualDualTensorPower>::eval() << '\n' << '\n';
        // make sure the tensor power is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<TensorPower,DualDualTensorPower>::v));
        // make sure that TensorPower::Dual and DualOf_c<TensorPower>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualTensorPower,DualOf_c<TensorPower>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,5,X> X5;
        typedef TensorPowerOfVectorSpaces_c<X5,3> T;
        typedef T::Dual DualT;
        typedef DualT::Dual DualDualT;
        std::cout << "TensorPowerOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualTensorPowerOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_c<TensorPowerOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                  << "DualDualTensorPowerOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor power of vector spaces is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that T::Dual and DualOf_c<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
    }

    {
        typedef Basis_c<X> BX;
        typedef TensorPowerOfBases_c<BX,3> T;
        typedef T::Dual DualT;
        typedef DualT::Dual DualDualT;
        std::cout << "TensorPowerOfBases = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualTensorPowerOfBases = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_c<TensorPowerOfBases>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                  << "DualDualTensorPowerOfBases = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor power of bases is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that T::Dual and DualOf_c<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,5,X> X5;
        typedef Basis_c<X> BX;
        typedef BasedVectorSpace_c<X5,BX> BasedX;
        assert(IsABasedVectorSpace_c<BasedX>::V);

        typedef TensorPowerOfVectorSpaces_c<X5,3> TVS;
        assert(IsAVectorSpace_c<TVS>::V);
        assert(IsATensorProduct_c<TVS>::V);
        assert(IsATensorProductOfVectorSpaces_c<TVS>::V);
        assert(IsATensorPower_c<TVS>::V);
        assert(IsATensorPowerOfVectorSpaces_c<TVS>::V);

        typedef TensorPowerOfBases_c<BX,3> TB;
        assert(IsABasis_c<TB>::V);
        assert(IsATensorProduct_c<TB>::V);
        assert(IsATensorPower_c<TB>::V);

        {
            typedef BasedTensorPowerOfVectorSpaces_c<TVS,TB> T;
            assert(IsATensorProduct_c<T>::V);
            assert(IsATensorPower_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsATensorProductOfVectorSpaces_c<T>::V);
            assert(IsATensorPowerOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorProductOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorPowerOfVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "BasedTensorPowerOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualBasedTensorPowerOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<BasedTensorPowerOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualBasedTensorPowerOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the based tensor power of vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }

        {
            typedef TensorPowerOfBasedVectorSpaces_c<BasedX,3> T;
            assert(IsATensorProduct_c<T>::V);
            assert(IsATensorPower_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsATensorProductOfVectorSpaces_c<T>::V);
            assert(IsATensorPowerOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorProductOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorPowerOfVectorSpaces_c<T>::V);
            assert(IsATensorProductOfBasedVectorSpaces_c<T>::V);
            assert(IsATensorPowerOfBasedVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "TensorPowerOfBasedVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualTensorPowerOfBasedVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<TensorPowerOfBasedVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualTensorPowerOfBasedVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the tensor power of based vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }
    }

    ////////////////////////////////

    {
        typedef SymmetricPower_c<X,3> SymmetricPower;
        typedef SymmetricPower::Dual DualSymmetricPower;
        typedef DualSymmetricPower::Dual DualDualSymmetricPower;
        std::cout << "SymmetricPower = "  << TypeStringOf_t<SymmetricPower>::eval() << '\n'
                  << "DualSymmetricPower = " << TypeStringOf_t<DualSymmetricPower>::eval() << '\n' 
                  << "DualOf_c<SymmetricPower>::T = " << TypeStringOf_t<DualOf_c<SymmetricPower>::T>::eval() << '\n'
                  << "DualDualSymmetricPower = " << TypeStringOf_t<DualDualSymmetricPower>::eval() << '\n' << '\n';
        // make sure the tensor power is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<SymmetricPower,DualDualSymmetricPower>::v));
        // make sure that SymmetricPower::Dual and DualOf_c<SymmetricPower>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualSymmetricPower,DualOf_c<SymmetricPower>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,5,X> X5;
        typedef SymmetricPowerOfVectorSpaces_c<X5,3> T;
        typedef T::Dual DualT;
        typedef DualT::Dual DualDualT;
        std::cout << "SymmetricPowerOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualSymmetricPowerOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_c<SymmetricPowerOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                  << "DualDualSymmetricPowerOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor power of vector spaces is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that T::Dual and DualOf_c<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
    }

    {
        typedef Basis_c<X> BX;
        typedef SymmetricPowerOfBases_c<BX,3> T;
        typedef T::Dual DualT;
        typedef DualT::Dual DualDualT;
        std::cout << "SymmetricPowerOfBases = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualSymmetricPowerOfBases = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_c<SymmetricPowerOfBases>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                  << "DualDualSymmetricPowerOfBases = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor power of bases is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that T::Dual and DualOf_c<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,5,X> X5;
        typedef Basis_c<X> BX;
        typedef BasedVectorSpace_c<X5,BX> BasedX;
        assert(IsABasedVectorSpace_c<BasedX>::V);

        typedef SymmetricPowerOfVectorSpaces_c<X5,3> TVS;
        assert(IsAVectorSpace_c<TVS>::V);
        assert(IsASymmetricPower_c<TVS>::V);
        assert(IsASymmetricPowerOfVectorSpaces_c<TVS>::V);

        typedef SymmetricPowerOfBases_c<BX,3> TB;
        assert(IsABasis_c<TB>::V);
        assert(IsASymmetricPower_c<TB>::V);
/*
        {
            typedef BasedSymmetricPowerOfVectorSpaces_c<TVS,TB> T;
            assert(IsASymmetricPower_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsASymmetricPowerOfVectorSpaces_c<T>::V);
            assert(IsABasedSymmetricPowerOfVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "BasedSymmetricPowerOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualBasedSymmetricPowerOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<BasedSymmetricPowerOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualBasedSymmetricPowerOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the based tensor power of vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }
*/
        {
            typedef SymmetricPowerOfBasedVectorSpaces_c<BasedX,3> T;
            assert(IsASymmetricPower_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsASymmetricPowerOfVectorSpaces_c<T>::V);
            assert(IsASymmetricPowerOfBasedVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "SymmetricPowerOfBasedVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualSymmetricPowerOfBasedVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<SymmetricPowerOfBasedVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualSymmetricPowerOfBasedVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the tensor power of based vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }
    }

    std::cout << '\n' << '\n';

    {
        typedef VectorSpace_c<RealField,3,X> VSX;
        typedef Basis_c<X> B;
        typedef BasedVectorSpace_c<VSX,B> BasedX;

        typedef ImplementationOf_t<float,BasedX> V;
        V v(1.0f, 2.0f, 3.0f);
        V w(8.0f, -2.0f, 6.0f);
        std::cout << TypeStringOf_t<V>::eval() << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        std::cout << FORMAT_VALUE(w) << '\n';

        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        std::cout << FORMAT_VALUE(v(i) + w(i)) << '\n';
        std::cout << FORMAT_VALUE(v(i) - w(i)) << '\n';

        V::Dual a(0.0f, 2.0f, -3.0f);
        std::cout << TypeStringOf_t<V::Dual>::eval() << '\n';
        std::cout << FORMAT_VALUE(a) << '\n';
        std::cout << FORMAT_VALUE(a(i)*v(i)) << '\n';
        std::cout << FORMAT_VALUE(v(i)*a(i)) << '\n';
        std::cout << FORMAT_VALUE(v(i)*a(j)) << '\n';

        // this should cause a compile error due to the non-naturality of the pairing
//        std::cout << FORMAT_VALUE(v(i)*w(i)) << '\n';

        // this should cause a compile error due to a mismatch of factor types
//        std::cout << FORMAT_VALUE(a(i) + v(i)) << '\n';

        std::cout << '\n' << '\n';
    }

    {
        typedef VectorSpace_c<RealField,3,X> VSX;
        typedef Basis_c<X> BX;
        typedef BasedVectorSpace_c<VSX,BX> BasedX;

        typedef VectorSpace_c<RealField,2,Y> VSY;
        typedef Basis_c<Y> BY;
        typedef BasedVectorSpace_c<VSY,BY> BasedY;

        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX,TypeList_t<BasedY::Dual> > > TPBVS;
        typedef ImplementationOf_t<float,BasedX> U;
        typedef ImplementationOf_t<float,BasedY> V;
        typedef ImplementationOf_t<float,TPBVS> T;
        std::cout << TypeStringOf_t<T>::eval() << '\n';

        T t(3.0f);
        std::cout << FORMAT_VALUE(t) << '\n';
        t[T::MultiIndex(0,0)] = 0.0f;
        t[T::MultiIndex(1,0)] = 4.0f;
        t[T::MultiIndex(2,0)] = -2.0f;
        t[T::MultiIndex(0,1)] = 3.0f;
        t[T::MultiIndex(1,1)] = 4.0f;
        t[T::MultiIndex(2,1)] = 5.0f;
        std::cout << "after assignment: " << FORMAT_VALUE(t) << '\n';

        V v(2.0f, 7.0f);
        std::cout << FORMAT_VALUE(v) << '\n';

        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        std::cout << FORMAT_VALUE(t(i|j)*v(j)) << '\n';

        U u(1.0f, 0.0f, -1.0f);
        std::cout << FORMAT_VALUE(u) << '\n';
        // this should cause a compile error due to the non-naturality of the pairing
//        std::cout << FORMAT_VALUE(u(i)*t(i|j)) << '\n';


        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX,TypeList_t<BasedY::Dual,TypeList_t<BasedY::Dual> > > > H;
        typedef ImplementationOf_t<float,H> E;
        E e(0.0f);
        for (E::ComponentIndex it; it.is_not_at_end(); ++it)
            e[it] = it.value();
//        std::cout << FORMAT_VALUE(e(i)) << '\n'; // this has problems: TODO: fix -- ExpressionTemplate_i may need a vector-indexable operator[]

        V w(1.0f, 3.0f);

        AbstractIndex_c<'k'> k;
        std::cout << FORMAT_VALUE(w) << '\n';
        std::cout << FORMAT_VALUE(e) << '\n';
        std::cout << FORMAT_VALUE(e(i|j|k)*v(j)*w(k)) << '\n';

        //E::Dual f(3.0f);

        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedY,TypeList_t<BasedY::Dual> > > EndomorphismOfY;
        typedef ImplementationOf_t<float,EndomorphismOfY> Endo;
        Endo A(0.0f);
        A[Endo::MultiIndex(0,0)] = 3.0f;
        A[Endo::MultiIndex(1,1)] = 4.0f;
        std::cout << FORMAT_VALUE(A) << '\n';
        std::cout << FORMAT_VALUE(A(i|j)*v(j)) << '\n';
        std::cout << FORMAT_VALUE(A(i|i)) << '\n';

        std::cout << '\n' << '\n';
    }

    return 0;
}
