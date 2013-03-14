#ifndef TENSOR2_HPP_
#define TENSOR2_HPP_

#include <iostream>

#include "core.hpp"
#include "expression_templates.hpp"
#include "vector.hpp"

// general 2-tensor with no symmetries -- most general type of 2-tensor
template <typename F1_, typename F2_>
struct Tensor2_t : Vector_t<typename F1_::Scalar,F1_::DIM*F2_::DIM>
{
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename F1_::Scalar,typename F2_::Scalar>::v>::v };

    typedef Vector_t<typename F1_::Scalar,F1_::DIM*F2_::DIM> Parent;
    typedef typename Parent::Scalar Scalar;
    using Parent::DIM;

    typedef F1_ F1;
    typedef F2_ F2;

    Tensor2_t (WithoutInitialization const &w) : Parent(w) { }
    Tensor2_t (Scalar fill) : Parent(fill) { }

    // type conversion operator for canonical coersion to the F1 or F2 factor type when the
    // tensor is F1 \otimes OneDimVectorSpace  or  OneDimVectorSpace \otimes F2.
//     template <typename Factor>
//     operator Factor const & () const
//     {
//         Lvd::Meta::Assert<(Lvd::Meta::TypesAreEqual<Factor,F1>::v && F2::DIM == 1) || (Lvd::Meta::TypesAreEqual<Factor,F2>::v && F1::DIM == 1)>();
//         return *reinterpret_cast<Factor const *>(&Parent::m[0]);
//     }
//     operator F1 const & () const
//     {
//         Lvd::Meta::Assert<(F2::DIM == 1)>();
//         return *reinterpret_cast<F1 const *>(&Parent::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
//     }
    F1 const &as_factor1 () const
    {
        Lvd::Meta::Assert<(F2::DIM == 1)>();
        return *reinterpret_cast<F1 *>(&Parent::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
    }
    // this could be implemented as "operator F1 & ()" but it would be bad to make implicit casts that can be used to change the value of this.
    F1 &as_factor1 ()
    {
        Lvd::Meta::Assert<(F2::DIM == 1)>();
        return *reinterpret_cast<F1 *>(&Parent::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
    }
    // type conversion operator for canonical coersion to the F2 factor type when the
    // tensor is a tensor product of a 1-dimensional vector space with F2.
//     operator F2 const & () const
//     {
//         Lvd::Meta::Assert<(F1::DIM == 1)>();
//         return *reinterpret_cast<F2 const *>(&Parent::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
//     }
    F2 const &as_factor2 () const
    {
        Lvd::Meta::Assert<(F1::DIM == 1)>();
        return *reinterpret_cast<F2 *>(&Parent::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
    }
    // this could be implemented as "operator F2 & ()" but it would be bad to make implicit casts that can be used to change the value of this.
    F2 &as_factor2 ()
    {
        Lvd::Meta::Assert<(F1::DIM == 1)>();
        return *reinterpret_cast<F2 *>(&Parent::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
    }

    struct DeprecatedIndex : public Parent::Index // TODO: deprecate (it's only used in the test code below)
    {
        DeprecatedIndex () { } // default constructor initializes to beginning
        explicit DeprecatedIndex (Uint32 i) : Parent::Index(i) { }
        DeprecatedIndex (typename F1::Index i1, typename F2::Index i2) : Parent::Index(F2::DIM*i1.value()+i2.value()) { }
        typename F1::Index subindex1 () const { return this->value() / F2::DIM; }
        typename F2::Index subindex2 () const { return this->value() % F2::DIM; }
    };

    // the argument is technically unnecessary, as its value is not used.  however,
    // this allows the template system to deduce the SYMBOL of the IndexType_t, so
    // it doesn't need to be specified explicitly.
    // in this, an outer product would be
    // IndexType_t<'i'> i;
    // IndexType_t<'j'> j;
    // u(i)*v(j)
    // this override of the Parent's operator() is necessary so that the expression template
    // knows that the operand is actually a Tensor2_t.
    template <char SYMBOL>
    ExpressionTemplate_IndexedTensor_t<Tensor2_t,TypeList_t<typename Parent::template Index_t<SYMBOL> > > operator () (
        typename Parent::template Index_t<SYMBOL> const &) const
    {
        return expr<SYMBOL>();
    }
    template <char SYMBOL>
    ExpressionTemplate_AssignableIndexedTensor_t<Tensor2_t,TypeList_t<typename Parent::template Index_t<SYMBOL> > > operator () (
        typename Parent::template Index_t<SYMBOL> const &)
    {
        return expr<SYMBOL>();
    }
    // the corresponding outer product example here would be
    // u.expr<'i'>() * v.expr<'j'>()
    // this override of the Parent's operator() is necessary so that the expression template
    // knows that the operand is actually a Tensor2_t.
    template <char SYMBOL>
    ExpressionTemplate_IndexedTensor_t<Tensor2_t,TypeList_t<typename Parent::template Index_t<SYMBOL> > > expr () const
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedTensor_t<Tensor2_t,TypeList_t<typename Parent::template Index_t<SYMBOL> > >(*this);
    }
    template <char SYMBOL>
    ExpressionTemplate_AssignableIndexedTensor_t<Tensor2_t,TypeList_t<typename Parent::template Index_t<SYMBOL> > > expr ()
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_AssignableIndexedTensor_t<Tensor2_t,TypeList_t<typename Parent::template Index_t<SYMBOL> > >(*this);
    }

    // a 2-tensor can be indexed by the pair of factor indices (F1::Index, F2::Index)
    template <char F1_SYMBOL, char F2_SYMBOL>
    ExpressionTemplate_IndexedTensor_t<Tensor2_t,
                                       TypeList_t<typename F1::template Index_t<F1_SYMBOL>,
                                       TypeList_t<typename F2::template Index_t<F2_SYMBOL> > > > operator () (
        typename F1::template Index_t<F1_SYMBOL> const &,
        typename F2::template Index_t<F2_SYMBOL> const &) const
    {
        return expr<F1_SYMBOL,F2_SYMBOL>();
    }
    template <char F1_SYMBOL, char F2_SYMBOL>
    ExpressionTemplate_AssignableIndexedTensor_t<Tensor2_t,
                                                 TypeList_t<typename F1::template Index_t<F1_SYMBOL>,
                                                 TypeList_t<typename F2::template Index_t<F2_SYMBOL> > > > operator () (
        typename F1::template Index_t<F1_SYMBOL> const &,
        typename F2::template Index_t<F2_SYMBOL> const &)
    {
        return expr<F1_SYMBOL,F2_SYMBOL>();
    }
    // the 2-index analog of expr<SYMBOL>()
    template <char F1_SYMBOL, char F2_SYMBOL>
    ExpressionTemplate_IndexedTensor_t<Tensor2_t,
                                       TypeList_t<typename F1::template Index_t<F1_SYMBOL>,
                                       TypeList_t<typename F2::template Index_t<F2_SYMBOL> > > > expr () const
    {
        Lvd::Meta::Assert<(F1_SYMBOL != '\0')>();
        Lvd::Meta::Assert<(F2_SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedTensor_t<Tensor2_t,
                                                  TypeList_t<typename F1::template Index_t<F1_SYMBOL>,
                                                  TypeList_t<typename F2::template Index_t<F2_SYMBOL> > > >(*this);
    }
    template <char F1_SYMBOL, char F2_SYMBOL>
    ExpressionTemplate_AssignableIndexedTensor_t<Tensor2_t,
                                                 TypeList_t<typename F1::template Index_t<F1_SYMBOL>,
                                                 TypeList_t<typename F2::template Index_t<F2_SYMBOL> > > > expr ()
    {
        Lvd::Meta::Assert<(F1_SYMBOL != '\0')>();
        Lvd::Meta::Assert<(F2_SYMBOL != '\0')>();
        return ExpressionTemplate_AssignableIndexedTensor_t<Tensor2_t,
                                                            TypeList_t<typename F1::template Index_t<F1_SYMBOL>,
                                                            TypeList_t<typename F2::template Index_t<F2_SYMBOL> > > >(*this);
    }
};

template <typename F1, typename F2>
std::ostream &operator << (std::ostream &out, Tensor2_t<F1,F2> const &t)
{
    typedef Tensor2_t<F1,F2> Tensor2;

    if (Tensor2::DIM == 0)
        return out << "[]";

    Uint32 k = 0;
    out << '\n';
    for (typename F1::Index i; i.is_not_at_end(); ++i)
    {
        out << '[';
        for (typename F2::Index j; j.is_not_at_end(); ++j)
        {
            out << t[k] << '\t';
            ++k;
        }
        out << "]\n";
    }
    return out;
}


#endif // TENSOR2_HPP_
