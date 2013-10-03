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
    // otherwise use the numerical representation
    else
        return AS_STRING(Uint32(symbol));
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
template <AbstractIndexSymbol SYMBOL1, AbstractIndexSymbol SYMBOL2>
TypeList_t<AbstractIndex_c<SYMBOL1>,TypeList_t<AbstractIndex_c<SYMBOL2> > > operator | (
    AbstractIndex_c<SYMBOL1> const &,
    AbstractIndex_c<SYMBOL2> const &)
{
    return TypeList_t<AbstractIndex_c<SYMBOL1>,TypeList_t<AbstractIndex_c<SYMBOL2> > >();
}

template <typename HeadType, typename BodyTypeList, AbstractIndexSymbol SYMBOL>
typename ConcatenationOfTypeLists_t<TypeList_t<HeadType,BodyTypeList>,TypeList_t<AbstractIndex_c<SYMBOL> > >::T operator | (
    TypeList_t<HeadType,BodyTypeList> const &,
    AbstractIndex_c<SYMBOL> const &)
{
    STATIC_ASSERT((EachTypeSatisfies_f<TypeList_t<HeadType,BodyTypeList>,IsAbstractIndex_p>::V), EACH_TYPE_MUST_BE_ABSTRACT_INDEX);
    // appending to a TypeList_t is a nontrivial operation, hence the use of ConcatenationOfTypeLists_t
    return typename ConcatenationOfTypeLists_t<TypeList_t<HeadType,BodyTypeList>,TypeList_t<AbstractIndex_c<SYMBOL> > >::T();
}

// base case
template <AbstractIndexSymbol SYMBOL>
TypeList_t<AbstractIndex_c<SYMBOL> > operator | (
    EmptyTypeList const &,
    AbstractIndex_c<SYMBOL> const &)
{
    return TypeList_t<AbstractIndex_c<SYMBOL> >();
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

// ///////////////////////////////////////////////////////////////////////////
// metafunctions for renaming sets of abstract indices
// ///////////////////////////////////////////////////////////////////////////

// this is really Map_e but without a domain check (has nothing to do with indices)
template <typename DomainIndexTypeList_, typename CodomainIndexTypeList_>
struct BaseIndexRenamer_e
{
private:
    enum { STATIC_ASSERT_IN_ENUM(!ContainsDuplicates_t<DomainIndexTypeList_>::V, DOMAIN_INDICES_MUST_NOT_CONTAIN_DUPLICATES) };
public:
    template <typename AbstractIndex_>
    struct Eval_f
    {
        // don't actually check that AbstractIndex_ is in DomainIndexTypeList_, since
        // the If_f below instantiates this type even if the type isn't "used" by the If_f.
        typedef typename Element_f<CodomainIndexTypeList_,IndexOfFirstOccurrence_f<DomainIndexTypeList_,AbstractIndex_>::V>::T T;
    };
};

// if AbstractIndex_ (which should be AbstractIndex_c<SYMBOL> for some
// value of SYMBOL) is in DomainIndexTypeList_, map it to the corresponding
// element of CodomainIndexTypeList_.  Otherwise, map it to
// AbstractIndex_c<Max-of-symbols-of(CodomainIndexTypeList) + SYMBOL>,
// noting that SYMBOL is necessarily greater than one.  This maps everything
// outside of DomainIndexTypeList_ injectively onto a range "above"
// DomainIndexTypeList_.
template <typename DomainIndexTypeList_, typename CodomainIndexTypeList_>
struct IndexRenamer_e
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE((And_f<typename OnEach_f<DomainIndexTypeList_,IsAbstractIndex_e>::T>::V),
                                      MUST_BE_TYPELIST_OF_ABSTRACT_INDEX_TYPES,
                                      DOMAIN),
        STATIC_ASSERT_IN_ENUM__UNIQUE((And_f<typename OnEach_f<CodomainIndexTypeList_,IsAbstractIndex_e>::T>::V),
                                      MUST_BE_TYPELIST_OF_ABSTRACT_INDEX_TYPES,
                                      CODOMAIN)
    };
    static AbstractIndexSymbol const OFFSET = Max_f<typename OnEach_f<CodomainIndexTypeList_,SymbolOf_e>::T,
                                                    AbstractIndexSymbol>::V;
public:
    template <typename AbstractIndex_>
    struct Eval_f
    {
    private:
        enum { STATIC_ASSERT_IN_ENUM(IsAbstractIndex_f<AbstractIndex_>::V, MUST_BE_ABSTRACT_INDEX) };
        typedef BaseIndexRenamer_e<DomainIndexTypeList_,CodomainIndexTypeList_> BaseIndexRenamer;
    public:
        typedef typename If_f<Contains_f<DomainIndexTypeList_,AbstractIndex_>::V,
                              typename BaseIndexRenamer::template Eval_f<AbstractIndex_>::T,
                              AbstractIndex_c<OFFSET + SymbolOf_f<AbstractIndex_>::V> >::T T;
    };
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_ABSTRACTINDEX_HPP_
