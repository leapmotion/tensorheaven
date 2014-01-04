// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/utility/linearembedding_checker.hpp by Victor Dods, created 2013/12/27
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_UTILITY_LINEAREMBEDDINGCHECKER_HPP_
#define TENH_CONCEPTUAL_UTILITY_LINEAREMBEDDINGCHECKER_HPP_

#include "tenh/core.hpp"

#include <algorithm>
#include <exception>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "tenh/conceptual/linearembedding.hpp"

namespace Tenh {

// forward declaration of DimensionOf_f metafunction
template <typename Concept_> struct DimensionOf_f;

template <typename Domain_, typename Codomain_, typename Compare_, typename Allocator_>
struct TypeStringOf_t<std::map<Domain_,Codomain_,Compare_,Allocator_> >
{
    static std::string eval () { return "std::map<" + type_string_of<Domain_>() + ',' + type_string_of<Codomain_>() + '>'; }
};

template <typename Element_, typename Compare_, typename Allocator_>
struct TypeStringOf_t<std::set<Element_,Compare_,Allocator_> >
{
    static std::string eval () { return "std::set<" + type_string_of<Element_>() + '>'; }
};

template <typename Element_, typename Allocator_>
struct TypeStringOf_t<std::vector<Element_,Allocator_> >
{
    static std::string eval () { return "std::vector<" + type_string_of<Element_>() + '>'; }
};

template <typename Element_, typename Compare_, typename Allocator_>
std::ostream &operator << (std::ostream &out, std::set<Element_,Compare_,Allocator_> const &s);

template <typename Element_, typename Allocator_>
std::ostream &operator << (std::ostream &out, std::vector<Element_,Allocator_> const &v);

template <typename Domain_, typename Codomain_, typename Compare_, typename Allocator_>
std::ostream &operator << (std::ostream &out, std::map<Domain_,Codomain_,Compare_,Allocator_> const &m)
{
    typedef std::map<Domain_,Codomain_,Compare_,Allocator_> Map;
    out << type_string_of<Map>() << "{\n";
    for (typename Map::const_iterator it = m.begin(), it_end = m.end(); it != it_end; ++it)
        out << "    " << it->first << " |--> " << it->second << '\n';
    return out << "}";
}

template <typename Element_, typename Compare_, typename Allocator_>
std::ostream &operator << (std::ostream &out, std::set<Element_,Compare_,Allocator_> const &s)
{
    typedef std::set<Element_,Compare_,Allocator_> Set;
    out << type_string_of<Set>() << '{';
    for (typename Set::const_iterator it = s.begin(), it_end = s.end(); it != it_end; ++it)
        out << ' ' << *it;
    return out << " }";
}

template <typename Element_, typename Allocator_>
std::ostream &operator << (std::ostream &out, std::vector<Element_,Allocator_> const &v)
{
    typedef std::vector<Element_,Allocator_> Vector;
    out << type_string_of<Vector>() << '{';
    for (typename Vector::const_iterator it = v.begin(), it_end = v.end(); it != it_end; ++it)
        out << ' ' << *it;
    return out << " }";
}

// because std::set_intersection has an inconvienent interface
template <typename Element_, typename Compare_, typename Allocator_>
std::set<Element_,Compare_,Allocator_> set_intersection (std::set<Element_,Compare_,Allocator_> const &s1,
                                                         std::set<Element_,Compare_,Allocator_> const &s2)
{
    typedef std::set<Element_,Compare_,Allocator_> Set;
    Set retval;
    std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::insert_iterator<Set>(retval, retval.end()));
    return retval;
}

