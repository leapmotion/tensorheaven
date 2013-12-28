// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/linearembedding.hpp by Victor Dods, created 2013/12/27
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_LINEAREMBEDDING_HPP_
#define TENH_CONCEPTUAL_LINEAREMBEDDING_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/concept.hpp"
#include "tenh/componentindex.hpp" // technically not conceptual code, but close enough.

namespace Tenh {

// * embed requires:
//     ~ static bool embedded_component_is_procedural_zero (ComponentIndex_t<UPSTAIRS_DIM> const &)
//     ~ static Scalar_ scalar_factor_for_embedded_component (ComponentIndex_t<UPSTAIRS_DIM> const &) // preferably is always 1, or is always plus-or-minus 1.
//     ~ static ComponentIndex_t<DOWNSTAIRS_DIM> source_component_index_for_embedded_component (ComponentIndex_t<UPSTAIRS_DIM> const &)
// * embed will be used as follows
//     diag.embed(dual(v*v),i) * h(i)       // h is of type v*v
//     diag.embed(sym<2>(dual(v)),i) * s(i) // s is of type sym<2>(v)
//     a(j*k).embed(k,dual(v*v),i) * h(i)   // h is of type v*v (same as before)
//     other_diag(i*j)          // would be equivalent to other_diag.embed(v*w,p).split(p,i*j), where other_diag is of type diag(v*w).
// * accessing an embedded expression template will work as follows:
//     x.embed(TargetSpace,i)[j] := component_is_procedural_zero(j) ? Scalar_(0) : scalar_factor_for_component(j)*x[source_component_index(j)];
// * probably this information should be provided in template specializations of
//     template <typename Domain_, typename Codomain_> struct Embedding_c;
//     this would allow embeddings to be requested, and if such an embedding doesn't exist,
//     it results in a compile error, as desired.
//
// * project is somewhat more complicated because it may access multiple components.
//     it requires:
//     ~ static Uint32 term_count_for_projected_component (ComponentIndex_t<DOWNSTAIRS_DIM> const &)
//     ~ static Scalar_ scalar_factor_for_projected_component (ComponentIndex_t<DOWNSTAIRS_DIM> const &i, Uint32 term) // preferably is always 1, or is always plus-or-minus 1.
//         // assert(term < term_count_for_component(i))
//     ~ static ComponentIndex_t<UPSTAIRS_DIM> source_component_index (ComponentIndex_t<DOWNSTAIRS_DIM> const &i, Uint32 term)
//         // assert(term < term_count_for_component(i))
// * project will be used as follows
//     (x(j)*y(k)).project(j*k, sym<2>(v), i) * s(i) // s is of type dual(sym<2>(v))
//     t.project(diag(v*w),i) * diag(i)              // diag is of type dual(diag(v*w))
// * accessing a projected expression template will work as follows:
//     x.project(TargetSpace,i)[j] := sum(k = 0 to term_count_for_component(j)-1 of scalar_factor_for_component(j,k)*x[source_component_index(j,k)]);
// * probably this information should be provided in template specializations of
//     template <typename Domain_, typename Codomain_> struct Projection_c;
//     this would allow projections to be requested, and if such a projection doesn't exist,
//     it results in a compile error, as desired.
//
// * the mutual adjointness property should provide a testable property to ensure that the
//     functions required by embed/project actually do what they claim.
//     TODO: figure out the criteria
//
// * in principle, the functions required by project could be determined uniquely from the
//     functions required by embed.

// id for when there is a canonical embedding of one space into another -- this will be the default
struct NaturalEmbedding { static std::string type_as_string () { return "NaturalEmbedding"; } };

// Domain_ and Codomain_ each need to be based vector spaces.  EmbeddingId_
template <typename Domain_, typename Codomain_, typename Scalar_, typename EmbeddingId_ = NaturalEmbedding> struct LinearEmbedding_c;

// id for identity embedding
struct IdentityEmbedding { static std::string type_as_string () { return "IdentityEmbedding"; } };

// identity embedding
template <typename Domain_, typename Scalar_>
struct LinearEmbedding_c<Domain_,Domain_,Scalar_,IdentityEmbedding>
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASED_VECTOR_SPACE_UNIQUELY(Domain_), MUST_BE_BASED_VECTOR_SPACE) };
public:
    typedef ComponentIndex_t<DimensionOf_f<Domain_>::V> ComponentIndex;

    static bool embedded_component_is_procedural_zero (ComponentIndex const &) { return false; }
    static Scalar_ scalar_factor_for_embedded_component (ComponentIndex const &) { return Scalar_(1); }
    static ComponentIndex source_component_index_for_embedded_component (ComponentIndex const &i) { return i; }

    static Uint32 term_count_for_projected_component (ComponentIndex const &) { return 1; }
    static Scalar_ scalar_factor_for_projected_component (ComponentIndex const &) { return Scalar_(1); }
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_LINEAREMBEDDING_HPP_
