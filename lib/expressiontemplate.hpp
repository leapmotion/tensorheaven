#ifndef EXPRESSIONTEMPLATE_HPP_
#define EXPRESSIONTEMPLATE_HPP_

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

#endif // EXPRESSIONTEMPLATE_HPP_
