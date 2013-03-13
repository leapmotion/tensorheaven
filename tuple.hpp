#ifndef TUPLE_HPP_
#define TUPLE_HPP_

#include "core.hpp"
#include "list.hpp"
#include "typetuple.hpp"

// should match the template params for TypeTuple_t
template <typename T0 = NullType,
          typename T1 = NullType,
          typename T2 = NullType,
          typename T3 = NullType,
          typename T4 = NullType,
          typename T5 = NullType,
          typename T6 = NullType,
          typename T7 = NullType>
struct Tuple_t
    :
    public List_t<typename TypeTuple_t<T0,T1,T2,T3,T4,T5,T6,T7>::T>
{
    typedef List_t<typename TypeTuple_t<T0,T1,T2,T3,T4,T5,T6,T7>::T> Parent;
    typedef Tuple_t<T1,T2,T3,T4,T5,T6,T7> BodyTuple;
    Tuple_t (T0 const &t0,
             T1 const &t1,
             T2 const &t2,
             T3 const &t3,
             T4 const &t4,
             T5 const &t5,
             T6 const &t6,
             T7 const &t7)
        :
        Parent(t0, BodyTuple(t1, t2, t3, t4, t5, t6, t7))
    { }
};

template <>
struct Tuple_t<NullType,NullType,NullType,NullType,NullType,NullType,NullType,NullType>
    :
    public List_t<EmptyTypeList>
{
    typedef List_t<EmptyTypeList> Parent;
    Tuple_t ()
        :
        Parent()
    { }
};

template <typename T0>
struct Tuple_t<T0,NullType,NullType,NullType,NullType,NullType,NullType,NullType>
    :
    public List_t<typename TypeTuple_t<T0>::T>
{
    typedef List_t<typename TypeTuple_t<T0>::T> Parent;
    Tuple_t (T0 const &t0)
        :
        Parent(t0)
    { }
};

template <typename T0, typename T1>
struct Tuple_t<T0,T1,NullType,NullType,NullType,NullType,NullType,NullType>
    :
    public List_t<typename TypeTuple_t<T0,T1>::T>
{
    typedef List_t<typename TypeTuple_t<T0,T1>::T> Parent;
    typedef Tuple_t<T1> BodyTuple;
    Tuple_t (T0 const &t0,
             T1 const &t1)
        :
        Parent(t0, BodyTuple(t1))
    { }
};

template <typename T0, typename T1, typename T2>
struct Tuple_t<T0,T1,T2,NullType,NullType,NullType,NullType,NullType>
    :
    public List_t<typename TypeTuple_t<T0,T1,T2>::T>
{
    typedef List_t<typename TypeTuple_t<T0,T1,T2>::T> Parent;
    typedef Tuple_t<T1,T2> BodyTuple;
    Tuple_t (T0 const &t0,
             T1 const &t1,
             T2 const &t2)
        :
        Parent(t0, BodyTuple(t1, t2))
    { }
};

template <typename T0, typename T1, typename T2, typename T3>
struct Tuple_t<T0,T1,T2,T3,NullType,NullType,NullType,NullType>
    :
    public List_t<typename TypeTuple_t<T0,T1,T2,T3>::T>
{
    typedef List_t<typename TypeTuple_t<T0,T1,T2,T3>::T> Parent;
    typedef Tuple_t<T1,T2,T3> BodyTuple;
    Tuple_t (T0 const &t0,
             T1 const &t1,
             T2 const &t2,
             T3 const &t3)
        :
        Parent(t0, BodyTuple(t1, t2, t3))
    { }
};

