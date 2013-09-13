// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/conceptualinheritancegraph.hpp by Vector Dods, created 2013/09/12
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_CONCEPTUALINHERITANCEGRAPH_HPP_
#define TENH_CONCEPTUAL_CONCEPTUALINHERITANCEGRAPH_HPP_

#include "tenh/core.hpp"

#include <cassert>
#include <set>
#include <string>

#include "tenh/meta/concept.hpp"

namespace Tenh {

struct Graph
{
    struct Node
    {
        std::string m_short_name;
        std::string m_full_name;

        Node (std::string const &full_name)
            :
            m_short_name(full_name),
            m_full_name(full_name)
        {
            assert(!m_full_name.empty());
        }
        Node (std::string const &short_name, std::string const &full_name)
            :
            m_short_name(short_name),
            m_full_name(full_name)
        {
            assert(!m_short_name.empty());
            assert(!m_full_name.empty());
        }

        bool operator < (Node const &r)
        {
            return this->m_short_name < r.m_short_name
                   ||
                   (this->m_short_name == r.m_short_name && this->m_full_name < r.m_full_name);
        }

        struct Order
        {
            bool operator () (Node const &l, Node const &r)
            {
                return l < r;
            }
        };
    };

    struct Edge
    {
        Node m_source;
        Node m_target;

        Edge (Node const &source, Node const &target) : m_source(source), m_target(target) { }

        bool operator < (Edge const &r)
        {
            return this->m_source < r.m_source
                   ||
                   (this->m_source == r.m_source && this->m_target < r.m_target);
        }

        struct Order
        {
            bool operator () (Edge const &l, Edge const &r)
            {
                return l < r;
            }
        };
    };

    typedef std::set<Node> Nodes;
    typedef std::set<Edge> Edges;

    // constructs an empty graph
    Graph () { }

    void clear () { m_nodes.clear(); m_edges.clear(); }

    void add_node (Node const &node) { m_nodes.insert(node); }
    void add_edge (Edge const &edge) { m_edges.insert(edge); }

    Nodes const &nodes () const { return m_nodes; }
    Edges const &edges () const { return m_edges; }

    Edges edges_having_source_node (Node const &node) const
    {
        Edges retval;
        for (Edges::const_iterator it = m_edges.begin(), it_end = m_edges.end(); it != it_end; ++it)
        {
            Edge const &edge = *it;
            if (edge.m_source == node)
                retval.insert(edge);
        }
        return retval;
    }
    Edges edges_having_target_node (Node const &node) const
    {
        Edges retval;
        for (Edges::const_iterator it = m_edges.begin(), it_end = m_edges.end(); it != it_end; ++it)
        {
            Edge const &edge = *it;
            if (edge.m_target == node)
                retval.insert(edge);
        }
        return retval;
    }

private:

    Nodes m_nodes;
    Edges m_edges;
};

template <typename HeadType_, typename BodyTypeList_>
void add_parent_concept_type_list_to_graph (Concept_ const &concept, TypeList_t<HeadType_,BodyTypeList_> const &parent_concept_type_list, Graph &g)
{
    Node concept_node(TypeStringOf_t<Concept_>::eval());
    assert(g.nodes().find(concept_node) != g.nodes().end());
    Node parent_node(TypeStringOf_t<HeadType_>::eval());
    g.add_node(parent_node);
    g.add_edge(Edge(concept_node, parent_node));
    add_parent_concept_type_list_to_graph(BodyTypeList_(), g);
}

void add_type_list_as_nodes_to_graph (EmptyTypeList const &, Graph &g)
{
    // nothing to add
}

template <typename Concept_>
void add_concept_ancestors_to_graph (Concept_ const &, Graph &g)
{
    add_type_list_as_nodes_to_graph(typename AncestorsOf_t<Concept_>::T(), g);

}

template <typename Concept_>
void add_concept_hierarchy_to_graph (Concept_ const &root, Graph &g)
{
    add_concept_to_graph_recursive(Concept
}

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_CONCEPTUALINHERITANCEGRAPH_HPP_