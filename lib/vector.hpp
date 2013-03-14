#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <string>

#include "core.hpp"
#include "expression_templates.hpp"
#include "typestringof.hpp"

template <typename Scalar_, Uint32 DIM_> // don't worry about type ID for now
struct Vector_t
{
    typedef Scalar_ Scalar;
    static Uint32 const DIM = DIM_;

    // for use in operator [] for actual evaluation of tensor components
    struct Index
    {
        static Uint32 const COMPONENT_COUNT = Vector_t::DIM;

        Index () : m(0) { }
        Index (Uint32 i) : m(i) { }

        bool is_at_end () const { return m >= COMPONENT_COUNT; }
        bool is_not_at_end () const { return m < COMPONENT_COUNT; }
        Uint32 value () const { return m; } // for the specific memory addressing scheme that Vector_t uses
        void operator ++ () { ++m; }
        void reset () { m = 0; }

        static std::string type_as_string () { return TypeStringOf_t<Vector_t>::eval() + "::Index"; }

    private:

        Uint32 m;
    };

    // for use in operator () for creation of expression templates (indexed tensor expressions)
    template <char SYMBOL>
    struct Index_t : public Index
    {
        Index_t () { }
        Index_t (Uint32 i) : Index(i) { }

        static std::string type_as_string () { return TypeStringOf_t<Vector_t>::eval() + "::Index_t<'" + SYMBOL + "'>"; }
    };

    explicit Vector_t (WithoutInitialization const &) { }
    explicit Vector_t (Scalar fill) { for (Uint32 i = 0; i < DIM; ++i) m[i] = fill; }
    Vector_t (Scalar x0, Scalar x1) { Lvd::Meta::Assert<(DIM == 2)>(); m[0] = x0; m[1] = x1; }
    Vector_t (Scalar x0, Scalar x1, Scalar x2) { Lvd::Meta::Assert<(DIM == 3)>(); m[0] = x0; m[1] = x1; m[2] = x2; }
    Vector_t (Scalar x0, Scalar x1, Scalar x2, Scalar x3) { Lvd::Meta::Assert<(DIM == 4)>(); m[0] = x0; m[1] = x1; m[2] = x2; m[3] = x3; }

    // type conversion operator for canonical coersion to Scalar type when the vector is 1-dimensional
    operator Scalar const & () const { Lvd::Meta::Assert<(DIM == 1)>(); return m[0]; }
    // this could be implemented as "operator Scalar & ()" but it would be bad to make implicit casts that can be used to change the value of this.
    Scalar &as_scalar () { Lvd::Meta::Assert<(DIM == 1)>(); return m[0]; } // can use this to assign from Scalar

    // NOTE: operator [] will be used to return values, while
    // operator () will be used to create expression templates
    // for the purposes of indexed contractions.
    // TODO: make Index type encode the guarantee that it's value will always be valid
    Scalar operator [] (Index const &i) const
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

    // the argument is technically unnecessary, as its value is not used.  however,
    // this allows the template system to deduce the SYMBOL of the IndexType_t, so
    // it doesn't need to be specified explicitly.
    // in this, an outer product would be
    // IndexType_t<'i'> i;
    // IndexType_t<'j'> j;
    // u(i)*v(j)
    template <char SYMBOL>
    ExpressionTemplate_IndexAsVector_t<Vector_t,Index_t<SYMBOL> > operator () (Index_t<SYMBOL> const &) const
    {
        return expr<SYMBOL>();
    }
    // the corresponding outer product example here would be
    // u.expr<'i'>() * v.expr<'j'>()
    template <char SYMBOL>
    ExpressionTemplate_IndexAsVector_t<Vector_t,Index_t<SYMBOL> > expr () const
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_IndexAsVector_t<Vector_t,Index_t<SYMBOL> >(*this);
    }

    static std::string type_as_string () { return "Vector_t<" + TypeStringOf_t<Scalar>::eval() + ',' + AS_STRING(DIM) + '>'; }

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

// NOTE: these don't seem to work -- maybe the compiler isn't smart enough to match the types
template <typename Scalar, Uint32 DIM>
std::ostream &operator << (std::ostream &out, typename Vector_t<Scalar,DIM>::Index const &i)
{
    return out << i.value();
}

template <typename Scalar, Uint32 DIM, char SYMBOL>
std::ostream &operator << (std::ostream &out, typename Vector_t<Scalar,DIM>::template Index_t<SYMBOL> const &i)
{
    return out << i.value();
}

// NOTE: this is not used yet, but is sort of the conceptual placeholder for the natural pairing on a
// vector space (with respect to a particular basis)
// this default implementation should work for any vector space that has an Index type (but not e.g. Tensor2Simple_t).
// template specialization can be used to define other implementations (e.g. Tensor2Simple_t).
template <typename Vector>
struct DotProduct_t
{
    typedef typename Vector::Scalar Scalar;
    typedef typename Vector::Index Index;

    static Scalar eval (Vector const &l, Vector const &r)
    {
        Scalar retval(0);
        for (Index i; i.is_not_at_end(); ++i)
            retval += l[i]*r[i];
        return retval;
    }
};


#endif // VECTOR_HPP_
