// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/vector.hpp by Victor Dods, created 2013/08/10
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_VECTOR_HPP_
#define TENH_IMPLEMENTATION_VECTOR_HPP_

#include "tenh/core.hpp"

#include "tenh/array.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/interface/vector.hpp"

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
    typedef typename Parent_Vector_i::ComponentIndex ComponentIndex;
    typedef typename Parent_Vector_i::MultiIndex MultiIndex;

    typedef typename DualOf_c<ImplementationOf_t>::T Dual; // relies on the template specialization below

    explicit ImplementationOf_t (WithoutInitialization const &w) : Parent_Array_t(w) { }
    // probably only useful for zero element (because this is basis-dependent)
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

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_VECTOR_HPP_
