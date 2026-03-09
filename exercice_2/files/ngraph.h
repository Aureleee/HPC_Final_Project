/**
 * ngraph.h
 * ─────────────────────────────────────────────────────────────────────────────
 * Déclaration du template tGraph<T> (graphe orienté/non-orienté générique).
 *
 * Ce fichier est la partie « .h » issue du découpage de ngraph.hpp.
 * L'implémentation des méthodes NON-inline se trouve dans ngraph.cpp.
 *
 * Dépendance : set_ops.hpp (opérateurs ensemblistes sur std::set<T>)
 * ─────────────────────────────────────────────────────────────────────────────
 */

#ifndef NGRAPH_H_
#define NGRAPH_H_

#include <iostream>
#include <set>
#include <map>
#include <utility>      // std::pair
#include <iterator>     // std::inserter
#include <vector>       // edge_list()
#include <string>
#include <algorithm>
#include <sstream>      // opérateurs << et >>
#include "set_ops.hpp"

namespace NGraph
{

/**
 * @class tGraph
 * @brief Graphe générique orienté (ou non-orienté) paramétré par le type
 *        de sommet T.
 *
 * Structure interne : liste d'adjacence représentée par une std::map
 *   sommet → (ensemble des prédécesseurs, ensemble des successeurs)
 *
 * Complexité mémoire : O(V + E)
 */
template <typename T>
class tGraph
{
public:
    // ── Alias de types publics ───────────────────────────────────────────────
    typedef T                                       vertex;
    typedef T                                       value_type;
    typedef std::pair<vertex, vertex>               edge;
    typedef std::set<vertex>                        vertex_set;
    typedef std::set<edge>                          edge_set;
    typedef std::pair<vertex_set, vertex_set>       in_out_edge_sets;
    typedef std::map<vertex, in_out_edge_sets>      adj_graph;

    typedef typename edge_set::iterator             edge_iterator;
    typedef typename edge_set::const_iterator       const_edge_iterator;
    typedef typename vertex_set::iterator           vertex_iterator;
    typedef typename vertex_set::const_iterator     const_vertex_iterator;

    // Alias pratiques pour les voisins
    typedef typename vertex_set::iterator           vertex_neighbor_iterator;
    typedef typename vertex_set::const_iterator     vertex_neighbor_const_iterator;

    // Type d'une ligne dans le format texte du graphe
    enum line_type { VERTEX, EDGE, COMMENT, EMPTY };

private:
    // ── Données membres ──────────────────────────────────────────────────────
    adj_graph    G_;          ///< Représentation par liste d'adjacence
    unsigned int num_edges_;  ///< Nombre d'arêtes (mis à jour dynamiquement)
    bool         undirected_; ///< Vrai si le graphe est non-orienté

public:
    // ── Itérateurs sur la structure interne (map sommet→voisins) ─────────────
    typedef typename adj_graph::iterator       iterator;
    typedef typename adj_graph::const_iterator const_iterator;
    typedef iterator       node_iterator;
    typedef const_iterator const_node_iterator;

    // ── Accesseurs simples (inline) ──────────────────────────────────────────
    unsigned int num_vertices() const { return G_.size(); }
    unsigned int num_nodes()    const { return G_.size(); }
    unsigned int num_edges()    const { return num_edges_; }
    bool is_undirected()        const { return undirected_; }
    bool is_directed()          const { return !undirected_; }

    iterator       begin()       { return G_.begin(); }
    const_iterator begin() const { return G_.begin(); }
    iterator       end()         { return G_.end();   }
    const_iterator end()   const { return G_.end();   }

    // ── Constructeurs / destructeur ──────────────────────────────────────────
    tGraph();
    explicit tGraph(std::istream &s);
    tGraph(const tGraph &B);
    explicit tGraph(const edge_set &E);

    // ── Configuration ────────────────────────────────────────────────────────
    void set_undirected() { undirected_ = true; }
    void clear();

    // ── Recherche d'un sommet ────────────────────────────────────────────────
    iterator       find(const vertex &a);
    const_iterator find(const vertex &a) const;

    // ── Accès aux voisins ────────────────────────────────────────────────────
    const vertex_set &in_neighbors (const vertex &a) const;
          vertex_set &in_neighbors (const vertex &a);
    const vertex_set &out_neighbors(const vertex &a) const;
          vertex_set &out_neighbors(const vertex &a);

    vertex_neighbor_iterator       out_neighbors_begin(const vertex &a);
    vertex_neighbor_const_iterator out_neighbors_begin(const vertex &a) const;
    vertex_neighbor_iterator       out_neighbors_end  (const vertex &a);
    vertex_neighbor_const_iterator out_neighbors_end  (const vertex &a) const;

