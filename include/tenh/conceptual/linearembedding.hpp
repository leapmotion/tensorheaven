// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/linearembedding.hpp by Victor Dods, created 2013/12/27
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_LINEAREMBEDDING_HPP_
#define TENH_CONCEPTUAL_LINEAREMBEDDING_HPP_

#include "tenh/core.hpp"

#include <exception>
#include <sstream>
#include <string>

#include "tenh/componentindex.hpp" // technically not conceptual code, but close enough.
#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/vectorspace.hpp"
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
//     x.embed(TargetSpace,i)[j] := component_is_procedural_zero(j) ? Scalar_(0) : scalar_factor_for_component(j)*x[source_component_index(j)];
// * probably this information should be provided in template specializations of
//     template <typename Domain_, typename Codomain_> struct Embedding_c;
//     this would allow embeddings to be requested, and if such an embedding doesn't exist,
//     it results in a compile error, as desired.
//
// * coembed is somewhat more complicated because it may access multiple components.
//     it requires:
//     ~ static Uint32 term_count_for_coembedded_component (ComponentIndex_t<DOWNSTAIRS_DIM> const &)
//     ~ static Scalar_ scalar_factor_for_coembedded_component (ComponentIndex_t<DOWNSTAIRS_DIM> const &i, Uint32 term) // preferably is always 1, or is always plus-or-minus 1.
//         // assert(term < term_count_for_component(i))
//     ~ static ComponentIndex_t<UPSTAIRS_DIM> source_component_index_for_coembedded_component (ComponentIndex_t<DOWNSTAIRS_DIM> const &i, Uint32 term)
//         // assert(term < term_count_for_component(i))
// * coembed will be used as follows
//     (x(j)*y(k)).coembed(j*k, sym<2>(v), i) * s(i) // s is of type dual(sym<2>(v))
//     t.coembed(diag(v*w),i) * diag(i)              // diag is of type dual(diag(v*w))
// * accessing a coembedded expression template will work as follows:
//     x.coembed(TargetSpace,i)[j] := sum(k = 0 to term_count_for_component(j)-1 of scalar_factor_for_component(j,k)*x[source_component_index(j,k)]);
// * probably this information should be provided in template specializations of
//     template <typename Domain_, typename Codomain_> struct Projection_c;
//     this would allow coembeddings to be requested, and if such a coembedding doesn't exist,
//     it results in a compile error, as desired.
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
struct NaturalEmbedding { static std::string type_as_string () { return "NaturalEmbedding"; } };

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

// because there will be so many template specializations of LinearEmbedding_c, all
// with what would be identical type_as_string functions, just do it via metafunction once.
template <typename Domain_,
          typename Codomain_,
          typename Scalar_,
          typename EmbeddingId_,
          bool ENABLE_EXCEPTIONS_>
struct TypeStringOf_t<LinearEmbedding_c<Domain_,Codomain_,Scalar_,EmbeddingId_,ENABLE_EXCEPTIONS_> >
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

// id for identity embedding
struct IdentityEmbedding { static std::string type_as_string () { return "IdentityEmbedding"; } };

// canonical identity embedding
template <typename OnBasedVectorSpace_, typename Scalar_, bool ENABLE_EXCEPTIONS_>
struct LinearEmbedding_c<OnBasedVectorSpace_,OnBasedVectorSpace_,Scalar_,IdentityEmbedding,ENABLE_EXCEPTIONS_>
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASED_VECTOR_SPACE_UNIQUELY(OnBasedVectorSpace_), MUST_BE_BASED_VECTOR_SPACE) };
public:
    typedef ComponentIndex_t<DimensionOf_f<OnBasedVectorSpace_>::V> ComponentIndex;

    struct CoembedIndexIterator
    {
        CoembedIndexIterator (ComponentIndex const &i) : m(i) { }
        void operator ++ () { ++m; }
        bool is_not_at_end () const { return m.is_not_at_end(); }
        Scalar_ scale_factor () const { return Scalar_(1); }
        ComponentIndex const &component_index () const { return m; }
    private:
        ComponentIndex m;
    };

    // because this always returns false, there is no need for the other two functions to ever throw.
    static bool embedded_component_is_procedural_zero (ComponentIndex const &) { return false; }
    static Scalar_ scalar_factor_for_embedded_component (ComponentIndex const &) { return Scalar_(1); }
    static ComponentIndex source_component_index_for_embedded_component (ComponentIndex const &i) { return i; }

    static Uint32 term_count_for_coembedded_component (ComponentIndex const &) { return 1; }
    static Scalar_ scalar_factor_for_coembedded_component (ComponentIndex const &, Uint32) { return Scalar_(1); }
    static ComponentIndex source_component_index_for_coembedded_component (ComponentIndex const &i, Uint32) { return i; }
};

