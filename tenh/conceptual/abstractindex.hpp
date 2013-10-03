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

inline std::string abstract_index_symbol_as_string (AbstractIndexSymbol symbol)
{
    assert(symbol > 0 && "invalid AbstractIndexSymbol");
    // for alphabetical chars, use the ascii representation.
    if ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z'))
        return std::string(1, char(symbol));
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
{ static bool const V = true; };

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

// operator overloads for stringing together type lists of abstract indices.
// the rest of the operator overloads (involving TypeList_t and EmptyTypeList)
// are in typelist.hpp.  this function just bootstraps the process.
template <AbstractIndexSymbol SYMBOL1, AbstractIndexSymbol SYMBOL2>
TypeList_t<AbstractIndex_c<SYMBOL1>,TypeList_t<AbstractIndex_c<SYMBOL2> > > operator | (
    AbstractIndex_c<SYMBOL1> const &,
    AbstractIndex_c<SYMBOL2> const &)
{
    return TypeList_t<AbstractIndex_c<SYMBOL1>,TypeList_t<AbstractIndex_c<SYMBOL2> > >();
}

// ///////////////////////////////////////////////////////////////////////////
// for stringifying an abstract multiindex
// ///////////////////////////////////////////////////////////////////////////

template <AbstractIndexSymbol HEAD_SYMBOL, typename BodyAbstractIndexTypeList>
std::string symbol_string_of_abstract_index_type_list (TypeList_t<AbstractIndex_c<HEAD_SYMBOL>,BodyAbstractIndexTypeList> const &)
{
    return abstract_index_symbol_as_string(HEAD_SYMBOL) + ((BodyAbstractIndexTypeList::LENGTH > 0) ?
                                                           '|' + symbol_string_of_abstract_index_type_list(BodyAbstractIndexTypeList()) :
                                                           std::string());
}

inline std::string symbol_string_of_abstract_index_type_list (EmptyTypeList)
{
    return std::string();
}

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_ABSTRACTINDEX_HPP_
