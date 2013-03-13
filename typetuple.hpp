#ifndef TYPETUPLE_HPP_
#define TYPETUPLE_HPP_

#include "core.hpp"
#include "typelist.hpp"

// fucking stupid kludge to deal with the lack of variadic templates

template <typename T0 = NullType,
          typename T1 = NullType,
          typename T2 = NullType,
          typename T3 = NullType,
          typename T4 = NullType,
          typename T5 = NullType,
          typename T6 = NullType,
          typename T7 = NullType> 
struct TypeTuple_t
{
    typedef TypeList_t<T0,
            TypeList_t<T1,
            TypeList_t<T2,
            TypeList_t<T3,
            TypeList_t<T4,
            TypeList_t<T5,
            TypeList_t<T6,
            TypeList_t<T7> > > > > > > > T;
};

template <>
struct TypeTuple_t<NullType,NullType,NullType,NullType,NullType,NullType,NullType,NullType>
{
    typedef EmptyTypeList T;
};

template <typename T0>
struct TypeTuple_t<T0,NullType,NullType,NullType,NullType,NullType,NullType,NullType>
{
    typedef TypeList_t<T0> T;
};

template <typename T0,
          typename T1>
struct TypeTuple_t<T0,T1,NullType,NullType,NullType,NullType,NullType,NullType>
{
    typedef TypeList_t<T0,
            TypeList_t<T1> > T;
};

template <typename T0,
          typename T1,
          typename T2>
struct TypeTuple_t<T0,T1,T2,NullType,NullType,NullType,NullType,NullType>
{
    typedef TypeList_t<T0,
            TypeList_t<T1,
            TypeList_t<T2> > > T;
};

template <typename T0,
          typename T1,
          typename T2,
          typename T3>
struct TypeTuple_t<T0,T1,T2,T3,NullType,NullType,NullType,NullType>
{
    typedef TypeList_t<T0,
            TypeList_t<T1,
            TypeList_t<T2,
            TypeList_t<T3> > > > T;
};

template <typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4>
struct TypeTuple_t<T0,T1,T2,T3,T4,NullType,NullType,NullType>
{
    typedef TypeList_t<T0,
            TypeList_t<T1,
            TypeList_t<T2,
            TypeList_t<T3,
            TypeList_t<T4> > > > > T;
};

template <typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5>
struct TypeTuple_t<T0,T1,T2,T3,T4,T5,NullType,NullType>
{
    typedef TypeList_t<T0,
            TypeList_t<T1,
            TypeList_t<T2,
            TypeList_t<T3,
            TypeList_t<T4,
            TypeList_t<T5> > > > > > T;
};

template <typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6>
struct TypeTuple_t<T0,T1,T2,T3,T4,T5,T6,NullType>
{
    typedef TypeList_t<T0,
            TypeList_t<T1,
            TypeList_t<T2,
            TypeList_t<T3,
            TypeList_t<T4,
            TypeList_t<T5,
            TypeList_t<T6> > > > > > > T;
};

#endif // TYPETUPLE_HPP_