// because std::set_union has an inconvienent interface
template <typename Element_, typename Compare_, typename Allocator_>
std::set<Element_,Compare_,Allocator_> set_union (std::set<Element_,Compare_,Allocator_> const &s1,
                                                  std::set<Element_,Compare_,Allocator_> const &s2)
{
    typedef std::set<Element_,Compare_,Allocator_> Set;
    Set retval;
    std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), std::insert_iterator<Set>(retval, retval.end()));
    return retval;
}

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

    // embedded_component_is_procedural_zero effectively specifies the domain for
    // scalar_factor_for_embedded_component and source_component_index_for_embedded_component.
    // check that the domains of those functions actually match.
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
                throw std::logic_error("embedded_component_is_procedural_zero(" + FORMAT(i) +
                                       ") returned true but scalar_factor_for_embedded_component(" +
                                       FORMAT(i) + ") failed to throw.");
            } catch (std::domain_error const &e) { // note that domain_error inherits logic_error.
                // catching this exception indicates scalar_factor_for_embedded_component functioned
                // correctly for that particular input value.
            } catch (std::logic_error const &e) {
                errors << "scalar_factor_for_embedded_component(" << i << ") threw exception \"" << e.what() << "\"\n";
                errors_were_encountered = true;
            } catch (...) {
                errors << "scalar_factor_for_embedded_component(" << i << ") threw unspecified exception\n";
                errors_were_encountered = true;
            }
            try {
                // if the component is a procedural zero, expect a domain_error exception.
                // otherwise, no exception should be thrown.
                DomainComponentIndex d(LinearEmbedding::source_component_index_for_embedded_component(i));
                (void) d;
                // if we got this far, then it is an error.
                throw std::logic_error("embedded_component_is_procedural_zero(" + FORMAT(i) +
                                       ") returned true but source_component_index_for_embedded_component(" +
                                       FORMAT(i) + ") failed to throw.");
            } catch (std::domain_error const &e) { // note that domain_error inherits logic_error.
                // catching this exception indicates source_component_index_for_embedded_component functioned
                // correctly for that particular input value.
            } catch (std::logic_error const &e) {
                errors << "embedded_component_is_procedural_zero(" << i << ") threw exception \"" << e.what() << "\"\n";
                errors_were_encountered = true;
            } catch (...) {
                errors << "embedded_component_is_procedural_zero(" << i << ") threw unspecified exception\n";
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
                errors << "embedded_component_is_procedural_zero(" << i
                       << ") returned false, but an exception \"" << e.what()
                       << "\" was incorrectly thrown when calling scalar_factor_for_embedded_component("
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
                errors << "embedded_component_is_procedural_zero(" << i
                       << ") returned false, but an exception \"" << e.what()
                       << "\" was incorrectly thrown when calling source_component_index_for_embedded_component("
                       << i.value() << ").\n"; // record the error
                errors_were_encountered = true;
            }
        }
    }

    // if there were errors at this point, there's no reason to continue
    if (errors_were_encountered)
    {
        // return the error string if it was requested.
        if (error_string != NULL)
            *error_string = errors.str();
        return false;
    }

    // at this point, the embedding functions are consistent, so construct some containers to
    // make the further analysis easier.

    typedef std::set<CodomainComponentIndex> EmbeddingMapsDomainSet;
    typedef std::map<CodomainComponentIndex,Scalar_> EmbeddingScaleFactorMap;
    typedef std::map<CodomainComponentIndex,DomainComponentIndex> EmbeddingSourceComponentIndexMap;

    EmbeddingMapsDomainSet embedding_maps_domain;
    EmbeddingScaleFactorMap embedding_scale_factor;
    EmbeddingSourceComponentIndexMap embedding_source_component_index;
    for (CodomainComponentIndex i; i.is_not_at_end(); ++i)
    {
        if (!LinearEmbedding::embedded_component_is_procedural_zero(i))
        {
            embedding_maps_domain.insert(i);
            embedding_scale_factor[i] = LinearEmbedding::scalar_factor_for_embedded_component(i);
            embedding_source_component_index[i] = LinearEmbedding::source_component_index_for_embedded_component(i);
        }
    }

//     std::cerr << FORMAT_VALUE(embedding_maps_domain) << '\n';
//     std::cerr << FORMAT_VALUE(embedding_scale_factor) << '\n';
//     std::cerr << FORMAT_VALUE(embedding_source_component_index) << '\n';

    typedef std::set<Scalar_> ScaleFactorSet;
    typedef std::set<CodomainComponentIndex> ComponentIndexSet;
    typedef std::map<DomainComponentIndex,ScaleFactorSet> CoembeddingScaleFactorMap;
    typedef std::map<DomainComponentIndex,ComponentIndexSet> CoembeddingComponentIndexMap;

    CoembeddingScaleFactorMap coembedding_scale_factor;
    CoembeddingComponentIndexMap coembedding_component_index;
    for (DomainComponentIndex i; i.is_not_at_end(); ++i)
    {
        for (typename LinearEmbedding::CoembedIndexIterator j(i); j.is_not_at_end(); ++j)
        {
            coembedding_scale_factor[i].insert(j.scale_factor());
            coembedding_component_index[i].insert(j.component_index());
        }
    }

