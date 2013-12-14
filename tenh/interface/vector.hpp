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
// component_access_without_range_check methods.
template <typename Derived_, typename Scalar_, typename BasedVectorSpace_, bool COMPONENTS_ARE_IMMUTABLE_>
struct Vector_i
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!TypesAreEqual_f<Derived_,NullType>::V), DERIVED_MUST_NOT_BE_NULL_TYPE),
        STATIC_ASSERT_IN_ENUM(IS_BASED_VECTOR_SPACE_UNIQUELY(BasedVectorSpace_), MUST_BE_BASED_VECTOR_SPACE)
    };

    typedef Derived_ Derived;
    typedef Scalar_ Scalar;
    typedef BasedVectorSpace_ BasedVectorSpace;
    static Uint32 const DIM = DimensionOf_f<BasedVectorSpace>::V;

    typedef ComponentIndex_t<DIM> ComponentIndex;
    typedef MultiIndex_t<TypeList_t<ComponentIndex> > MultiIndex;

    // this is necessary so that ExpressionTemplate_IndexedObject_t knows that a vector is
    // a 1-tensor having a particular type.
    typedef TypeList_t<BasedVectorSpace> FactorTypeList;

    // these definitions must coincide with the Array_i ones (if used in conjunction with Array_i)
    static bool const COMPONENTS_ARE_IMMUTABLE = COMPONENTS_ARE_IMMUTABLE_;
    typedef typename If_f<COMPONENTS_ARE_IMMUTABLE_,Scalar_,Scalar_ const &>::T ComponentAccessConstReturnType;
    typedef typename If_f<COMPONENTS_ARE_IMMUTABLE_,Scalar_,Scalar_ &>::T ComponentAccessNonConstReturnType;

    static Uint32 dim () { return DIM; }

    // TODO: add "operator Scalar () const { return as_derived().operator Scalar(); }" with a static assert that DIM == 1

    // TODO: only allow when Basis = Euclidean once strongly-typed vectors are implemented
    // type conversion operator for canonical coercion to Scalar type when the vector is 1-dimensional
    operator ComponentAccessConstReturnType () const
    {
        STATIC_ASSERT((DIM == 1), ONLY_ONE_DIMENSIONAL_VECTORS_CAN_BE_CONVERTED_TO_SCALARS);
        return as_derived().Derived::operator[](ComponentIndex(0, DONT_CHECK_RANGE));
    }
    // this could be implemented as "operator Scalar & ()" but it would be bad to make implicit casts that can be used to change the value of this.
    ComponentAccessNonConstReturnType as_scalar ()
    {
        STATIC_ASSERT((DIM == 1), ONLY_ONE_DIMENSIONAL_VECTORS_CAN_BE_CONVERTED_TO_SCALARS);
        return as_derived().Derived::operator[](ComponentIndex(0, DONT_CHECK_RANGE));
    }

    // accessor as Derived type
    Derived const &as_derived () const { return *static_cast<Derived const *>(this); }
    Derived &as_derived () { return *static_cast<Derived *>(this); }

    // // NOTE: will not currently work for complex types
    // typename AssociatedFloatingPointType_t<Scalar>::T squared_norm () const
    // {
    //     AbstractIndex_c<'i'> i;
    //     // TODO: once strong typing is done, this will require an inner product
    //     return operator()(i)*operator()(i);
    // }
    // // NOTE: will not currently work for complex types
    // typename AssociatedFloatingPointType_t<Scalar>::T norm () const
    // {
    //     return std::sqrt(squared_norm());
    // }

    // NOTE: operator [] will be used to return values, while operator () will be
    // used to create expression templates for the purposes of indexed contractions.

    ComponentAccessConstReturnType operator [] (ComponentIndex const &i) const { return as_derived().operator[](i); }
    ComponentAccessNonConstReturnType operator [] (ComponentIndex const &i) { return as_derived().operator[](i); }

    template <typename Index_>
    ComponentAccessConstReturnType operator [] (MultiIndex_t<TypeList_t<Index_> > const &m) const { return as_derived().operator[](m.head()); }
    template <typename Index_>
    ComponentAccessNonConstReturnType operator [] (MultiIndex_t<TypeList_t<Index_> > const &m) { return as_derived().operator[](m.head()); }

    // because the return type for "operator () (...) const" is an abomination, use this helper.
    template <AbstractIndexSymbol SYMBOL_>
    struct IndexedExpressionConstType_f
    {
        typedef ExpressionTemplate_IndexedObject_t<Vector_i,
                                                   TypeList_t<BasedVectorSpace>,
                                                   TypeList_t<DimIndex_t<SYMBOL_,DIM> >,
                                                   EmptyTypeList,
                                                   FORCE_CONST,
                                                   CHECK_FOR_ALIASING> T;
    };
    // because the return type for "operator () (...)" is an abomination, use this helper.
    template <AbstractIndexSymbol SYMBOL_>
    struct IndexedExpressionNonConstType_f
    {
        typedef ExpressionTemplate_IndexedObject_t<Vector_i,
                                                   TypeList_t<BasedVectorSpace>,
                                                   TypeList_t<DimIndex_t<SYMBOL_,DIM> >,
                                                   EmptyTypeList,
                                                   DONT_FORCE_CONST,
                                                   CHECK_FOR_ALIASING> T;
    };

    // the argument is technically unnecessary, as its value is not used.  however,
    // this allows the template system to deduce the SYMBOL of the AbstractIndex_c, so
    // it doesn't need to be specified explicitly.
    // in this, an outer product would be
    // AbstractIndex_c<'i'> i;
    // AbstractIndex_c<'j'> j;
    // u(i)*v(j)
    template <AbstractIndexSymbol SYMBOL_>
    typename IndexedExpressionConstType_f<SYMBOL_>::T operator () (AbstractIndex_c<SYMBOL_> const &) const
    {
        STATIC_ASSERT((SYMBOL_ != '\0'), ABSTRACT_INDEX_SYMBOL_MUST_BE_POSITIVE);
        return typename IndexedExpressionConstType_f<SYMBOL_>::T(as_derived());
    }
    template <AbstractIndexSymbol SYMBOL_>
    typename IndexedExpressionNonConstType_f<SYMBOL_>::T operator () (AbstractIndex_c<SYMBOL_> const &)
    {
        STATIC_ASSERT((SYMBOL_ != '\0'), ABSTRACT_INDEX_SYMBOL_MUST_BE_POSITIVE);
        return typename IndexedExpressionNonConstType_f<SYMBOL_>::T(as_derived());
    }

    template <AbstractIndexSymbol SYMBOL_>
    typename IndexedExpressionConstType_f<SYMBOL_>::T operator () (TypeList_t<AbstractIndex_c<SYMBOL_> > const &) const
    {
        STATIC_ASSERT((SYMBOL_ != '\0'), ABSTRACT_INDEX_SYMBOL_MUST_BE_POSITIVE);
        return typename IndexedExpressionConstType_f<SYMBOL_>::T(as_derived());
    }
    template <AbstractIndexSymbol SYMBOL_>
    typename IndexedExpressionNonConstType_f<SYMBOL_>::T operator () (TypeList_t<AbstractIndex_c<SYMBOL_> > const &)
    {
        STATIC_ASSERT((SYMBOL_ != '\0'), ABSTRACT_INDEX_SYMBOL_MUST_BE_POSITIVE);
        return typename IndexedExpressionNonConstType_f<SYMBOL_>::T(as_derived());
    }

    Uint32 allocation_size_in_bytes () const { return as_derived().allocation_size_in_bytes(); }
    Scalar_ const *pointer_to_allocation () const { return as_derived().pointer_to_allocation(); }
    Scalar_ *pointer_to_allocation () { return as_derived().pointer_to_allocation(); }
    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const { return as_derived().overlaps_memory_range(ptr, range); }

    // NOTE: these are sort of part of the Tensor_i interface, but need Vector_i's cooperation.
    // if the return value for a particular MultiIndex is false, then that component is understood to be zero.
    static bool component_is_immutable_zero (MultiIndex const &m) { return false; }
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return Scalar(1); }
    static ComponentIndex vector_index_of (MultiIndex const &m) { return m.head(); }

    static std::string type_as_string ()
    {
        return "Vector_i<" + type_string_of<Derived>() + ','
                           + type_string_of<Scalar>() + ','
                           + type_string_of<BasedVectorSpace>() + ','
                           + AS_STRING((COMPONENTS_ARE_IMMUTABLE_ ? "IMMUTABLE_COMPONENTS" : "MUTABLE_COMPONENTS")) + '>';
    }
};

template <typename T_> struct IsAVector_i { static bool const V = false; };
template <typename Derived_, typename Scalar_, typename BasedVectorSpace_, bool COMPONENTS_ARE_IMMUTABLE_> struct IsAVector_i<Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> > { static bool const V = true; };

template <typename Derived_, typename Scalar_, typename BasedVectorSpace_, bool COMPONENTS_ARE_IMMUTABLE_>
std::ostream &operator << (std::ostream &out, Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &v)
{
    typedef Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> Vector;

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
