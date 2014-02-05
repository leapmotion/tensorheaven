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
#include "tenh/meta/typle.hpp"
#include "tenh/meta/typle_utility.hpp"

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
        return "i_" + FORMAT(Uint32(symbol));
}

// for use in operator () for creation of expression templates (indexed tensor expressions)
template <AbstractIndexSymbol SYMBOL_>
struct AbstractIndex_c
{
    static_assert(SYMBOL_ > 0, "AbstractIndex symbol must be positive.");

    typedef Typle_t<> ParentTyple;

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
template <AbstractIndexSymbol SYMBOL0_, AbstractIndexSymbol SYMBOL1_>
Typle_t<AbstractIndex_c<SYMBOL0_>,AbstractIndex_c<SYMBOL1_>> operator * (
    AbstractIndex_c<SYMBOL0_> const &,
    AbstractIndex_c<SYMBOL1_> const &)
{
    return Typle_t<AbstractIndex_c<SYMBOL0_>,AbstractIndex_c<SYMBOL1_>>();
}

// for creating multiindices
template <AbstractIndexSymbol SYMBOL_>
Typle_t<AbstractIndex_c<SYMBOL_>> operator * (
    Typle_t<> const &,
    AbstractIndex_c<SYMBOL_> const &)
{
    return Typle_t<AbstractIndex_c<SYMBOL_>>();
}

// for creating multiindices
template <AbstractIndexSymbol SYMBOL_>
Typle_t<AbstractIndex_c<SYMBOL_>> operator * (
    AbstractIndex_c<SYMBOL_> const &,
    Typle_t<> const &)
{
    return Typle_t<AbstractIndex_c<SYMBOL_>>();
}

// for creating multiindices
template <AbstractIndexSymbol SYMBOL_, typename... Types_>
typename Concat2Typles_f<Typle_t<Types_...>,Typle_t<AbstractIndex_c<SYMBOL_>>>::T operator * (
    Typle_t<Types_...> const &,
    AbstractIndex_c<SYMBOL_> const &)
{
    return typename Concat2Typles_f<Typle_t<Types_...>,Typle_t<AbstractIndex_c<SYMBOL_>>>::T();
}

// for creating multiindices
template <AbstractIndexSymbol SYMBOL_, typename... Types_>
typename Concat2Typles_f<Typle_t<AbstractIndex_c<SYMBOL_>>,Typle_t<Types_...>>::T operator * (
    AbstractIndex_c<SYMBOL_> const &,
    Typle_t<Types_...> const &)
{
    return typename Concat2Typles_f<Typle_t<AbstractIndex_c<SYMBOL_>>,Typle_t<Types_...>>::T();
}

// for creating multiindices
template <AbstractIndexSymbol SYMBOL0_, typename... Types0_,
          AbstractIndexSymbol SYMBOL1_, typename... Types1_>
typename Concat2Typles_f<Typle_t<AbstractIndex_c<SYMBOL0_>,Types0_...>,
                         Typle_t<AbstractIndex_c<SYMBOL1_>,Types1_...>>::T operator * (
    Typle_t<AbstractIndex_c<SYMBOL0_>,Types0_...> const &,
    Typle_t<AbstractIndex_c<SYMBOL1_>,Types1_...> const &)
{
    return typename Concat2Typles_f<Typle_t<AbstractIndex_c<SYMBOL0_>,Types0_...>,
                                    Typle_t<AbstractIndex_c<SYMBOL1_>,Types1_...>>::T();
}

// ///////////////////////////////////////////////////////////////////////////
// for creating abstract multiindices with a given range of symbols
// ///////////////////////////////////////////////////////////////////////////

template <Uint32 INDEX_COUNT_, AbstractIndexSymbol STARTING_SYMBOL_>
struct AbstractIndexRangeTyple_f
{
    typedef typename HeadBodyTyple_f<AbstractIndex_c<STARTING_SYMBOL_>,
                                     typename AbstractIndexRangeTyple_f<INDEX_COUNT_-1,STARTING_SYMBOL_+1>::T>::T T;
};

template <AbstractIndexSymbol STARTING_SYMBOL_>
struct AbstractIndexRangeTyple_f<0,STARTING_SYMBOL_>
{
    typedef Typle_t<> T;
};

// ///////////////////////////////////////////////////////////////////////////
// for stringifying an abstract multiindex
// ///////////////////////////////////////////////////////////////////////////

template <AbstractIndexSymbol HEAD_SYMBOL_, typename... Types_>
std::string symbol_string_of_abstract_index_typle (Typle_t<AbstractIndex_c<HEAD_SYMBOL_>,Types_...> const &)
{
    typedef Typle_t<Types_...> BodyTyple;
    return abstract_index_symbol_as_string(HEAD_SYMBOL_, DONT_USE_QUOTES_FOR_ALPHABETIC)
           +
           ((Length_f<BodyTyple>::V > 0) ?
            '*' + symbol_string_of_abstract_index_typle(BodyTyple()) :
            std::string());
}

inline std::string symbol_string_of_abstract_index_typle (Typle_t<> const &)
{
    return std::string();
}

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_ABSTRACTINDEX_HPP_
