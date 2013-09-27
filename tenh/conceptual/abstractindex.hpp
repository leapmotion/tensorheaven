// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/abstractindex.hpp by Victor Dods, created 2013/08/03
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_ABSTRACTINDEX_HPP_
#define TENH_CONCEPTUAL_ABSTRACTINDEX_HPP_

#include <ostream>
#include <string>

#include "tenh/core.hpp"

#include "tenh/conceptual/concept.hpp"
#include "tenh/meta/typelist.hpp"
#include "tenh/meta/typelist_utility.hpp"

namespace Tenh {

// for use in operator () for creation of expression templates (indexed tensor expressions)
template <char SYMBOL_>
struct AbstractIndex_c
{
    typedef EmptyTypeList ParentTypeList;

    static char const SYMBOL = SYMBOL_;

    static std::string type_as_string () { return std::string("AbstractIndex_c<'") + SYMBOL_ + "'>"; }
};

template <char SYMBOL_>
struct IsConcept_f<AbstractIndex_c<SYMBOL_> >
{ static bool const V = true; };

template <typename T> struct IsAbstractIndex_f { static bool const V = false; };
template <char SYMBOL> struct IsAbstractIndex_f<AbstractIndex_c<SYMBOL> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(AbstractIndex);
// special convenience macros
#define IS_ABSTRACT_INDEX_UNIQUELY(Concept) HasUniqueAbstractIndexStructure_f<Concept>::V
#define AS_ABSTRACT_INDEX(Concept) UniqueAbstractIndexStructureOf_f<Concept>::T


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
    STATIC_ASSERT((EachTypeSatisfies_f<TypeList_t<HeadType,BodyTypeList>,IsAbstractIndex_p>::V), EACH_TYPE_MUST_BE_ABSTRACT_INDEX);
    // appending to a TypeList_t is a nontrivial operation, hence the use of ConcatenationOfTypeLists_t
    return typename ConcatenationOfTypeLists_t<TypeList_t<HeadType,BodyTypeList>,TypeList_t<AbstractIndex_c<SYMBOL> > >::T();
}

// base case
template <char SYMBOL>
TypeList_t<AbstractIndex_c<SYMBOL> > operator | (
    EmptyTypeList const &,
    AbstractIndex_c<SYMBOL> const &)
{
    return TypeList_t<AbstractIndex_c<SYMBOL> >();
}

template <char HEAD_SYMBOL, typename BodyAbstractIndexTypeList>
std::string symbol_string_of_abstract_index_type_list (TypeList_t<AbstractIndex_c<HEAD_SYMBOL>,BodyAbstractIndexTypeList> const &)
{
    return HEAD_SYMBOL + ((BodyAbstractIndexTypeList::LENGTH > 0) ?
                          '|' + symbol_string_of_abstract_index_type_list(BodyAbstractIndexTypeList()) :
                          std::string());
}

std::string symbol_string_of_abstract_index_type_list (EmptyTypeList)
{
    return std::string();
}

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_ABSTRACTINDEX_HPP_