    // ── Degrés ───────────────────────────────────────────────────────────────
    unsigned int in_degree (const vertex &a) const;
    unsigned int out_degree(const vertex &a) const;
    unsigned int degree    (const vertex &a) const;
    bool isolated(const vertex &a) const;

    // ── Insertion de sommets et d'arêtes ─────────────────────────────────────
    void insert_vertex(const vertex &a);
    void insert_new_vertex_inout_list(const vertex &a,
                                      const vertex_set &IN,
                                      const vertex_set &OUT);
    void insert_edge(iterator pa, iterator pb);
    void insert_edge_noloop(iterator pa, iterator pb);
    void insert_edge(const vertex &a, const vertex &b);
    void insert_edge_noloop(const vertex &a, const vertex &b);
    void insert_edge(const edge &E);
    void insert_undirected_edge(const vertex &a, const vertex &b);
    void insert_undirected_edge(const edge &E);

    // ── Suppression de sommets et d'arêtes ───────────────────────────────────
    bool remove_edge(iterator pa, iterator pb);
    void remove_edge(const vertex &a, const vertex &b);
    void remove_edge(const edge &E);
    void remove_undirected_edge(const vertex &a, const vertex &b);
    void remove_undirected_edge(const edge &e);
    void remove_vertex(iterator pa);
    void remove_vertex(const vertex &a);
    void remove_vertex_set(const vertex_set &V);

    // ── Test d'appartenance ──────────────────────────────────────────────────
    bool includes_vertex(const vertex &a) const;
    bool includes_edge(const vertex &a, const vertex &b) const;
    bool includes_edge(const edge &e) const;

    // ── Export sous forme de liste d'arêtes ──────────────────────────────────
    std::vector<edge> edge_list() const;

    // ── Opérations sur les graphes ───────────────────────────────────────────
    tGraph &plus_eq(const tGraph &B);
    tGraph  plus   (const tGraph &B) const;
    tGraph  intersect(const tGraph &B) const;
    tGraph  minus    (const tGraph &B) const;

    tGraph operator+(const tGraph &B)  const;
    tGraph operator*(const tGraph &B)  const;
    tGraph operator-(const tGraph &B)  const;
    tGraph &operator+=(const tGraph &B);

    // ── Sous-graphe ──────────────────────────────────────────────────────────
    tGraph       subgraph(const vertex_set &A) const;
    unsigned int subgraph_size(const vertex_set &A) const;
    double       subgraph_sparsity(const vertex_set &A) const;

    // ── Absorption (fusion de sommets) ───────────────────────────────────────
    void     absorb(iterator pa, iterator pb);
    void     absorb(vertex a, vertex b);
    iterator smart_absorb(iterator pa, iterator pb);
    vertex   smart_absorb(vertex a, vertex b);

    // ── Affichage ────────────────────────────────────────────────────────────
    void print() const;

    // ── Méthodes statiques sur les itérateurs ────────────────────────────────
    static const vertex    &node(const_iterator p);
    static const vertex    &node(iterator p);
    static const vertex    &node(const_vertex_iterator p);

    static const vertex_set &in_neighbors (const_iterator p);
    static       vertex_set &in_neighbors (iterator p);
    static const vertex_set &out_neighbors(const_iterator p);
    static       vertex_set &out_neighbors(iterator p);

    static const_vertex_iterator in_begin (const_iterator p);
    static const_vertex_iterator in_end   (const_iterator p);
    static const_vertex_iterator out_begin(const_iterator p);
    static vertex_iterator       out_begin(iterator p);

    static unsigned int num_edges (const_iterator p);
    static unsigned int num_edges (iterator p);
    static unsigned int out_degree(const_iterator p);
    static unsigned int out_degree(iterator p);
    static unsigned int in_degree (const_iterator p);
    static unsigned int in_degree (iterator p);
    static unsigned int degree    (const_iterator p);
    static unsigned int degree    (iterator p);
    static bool isolated(const_iterator p);
    static bool isolated(iterator p);

    // ── Lecture d'une ligne (format texte) ───────────────────────────────────
    static std::istream &read_line(std::istream &s, T &v1, T &v2,
                                   std::string &line, line_type &t);
};

// ── Alias pratiques pour les types courants ───────────────────────────────────
typedef tGraph<unsigned int> Graph;
typedef tGraph<int>          iGraph;
typedef tGraph<std::string>  sGraph;

// ── Opérateurs de flux (déclarés ici, définis dans ngraph.cpp) ────────────────
template <typename T>
std::istream &operator>>(std::istream &s, tGraph<T> &G);

template <typename T>
std::ostream &operator<<(std::ostream &s, const tGraph<T> &G);

} // namespace NGraph

// Les templates doivent être inclus dans le .h pour que le compilateur
// puisse les instancier. On inclut donc le fichier d'implémentation ici.
#include "ngraph.cpp"

#endif // NGRAPH_H_
