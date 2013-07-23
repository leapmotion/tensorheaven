// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/vectorspace.hpp by Victor Dods, created 2013/07/21
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_VECTORSPACE_HPP_
#define TENH_CONCEPTUAL_VECTORSPACE_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/dual.hpp"
#include "tenh/conceptual/field.hpp"

namespace Tenh {

template <typename Field_, Uint32 DIM_, typename Id_>
struct VectorSpace_c
{
    enum { STATIC_ASSERT_IN_ENUM(IsAField_t<Field_>::V, MUST_BE_FIELD) };

	typedef Field_ Field;
	static Uint32 const DIM = DIM_;
	typedef Id_ Id;
    typedef typename DualOf_c<VectorSpace_c>::T Dual; // relies on the template specialization below

    static std::string type_as_string ()
    {
        return "VectorSpace_c<" + TypeStringOf_t<Field>::eval() + ',' 
                                + AS_STRING(DIM) + ',' + TypeStringOf_t<Id>::eval() + '>';
    }
};

template <typename T> struct IsAVectorSpace_t { static bool const V = false; };
template <typename Field, Uint32 DIM, typename Id> struct IsAVectorSpace_t<VectorSpace_c<Field,DIM,Id> > { static bool const V = true; };

template <typename Field, Uint32 DIM, typename Id>
struct DualOf_c<VectorSpace_c<Field,DIM,Id> >
{
    typedef VectorSpace_c<Field,DIM,typename DualOf_c<Id>::T> T;
};


template <typename VectorSpace_, typename Basis_>
struct BasedVectorSpace_c : public VectorSpace_
{
	enum { STATIC_ASSERT_IN_ENUM(IsAVectorSpace_t<VectorSpace_>::V, MUST_BE_VECTOR_SPACE),
	       STATIC_ASSERT_IN_ENUM(IsABasis_t<Basis_>::V, MUST_BE_BASIS) };

	typedef VectorSpace_ VectorSpace;
	typedef typename VectorSpace::Field Field;
	using VectorSpace::DIM;
	typedef typename VectorSpace::Id Id;
    typedef typename DualOf_c<BasedVectorSpace_c>::T Dual; // relies on the template specialization below
    using VectorSpace::IS_VECTOR_SPACE;
    typedef Basis_ Basis;

    static std::string type_as_string ()
    {
        return "BasedVectorSpace_c<" + TypeStringOf_t<VectorSpace>::eval() + ',' + TypeStringOf_t<Basis>::eval() + '>';
    }
};

template <typename T> struct IsABasedVectorSpace_t { static bool const V = false; };
template <typename VectorSpace, typename Basis> struct IsABasedVectorSpace_t<BasedVectorSpace_c<VectorSpace,Basis> > { static bool const V = true; };

template <typename VectorSpace, typename Basis>
struct DualOf_c<BasedVectorSpace_c<VectorSpace,Basis> >
{
    typedef BasedVectorSpace_c<typename VectorSpace::Dual,typename Basis::Dual> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_VECTORSPACE_HPP_
