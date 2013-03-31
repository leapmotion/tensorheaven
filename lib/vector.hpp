#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <ostream>
#include <string>

#include "compoundindex.hpp"
#include "core.hpp"
#include "expression_templates.hpp"
#include "index.hpp"
#include "typestringof.hpp"

// NOTE: Scalar_ MUST be a POD data type.
template <typename Scalar_, Uint32 DIM_, typename Derived_ = NullType> // don't worry about type ID for now
struct Vector_t
{
    enum { DIMENSION_MUST_BE_POSITIVE = Lvd::Meta::Assert<(DIM_ > 0)>::v };

    typedef Scalar_ Scalar;
    static Uint32 const DIM = DIM_;
    // if Derived_ was the default NullType, then just use Vector_t as Derived.
    typedef typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,Vector_t,Derived_>::T Derived;
    // here is the "basic" (non-named) Index of this vector type, and it is aware of Derived
    typedef Index_t<Derived> Index;
    // the CompoundIndex_t encapsulation of Index
    typedef CompoundIndex_t<typename TypeTuple_t<Index>::T> CompoundIndex;

    explicit Vector_t (WithoutInitialization const &) { }
    explicit Vector_t (Scalar fill_with) { for (Uint32 i = 0; i < DIM; ++i) m[i] = fill_with; }
    Vector_t (Scalar x0, Scalar x1) { Lvd::Meta::Assert<(DIM == 2)>(); m[0] = x0; m[1] = x1; }
    Vector_t (Scalar x0, Scalar x1, Scalar x2) { Lvd::Meta::Assert<(DIM == 3)>(); m[0] = x0; m[1] = x1; m[2] = x2; }
    Vector_t (Scalar x0, Scalar x1, Scalar x2, Scalar x3) { Lvd::Meta::Assert<(DIM == 4)>(); m[0] = x0; m[1] = x1; m[2] = x2; m[3] = x3; }

    // accessor as Derived type
    Derived const &as_derived () const { return *static_cast<Derived const *>(this); }
    Derived &as_derived () { return *static_cast<Derived *>(this); }

    // TODO: only allow when Basis = Unit (or generic) once strongly-typed vectors are implemented
    // type conversion operator for canonical coercion to Scalar type when the vector is 1-dimensional
    operator Scalar const & () const { Lvd::Meta::Assert<(DIM == 1)>(); return m[0]; }
    // this could be implemented as "operator Scalar & ()" but it would be bad to make implicit casts that can be used to change the value of this.
    Scalar &as_scalar () { Lvd::Meta::Assert<(DIM == 1)>(); return m[0]; } // can use this to assign from Scalar

    // this SHOULD be inconvenient and ugly to call.  it should be used ONLY when you know for certain that 0 <= i < DIM
    Scalar const &component_access_without_range_check (Uint32 i) const { return m[i]; }
    // this SHOULD be inconvenient and ugly to call.  it should be used ONLY when you know for certain that 0 <= i < DIM
    Scalar &component_access_without_range_check (Uint32 i) { return m[i]; }

    // NOTE: operator [] will be used to return values, while
    // operator () will be used to create expression templates
    // for the purposes of indexed contractions.
    // TODO: make Index type encode the guarantee that it's value will always be valid
    Scalar const &operator [] (Index const &i) const
    {
        if (i.is_at_end())
            throw std::invalid_argument("index out of range");
        else
            return m[i.value()];
    }
    Scalar &operator [] (Index const &i)
    {
        if (i.is_at_end())
            throw std::invalid_argument("index out of range");
        else
            return m[i.value()];
    }
    template <typename Index_>
    Scalar const &operator [] (CompoundIndex_t<TypeList_t<Index_> > const &c) const
    {
        return operator[](c.template el<0>());
    }
    template <typename Index_>
    Scalar &operator [] (CompoundIndex_t<TypeList_t<Index_> > const &c)
    {
        return operator[](c.template el<0>());
    }

    // the argument is technically unnecessary, as its value is not used.  however,
    // this allows the template system to deduce the SYMBOL of the IndexType_t, so
    // it doesn't need to be specified explicitly.
    // in this, an outer product would be
    // IndexType_t<'i'> i;
    // IndexType_t<'j'> j;
    // u(i)*v(j)
    template <char SYMBOL> // TODO: should the expression template know about Derived instead of Vector_t ?
    ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList> operator () (NamedIndex_t<Derived,SYMBOL> const &) const
    {
        return expr<SYMBOL>();
    }
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList > operator () (NamedIndex_t<Derived,SYMBOL> const &)
    {
        return expr<SYMBOL>();
    }
    // the corresponding outer product example here would be
    // u.expr<'i'>() * v.expr<'j'>()
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList> expr () const
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList>(as_derived());
    }
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList> expr ()
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList>(as_derived());
    }

    static std::string type_as_string ()
    {
        // TODO: return Derived's type_as_string value?
//         if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
//             return "Vector_t<" + TypeStringOf_t<Scalar>::eval() + ',' + AS_STRING(DIM) + '>';
//         else
//             return Derived::type_as_string();
        // for now, just return this type string
        if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
            return "Vector_t<" + TypeStringOf_t<Scalar>::eval() + ',' + AS_STRING(DIM) + '>';
        else
            return "Vector_t<" + TypeStringOf_t<Scalar>::eval() + ',' + AS_STRING(DIM) + ',' + TypeStringOf_t<Derived>::eval() + '>';
    }

    Uint32 data_size_in_bytes () const { return sizeof(m); }
    Scalar const *data_pointer () const { return &m[0]; }
    Scalar *data_pointer () { return &m[0]; }

protected:

    Scalar m[DIM];
};

template <typename Scalar, Uint32 DIM>
std::ostream &operator << (std::ostream &out, Vector_t<Scalar,DIM> const &v)
{
    typedef Vector_t<Scalar,DIM> Vector;
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

#endif // VECTOR_HPP_
