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

// a "raw" metafunction has a domain and codomain one of which is not a
// type, and whose evaluation consists of "dereferencing" a type T or a
// value V via the scope operator.  some examples are
//
// template <Uint64 N_> struct Square_f { static Uint64 const V = N_*N_; };
// template <typename TypeList_> struct Length_f { static Uint64 const V = ...; };
// template <bool CONDITION_> struct FancyType_f { typedef ... T; };
//
// these raw metafunctions would be used as follows.
//   Square_f<36>::V
//   Length_f<TypeList_t<...> >::V
//   FancyType_f<true>::T     or     typename FancyType_f<true>::T
// 
// without knowing the domain/codomain, it's difficult to use this type
// of metafunction in a higher-order function.  thus leading to...
//
// a "primitive" metafunction is a metafunction whose domain and codomain
// are both types, and the evaluation of the function is done by dereferencing
// a type T.  This makes the interface regular.  An example is
// 
// template <typename T_>
// struct DoubleOf_f
// {
//     typedef TypeList_t<T_,TypeList_t<T_> > T;
// };
//
// the type can be a Value_t<ReturnType,VALUE> so that numeric-valued functions
// can be defined.  In that case, it is helpful to also define a
// "static ReturnType const V = VALUE" member so that extra dereferencing doesn't
// need to be done.  A primitive function can have multiple parameters.
//
// an "evaluator" is a structure that has a primitive metafunction called 
// Eval_f inside.  it has the form:
//
// template <...>
// struct FunctionName_e
// {
//     template <typename Arg_> struct Eval_f { typedef ... T; };
// };
//
// then FunctionName_e<...> can be passed into another template as a parameter
// which essentially allows the use of higher-order functions.

// ///////////////////////////////////////////////////////////////////////////
// macros to make evaluator wrappers so that metafunctions can be passed
// in as template parameters like in any real functional language
// ///////////////////////////////////////////////////////////////////////////

// if there is a primitive metafunction, say
// template <typename Arg_> struct FunctionName_f { typedef ... T; };
// then MAKE_1_ARY_TYPE_EVALUATOR(FunctionName) will define an evaluator
// called FunctionName_e for it.

#define MAKE_1_ARY_TYPE_EVALUATOR(MetaFunctionName) \
struct MetaFunctionName##_e \
{ \
    template <typename T_> \
    struct Eval_f \
    { \
        typedef typename MetaFunctionName##_f<T_>::T T; \
    }; \
}

// if there is a primitive metafunction, say
// template <typename Arg1_, Type2 Name2> struct FunctionName_f { typedef ... T; };
// then MAKE_2_ARY_TYPE_EVALUATOR(FunctionName, Type2, Name2) will define an
// evaluator called FunctionName_e<Type2 Name2> for it; its Eval_f only takes
// Arg1_ as a parameter -- the second parameter is specialized via the 
// parameterization of FunctionName_e.

#define MAKE_2_ARY_TYPE_EVALUATOR(MetaFunctionName, Type2, Name2) \
template <Type2 Name2> \
struct MetaFunctionName##_e \
{ \
    template <typename T_> \
    struct Eval_f \
    { \
        typedef typename MetaFunctionName##_f<T_,Name2>::T T; \
    }; \
}

// if there is a "primitive" metafunction, say
// template <typename Arg1_, Type2 Name2, Type3 Name3> struct FunctionName_f { typedef ... T; };
// then MAKE_2_ARY_TYPE_EVALUATOR(FunctionName, Type2, Name2, Type3, Name3) will define an
// evaluator called FunctionName_e<Type2 Name2, Type3 Name3> for it; its Eval_f only takes
// Arg1_ as a parameter -- the second and third parameters are specialized via the 
// parameterization of FunctionName_e.

#define MAKE_3_ARY_TYPE_EVALUATOR(MetaFunctionName, Type2, Name2, Type3, Name3) \
template <Type2 Name2, Type3 Name3> \
struct MetaFunctionName##_e \
{ \
    template <typename T_> \
    struct Eval_f \
    { \
        typedef typename MetaFunctionName##_f<T_,Name2,Name3>::T T; \
    }; \
}

// if there is a "primitive" metafunction, say
// template <typename Arg_> struct FunctionName_f { static ReturnType const V =  ...; };
// whose T type is of type Value_t<...> (e.g. Value_t<Uint32,123>), then
// MAKE_1_ARY_VALUE_EVALUATOR(FunctionName, ReturnType) will define an evaluator
// called FunctionName_e for it.  In this case, Eval_f also has a static
// const V value whose type is ReturnType.

#define MAKE_1_ARY_VALUE_EVALUATOR(MetaFunctionName, MetaFunctionValueType) \
struct MetaFunctionName##_e \
{ \
    template <typename T_> \
    struct Eval_f \
    { \
        static MetaFunctionValueType const V = MetaFunctionName##_f<T_>::V; \
        typedef Value_t<MetaFunctionValueType,V> T; \
    }; \
}

// if there is a "primitive" metafunction, say
// template <typename Arg1_, Type2 Name2> struct FunctionName_f { static ReturnType const V =  ...; };
// whose T type is of type Value_t<...> (e.g. Value_t<Uint32,123>), then
// MAKE_2_ARY_VALUE_EVALUATOR(FunctionName, ReturnType, Type2, Name2) will define an
// evaluator called FunctionName_e<Type2 Name2> for it; its Eval_f only takes
// Arg1_ as a parameter -- the second parameter is specialized via the 
// parameterization of FunctionName_e.  In this case, Eval_f also has a static
// const V value whose type is ReturnType.

#define MAKE_2_ARY_VALUE_EVALUATOR(MetaFunctionName, MetaFunctionValueType, Type2, Name2) \
template <Type2 Name2> \
struct MetaFunctionName##_e \
{ \
    template <typename T_> \
    struct Eval_f \
    { \
        static MetaFunctionValueType const V = MetaFunctionName##_f<T_,Name2>::V; \
        typedef Value_t<MetaFunctionValueType,V> T; \
    }; \
}

// if there is a "global" metafunction, say
// template <typename Arg1_, Type2 Name2, Type3 Name3> struct FunctionName_f { static ReturnType const V =  ...; };
// whose T type is of type Value_t<...> (e.g. Value_t<Uint32,123>), then
// MAKE_2_ARY_TYPE_EVALUATOR(FunctionName, ReturnType, Type2, Name2, Type3, Name3) will define an
// evaluator called FunctionName_e<Type2 Name2, Type3 Name3> for it; its Eval_f only takes
// Arg1_ as a parameter -- the second and third parameters are specialized via the 
// parameterization of FunctionName_e.  In this case, Eval_f also has a static
// const V value whose type is ReturnType.

#define MAKE_3_ARY_VALUE_EVALUATOR(MetaFunctionName, MetaFunctionValueType, Type2, Name2, Type3, Name3) \
template <Type2 Name2, Type3 Name3> \
struct MetaFunctionName##_e \
{ \
    template <typename T_> \
    struct Eval_f \
    { \
        static MetaFunctionValueType const V = MetaFunctionName##_f<T_,Name2,Name3>::V; \
        typedef Value_t<MetaFunctionValueType,V> T; \
    }; \
}

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
