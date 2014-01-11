// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/linearembedding.hpp by Victor Dods, created 2013/12/27
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_LINEAREMBEDDING_HPP_
#define TENH_CONCEPTUAL_LINEAREMBEDDING_HPP_

#include "tenh/core.hpp"

#include <vector>

#include "tenh/componentindex.hpp" // technically not conceptual code, but close enough.
#include "tenh/conceptual/concept.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

// * embed requires:
//     ~ static bool embedded_component_is_procedural_zero (ComponentIndex_t<UPSTAIRS_DIM> const &)
//     ~ static Scalar_ scalar_factor_for_embedded_component (ComponentIndex_t<UPSTAIRS_DIM> const &i)
//         // preferably is always 1, or is always plus-or-minus 1.
//         // only needs to be defined for when embedded_component_is_procedural_zero(i) is false.
//         // should throw std::domain_error if embedded_component_is_procedural_zero(i) is false (and exceptions are enabled)
//     ~ static ComponentIndex_t<DOWNSTAIRS_DIM> source_component_index_for_embedded_component (ComponentIndex_t<UPSTAIRS_DIM> const &i)
//         // only needs to be defined for when embedded_component_is_procedural_zero(i) is false
//         // should throw std::domain_error if embedded_component_is_procedural_zero(i) is false (and exceptions are enabled)
// * embed will be used as follows
//     diag.embed(dual(v*v),i) * h(i)       // h is of type v*v
//     diag.embed(sym<2>(dual(v)),i) * s(i) // s is of type sym<2>(v)
//     a(j*k).embed(k,dual(v*v),i) * h(i)   // h is of type v*v (same as before)
//     other_diag(i*j)          // would be equivalent to other_diag.embed(v*w,p).split(p,i*j), where other_diag is of type diag(v*w).
// * accessing an embedded expression template will work as follows:
//     x.embed(TargetSpace,i)[j] := embedded_component_is_procedural_zero(j) ?
//                                  Scalar_(0) :
//                                  scalar_factor_for_embedded_component(j)*x[source_component_index_for_embedded_component(j)];
// * probably this information should be provided in template specializations of
//     template <typename Domain_, typename Codomain_> struct Embedding_c;
//     this would allow embeddings to be requested, and if such an embedding doesn't exist,
//     it results in a compile error, as desired.
//
// * coembed is somewhat more complicated because it may access multiple components.
//     it requires definition of a struct called CoembedIndexIterator which has the following format:
//     struct CoembedIndexIterator
//     {
//         CoembedIndexIterator (DownstairsComponentIndex const &i); // iterator for downstairs component i
//         void operator ++ (); // increment the iterator
//         bool is_not_at_end () const; // indicates if the iteration is not finished
//         Scalar_ scale_factor () const; // scale factor for the currently iterated component
//         typedef UpstairsComponentIndex ComponentIndexReturnType; // necessary for typename uniformization
//         ComponentIndexReturnType component_index (); // upstairs component for current
//                                                      // could also return ComponentIndexReturnType const &
//     };
// * coembed will be used as follows
//     (x(j)*y(k)).coembed(j*k, sym<2>(v), i) * s(i) // s is of type dual(sym<2>(v))
//     t.coembed(diag(v*w),i) * diag(i)              // diag is of type dual(diag(v*w))
// * accessing a coembedded expression template will work as follows:
//     x.coembed(TargetSpace,i)[j] := sum(k = CoembedIndexIterator(j); k.is_not_at_end(); ++k) of
//                                    k.scale_factor()*x[k.component_index()]);
//
// * the embed/coembed expression template operations are defined by the following properties:
//     ~ if A has type downstairs_space and B has type dual(upstairs_space), then
//         A(i).embed(i,upstairs_space,j)*B(j) - A(i)*B(j).coembed(j,dual(downstairs_space),i) == 0.
//     ~ the embed operation is sparse in the sense that it accesses 0 or 1 components of
//       the embedded expression template, possibly multiplied by -1 (this restriction could
//       potentially be relaxed).
//     ~ the coembed operation, by definition, is adjoint to the embed operation, so it
//       is uniquely defined by the embed operation.  If the matrix of the embed operation
//       is written out, then it is the transpose of the matrix of the coembed operation.
//
// * the mutual adjointness property should provide a testable property to ensure that the
//     functions required by embed/coembed actually do what they claim.
//     TODO: figure out the criteria
//
// * in principle, the functions required by coembed could be determined uniquely from the
//     functions required by embed.

// id for when there is a canonical embedding of one space into another -- this will be the default
struct NaturalEmbedding { static std::string type_as_string (bool verbose) { return "NaturalEmbedding"; } };

static bool const ENABLE_EXCEPTIONS = true;
static bool const DISABLE_EXCEPTIONS = false;