//     std::cerr << FORMAT_VALUE(coembedding_scale_factor) << '\n';
//     std::cerr << FORMAT_VALUE(coembedding_component_index) << '\n';

    // ensure that the sets of coembedding_component_index are disjoint
    // and that the union of all the sets is the domain of the embedding maps
    ComponentIndexSet union_of_component_index_sets;
    for (typename CoembeddingComponentIndexMap::const_iterator it1 = coembedding_component_index.begin(),
                                                               it1_end = coembedding_component_index.end();
         it1 != it1_end;
         ++it1)
    {
        // include the set it1->second into union_of_component_index_sets
        union_of_component_index_sets = set_union(union_of_component_index_sets, it1->second);

        for (typename CoembeddingComponentIndexMap::const_iterator it2 = coembedding_component_index.begin(),
                                                                   it2_end = coembedding_component_index.end();
             it2 != it2_end;
             ++it2)
        {
            // only compare different elements
            if (it1 == it2)
                continue;

            if (!set_intersection(it1->second, it2->second).empty())
            {
                errors << "CoembedIndexIterator(" << it1->first << ") and CoembedIndexIterator("
                       << it2->first << ") " << "produce non-disjoint component_index() sets "
                       << it1->second << " and " << it2->second << " respectively.\n";
                errors_were_encountered = true;
            }
        }
    }
    if (union_of_component_index_sets != embedding_maps_domain)
    {
        errors << "union of CoembedIndexIterator component_index() values was different from embedding maps' domain; "
               << union_of_component_index_sets << " and " << embedding_maps_domain << " respectively.\n";
        errors_were_encountered = true;
    }

    // if there were errors at this point, there's no reason to continue
    if (errors_were_encountered)
    {
        // return the error string if it was requested.
        if (error_string != NULL)
            *error_string = errors.str();
        return false;
    }

    // create the inverse maps from the coembedding maps
//     EmbeddingMapsDomainSet coembedding_inverse_maps_domain;
    EmbeddingScaleFactorMap coembedding_inverse_scale_factor;
    EmbeddingSourceComponentIndexMap coembedding_inverse_component_index;
    for (DomainComponentIndex i; i.is_not_at_end(); ++i)
    {
        for (typename LinearEmbedding::CoembedIndexIterator j(i); j.is_not_at_end(); ++j)
        {
//             coembedding_inverse_maps_domain.insert(j.component_index());
            coembedding_inverse_scale_factor[j.component_index()] = j.scale_factor();
            coembedding_inverse_component_index[j.component_index()] = i;
        }
    }

//     std::cerr << FORMAT_VALUE(coembedding_inverse_scale_factor) << '\n';
//     std::cerr << FORMAT_VALUE(coembedding_inverse_component_index) << '\n';

    // ensure that coembed actually represents the inverse of embed in the appropriate sense
    if (embedding_scale_factor != coembedding_inverse_scale_factor)
    {
        errors << "embedding scale factor map and coembedding scale factor inverse map differ;\n"
               << FORMAT_VALUE(embedding_scale_factor) << " and\n"
               << FORMAT_VALUE(coembedding_inverse_scale_factor) << '\n';
        errors_were_encountered = true;
    }
    if (embedding_source_component_index != coembedding_inverse_component_index)
    {
        errors << "embedding source component index map and coembedding component index inverse map differ;\n"
               << FORMAT_VALUE(embedding_source_component_index) << " and\n"
               << FORMAT_VALUE(coembedding_inverse_component_index) << '\n';
        errors_were_encountered = true;
    }

    // return the error string if it was requested.
    if (error_string != NULL)
        *error_string = errors.str();

    return !errors_were_encountered;
}

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_UTILITY_LINEAREMBEDDINGCHECKER_HPP_
