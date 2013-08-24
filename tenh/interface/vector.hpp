// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/vector.hpp by Victor Dods, created 2013/04/07
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_VECTOR_HPP_
#define TENH_INTERFACE_VECTOR_HPP_

#include <cmath> // for the square root in Vector_i::norm (could break <cmath> and the definition of Vector_i::norm out into a separate file)
#include <ostream>

#include "tenh/core.hpp"

#include "tenh/componentindex.hpp"
#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/meta/typestringof.hpp"
#include "tenh/multiindex.hpp"

namespace Tenh {

// NOTE: Scalar_ MUST be a POD data type.  An implementation of this interface
// really only needs to provide implementations of the const and non-const
// component_access_without_range_check methods.  BasedVectorSpace_ should be
// a BasedVectorSpace_c type.
template <typename Derived_, typename Scalar_, typename BasedVectorSpace_>
struct Vector_i
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!Lvd::Meta::TypesAreEqual<Derived_,NullType>::v), DERIVED_MUST_NOT_BE_NULL_TYPE),
        STATIC_ASSERT_IN_ENUM(IS_BASED_VECTOR_SPACE_UNIQUELY(BasedVectorSpace_), MUST_BE_BASED_VECTOR_SPACE)
    };

    typedef Derived_ Derived;
    typedef Scalar_ Scalar;
    typedef BasedVectorSpace_ BasedVectorSpace;
    static Uint32 const DIM = AS_VECTOR_SPACE(BasedVectorSpace)::DIMENSION;

    typedef ComponentIndex_t<DIM> ComponentIndex;
    typedef MultiIndex_t<TypeList_t<ComponentIndex> > MultiIndex;

    // this is necessary so that ExpressionTemplate_IndexedObject_t knows that a vector is
    // a 1-tensor having a particular type.
    typedef TypeList_t<BasedVectorSpace> FactorTypeList;

    static Uint32 dim () { return DIM; }

    // TODO: only allow when Basis = Euclidean once strongly-typed vectors are implemented
    // type conversion operator for canonical coercion to Scalar type when the vector is 1-dimensional
    operator Scalar const & () const
    {
        STATIC_ASSERT((DIM == 1), ONLY_ONE_DIMENSIONAL_VECTORS_CAN_BE_CONVERTED_TO_SCALARS);
        return as_derived().Derived::operator[](ComponentIndex(0, DONT_CHECK_RANGE));
    }
    // this could be implemented as "operator Scalar & ()" but it would be bad to make implicit casts that can be used to change the value of this.
    Scalar &as_scalar ()
    {
        STATIC_ASSERT((DIM == 1), ONLY_ONE_DIMENSIONAL_VECTORS_CAN_BE_CONVERTED_TO_SCALARS);
        return as_derived().Derived::operator[](ComponentIndex(0, DONT_CHECK_RANGE));
    }

    // accessor as Derived type
    Derived const &as_derived () const { return *static_cast<Derived const *>(this); }
    Derived &as_derived () { return *static_cast<Derived *>(this); }

    // NOTE: will not currently work for complex types
    typename AssociatedFloatingPointType_t<Scalar>::T squared_norm () const
    {
        AbstractIndex_c<'i'> i;
        // TODO: once strong typing is done, this will require an inner product
        return operator()(i)*operator()(i);
    }
    // NOTE: will not currently work for complex types
    typename AssociatedFloatingPointType_t<Scalar>::T norm () const
    {
        return std::sqrt(squared_norm());
    }

    // NOTE: operator [] will be used to return values, while operator () will be
    // used to create expression templates for the purposes of indexed contractions.

    Scalar const &operator [] (ComponentIndex const &i) const { return as_derived().Derived::operator[](i); }
    Scalar &operator [] (ComponentIndex const &i) { return as_derived().Derived::operator[](i); }

    template <typename Index_>
    Scalar const &operator [] (MultiIndex_t<TypeList_t<Index_> > const &m) const { return as_derived().Derived::operator[](m.head()); }
    template <typename Index_>
    Scalar &operator [] (MultiIndex_t<TypeList_t<Index_> > const &m) { return as_derived().Derived::operator[](m.head()); }

    // the argument is technically unnecessary, as its value is not used.  however,
    // this allows the template system to deduce the SYMBOL of the AbstractIndex_c, so
    // it doesn't need to be specified explicitly.
    // in this, an outer product would be
    // AbstractIndex_c<'i'> i;
    // AbstractIndex_c<'j'> j;
    // u(i)*v(j)
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Vector_i,
                                       TypeList_t<BasedVectorSpace>,
                                       TypeList_t<DimIndex_t<SYMBOL,DIM> >,
                                       EmptyTypeList,
                                       FORCE_CONST,
                                       CHECK_FOR_ALIASING> operator () (AbstractIndex_c<SYMBOL> const &) const
    {
        STATIC_ASSERT((SYMBOL != '\0'), ABSTRACTINDEX_SYMBOL_MUST_NOT_BE_NULL);
        return ExpressionTemplate_IndexedObject_t<Vector_i,
                                                  TypeList_t<BasedVectorSpace>,
                                                  TypeList_t<DimIndex_t<SYMBOL,DIM> >,
                                                  EmptyTypeList,
                                                  FORCE_CONST,
                                                  CHECK_FOR_ALIASING>(as_derived());
    }
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Vector_i,
                                       TypeList_t<BasedVectorSpace>,
                                       TypeList_t<DimIndex_t<SYMBOL,DIM> >,
                                       EmptyTypeList,
                                       DONT_FORCE_CONST,
                                       CHECK_FOR_ALIASING> operator () (AbstractIndex_c<SYMBOL> const &)
    {
        STATIC_ASSERT((SYMBOL != '\0'), ABSTRACTINDEX_SYMBOL_MUST_NOT_BE_NULL);
        return ExpressionTemplate_IndexedObject_t<Vector_i,
                                                  TypeList_t<BasedVectorSpace>,
                                                  TypeList_t<DimIndex_t<SYMBOL,DIM> >,
                                                  EmptyTypeList,
                                                  DONT_FORCE_CONST,
                                                  CHECK_FOR_ALIASING>(as_derived());
    }

    // NOTE: these are sort of part of the Tensor_i interface, but need Vector_i's cooperation.
    // if the return value for a particular MultiIndex is false, then that component is understood to be zero.
    static bool component_is_immutable_zero (MultiIndex const &m) { return false; }
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return Scalar(1); }
    static ComponentIndex vector_index_of (MultiIndex const &m) { return m.head(); }

    static std::string type_as_string ()
    {
        return "Vector_i<" + TypeStringOf_t<Derived>::eval() + ','
                           + TypeStringOf_t<Scalar>::eval() + ','
                           + TypeStringOf_t<BasedVectorSpace>::eval() + '>';
    }
};

template <typename T> struct IsAVector_i { static bool const V = false; };
template <typename Derived, typename Scalar, typename BasedVectorSpace> struct IsAVector_i<Vector_i<Derived,Scalar,BasedVectorSpace> > { static bool const V = true; };

template <typename Derived, typename Scalar, typename BasedVectorSpace>
std::ostream &operator << (std::ostream &out, Vector_i<Derived,Scalar,BasedVectorSpace> const &v)
{
    typedef Vector_i<Derived,Scalar,BasedVectorSpace> Vector;

    if (Vector::DIM == 0)
        return out << "()";

    typename Vector::ComponentIndex i; // initialized to the beginning automatically
    out << '(' << v[i];
    ++i;
    for ( ; i.is_not_at_end(); ++i)
        out << ", " << v[i];
    return out << ')';
}

} // end of namespace Tenh

#endif // TENH_INTERFACE_VECTOR_HPP_
