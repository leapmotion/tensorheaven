// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/conceptualinheritancegraph.hpp by Vector Dods, created 2013/09/12
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_CONCEPTUALINHERITANCEGRAPH_HPP_
#define TENH_CONCEPTUAL_CONCEPTUALINHERITANCEGRAPH_HPP_

#include "tenh/core.hpp"

#include <cassert>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <string>

#include "tenh/conceptual/concept.hpp"

namespace Tenh {

struct Graph
{
    struct Node
    {
        std::string m_shortified_name;
        std::string m_full_name;

        Node (std::string const &shortified_name, std::string const &full_name)
            :
            m_shortified_name(shortified_name),
            m_full_name(full_name)
        {
            assert(!m_shortified_name.empty());
            assert(!m_full_name.empty());
        }

        bool operator == (Node const &r) const
        {
            return this->m_shortified_name == r.m_shortified_name && this->m_full_name == r.m_full_name;
        }
        bool operator < (Node const &r) const
        {
            return this->m_shortified_name < r.m_shortified_name
                   ||
                   (this->m_shortified_name == r.m_shortified_name && this->m_full_name < r.m_full_name);
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

        bool operator < (Edge const &r) const
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
    void add_edge (Node const &source, Node const &target)
    {
        assert(m_nodes.find(source) != m_nodes.end() && "the source node must already be in the graph");
        assert(m_nodes.find(target) != m_nodes.end() && "the target node must already be in the graph");
        m_edges.insert(Edge(source, target));
    }

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

    void print_as_dot_graph (std::ostream &out) const
    {
        // have to use identifiers instead of the full strings for the node names in dot,
        // so this part is to construct a map from nodes to identifiers (and back)
        typedef std::map<Node,std::string> NodeIdentifierMap;
        NodeIdentifierMap node_identifier_map;
        Uint32 id_number = 0;
        for (Nodes::const_iterator it = m_nodes.begin(), it_end = m_nodes.end(); it != it_end; ++it)
        {
            Node const &n = *it;
            std::string id(FORMAT("id" << id_number++));
            node_identifier_map[n] = id;
        }

        // generate the dot graph
        out << "digraph {\n";
        for (Nodes::const_iterator it = m_nodes.begin(), it_end = m_nodes.end(); it != it_end; ++it)
        {
            Node const &n = *it;
            out << "    " << node_identifier_map[n] << " [label=\"";
            print_string_with_newlines_as_char_literals(out, n.m_shortified_name);
            out << "\", fontname=\"courier\", labeljust=\"l\", shape=box];\n";
        }
        out << '\n';
        for (Edges::const_iterator it = m_edges.begin(), it_end = m_edges.end(); it != it_end; ++it)
        {
            Edge const &e = *it;
            out << "    " << node_identifier_map[e.m_source] << " -> " << node_identifier_map[e.m_target] << ";\n";
        }
        out << "}\n";
    }

private:

    static void print_string_with_newlines_as_char_literals (std::ostream &out, std::string const &s)
    {
        for (std::string::const_iterator it = s.begin(), it_end = s.end(); it != it_end; ++it)
        {
            char const &c = *it;
            if (c == '\n')
                out << "\\n";
            else
                out << c;
        }
    }

    Nodes m_nodes;
    Edges m_edges;
};

inline std::ostream &operator << (std::ostream &out, Graph::Node const &n)
{
    if (n.m_shortified_name == n.m_full_name)
        return out << "Node(" << n.m_full_name << ')';
    else
        return out << "Node(" << n.m_shortified_name << ',' << n.m_full_name << ')';
}

inline std::ostream &operator << (std::ostream &out, Graph::Edge const &e)
{
    return out << "Edge(" << e.m_source << " -> " << e.m_target << ')';
}

inline std::ostream &operator << (std::ostream &out, Graph const &g)
{
    out << "Graph(\n";
    for (Graph::Nodes::const_iterator it = g.nodes().begin(), it_end = g.nodes().end(); it != it_end; ++it)
    {
        Graph::Node const &n = *it;
        out << '\t' << n << '\n';
    }
    out << '\n';
    for (Graph::Edges::const_iterator it = g.edges().begin(), it_end = g.edges().end(); it != it_end; ++it)
    {
        Graph::Edge const &e = *it;
        out << '\t' << e << '\n';
    }
    return out << ")\n";
}

// base case
template <Uint32 SHORTIFY_DEPTH_, typename Concept_>
void add_parent_concept_type_list_to_graph_recursive (Concept_ const &, EmptyTypeList const &, Graph &g)
{
    // nothing to add
}

template <Uint32 SHORTIFY_DEPTH_, typename Concept_, typename HeadType_, typename BodyTypeList_>
void add_parent_concept_type_list_to_graph_recursive (
    Concept_ const &concept,
    TypeList_t<HeadType_,BodyTypeList_> const &,// This is the parent_concept_type_list
    Graph &g)
{
    // add the parent nodes and edges connecting concept to them
    Graph::Node concept_node(FORMAT((Pretty<TypeStringOf_t<Concept_>,SHORTIFY_DEPTH_>())),
                             FORMAT((Pretty<TypeStringOf_t<Concept_>,0>())));
    assert(g.nodes().find(concept_node) != g.nodes().end());
    Graph::Node parent_node(FORMAT((Pretty<TypeStringOf_t<HeadType_>,SHORTIFY_DEPTH_>())),
                            FORMAT((Pretty<TypeStringOf_t<HeadType_>,0>())));
    g.add_node(parent_node);
    g.add_edge(concept_node, parent_node);
    add_parent_concept_type_list_to_graph_recursive<SHORTIFY_DEPTH_>(concept, BodyTypeList_(), g);

    // call this function recursively on each parent
    add_parent_concept_type_list_to_graph_recursive<SHORTIFY_DEPTH_>(HeadType_(), typename HeadType_::ParentTypeList(), g);
}

template <Uint32 SHORTIFY_DEPTH_, typename Concept_>
void add_concept_hierarchy_to_graph (Concept_ const &root, Graph &g)
{
    Graph::Node n(FORMAT((Pretty<TypeStringOf_t<Concept_>,SHORTIFY_DEPTH_>())),
                  FORMAT((Pretty<TypeStringOf_t<Concept_>,0>())));
    g.add_node(n);
    add_parent_concept_type_list_to_graph_recursive<SHORTIFY_DEPTH_>(root, typename Concept_::ParentTypeList(), g);
}

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_CONCEPTUALINHERITANCEGRAPH_HPP_