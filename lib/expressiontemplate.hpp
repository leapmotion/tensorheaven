#ifndef EXPRESSIONTEMPLATE_HPP_
#define EXPRESSIONTEMPLATE_HPP_

#include <ostream>

#include "core.hpp"
#include "compoundindex.hpp"

// this is essentially a compile-time interface, requiring:
// - a Derived type (should be the type of the thing that ultimately inherits this)
// - a Scalar type (should be the scalar type of the expression template's tensor operand)
// - a FreeIndexTypeList type (the free indices of this expression template)
// - a UsedIndexTypeList type (the indices that have been used further down the AST)
// requires also particular methods:
//   operator Scalar () const // conversion to scalar -- always declared, but should
//                            // only compile if the conversion is well-defined (e.g. no free indices)
//   Scalar operator [] (CompoundIndex const &) const // accessor using CompoundIndex_t<FreeIndexTypeList>
//   template <typename OtherTensor> bool uses_tensor (OtherTensor const &) const // used in checking for aliasing
template <typename Derived_, typename Scalar_, typename FreeIndexTypeList_, typename UsedIndexTypeList_>
struct ExpressionTemplate_i // _i is for "compile-time interface"
{
    typedef Derived_ Derived;
    // these typedefs make the Derived-specified typedefs available at the baseclass level,
    typedef Scalar_ Scalar;
    typedef FreeIndexTypeList_ FreeIndexTypeList;
    typedef UsedIndexTypeList_ UsedIndexTypeList;
    typedef CompoundIndex_t<FreeIndexTypeList> CompoundIndex;
    static bool const IS_EXPRESSION_TEMPLATE = true;

    // TODO: some consistency checks on the various types

    // compile-time interface methods
    operator Scalar () const { return as_derived().operator Scalar(); }
    Scalar operator [] (CompoundIndex const &c) const { return as_derived().operator[](c); }
    template <typename OtherTensor> bool uses_tensor (OtherTensor const &t) const { return as_derived().template uses_tensor<OtherTensor>(t); }

    // for accessing this as the Derived type
    Derived const &as_derived () const { return *static_cast<Derived const *>(this); }
    Derived &as_derived () { return *static_cast<Derived *>(this); }
};

// it's just more convenient to write specializations for particular numbers of free indices

// specialization for 1 index
template <typename Derived, typename Scalar, typename FreeIndex1, typename UsedIndexTypeList>
std::ostream &operator << (std::ostream &out, ExpressionTemplate_i<Derived,Scalar,TypeList_t<FreeIndex1>,UsedIndexTypeList> const &e)
{
    FreeIndex1 i; // initialized to the beginning automatically
    out << '(' << e[i];
    ++i;
    for ( ; i.is_not_at_end(); ++i)
        out << ", " << e[i];
    return out << ")(" << FreeIndex1::SYMBOL << ')';
}

// specialization for 2 indices
template <typename Derived, typename Scalar, typename FreeIndex1, typename FreeIndex2, typename UsedIndexTypeList>
std::ostream &operator << (std::ostream &out, ExpressionTemplate_i<Derived,Scalar,TypeList_t<FreeIndex1,TypeList_t<FreeIndex2> >,UsedIndexTypeList> const &e)
{
    typename Derived::CompoundIndex c;
    out << "\n[";
    for (FreeIndex1 i; i.is_not_at_end(); ++i)
    {
        if (i.value() != 0)
            out << ' ';
        out << '[';
        for (FreeIndex2 j; j.is_not_at_end(); ++j)
        {
            out << e[c] << '\t';
            ++c;
        }
        out << ']';
        FreeIndex1 next(i);
        ++next;
        if (next.is_not_at_end())
            out << '\n';
    }
    out << "](" << FreeIndex1::SYMBOL << ',' << FreeIndex2::SYMBOL << ")\n";
    return out;
}

// specialization for 3 indices
template <typename Derived, typename Scalar, typename FreeIndex1, typename FreeIndex2, typename FreeIndex3, typename UsedIndexTypeList>
std::ostream &operator << (std::ostream &out, ExpressionTemplate_i<Derived,Scalar,TypeList_t<FreeIndex1,TypeList_t<FreeIndex2,TypeList_t<FreeIndex3> > >,UsedIndexTypeList> const &e)
{
    typename Derived::CompoundIndex c;
    out << "\n[";
    for (FreeIndex1 i; i.is_not_at_end(); ++i)
    {
        if (i.value() != 0)
            out << ' ';
        out << '[';
        for (FreeIndex2 j; j.is_not_at_end(); ++j)
        {
            if (j.value() != 0)
                out << "  ";
            out << '[';
            for (FreeIndex3 k; k.is_not_at_end(); ++k)
            {
                out << e[c] << '\t';
                ++c;
            }
            out << ']';
            FreeIndex2 next(j);
            ++next;
            if (next.is_not_at_end())
                out << '\n';
        }
        out << ']';
        FreeIndex1 next(i);
        ++next;
        if (next.is_not_at_end())
            out << "\n\n";
    }
    out << "](" << FreeIndex1::SYMBOL << ',' << FreeIndex2::SYMBOL << ',' << FreeIndex3::SYMBOL << ")\n";
    return out;
}

#endif // EXPRESSIONTEMPLATE_HPP_
