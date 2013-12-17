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

// ///////////////////////////////////////////////////////////////////////////
// AbstractIndex_c
// ///////////////////////////////////////////////////////////////////////////

typedef Uint32 AbstractIndexSymbol;

static bool const USE_QUOTES_FOR_ALPHABETIC = true;
static bool const DONT_USE_QUOTES_FOR_ALPHABETIC = false;

inline std::string abstract_index_symbol_as_string (AbstractIndexSymbol symbol, bool use_quotes_for_alphabetic = USE_QUOTES_FOR_ALPHABETIC)
{
    assert(symbol > 0 && "invalid AbstractIndexSymbol");
    // for alphabetical chars, use the ascii representation.
    if ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z'))
        return use_quotes_for_alphabetic ? ('\'' + std::string(1, char(symbol)) + '\'') : std::string(1, char(symbol));
    // otherwise use i_###, where ### is the symbol value
    else
        return "i_" + AS_STRING(Uint32(symbol));
}

// for use in operator () for creation of expression templates (indexed tensor expressions)
template <AbstractIndexSymbol SYMBOL_>
struct AbstractIndex_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM((SYMBOL_ > 0), ABSTRACT_INDEX_SYMBOL_MUST_BE_POSITIVE) };
public:
    typedef EmptyTypeList ParentTypeList;

    static AbstractIndexSymbol const SYMBOL = SYMBOL_;

    static std::string type_as_string () { return std::string("AbstractIndex_c<") + abstract_index_symbol_as_string(SYMBOL_) + '>'; }
};

template <AbstractIndexSymbol SYMBOL_>
struct IsConcept_f<AbstractIndex_c<SYMBOL_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsAbstractIndex_f { static bool const V = false; };
template <AbstractIndexSymbol SYMBOL> struct IsAbstractIndex_f<AbstractIndex_c<SYMBOL> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(AbstractIndex);
// special convenience macros
#define IS_ABSTRACT_INDEX_UNIQUELY(Concept) HasUniqueAbstractIndexStructure_f<Concept>::V
#define AS_ABSTRACT_INDEX(Concept) UniqueAbstractIndexStructureOf_f<Concept>::T

MAKE_1_ARY_VALUE_EVALUATOR(IsAbstractIndex, bool);

// property IDs

struct Symbol { }; // could this be a forward declaration?

// BaseProperty_f accessors

template <AbstractIndexSymbol SYMBOL_>
struct BaseProperty_f<AbstractIndex_c<SYMBOL_>,Symbol>
{
    typedef Value_t<AbstractIndexSymbol,SYMBOL_> T;
};

// named property accessors

template <typename Concept_> struct SymbolOf_f { static AbstractIndexSymbol const V = PropertyValue_f<Concept_,Symbol>::V; };

MAKE_1_ARY_VALUE_EVALUATOR(SymbolOf, AbstractIndexSymbol);

// ///////////////////////////////////////////////////////////////////////////
// operator overloads for creating abstract multiindices
// ///////////////////////////////////////////////////////////////////////////

// for creating multiindices
template <AbstractIndexSymbol SYMBOL1_, AbstractIndexSymbol SYMBOL2_>
TypeList_t<AbstractIndex_c<SYMBOL1_>,TypeList_t<AbstractIndex_c<SYMBOL2_> > > operator * (
    AbstractIndex_c<SYMBOL1_> const &,
    AbstractIndex_c<SYMBOL2_> const &)
{
    return TypeList_t<AbstractIndex_c<SYMBOL1_>,TypeList_t<AbstractIndex_c<SYMBOL2_> > >();
}

// for creating multiindices
template <AbstractIndexSymbol SYMBOL_>
TypeList_t<AbstractIndex_c<SYMBOL_> > operator * (
    EmptyTypeList const &,
    AbstractIndex_c<SYMBOL_> const &)
{
    return TypeList_t<AbstractIndex_c<SYMBOL_> >();
}

// for creating multiindices
template <AbstractIndexSymbol SYMBOL_>
TypeList_t<AbstractIndex_c<SYMBOL_> > operator * (
    AbstractIndex_c<SYMBOL_> const &,
    EmptyTypeList const &)
{
    return TypeList_t<AbstractIndex_c<SYMBOL_> >();
}

