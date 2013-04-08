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
template <typename Derived_, typename Scalar_, Uint32 DIM_> // don't worry about type ID for now
struct Vector_i
{
    enum { DIMENSION_MUST_BE_POSITIVE = Lvd::Meta::Assert<(DIM_ > 0)>::v }; // TODO: zero-dimensional vector spaces (?)

    typedef Derived_ Derived;
    typedef Scalar_ Scalar;
    static Uint32 const DIM = DIM_;
    // here is the "basic" (non-named) Index of this vector type, and it is aware of Derived
    typedef Index_t<Derived> Index;
    // the MultiIndex_t encapsulation of Index
    typedef MultiIndex_t<TypeList_t<Index> > MultiIndex; // TODO: rename to VectorMultiIndex (?)

    // TODO: only allow when Basis = Unit (or generic) once strongly-typed vectors are implemented
    // type conversion operator for canonical coercion to Scalar type when the vector is 1-dimensional
    operator Scalar const & () const
    {
        Lvd::Meta::Assert<(DIM == 1)>();
        return component_access_without_range_check(0);
    }
    // this could be implemented as "operator Scalar & ()" but it would be bad to make implicit casts that can be used to change the value of this.
    Scalar &as_scalar ()
    {
        Lvd::Meta::Assert<(DIM == 1)>();
        return component_access_without_range_check(0);
    }

    // accessor as Derived type
    Derived const &as_derived () const { return *static_cast<Derived const *>(this); }
    Derived &as_derived () { return *static_cast<Derived *>(this); }

    // NOTE: operator [] will be used to return values, while
    // operator () will be used to create expression templates
    // for the purposes of indexed contractions.
    // TODO: make Index type encode the guarantee that it's value will always be valid
    Scalar const &operator [] (Index const &i) const
    {
        if (i.is_at_end())
            throw std::invalid_argument("index out of range");
        else
            return component_access_without_range_check(i.value());
    }
    Scalar &operator [] (Index const &i)
    {
        if (i.is_at_end())
            throw std::invalid_argument("index out of range");
        else
            return component_access_without_range_check(i.value());
    }
    template <typename Index_>
    Scalar const &operator [] (MultiIndex_t<TypeList_t<Index_> > const &m) const
    {
        return operator[](m.template el<0>());
    }
    template <typename Index_>
    Scalar &operator [] (MultiIndex_t<TypeList_t<Index_> > const &m)
    {
        return operator[](m.template el<0>());
    }

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
                                       FORCE_CONST> operator () (TypedIndex_t<Derived,SYMBOL> const &) const
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedObject_t<Derived,
                                                  TypeList_t<TypedIndex_t<Derived,SYMBOL> >,
                                                  EmptyTypeList,
                                                  FORCE_CONST>(as_derived());
    }
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,
                                       TypeList_t<TypedIndex_t<Derived,SYMBOL> >,
                                       EmptyTypeList,
                                       DONT_FORCE_CONST> operator () (TypedIndex_t<Derived,SYMBOL> const &)
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedObject_t<Derived,
                                                  TypeList_t<TypedIndex_t<Derived,SYMBOL> >,
                                                  EmptyTypeList,
                                                  DONT_FORCE_CONST>(as_derived());
    }

    // NOTE: these are sort of part of the Tensor_i interface, but need Vector_i's cooperation.
    // if the return value for a particular MultiIndex is false, then that component is understood to be zero.
    static bool component_corresponds_to_memory_location (Index const &i) { return true; }
    static bool component_corresponds_to_memory_location (MultiIndex const &m) { return true; }
    static Scalar scalar_factor_for_component (Index const &m) { return Scalar(1); }
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return Scalar(1); }
    static Index vector_index_of (Index const &i) { return i; }
    static Index vector_index_of (MultiIndex const &m) { return m.head(); }

    static std::string type_as_string ()
    {
        return "Vector_i<" + TypeStringOf_t<Derived>::eval() + ',' + TypeStringOf_t<Scalar>::eval() + ',' + AS_STRING(DIM) + '>';
    }

private:

    // this SHOULD be inconvenient and ugly to call.  it should be used ONLY when you know for certain that 0 <= i < DIM
    Scalar const &component_access_without_range_check (Uint32 i) const { return this->as_derived().component_access_without_range_check(i); }
    // this SHOULD be inconvenient and ugly to call.  it should be used ONLY when you know for certain that 0 <= i < DIM
    Scalar &component_access_without_range_check (Uint32 i) { return this->as_derived().component_access_without_range_check(i); }

    // accessor as Derived type -- TODO: somehow integrate with other as_derived methods (?)
//     Derived const &as_derived_ () const { return *static_cast<Derived const *>(this); }
//     Derived &as_derived_ () { return *static_cast<Derived *>(this); }
};

template <typename Derived, typename Scalar, Uint32 DIM>
std::ostream &operator << (std::ostream &out, Vector_i<Derived,Scalar,DIM> const &v)
{
    typedef Vector_i<Derived,Scalar,DIM> Vector;
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
