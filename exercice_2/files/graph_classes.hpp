/**
 * graph_classes.hpp
 * ─────────────────────────────────────────────────────────────────────────────
 * Implémentation des classes Graph, SubGraph, Tree.
 *
 * Ces classes sont des abstractions de haut niveau qui encapsulent NGraph::tGraph.
 * ─────────────────────────────────────────────────────────────────────────────
 */

#ifndef GRAPH_CLASSES_HPP_
#define GRAPH_CLASSES_HPP_

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <stdexcept>

// ═══════════════════════════════════════════════════════════════════════════
// Classe Graph  – graphe orienté générique basé sur liste d'adjacence
// ═══════════════════════════════════════════════════════════════════════════
class Graph
{
public:
    // ── Types publics ────────────────────────────────────────────────────────
    using Vertex   = int;
    using Edge     = std::pair<Vertex, Vertex>;
    using AdjList  = std::map<Vertex, std::set<Vertex>>;

private:
    AdjList      adj_;       ///< liste d'adjacence
    bool         directed_;  ///< true = orienté, false = non-orienté
    int          num_edges_; ///< compteur d'arêtes

public:
    // ── Constructeurs ────────────────────────────────────────────────────────
    explicit Graph(bool directed = true)
        : directed_(directed), num_edges_(0) {}

    // ── Accesseurs ───────────────────────────────────────────────────────────
    int  num_vertices() const { return static_cast<int>(adj_.size()); }
    int  num_edges()    const { return num_edges_; }
    bool is_directed()  const { return directed_; }

    // ── Insertion ────────────────────────────────────────────────────────────
    void add_vertex(Vertex v)
    {
        adj_[v]; // crée l'entrée si elle n'existe pas
    }

    void add_edge(Vertex u, Vertex v)
    {
        adj_[u]; adj_[v]; // garantit que les deux sommets existent
        if (adj_[u].insert(v).second) ++num_edges_;
        if (!directed_) adj_[v].insert(u);
    }

    // ── Suppression ──────────────────────────────────────────────────────────
    void remove_edge(Vertex u, Vertex v)
    {
        if (adj_.count(u) && adj_[u].erase(v)) --num_edges_;
        if (!directed_ && adj_.count(v))        adj_[v].erase(u);
    }

    void remove_vertex(Vertex v)
    {
        adj_.erase(v);
        for (auto &[u, neighbors] : adj_)
        {
            if (neighbors.erase(v) && directed_) --num_edges_;
        }
    }

    // ── Requêtes ─────────────────────────────────────────────────────────────
    bool has_vertex(Vertex v) const { return adj_.count(v) > 0; }
    bool has_edge(Vertex u, Vertex v) const
    {
        auto it = adj_.find(u);
        return it != adj_.end() && it->second.count(v) > 0;
    }

    std::set<Vertex> neighbors(Vertex v) const
    {
        auto it = adj_.find(v);
        return (it != adj_.end()) ? it->second : std::set<Vertex>{};
    }

    int degree(Vertex v) const
    {
        auto it = adj_.find(v);
        return (it != adj_.end()) ? static_cast<int>(it->second.size()) : 0;
    }

    // ── Itération ────────────────────────────────────────────────────────────
    AdjList::const_iterator begin() const { return adj_.begin(); }
    AdjList::const_iterator end()   const { return adj_.end();   }

    // ── Affichage ────────────────────────────────────────────────────────────
    friend std::ostream &operator<<(std::ostream &os, const Graph &g)
    {
        os << "Graph(" << (g.directed_ ? "dirigé" : "non-dirigé") << ")\n";
        os << "  Sommets : " << g.num_vertices()
           << "  Arêtes : " << g.num_edges() << "\n";
        for (auto &[u, neighbors] : g.adj_)
        {
            os << "  " << u << " -> { ";
            for (Vertex w : neighbors) os << w << " ";
            os << "}\n";
        }
        return os;
    }
};