// for creating multiindices
template <AbstractIndexSymbol SYMBOL1_, typename BodyTypeList_, AbstractIndexSymbol SYMBOL2_>
typename ConcatenationOfTypeLists_t<TypeList_t<AbstractIndex_c<SYMBOL1_>,BodyTypeList_>,
                                    TypeList_t<AbstractIndex_c<SYMBOL2_> > >::T operator * (
    TypeList_t<AbstractIndex_c<SYMBOL1_>,BodyTypeList_> const &,
    AbstractIndex_c<SYMBOL2_> const &)
{
    return typename ConcatenationOfTypeLists_t<TypeList_t<AbstractIndex_c<SYMBOL1_>,BodyTypeList_>,
                                               TypeList_t<AbstractIndex_c<SYMBOL2_> > >::T();
}

// for creating multiindices
template <AbstractIndexSymbol SYMBOL1_, AbstractIndexSymbol SYMBOL2_, typename BodyTypeList_>
TypeList_t<AbstractIndex_c<SYMBOL1_>,TypeList_t<AbstractIndex_c<SYMBOL2_>,BodyTypeList_> > operator * (
    AbstractIndex_c<SYMBOL1_> const &,
    TypeList_t<AbstractIndex_c<SYMBOL2_>,BodyTypeList_> const &)
{
    return TypeList_t<AbstractIndex_c<SYMBOL1_>,TypeList_t<AbstractIndex_c<SYMBOL2_>,BodyTypeList_> >();
}

// for creating multiindices
template <AbstractIndexSymbol SYMBOL1_, typename BodyTypeList1_,
          AbstractIndexSymbol SYMBOL2_, typename BodyTypeList2_>
typename ConcatenationOfTypeLists_t<TypeList_t<AbstractIndex_c<SYMBOL1_>,BodyTypeList1_>,
                                    TypeList_t<AbstractIndex_c<SYMBOL2_>,BodyTypeList2_> >::T operator * (
    TypeList_t<AbstractIndex_c<SYMBOL1_>,BodyTypeList1_> const &,
    TypeList_t<AbstractIndex_c<SYMBOL2_>,BodyTypeList2_> const &)
{
    return typename ConcatenationOfTypeLists_t<TypeList_t<AbstractIndex_c<SYMBOL1_>,BodyTypeList1_>,
                                               TypeList_t<AbstractIndex_c<SYMBOL2_>,BodyTypeList2_> >::T();
}

// ///////////////////////////////////////////////////////////////////////////
// for creating abstract multiindices with a given range of symbols
// ///////////////////////////////////////////////////////////////////////////

template <Uint32 INDEX_COUNT_, AbstractIndexSymbol STARTING_SYMBOL_>
struct AbstractIndexRangeTypeList_f
{
    typedef TypeList_t<AbstractIndex_c<STARTING_SYMBOL_>,
                       typename AbstractIndexRangeTypeList_f<INDEX_COUNT_-1,STARTING_SYMBOL_+1>::T> T;
};

template <AbstractIndexSymbol STARTING_SYMBOL_>
struct AbstractIndexRangeTypeList_f<0,STARTING_SYMBOL_>
{
    typedef EmptyTypeList T;
};

// ///////////////////////////////////////////////////////////////////////////
// for stringifying an abstract multiindex
// ///////////////////////////////////////////////////////////////////////////

template <AbstractIndexSymbol HEAD_SYMBOL, typename BodyAbstractIndexTypeList>
std::string symbol_string_of_abstract_index_type_list (TypeList_t<AbstractIndex_c<HEAD_SYMBOL>,BodyAbstractIndexTypeList> const &)
{
    return abstract_index_symbol_as_string(HEAD_SYMBOL, DONT_USE_QUOTES_FOR_ALPHABETIC)
           +
           ((BodyAbstractIndexTypeList::LENGTH > 0) ?
            '*' + symbol_string_of_abstract_index_type_list(BodyAbstractIndexTypeList()) :
            std::string());
}

inline std::string symbol_string_of_abstract_index_type_list (EmptyTypeList)
{
    return std::string();
}

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_ABSTRACTINDEX_HPP_
