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
#include "tenh/interface/vector.hpp"

namespace Tenh {

// NOTE: Scalar_ MUST be a POD data type.
template <typename Scalar_, Uint32 DIM_, typename Derived_ = NullType> // don't worry about type ID for now
struct Vector_t : public Vector_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                         Vector_t<Scalar_,DIM_,Derived_>,
                                                         Derived_>::T,
                                  Scalar_,
                                  DIM_>,
                  public Array_t<Scalar_,DIM_>
{
    typedef Vector_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                            Vector_t<Scalar_,DIM_,Derived_>,
                                            Derived_>::T,
                     Scalar_,
                     DIM_> Parent_Vector_i;
    typedef Array_t<Scalar_,DIM_> Parent_Array_t;
    typedef typename Parent_Vector_i::Derived Derived;
    typedef typename Parent_Vector_i::Scalar Scalar;
    using Parent_Vector_i::DIM;
    typedef typename Parent_Vector_i::Index Index;
    typedef typename Parent_Vector_i::MultiIndex MultiIndex;

    explicit Vector_t (WithoutInitialization const &w) : Parent_Array_t(w) { }
    explicit Vector_t (Scalar fill_with) : Parent_Array_t(fill_with) { }
    Vector_t (Scalar x0, Scalar x1) : Parent_Array_t(x0, x1) { }
    Vector_t (Scalar x0, Scalar x1, Scalar x2) : Parent_Array_t(x0, x1, x2) { }
    Vector_t (Scalar x0, Scalar x1, Scalar x2, Scalar x3) : Parent_Array_t(x0, x1, x2, x3) { }

    using Parent_Vector_i::as_derived;
    using Parent_Vector_i::operator[];
    using Parent_Array_t::component_access_without_range_check;
    using Parent_Array_t::data_size_in_bytes;
    using Parent_Array_t::data_pointer;

    static std::string type_as_string ()
    {
        if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
            return "Vector_t<" + TypeStringOf_t<Scalar>::eval() + ',' + AS_STRING(DIM) + '>';
        else
            return "Vector_t<" + TypeStringOf_t<Scalar>::eval() + ',' + AS_STRING(DIM) + ',' + TypeStringOf_t<Derived>::eval() + '>';
    }
    
private:

    using Parent_Array_t::operator[]; // this should not be publicly accessible
};

} // end of namespace Tenh

#endif // TENH_VECTOR_HPP_
