// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/vector.hpp by Victor Dods, created 2013/04/07
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_VECTOR_HPP_
#define TENH_INTERFACE_VECTOR_HPP_

#include <ostream>

#include "tenh/core.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/index.hpp"
#include "tenh/meta/typestringof.hpp"
#include "tenh/multiindex.hpp"

namespace Tenh {

// NOTE: Scalar_ MUST be a POD data type.  An implementation of this interface
// really only needs to provide implementations of the const and non-const
// component_access_without_range_check methods.
template <typename Derived_, typename Scalar_, Uint32 DIM_, typename Basis_> // don't worry about type ID for now
struct Vector_i
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!Lvd::Meta::TypesAreEqual<Derived_,NullType>::v), DERIVED_MUST_NOT_BE_NULL_TYPE),
        STATIC_ASSERT_IN_ENUM((DIM_ > 0), DIMENSION_MUST_BE_POSITIVE) // TODO: zero-dimensional vector spaces (?)
    }; 

    typedef Derived_ Derived;
    typedef Scalar_ Scalar;
    static Uint32 const DIM = DIM_;
    typedef Basis_ Basis;
    // here is the "basic" (non-named) Index of this vector type, and it is aware of Derived
    typedef Index_t<Derived> Index;
    // the MultiIndex_t encapsulation of Index
    typedef MultiIndex_t<TypeList_t<Index> > MultiIndex;

    // TODO: only allow when Basis = Unit (or generic) once strongly-typed vectors are implemented
    // type conversion operator for canonical coercion to Scalar type when the vector is 1-dimensional
    operator Scalar const & () const
    {
        STATIC_ASSERT((DIM == 1), ONLY_ONE_DIMENSIONAL_VECTORS_CAN_BE_CONVERTED_TO_SCALARS);
        return as_derived().Derived::operator[](Index(0, DONT_CHECK_RANGE));
    }
    // this could be implemented as "operator Scalar & ()" but it would be bad to make implicit casts that can be used to change the value of this.
    Scalar &as_scalar ()
    {
        STATIC_ASSERT((DIM == 1), ONLY_ONE_DIMENSIONAL_VECTORS_CAN_BE_CONVERTED_TO_SCALARS);
        return as_derived().Derived::operator[](Index(0, DONT_CHECK_RANGE));
    }

    // accessor as Derived type
    Derived const &as_derived () const { return *static_cast<Derived const *>(this); }
    Derived &as_derived () { return *static_cast<Derived *>(this); }

    // NOTE: operator [] will be used to return values, while operator () will be
    // used to create expression templates for the purposes of indexed contractions.

    Scalar const &operator [] (Index const &i) const { return as_derived().Derived::operator[](i); }
    Scalar &operator [] (Index const &i) { return as_derived().Derived::operator[](i); }

    template <typename Index_>
    Scalar const &operator [] (MultiIndex_t<TypeList_t<Index_> > const &m) const { return as_derived().Derived::operator[](m.head()); }
    template <typename Index_>
    Scalar &operator [] (MultiIndex_t<TypeList_t<Index_> > const &m) { return as_derived().Derived::operator[](m.head()); }

    // the argument is technically unnecessary, as its value is not used.  however,
    // this allows the template system to deduce the SYMBOL of the TypedIndex_t, so
    // it doesn't need to be specified explicitly.
    // in this, an outer product would be
    // TypedIndex_t<'i'> i;
    // TypedIndex_t<'j'> j;
    // u(i)*v(j)
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,
                                       TypeList_t<TypedIndex_t<Derived,SYMBOL> >,
                                       EmptyTypeList,
                                       FORCE_CONST,
                                       CHECK_FOR_ALIASING> operator () (TypedIndex_t<Derived,SYMBOL> const &) const
    {
        STATIC_ASSERT((SYMBOL != '\0'), TYPEDINDEX_SYMBOL_MUST_NOT_BE_NULL);
        return ExpressionTemplate_IndexedObject_t<Derived,
                                                  TypeList_t<TypedIndex_t<Derived,SYMBOL> >,
                                                  EmptyTypeList,
                                                  FORCE_CONST,
                                                  CHECK_FOR_ALIASING>(as_derived());
    }
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,
                                       TypeList_t<TypedIndex_t<Derived,SYMBOL> >,
                                       EmptyTypeList,
                                       DONT_FORCE_CONST,
                                       CHECK_FOR_ALIASING> operator () (TypedIndex_t<Derived,SYMBOL> const &)
    {
        STATIC_ASSERT((SYMBOL != '\0'), TYPEDINDEX_SYMBOL_MUST_NOT_BE_NULL);
        return ExpressionTemplate_IndexedObject_t<Derived,
                                                  TypeList_t<TypedIndex_t<Derived,SYMBOL> >,
                                                  EmptyTypeList,
                                                  DONT_FORCE_CONST,
                                                  CHECK_FOR_ALIASING>(as_derived());
    }

    // NOTE: these are sort of part of the Tensor_i interface, but need Vector_i's cooperation.
    // if the return value for a particular MultiIndex is false, then that component is understood to be zero.
    static bool component_is_immutable_zero (Index const &i) { return false; }
    static bool component_is_immutable_zero (MultiIndex const &m) { return false; }
    static Scalar scalar_factor_for_component (Index const &m) { return Scalar(1); }
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return Scalar(1); }
    static Index vector_index_of (Index const &i) { return i; }
    static Index vector_index_of (MultiIndex const &m) { return m.head(); }

    static std::string type_as_string ()
    {
        // if Basis is StandardEuclideanBasis, leave it out of the type string, since it's the default value
        // and it tends to clutter up output pretty mightily.
        if (Lvd::Meta::TypesAreEqual<Basis,StandardEuclideanBasis>())
        {
            return "Vector_i<" + TypeStringOf_t<Derived>::eval() + ',' + TypeStringOf_t<Scalar>::eval() + ',' 
                               + AS_STRING(DIM) + '>';
        }
        else
        {
            return "Vector_i<" + TypeStringOf_t<Derived>::eval() + ',' + TypeStringOf_t<Scalar>::eval() + ',' 
                               + AS_STRING(DIM) + ',' + TypeStringOf_t<Basis>::eval() + '>';
        }
    }
};

template <typename Derived, typename Scalar, Uint32 DIM, typename Basis>
std::ostream &operator << (std::ostream &out, Vector_i<Derived,Scalar,DIM,Basis> const &v)
{
    typedef Vector_i<Derived,Scalar,DIM,Basis> Vector;
    typedef typename Vector::Index Index;

    if (DIM == 0)
        return out << "()";

    Index i; // initialized to the beginning automatically
    out << '(' << v[i];
    ++i;
    for ( ; i.is_not_at_end(); ++i)
        out << ", " << v[i];
    return out << ')';
}

} // end of namespace Tenh

#endif // TENH_INTERFACE_VECTOR_HPP_