// this can be used to verify that the functions associated with coembed are
// consistent with those of embed, for a particular linear embedding.
template <typename Domain_, typename Codomain_, typename Scalar_, typename EmbeddingId_>
bool linear_embedding_is_consistent (LinearEmbedding_c<Domain_,Codomain_,Scalar_,EmbeddingId_,ENABLE_EXCEPTIONS> const &,
                                     std::string *error_string = NULL)
{
    typedef LinearEmbedding_c<Domain_,Codomain_,Scalar_,EmbeddingId_,ENABLE_EXCEPTIONS> LinearEmbedding;
    typedef ComponentIndex_t<DimensionOf_f<Domain_>::V> DomainComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<Codomain_>::V> CodomainComponentIndex;

    bool errors_were_encountered = false;
    std::ostringstream errors;
    errors << type_string_of<LinearEmbedding>() << " consistency check errors:\n";

    for (CodomainComponentIndex i; i.is_not_at_end(); ++i)
    {
        // if this component is a procedural zero, then expect an exception out of each
        // of scalar_factor_for_embedded_component and source_component_index_for_embedded_component
        if (LinearEmbedding::embedded_component_is_procedural_zero(i))
        {
            try {
                // if the component is a procedural zero, expect a domain_error exception.
                // otherwise, no exception should be thrown.
                Scalar_ s(LinearEmbedding::scalar_factor_for_embedded_component(i));
                (void) s;
                // if we got this far, then it is an error.
                throw std::logic_error("embedded_component_is_procedural_zero(" + FORMAT(i.value()) +
                                       ") returned true but scalar_factor_for_embedded_component(" +
                                       FORMAT(i.value()) + ") failed to throw.");
            } catch (std::domain_error const &e) { // note that domain_error inherits logic_error.
                // catching this exception indicates scalar_factor_for_embedded_component functioned
                // correctly for that particular input value.
            } catch (std::logic_error const &e) {
                errors << e.what() << '\n'; // record the error
                errors_were_encountered = true;
            }
            try {
                // if the component is a procedural zero, expect a domain_error exception.
                // otherwise, no exception should be thrown.
                DomainComponentIndex d(LinearEmbedding::source_component_index_for_embedded_component(i));
                (void) d;
                // if we got this far, then it is an error.
                throw std::logic_error("embedded_component_is_procedural_zero(" + FORMAT(i.value()) +
                                       ") returned true but source_component_index_for_embedded_component(" +
                                       FORMAT(i.value()) + ") failed to throw.");
            } catch (std::domain_error const &e) { // note that domain_error inherits logic_error.
                // catching this exception indicates source_component_index_for_embedded_component functioned
                // correctly for that particular input value.
            } catch (std::logic_error const &e) {
                errors << e.what() << '\n'; // record the error
                errors_were_encountered = true;
            }
        }
        // otherwise expect no exception out of each of
        // scalar_factor_for_embedded_component and source_component_index_for_embedded_component
        else
        {
            try {
                // if the component is a procedural zero, expect a domain_error exception.
                // otherwise, no exception should be thrown.
                Scalar_ s(LinearEmbedding::scalar_factor_for_embedded_component(i));
                (void) s;
                // if we got this far, then it is correct.
            } catch (std::exception const &e) {
                errors << "embedded_component_is_procedural_zero(" << i.value()
                       << ") returned false, but an exception \"" << e.what()
                       << "\" was caught when calling scalar_factor_for_embedded_component("
                       << i.value() << ").\n"; // record the error
                errors_were_encountered = true;
            }
            try {
                // if the component is a procedural zero, expect a domain_error exception.
                // otherwise, no exception should be thrown.
                DomainComponentIndex d(LinearEmbedding::source_component_index_for_embedded_component(i));
                (void) d;
                // if we got this far, then it is correct.
            } catch (std::exception const &e) {
                errors << "embedded_component_is_procedural_zero(" << i.value()
                       << ") returned false, but an exception \"" << e.what()
                       << "\" was caught when calling source_component_index_for_embedded_component("
                       << i.value() << ").\n"; // record the error
                errors_were_encountered = true;
            }
        }
    }

    // TODO: verify coembed portion

    // return the error string if it was requested.
    if (error_string != NULL)
        *error_string = errors.str();

    return !errors_were_encountered;
}

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_LINEAREMBEDDING_HPP_
