// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/function.hpp by Victor Dods, created 2013/12/25
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

/// @file meta/function.hpp
/// @headerfile function.hpp "tenh/meta/function.hpp"
/// @brief Contains primitives and helpers for using and constructing
/// metafunctions.
#ifndef TENH_META_FUNCTION_HPP_
#define TENH_META_FUNCTION_HPP_

#include "tenh/core.hpp"

#include "tenh/meta/core.hpp"
#include "tenh/meta/typelist.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// higher-order metafunctions
// ///////////////////////////////////////////////////////////////////////////

// apply a metafunction to each element in a TypeList_t

template <typename TypeList_, typename FunctionEvaluator_>
struct OnEach_f
{
    typedef TypeList_t<typename FunctionEvaluator_::template Eval_f<typename TypeList_::HeadType>::T,
                       typename OnEach_f<typename TypeList_::BodyTypeList,FunctionEvaluator_>::T> T;
};

template <typename FunctionEvaluator_>
struct OnEach_f<EmptyTypeList,FunctionEvaluator_>
{
    typedef EmptyTypeList T;
};

// composition of metafunctions -- each type in the typelist must be an evaluator.

// forward declaration
template <typename EvaluatorTypeList_> struct CompositionOf_e;

template <typename HeadEvaluator_, typename EvaluatorBodyTypeList_>
struct CompositionOf_e<TypeList_t<HeadEvaluator_,EvaluatorBodyTypeList_> >
{
private:
    typedef CompositionOf_e<EvaluatorBodyTypeList_> BodyComposition;
public:
    template <typename T_>
    struct Eval_f
    {
    private:
        typedef typename BodyComposition::template Eval_f<T_>::T BodyEvaluation;
    public:
        typedef typename HeadEvaluator_::template Eval_f<BodyEvaluation>::T T;
    };
};

template <typename HeadEvaluator_>
struct CompositionOf_e<TypeList_t<HeadEvaluator_> >
{
    template <typename T_>
    struct Eval_f
    {
        typedef typename HeadEvaluator_::template Eval_f<T_>::T T;
    };
};

} // end of namespace Tenh

#endif // !defined(TENH_META_FUNCTION_HPP_)
