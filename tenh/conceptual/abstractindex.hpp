// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/abstractindex.hpp by Victor Dods, created 2013/08/03
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_ABSTRACTINDEX_HPP_
#define TENH_CONCEPTUAL_ABSTRACTINDEX_HPP_

#include <ostream>
#include <string>

#include "tenh/core.hpp"

#include "tenh/meta/typelist.hpp"
#include "tenh/meta/typelist_utility.hpp"

namespace Tenh {

// for use in operator () for creation of expression templates (indexed tensor expressions)
template <char SYMBOL_>
struct AbstractIndex_c
{
    static char const SYMBOL = SYMBOL_;

//    AbstractIndex_c () { }

    static std::string type_as_string () { return std::string("AbstractIndex_c<'") + SYMBOL + "'>"; }
};

template <typename T> struct IsAnAbstractIndex_c { static bool const V = false; };
template <char SYMBOL> struct IsAnAbstractIndex_c<AbstractIndex_c<SYMBOL> > { static bool const V = true; };

template <typename TypeList>
struct EachTypeIsAnAbstractIndex_c
{
    static bool const V = IsAnAbstractIndex_c<typename TypeList::HeadType>::V &&
                          EachTypeIsAnAbstractIndex_c<typename TypeList::BodyTypeList>::V;
    operator bool () const { return V; }
};

// vacuously true
template <>
struct EachTypeIsAnAbstractIndex_c<EmptyTypeList>
{
    static bool const V = true;
    operator bool () const { return V; }
};

// operator overloads for stringing together type lists of abstract indices.
template <char SYMBOL1, char SYMBOL2>
TypeList_t<AbstractIndex_c<SYMBOL1>,TypeList_t<AbstractIndex_c<SYMBOL2> > > operator | (
    AbstractIndex_c<SYMBOL1> const &,
    AbstractIndex_c<SYMBOL2> const &)
{
    return TypeList_t<AbstractIndex_c<SYMBOL1>,TypeList_t<AbstractIndex_c<SYMBOL2> > >();
}

template <typename HeadType, typename BodyTypeList, char SYMBOL>
typename ConcatenationOfTypeLists_t<TypeList_t<HeadType,BodyTypeList>,TypeList_t<AbstractIndex_c<SYMBOL> > >::T operator | (
    TypeList_t<HeadType,BodyTypeList> const &,
    AbstractIndex_c<SYMBOL> const &)
{
    STATIC_ASSERT((EachTypeIsAnAbstractIndex_c<TypeList_t<HeadType,BodyTypeList> >::V), EACH_TYPE_MUST_BE_ABSTRACT_INDEX);
    // appending to a TypeList_t is a nontrivial operation, hence the use of ConcatenationOfTypeLists_t
    return typename ConcatenationOfTypeLists_t<TypeList_t<HeadType,BodyTypeList>,TypeList_t<AbstractIndex_c<SYMBOL> > >::T();
}

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_ABSTRACTINDEX_HPP_