// ═══════════════════════════════════════════════════════════════════════════
// Classe SubGraph – sous-graphe induit par un sous-ensemble de sommets
// ═══════════════════════════════════════════════════════════════════════════
class SubGraph : public Graph
{
private:
    const Graph  *parent_;       ///< graphe parent (référence)
    std::set<int> vertex_subset_; ///< sous-ensemble de sommets

public:
    /**
     * @brief Construit le sous-graphe de `parent` induit par `vertices`.
     *
     * Un sous-graphe induit contient exactement les arêtes du graphe parent
     * dont les DEUX extrémités appartiennent au sous-ensemble.
     */
    SubGraph(const Graph &parent, const std::set<int> &vertices)
        : Graph(parent.is_directed()), parent_(&parent),
          vertex_subset_(vertices)
    {
        // Ajout des sommets du sous-ensemble
        for (int v : vertices) add_vertex(v);

        // Ajout des arêtes induites
        for (int u : vertices)
            for (int w : parent.neighbors(u))
                if (vertices.count(w)) add_edge(u, w);
    }

    const std::set<int> &vertex_subset() const { return vertex_subset_; }

    friend std::ostream &operator<<(std::ostream &os, const SubGraph &sg)
    {
        os << "SubGraph (sous-ensemble : { ";
        for (int v : sg.vertex_subset_) os << v << " ";
        os << "})\n";
        os << static_cast<const Graph &>(sg);
        return os;
    }
};


// ═══════════════════════════════════════════════════════════════════════════
// Classe Tree – arbre enraciné
//
// Un arbre est un graphe connexe acyclique. Ici on l'implémente en maintenant
// explicitement le parent de chaque nœud (structure classique d'arbre enraciné).
// ═══════════════════════════════════════════════════════════════════════════
class Tree
{
public:
    using Node = int;

private:
    Node                     root_;
    std::map<Node, Node>     parent_;   ///< parent_[v] = père de v
    std::map<Node, std::vector<Node>> children_; ///< enfants de chaque nœud
    std::set<Node>           nodes_;

public:
    explicit Tree(Node root) : root_(root)
    {
        nodes_.insert(root);
        children_[root]; // initialise la liste d'enfants
    }

    // ── Accesseurs ───────────────────────────────────────────────────────────
    Node root()       const { return root_; }
    int  num_nodes()  const { return static_cast<int>(nodes_.size()); }
    int  num_edges()  const { return num_nodes() - 1; } // propriété d'un arbre

    bool is_root(Node v)  const { return v == root_; }
    bool has_node(Node v) const { return nodes_.count(v) > 0; }

    Node parent(Node v) const
    {
        if (v == root_) throw std::runtime_error("La racine n'a pas de parent.");
        return parent_.at(v);
    }

    const std::vector<Node> &children(Node v) const
    {
        static const std::vector<Node> empty;
        auto it = children_.find(v);
        return (it != children_.end()) ? it->second : empty;
    }

    // ── Insertion ────────────────────────────────────────────────────────────
    /**
     * @brief Ajoute un nœud `child` comme enfant de `parent_node`.
     * @throws std::runtime_error si parent_node n'existe pas dans l'arbre.
     */
    void add_child(Node parent_node, Node child)
    {
        if (!has_node(parent_node))
            throw std::runtime_error("Nœud parent introuvable dans l'arbre.");
        if (has_node(child))
            throw std::runtime_error("Le nœud existe déjà dans l'arbre.");

        nodes_.insert(child);
        parent_[child] = parent_node;
        children_[parent_node].push_back(child);
        children_[child]; // initialise la liste d'enfants (vide)
    }

    // ── Calcul de profondeur ──────────────────────────────────────────────────
    int depth(Node v) const
    {
        int d = 0;
        Node cur = v;
        while (cur != root_) { cur = parent_.at(cur); ++d; }
        return d;
    }

    // ── Affichage récursif (DFS) ──────────────────────────────────────────────
    void print_subtree(Node v, int indent = 0) const
    {
        std::cout << std::string(indent * 2, ' ') << v << "\n";
        for (Node c : children(v)) print_subtree(c, indent + 1);
    }

    friend std::ostream &operator<<(std::ostream &os, const Tree &t)
    {
        os << "Tree (racine=" << t.root_
           << ", " << t.num_nodes() << " nœuds, "
           << t.num_edges() << " arêtes)\n";
        t.print_subtree(t.root_);
        return os;
    }
};

#endif // GRAPH_CLASSES_HPP_