// Domain_ and Codomain_ each need to be based vector spaces.  EmbeddingId_ allows
// specification of particular embeddings (e.g. NaturalEmbedding, IdentityEmbedding).
// If ENABLE_EXCEPTIONS_ is true, then the functions are required to throw on invalid
// input -- in particular, scalar_factor_for_embedded_component(i) and
// source_component_index_for_embedded_component(i) should throw iff
// embedded_component_is_procedural_zero(i) returns true.  this mode is used
// by the linear_embedding_is_consistent checker function.
template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename EmbeddingId_,
          bool ENABLE_EXCEPTIONS_>
struct LinearEmbedding_c;

// this metafunction exists so that some measure of indirection can be used in specifying
// linear embeddings -- e.g. the "natural embedding" of a tensor product into itself
// (needed for completeness) is the identity embedding, which is already defined in
// vectorspace.hpp.  the default definition of this function just gives the corresponding
// LinearEmbedding_c.
template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename EmbeddingId_,
          bool ENABLE_EXCEPTIONS_>
struct LinearEmbedding_f
{
    typedef LinearEmbedding_c<Domain_,Codomain_,Scalar_,EmbeddingId_,ENABLE_EXCEPTIONS_> T;
};

template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename EmbeddingId_,
          bool ENABLE_EXCEPTIONS_>
struct CoembedIndexIterator_f
{
    typedef typename LinearEmbedding_f<Domain_,Codomain_,Scalar_,EmbeddingId_,ENABLE_EXCEPTIONS_>::T::CoembedIndexIterator T;
};

// because there will be so many template specializations of LinearEmbedding_c, all
// with what would be identical type_as_string functions, just do it via metafunction once.
template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename EmbeddingId_,
          bool ENABLE_EXCEPTIONS_>
struct TypeStringOf_t<LinearEmbedding_c<Domain_,Codomain_,Scalar_,EmbeddingId_,ENABLE_EXCEPTIONS_>,VERBOSE>
{
    static std::string eval ()
    {
        return "LinearEmbedding_c<" + type_string_of<Domain_>() + ','
                                    + type_string_of<Codomain_>() + ','
                                    + type_string_of<Scalar_>() + ','
                                    + type_string_of<EmbeddingId_>() + ','
                                    + FORMAT(ENABLE_EXCEPTIONS_) + '>';
    }
};

template <typename Concept_> struct DimensionOf_f; // forward declaration

template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename EmbeddingId_>
struct CoembedLookupTable_t
{
    typedef ComponentIndex_t<DimensionOf_f<Domain_>::V> DomainComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Codomain_>::V> CodomainComponentIndex;
    typedef std::pair<Scalar_,CodomainComponentIndex> ScaleFactorAndComponentIndex;
    typedef std::vector<ScaleFactorAndComponentIndex> ScaleFactorAndComponentIndexVector;

    CoembedLookupTable_t ()
    {
        // compute the table
        typedef typename LinearEmbedding_f<Domain_,Codomain_,Scalar_,EmbeddingId_,DISABLE_EXCEPTIONS>::T LinearEmbedding;
        for (CodomainComponentIndex i; i.is_not_at_end(); ++i)
        {
            if (!LinearEmbedding::embedded_component_is_procedural_zero(i))
            {
                DomainComponentIndex j(LinearEmbedding::source_component_index_for_embedded_component(i));
                m_lookup[j.value()].push_back(ScaleFactorAndComponentIndex(LinearEmbedding::scalar_factor_for_embedded_component(i), i));
            }
        }
    }

    static Uint32 const DOMAIN_DIM = DimensionOf_f<Domain_>::V;
    ScaleFactorAndComponentIndexVector m_lookup[DOMAIN_DIM];
};

template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename EmbeddingId_>
struct LookupTableCoembedIndexIterator_t
{
    typedef ComponentIndex_t<DimensionOf_f<Domain_>::V> DomainComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Codomain_>::V> CodomainComponentIndex;

    LookupTableCoembedIndexIterator_t (DomainComponentIndex const &i)
        :
        m_it(m_lookup_table.m_lookup[i.value()].begin()),
        m_it_end(m_lookup_table.m_lookup[i.value()].end())
    { }
    void operator ++ () { ++m_it; }
    bool is_not_at_end () const { return m_it != m_it_end; }
    Scalar_ scale_factor () const { return m_it->first; }
    typedef CodomainComponentIndex ComponentIndexReturnType;
    ComponentIndexReturnType const &component_index () { return m_it->second; }

private:

    typedef CoembedLookupTable_t<Domain_,Codomain_,Scalar_,EmbeddingId_> CoembedLookupTable;
    static CoembedLookupTable const m_lookup_table;
    typename CoembedLookupTable::ScaleFactorAndComponentIndexVector::const_iterator m_it;
    typename CoembedLookupTable::ScaleFactorAndComponentIndexVector::const_iterator m_it_end;
};

template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename EmbeddingId_>
CoembedLookupTable_t<Domain_,Codomain_,Scalar_,EmbeddingId_> const LookupTableCoembedIndexIterator_t<Domain_,Codomain_,Scalar_,EmbeddingId_>::m_lookup_table;

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_LINEAREMBEDDING_HPP_
