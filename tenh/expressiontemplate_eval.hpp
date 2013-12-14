// ///////////////////////////////////////////////////////////////////////////
// tenh/expressiontemplate_eval.hpp by Victor Dods, created 2013/04/09
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_EXPRESSIONTEMPLATE_EVAL_HPP_
#define TENH_EXPRESSIONTEMPLATE_EVAL_HPP_

#include "tenh/core.hpp"

#include "tenh/implementation/tensor.hpp"
#include "tenh/interface/expressiontemplate.hpp"

namespace Tenh {

// ////////////////////////////////////////////////////////////////////////////
// evaluating an indexed expression for the purpose of avoiding aliasing
// ////////////////////////////////////////////////////////////////////////////

// this is an expression template which caches the evaluation of its operand
// so that aliasing can be avoided in particular expression template assignments.
// TODO: should this optionally USE_PREALLOCATED_ARRAY ?
template <typename Operand>
struct ExpressionTemplate_Eval_t
    :
    public ExpressionTemplate_i<ExpressionTemplate_Eval_t<Operand>,
                                typename Operand::Scalar,
                                typename Operand::FreeFactorTypeList,
                                typename Operand::FreeDimIndexTypeList,
                                typename Operand::UsedDimIndexTypeList>
{
    enum
    {
        STATIC_ASSERT_IN_ENUM(IsExpressionTemplate_f<Operand>::V, OPERAND_IS_EXPRESSION_TEMPLATE)
    };

    typedef ExpressionTemplate_i<ExpressionTemplate_Eval_t<Operand>,
                                 typename Operand::Scalar,
                                 typename Operand::FreeFactorTypeList,
                                 typename Operand::FreeDimIndexTypeList,
                                 typename Operand::UsedDimIndexTypeList> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeFactorTypeList FreeFactorTypeList;
    typedef typename Parent::FreeDimIndexTypeList FreeDimIndexTypeList;
    typedef typename Parent::UsedDimIndexTypeList UsedDimIndexTypeList;
    typedef typename Parent::MultiIndex MultiIndex;

    ExpressionTemplate_Eval_t (Operand const &operand)
        :
        m_operand(operand),
        m_eval_is_cached(false),
        m_cached_tensor(Static<WithoutInitialization>::SINGLETON)
    { }

//     operator Scalar () const // TODO: only use this in the no-free-index one
//     {
//         STATIC_ASSERT_TYPELIST_IS_EMPTY(FreeIndexTypeList);
//         return operator[](MultiIndex());
//     }

    // requires InnerProduct_t to be implemented for all free-indexed types
    // NOTE: will not currently work for complex types
    typename AssociatedFloatingPointType_t<Scalar>::T squared_norm () const
    {
        ensure_tensor_is_cached();
        return m_cached_tensor.squared_norm();
    }
    // requires InnerProduct_t to be implemented for all free-indexed types
    // NOTE: will not currently work for complex types
    typename AssociatedFloatingPointType_t<Scalar>::T norm () const
    {
        this->ensure_tensor_is_cached();
        return m_cached_tensor.norm();
    }

    Scalar const &operator [] (MultiIndex const &m) const
    {
        this->ensure_tensor_is_cached();
        return m_cached_tensor[m];
    }

    typedef TensorProductOfBasedVectorSpaces_c<FreeFactorTypeList> ExpressionTensorType;
    typedef ImplementationOf_t<ExpressionTensorType,Scalar,UseMemberArray> EvaluatedTensor;

    // returns the cached value, caching it if necessary.
    EvaluatedTensor const &value () const
    {
        this->ensure_tensor_is_cached();
        return m_cached_tensor;
    }
    // used e.g. for passing the value of an indexed expression into a function,
    // such as in
    //   function((x(i)*y(j)).eval())
    // where the parameter to function is of type EvaluatedTensor const &,
    // or BaseClass const &, where BaseClass is a base class of EvaluatedTensor.
    operator EvaluatedTensor const & () const { return value(); }
    // operator EvaluatedTensor () const { return value(); }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const { return false; }

    Operand const &operand () const { return m_operand; }

    static std::string type_as_string ()
    {
        return "ExpressionTemplate_Eval_t<" + type_string_of<Operand>() + '>';
    }

private:

    void ensure_tensor_is_cached () const
    {
        if (!m_eval_is_cached)
        {
            // this should populate m_cached_tensor via expression templates
            m_cached_tensor(FreeDimIndexTypeList()).no_alias() = m_operand;
            m_eval_is_cached = true;
        }
    }

    void operator = (ExpressionTemplate_Eval_t const &);

    Operand const &m_operand;
    mutable bool m_eval_is_cached;
    mutable EvaluatedTensor m_cached_tensor;
};

template <typename Operand_>
struct IsExpressionTemplate_f<ExpressionTemplate_Eval_t<Operand_> >
{
    static bool const V = true;
private:
    IsExpressionTemplate_f();
};

// definitions of the squared_norm and norm methods of ExpressionTemplate_i had to wait until ExpressionTemplate_Eval_t was defined.
template <typename Derived, typename Scalar, typename FreeFactorTypeList, typename FreeIndexTypeList, typename UsedIndexTypeList>
typename AssociatedFloatingPointType_t<Scalar>::T ExpressionTemplate_i<Derived,Scalar,FreeFactorTypeList,FreeIndexTypeList,UsedIndexTypeList>::squared_norm () const
{
    return eval().squared_norm();
}

template <typename Derived, typename Scalar, typename FreeFactorTypeList, typename FreeIndexTypeList, typename UsedIndexTypeList>
typename AssociatedFloatingPointType_t<Scalar>::T ExpressionTemplate_i<Derived,Scalar,FreeFactorTypeList,FreeIndexTypeList,UsedIndexTypeList>::norm () const
{
    return eval().norm();
}

} // end of namespace Tenh

#endif // TENH_EXPRESSIONTEMPLATE_EVAL_HPP_
