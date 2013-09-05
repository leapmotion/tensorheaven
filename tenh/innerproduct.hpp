// ///////////////////////////////////////////////////////////////////////////
// tenh/innerproduct.hpp by Victor Dods, created 2013/03/19
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INNERPRODUCT_HPP_
#define TENH_INNERPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/implementation/diagonal2tensor.hpp" // TODO: replace with scalar2tensor.hpp later

namespace Tenh {

// TODO: figure out if "inner product" is a concept, and could be moved into
// conceptual, and then an ImplementationOf_t template created for it.

struct StandardEuclidean
{
    static std::string type_as_string () { return "StandardEuclidean"; } // could be just "E" for short
};

// convenience typedef for the standard Euclidean basis (basically is a pre-existing
// name for an orthonormal basis).
typedef Basis_c<StandardEuclidean> StandardEuclideanBasis;

// the StandardEuclideanBasis is self-dual, though the space it refers to may not be.
template <>
struct DualOf_f<StandardEuclideanBasis>
{
    typedef StandardEuclideanBasis T;
};



template <typename BasedVectorSpace_, typename Scalar_>
struct InnerProduct_t;

template <typename BasedVectorSpace_, typename Scalar_>
struct DualOf_f<InnerProduct_t<BasedVectorSpace_,Scalar_> >
{
    typedef InnerProduct_t<typename DualOf_f<BasedVectorSpace_>::T,Scalar_> T;
};

// template specialization for the standard inner product on a Euclidean space.
// TODO: could pass in this type as Derived to ImplementationOf_t, so that the
// EmbeddableAsTensor_i that it inherits correctly uses the operator[] of this
// type, which can return constant Scalar_(1), instead of storing 1 and using
// ImplementationOf_t<Scalar_,Diagonal2Tensor...>.  OR, create Diagonal2Tensor_i
// interface which inherits EmbeddableAsTensor_i (and eventually create
// Scalar2Tensor_i) and have this implement Diagonal2Tensor_i.
template <typename VectorSpace_, typename Scalar_>
struct InnerProduct_t<BasedVectorSpace_c<VectorSpace_,StandardEuclideanBasis>,Scalar_>
    :
    public ImplementationOf_t<
                Scalar_,
                Diagonal2TensorProductOfBasedVectorSpaces_c<
                    typename DualOf_f<BasedVectorSpace_c<VectorSpace_,StandardEuclideanBasis> >::T,
                    typename DualOf_f<BasedVectorSpace_c<VectorSpace_,StandardEuclideanBasis> >::T> >
{
private:
    typedef ImplementationOf_t<
                Scalar_,
                Diagonal2TensorProductOfBasedVectorSpaces_c<
                    typename DualOf_f<BasedVectorSpace_c<VectorSpace_,StandardEuclideanBasis> >::T,
                    typename DualOf_f<BasedVectorSpace_c<VectorSpace_,StandardEuclideanBasis> >::T> > Parent;
public:
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::BasedVectorSpace BasedVectorSpace;
    using Parent::DIM;
    typedef typename Parent::ComponentIndex ComponentIndex;

    typedef typename Parent::TensorProductOfBasedVectorSpaces TensorProductOfBasedVectorSpaces;
    typedef typename Parent::FactorTypeList FactorTypeList;
    typedef typename Parent::MultiIndex MultiIndex;
    static Uint32 const ORDER = 2;

    typedef typename DualOf_f<InnerProduct_t>::T Dual; // relies on the template specialization above

    // because the standard Euclidean basis is orthonormal, the inner product is the identity.
    // TODO: see note above this structure about Scalar2Tensor_i and so forth
    InnerProduct_t () : Parent(Scalar(1)) { }
};

} // end of namespace Tenh

#endif // TENH_INNERPRODUCT_HPP_