template <typename T0, typename T1, typename T2, typename T3, typename T4>
struct Tuple_t<T0,T1,T2,T3,T4,NullType,NullType,NullType>
    :
    public List_t<typename TypeTuple_t<T0,T1,T2,T3,T4>::T>
{
    typedef List_t<typename TypeTuple_t<T0,T1,T2,T3,T4>::T> Parent;
    typedef Tuple_t<T1,T2,T3,T4> BodyTuple;
    Tuple_t (T0 const &t0,
             T1 const &t1,
             T2 const &t2,
             T3 const &t3,
             T4 const &t4)
        :
        Parent(t0, BodyTuple(t1, t2, t3, t4))
    { }
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
struct Tuple_t<T0,T1,T2,T3,T4,T5,NullType,NullType>
    :
    public List_t<typename TypeTuple_t<T0,T1,T2,T3,T4,T5>::T>
{
    typedef List_t<typename TypeTuple_t<T0,T1,T2,T3,T4,T5>::T> Parent;
    typedef Tuple_t<T1,T2,T3,T4,T5> BodyTuple;
    Tuple_t (T0 const &t0,
             T1 const &t1,
             T2 const &t2,
             T3 const &t3,
             T4 const &t4,
             T5 const &t5)
        :
        Parent(t0, BodyTuple(t1, t2, t3, t4, t5))
    { }
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
struct Tuple_t<T0,T1,T2,T3,T4,T5,T6,NullType>
    :
    public List_t<typename TypeTuple_t<T0,T1,T2,T3,T4,T5,T6>::T>
{
    typedef List_t<typename TypeTuple_t<T0,T1,T2,T3,T4,T5,T6>::T> Parent;
    typedef Tuple_t<T1,T2,T3,T4,T5,T6> BodyTuple;
    Tuple_t (T0 const &t0,
             T1 const &t1,
             T2 const &t2,
             T3 const &t3,
             T4 const &t4,
             T5 const &t5,
             T6 const &t6)
        :
        Parent(t0, BodyTuple(t1, t2, t3, t4, t5, t6))
    { }
};

// template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
// struct Tuple_t<T0,T1,T2,T3,T4,T5,T6,T7>
//     :
//     public List_t<TypeTuple_t<T0,T1,T2,T3,T4,T5,T6,T7> >
// {
//     typedef List_t<TypeTuple_t<T0,T1,T2,T3,T4,T5,T6,T7> > Parent;
//     typedef Tuple_t<typename Parent::BodyType> BodyTuple;
//     Tuple_t (T0 const &t0,
//              T1 const &t1,
//              T2 const &t2,
//              T3 const &t3,
//              T4 const &t4,
//              T5 const &t5,
//              T6 const &t6,
//              T7 const &t7)
//         :
//         Parent(t0, BodyTuple(t1, t2, t3, t4, t5, t6, t7))
//     { }
// };

/*
// generic one for when you don't want/can't pass initializers into a constructor
template <typename TypeList>
struct Tuple_t : public List_t<TypeList>
{
    Tuple_t () { } // this only works if all members have default constructors
};

// specializations for particular numbers of types

template <> // try not to actually construct one of these, because it isn't guaranteed to use 0 memory.
struct Tuple_t<EmptyTypeList> : public List_t<EmptyTypeList>
{
    Tuple_t () : List_t<EmptyTypeList>() { }
};

template <typename T0>
struct Tuple_t<TypeList_t<T0> > : public List_t<TypeList_t<T0> >
{
    Tuple_t (T0 const &v0)
        :
        List_t<TypeList_t<T0> >(v0)
    { }

    // type cast operator for T0?
};

template <typename T0, typename T1>
struct Tuple_t<TypeList_t<T0,
               TypeList_t<T1> > >
    : public List_t<TypeList_t<T0,
                    TypeList_t<T1> > >
{
    Tuple_t (T0 const &v0, T1 const &v1)
        :
        List_t<TypeList_t<T0,
               TypeList_t<T1> > >(v0,
        List_t<TypeList_t<T1> >(v1))
    { }
};

template <typename T0, typename T1, typename T2>
struct Tuple_t<TypeList_t<T0,
               TypeList_t<T1,
               TypeList_t<T2> > > >
    : public List_t<TypeList_t<T0,
                    TypeList_t<T1,
                    TypeList_t<T2> > > >
{
    Tuple_t (T0 const &v0, T1 const &v1, T2 const &v2)
        :
        List_t<TypeList_t<T0,
               TypeList_t<T1,
               TypeList_t<T2> > > >(v0,
        List_t<TypeList_t<T1,
               TypeList_t<T2> > >(v1,
        List_t<TypeList_t<T2> >(v2)))
    { }
};

template <typename T0, typename T1, typename T2, typename T3>
struct Tuple_t<TypeList_t<T0,
               TypeList_t<T1,
               TypeList_t<T2,
               TypeList_t<T3> > > > >
    : public List_t<TypeList_t<T0,
                    TypeList_t<T1,
                    TypeList_t<T2,
                    TypeList_t<T3> > > > >
{
    Tuple_t (T0 const &v0, T1 const &v1, T2 const &v2, T3 const &v3)
        :
        List_t<TypeList_t<T0,
               TypeList_t<T1,
               TypeList_t<T2,
               TypeList_t<T3> > > > >(v0,
        List_t<TypeList_t<T1,
               TypeList_t<T2,
               TypeList_t<T3> > > >(v1,
        List_t<TypeList_t<T2,
               TypeList_t<T3> > >(v2,
        List_t<TypeList_t<T3> >(v3))))
    { }
};

template <typename T0, typename T1, typename T2, typename T3, typename T4>
struct Tuple_t<TypeList_t<T0,
               TypeList_t<T1,
               TypeList_t<T2,
               TypeList_t<T3,
               TypeList_t<T4> > > > > >
    : public List_t<TypeList_t<T0,
                    TypeList_t<T1,
                    TypeList_t<T2,
                    TypeList_t<T3,
                    TypeList_t<T4> > > > > >
{
    Tuple_t (T0 const &v0, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4)
        :
        List_t<TypeList_t<T0,
               TypeList_t<T1,
               TypeList_t<T2,
               TypeList_t<T3,
               TypeList_t<T4> > > > > >(v0,
        List_t<TypeList_t<T1,
               TypeList_t<T2,
               TypeList_t<T3,
               TypeList_t<T4> > > > >(v1,
        List_t<TypeList_t<T2,
               TypeList_t<T3,
               TypeList_t<T4> > > >(v2,
        List_t<TypeList_t<T3,
               TypeList_t<T4> > >(v3,
        List_t<TypeList_t<T4> >(v4)))))
    { }
};
*/
#endif // TUPLE_HPP_
