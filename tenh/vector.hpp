// ///////////////////////////////////////////////////////////////////////////
// tenh/vector.hpp by Victor Dods, created 2013/03/12
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_VECTOR_HPP_
#define TENH_VECTOR_HPP_

#include <ostream>
#include <string>

#include "tenh/array.hpp"
#include "tenh/core.hpp"
#include "tenh/euclideanembedding.hpp"
#include "tenh/interface/vector.hpp"

namespace Tenh {

// NOTE: Scalar_ MUST be a POD data type.  BasedVectorSpace_ must be a BasedVectorSpace_c type.
template <typename Scalar_, typename BasedVectorSpace_, typename Derived_ = NullType>
struct Vector_t
    :
    public Vector_i<typename DerivedType_t<Derived_,Vector_t<Scalar_,DIM_,Basis_,Derived_> >::T,Scalar_,BasedVectorSpace_>,
    private Array_t<Scalar_,DIM_,typename DerivedType_t<Derived_,Vector_t<Scalar_,BasedVectorSpace_,Derived_> >::T>
    // Array_t is privately inherited because it is an implementation detail
{
    typedef Vector_i<typename DerivedType_t<Derived_,Vector_t<Scalar_,DIM_,Basis_,Derived_> >::T,Scalar_,BasedVectorSpace_> Parent_Vector_i;
    typedef Array_t<Scalar_,DIM_,typename DerivedType_t<Derived_,Vector_t<Scalar_,BasedVectorSpace_,Derived_> >::T> Parent_Array_t;
    typedef typename Parent_Vector_i::Derived Derived;
    typedef typename Parent_Vector_i::Scalar Scalar;
    typedef typename Parent_Vector_i::BasedVectorSpace BasedVectorSpace;
    static Uint32 const DIM = Parent_Vector_i::DIM;
    typedef typename Parent_Vector_i::Basis Basis;
    typedef typename Parent_Vector_i::Index Index;
    typedef typename Parent_Vector_i::MultiIndex MultiIndex;

    explicit Vector_t (WithoutInitialization const &w) : Parent_Array_t(w) { }
    explicit Vector_t (Scalar fill_with) : Parent_Array_t(fill_with) { }
    Vector_t (Scalar x0, Scalar x1) : Parent_Array_t(x0, x1) { }
    Vector_t (Scalar x0, Scalar x1, Scalar x2) : Parent_Array_t(x0, x1, x2) { }
    Vector_t (Scalar x0, Scalar x1, Scalar x2, Scalar x3) : Parent_Array_t(x0, x1, x2, x3) { }

    using Parent_Vector_i::as_derived;
    using Parent_Array_t::operator[];
    using Parent_Array_t::allocation_size_in_bytes;
    using Parent_Array_t::pointer_to_allocation;

    static std::string type_as_string ()
    {
        std::string derived_string;
        if (!Lvd::Meta::TypesAreEqual<Derived_,NullType>())
            derived_string = ',' + TypeStringOf_t<Derived>::eval();

        return "Vector_t<" + TypeStringOf_t<Scalar>::eval() + ',' + TypeStringOf_t<BasedVectorSpace>::eval() + derived_string + '>';
    }

private:

    // this has no definition, and is designed to generate a compiler error if used (use the one accepting WithoutInitialization instead).
    // TODO: may need to make this public to allow 0-dimensional vectors, adding a static assert to check that it's actually 0-dimensional
    // and not being used improperly.
    Vector_t ();
};
/*
// NOTE: while this is a tensor, it isn't a tensor space, and so it technically shouldn't be used as a factor
// type in a tensor product.  this is essentially a constant value -- it has only const accessors and can't be written to.
// TODO: once bases IDs are implemented, change this to not provide a default implementation,
// but only a specialization for Vector_t<Scalar,DIM,StandardEuclideanBasis>
template <typename Scalar_, Uint32 DIM_, typename Derived_>
struct EuclideanEmbedding_t<Vector_t<Scalar_,DIM_,StandardEuclideanBasis,Derived_> >
    :
    public EuclideanEmbedding_Parent_Tensor_i<Vector_t<Scalar_,DIM_,StandardEuclideanBasis,Derived_> >::T
{
    typedef typename EuclideanEmbedding_Parent_Tensor_i<Vector_t<Scalar_,DIM_,StandardEuclideanBasis,Derived_> >::T Parent_Tensor_i;
    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    using Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::Basis Basis;
    typedef typename Parent_Tensor_i::Index Index;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::ORDER;
    typedef Vector_t<Scalar_,DIM_,StandardEuclideanBasis,Derived_> Vector;

    // 1 on the diagonal, 0 otherwise
    Scalar operator [] (Index const &i) const
    {
        return (i.value() % Vector::DIM == i.value() / Vector::DIM) ? Scalar(1) : Scalar(0);
    }
    Scalar operator [] (MultiIndex const &m) const
    {
        return m.template el<0>() == m.template el<1>() ? Scalar(1) : Scalar(0);
    }

    // NOTE: these may be unnecessary/undesired, because this type does not represent a vector space
//     using Parent_Tensor_i::component_is_immutable_zero;
//     using Parent_Tensor_i::scalar_factor_for_component;
//     using Parent_Tensor_i::vector_index_of;

    static std::string type_as_string ()
    {
        return "EuclideanEmbedding_t<" + TypeStringOf_t<Vector>::eval() + '>';
    }
};

template <typename Scalar_, Uint32 DIM_, typename Derived_>
struct EuclideanEmbeddingInverse_t<Vector_t<Scalar_,DIM_,StandardEuclideanBasis,Derived_> >
    :
    public EuclideanEmbeddingInverse_Parent_Tensor_i<Vector_t<Scalar_,DIM_,StandardEuclideanBasis,Derived_> >::T
{
    typedef typename EuclideanEmbeddingInverse_Parent_Tensor_i<Vector_t<Scalar_,DIM_,StandardEuclideanBasis,Derived_> >::T Parent_Tensor_i;
    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    using Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::Basis Basis;
    typedef typename Parent_Tensor_i::Index Index;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::ORDER;
    typedef Vector_t<Scalar_,DIM_,StandardEuclideanBasis,Derived_> Vector;

    // 1 on the diagonal, 0 otherwise
    Scalar operator [] (Index const &i) const
    {
        return (i.value() % Vector::DIM == i.value() / Vector::DIM) ? Scalar(1) : Scalar(0);
    }
    Scalar operator [] (MultiIndex const &m) const
    {
        return m.template el<0>() == m.template el<1>() ? Scalar(1) : Scalar(0);
    }

    // NOTE: these may be unnecessary/undesired, because this type does not represent a vector space
//     using Parent_Tensor_i::component_is_immutable_zero;
//     using Parent_Tensor_i::scalar_factor_for_component;
//     using Parent_Tensor_i::vector_index_of;

    static std::string type_as_string ()
    {
        return "EuclideanEmbeddingInverse_t<" + TypeStringOf_t<Vector>::eval() + '>';
    }
};
*/
} // end of namespace Tenh

#endif // TENH_VECTOR_HPP_
