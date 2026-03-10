/**
 * ngraph.cpp
 * ─────────────────────────────────────────────────────────────────────────────
 * Implémentation des méthodes du template tGraph<T>.
 *
 * IMPORTANT : ce fichier est inclus par ngraph.h (via #include "ngraph.cpp")
 * car les templates C++ nécessitent que définition et déclaration soient
 * visibles ensemble lors de la compilation. Il NE DOIT PAS être compilé
 * directement en tant qu'unité de compilation indépendante.
 *
 * Convention : guard d'inclusion NGRAPH_IMPL_ pour éviter les inclusions
 * multiples.
 * ─────────────────────────────────────────────────────────────────────────────
 */

#ifndef NGRAPH_IMPL_
#define NGRAPH_IMPL_

// Pas de #include "ngraph.h" ici pour éviter la circularité ;
// ce fichier est toujours inclus depuis ngraph.h qui a déjà tout importé.

namespace NGraph
{

// ═══════════════════════════════════════════════════════════════════════════
// Constructeurs
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
tGraph<T>::tGraph()
    : G_(), num_edges_(0), undirected_(false) {}

template <typename T>
tGraph<T>::tGraph(std::istream &s)
    : G_(), num_edges_(0), undirected_(false)
{
    s >> *this; // délègue à operator>>
}

template <typename T>
tGraph<T>::tGraph(const tGraph &B)
    : G_(B.G_), num_edges_(B.num_edges_), undirected_(B.undirected_) {}

template <typename T>
tGraph<T>::tGraph(const edge_set &E)
    : G_(), num_edges_(0), undirected_(false)
{
    for (typename edge_set::const_iterator p = E.begin(); p != E.end(); ++p)
        insert_edge(*p);
}

// ═══════════════════════════════════════════════════════════════════════════
// Nettoyage
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
void tGraph<T>::clear()
{
    G_.clear();
    num_edges_  = 0;
    undirected_ = false;
}

// ═══════════════════════════════════════════════════════════════════════════
// Recherche d'un sommet
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
typename tGraph<T>::iterator tGraph<T>::find(const vertex &a)
{
    return G_.find(a);
}

template <typename T>
typename tGraph<T>::const_iterator tGraph<T>::find(const vertex &a) const
{
    return G_.find(a);
}

// ═══════════════════════════════════════════════════════════════════════════
// Accès aux voisins (instance)
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
const typename tGraph<T>::vertex_set &
tGraph<T>::in_neighbors(const vertex &a) const
{
    return find(a)->second.first;
}

template <typename T>
typename tGraph<T>::vertex_set &
tGraph<T>::in_neighbors(const vertex &a)
{
    return G_[a].first;
}

template <typename T>
const typename tGraph<T>::vertex_set &
tGraph<T>::out_neighbors(const vertex &a) const
{
    return find(a)->second.second;
}

template <typename T>
typename tGraph<T>::vertex_set &
tGraph<T>::out_neighbors(const vertex &a)
{
    return G_[a].second;
}

// ─── Itérateurs sur les voisins sortants ─────────────────────────────────────
template <typename T>
typename tGraph<T>::vertex_neighbor_iterator
tGraph<T>::out_neighbors_begin(const vertex &a)
{
    return out_neighbors(a).begin();
}

template <typename T>
typename tGraph<T>::vertex_neighbor_const_iterator
tGraph<T>::out_neighbors_begin(const vertex &a) const
{
    return out_neighbors(a).begin();
}

template <typename T>
typename tGraph<T>::vertex_neighbor_iterator
tGraph<T>::out_neighbors_end(const vertex &a)
{
    return out_neighbors(a).end();
}

template <typename T>
typename tGraph<T>::vertex_neighbor_const_iterator
tGraph<T>::out_neighbors_end(const vertex &a) const
{
    return out_neighbors(a).end();
}

// ═══════════════════════════════════════════════════════════════════════════
// Degrés
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
unsigned int tGraph<T>::in_degree(const vertex &a) const
{
    return in_neighbors(a).size();
}

template <typename T>
unsigned int tGraph<T>::out_degree(const vertex &a) const
{
    return out_neighbors(a).size();
}

template <typename T>
unsigned int tGraph<T>::degree(const vertex &a) const
{
    const_iterator p = find(a);
    if (p == end()) return 0;
    return degree(p);
}

template <typename T>
bool tGraph<T>::isolated(const vertex &a) const
{
    const_iterator p = find(a);
    if (p != end()) return isolated(p);
    return false;
}

// ═══════════════════════════════════════════════════════════════════════════
// Insertion de sommets et d'arêtes
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
void tGraph<T>::insert_vertex(const vertex &a)
{
    G_[a]; // crée l'entrée si elle n'existe pas encore
}

template <typename T>
void tGraph<T>::insert_new_vertex_inout_list(const vertex &a,
                                              const vertex_set &IN,
                                              const vertex_set &OUT)
{
    typename adj_graph::iterator p = find(a);
    if (p != G_.end())
        num_edges_ -= p->second.second.size(); // retire les anciennes arêtes

    G_[a] = std::make_pair(IN, OUT);
    num_edges_ += OUT.size();
}

// ─── Insertion d'arête via itérateurs ────────────────────────────────────────
template <typename T>
void tGraph<T>::insert_edge(iterator pa, iterator pb)
{
    vertex a = node(pa);
    vertex b = node(pb);

    // En mode non-orienté, on impose a < b pour éviter les doublons
    if (is_undirected())
    {
        vertex smallest = (a < b) ? a : b;
        if (smallest == b)
        {
            std::swap(a, b);
            std::swap(pa, pb);
        }
    }

    unsigned int old_size = out_neighbors(pa).size();
    out_neighbors(pa).insert(b);
    in_neighbors(pb).insert(a);

    if (out_neighbors(pa).size() > old_size)
        num_edges_++;
}

template <typename T>
void tGraph<T>::insert_edge_noloop(iterator pa, iterator pb)
{
    if (pa == pb) return;
    insert_edge(pa, pb);
}

// ─── Insertion d'arête via les sommets ───────────────────────────────────────
template <typename T>
void tGraph<T>::insert_edge(const vertex &a, const vertex &b)
{
    iterator pa = find(a);
    if (pa == G_.end()) { insert_vertex(a); pa = find(a); }

    iterator pb = find(b);
    if (pb == G_.end()) { insert_vertex(b); pb = find(b); }

    insert_edge(pa, pb);
}

template <typename T>
void tGraph<T>::insert_edge_noloop(const vertex &a, const vertex &b)
{
    if (a == b) return;
    insert_edge(a, b);
}

template <typename T>
void tGraph<T>::insert_edge(const edge &E)
{
    insert_edge(E.first, E.second);
}

template <typename T>
void tGraph<T>::insert_undirected_edge(const vertex &a, const vertex &b)
{
    (a < b) ? insert_edge(a, b) : insert_edge(b, a);
}

template <typename T>
void tGraph<T>::insert_undirected_edge(const edge &E)
{
    insert_undirected_edge(E.first, E.second);
}

// ═══════════════════════════════════════════════════════════════════════════
// Suppression de sommets et d'arêtes
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
bool tGraph<T>::remove_edge(iterator pa, iterator pb)
{
    if (pa == end() || pb == end()) return false;

    vertex a = node(pa);
    vertex b = node(pb);

    if (is_undirected() && a > b)
    {
        std::swap(a, b);
        std::swap(pa, pb);
    }

    unsigned int old_size = out_neighbors(pa).size();
    out_neighbors(pa).erase(b);
    in_neighbors(pb).erase(a);
    if (out_neighbors(pa).size() < old_size)
        num_edges_--;

    return true;
}

template <typename T>
void tGraph<T>::remove_edge(const vertex &a, const vertex &b)
{
    iterator pa = find(a);
    if (pa == end()) return;
    iterator pb = find(b);
    if (pb == end()) return;
    remove_edge(pa, pb);
}

template <typename T>
void tGraph<T>::remove_edge(const edge &E)
{
    remove_edge(E.first, E.second);
}

template <typename T>
void tGraph<T>::remove_undirected_edge(const vertex &a, const vertex &b)
{
    (a < b) ? remove_edge(a, b) : remove_edge(b, a);
}

template <typename T>
void tGraph<T>::remove_undirected_edge(const edge &e)
{
    remove_undirected_edge(e.first, e.second);
}

template <typename T>
void tGraph<T>::remove_vertex(iterator pa)
{
    // Copie les voisins pour éviter d'invalider les itérateurs pendant l'effacement
    vertex_set out_edges = out_neighbors(pa);
    vertex_set in_edges  = in_neighbors(pa);

    for (typename vertex_set::iterator p = out_edges.begin();
         p != out_edges.end(); ++p)
        remove_edge(pa, find(*p));

    for (typename vertex_set::iterator p = in_edges.begin();
         p != in_edges.end(); ++p)
        remove_edge(find(*p), pa);

    G_.erase(node(pa));
}

template <typename T>
void tGraph<T>::remove_vertex(const vertex &a)
{
    iterator pa = find(a);
    if (pa != G_.end()) remove_vertex(pa);
}

template <typename T>
void tGraph<T>::remove_vertex_set(const vertex_set &V)
{
    for (typename vertex_set::const_iterator p = V.begin(); p != V.end(); ++p)
        remove_vertex(*p);
}

// ═══════════════════════════════════════════════════════════════════════════
// Test d'appartenance
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
bool tGraph<T>::includes_vertex(const vertex &a) const
{
    return find(a) != G_.end();
}

template <typename T>
bool tGraph<T>::includes_edge(const vertex &a, const vertex &b) const
{
    return includes_vertex(a) ? includes_elm(out_neighbors(a), b) : false;
}

template <typename T>
bool tGraph<T>::includes_edge(const edge &e) const
{
    return includes_edge(e.first, e.second);
}

// ═══════════════════════════════════════════════════════════════════════════
// Export liste d'arêtes
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
std::vector<typename tGraph<T>::edge> tGraph<T>::edge_list() const
{
    std::vector<edge> E;
    for (typename tGraph::const_iterator p = begin(); p != end(); ++p)
    {
        const vertex     &a   = tGraph::node(p);
        const vertex_set &out = tGraph::out_neighbors(p);
        for (typename vertex_set::const_iterator t = out.begin();
             t != out.end(); ++t)
            E.push_back(edge(a, *t));
    }
    return E;
}

// ═══════════════════════════════════════════════════════════════════════════
// Opérations sur les graphes
// ═══════════════════════════════════════════════════════════════════════════

// Union : this ∪ B  (modifie this en place)
template <typename T>
tGraph<T> &tGraph<T>::plus_eq(const tGraph &B)
{
    for (const_iterator p = B.begin(); p != B.end(); ++p)
    {
        const vertex     &v   = node(p);
        insert_vertex(v);
        const vertex_set &out = out_neighbors(p);
        for (typename vertex_set::const_iterator q = out.begin();
             q != out.end(); ++q)
            insert_edge(v, *q);
    }
    return *this;
}

// Union : this ∪ B  (renvoie un nouveau graphe)
template <typename T>
tGraph<T> tGraph<T>::plus(const tGraph &B) const
{
    tGraph U(*this);
    U.plus_eq(B);
    return U;
}

// Intersection : this ∩ B
template <typename T>
tGraph<T> tGraph<T>::intersect(const tGraph &B) const
{
    tGraph G;
    for (const_iterator p = begin(); p != end(); ++p)
    {
        const vertex &v = node(p);
        if (B.includes_vertex(v)) G.insert_vertex(v);
        const vertex_set &out = out_neighbors(p);
        for (typename vertex_set::const_iterator q = out.begin();
             q != out.end(); ++q)
            if (B.includes_edge(v, *q)) G.insert_edge(v, *q);
    }
    return G;
}

// Différence : this \ B
template <typename T>
tGraph<T> tGraph<T>::minus(const tGraph &B) const
{
    tGraph G;
    for (const_iterator p = begin(); p != end(); ++p)
    {
        const vertex &v = node(p);
        if (isolated(p))
        {
            if (!B.isolated(v)) G.insert_vertex(v);
        }
        else
        {
            const vertex_set &out = out_neighbors(p);
            for (typename vertex_set::const_iterator q = out.begin();
                 q != out.end(); ++q)
                if (!B.includes_edge(v, *q)) G.insert_edge(v, *q);
        }
    }
    return G;
}

// Opérateurs
template <typename T>
tGraph<T> tGraph<T>::operator+(const tGraph &B) const { return plus(B); }

template <typename T>
tGraph<T> tGraph<T>::operator*(const tGraph &B) const { return intersect(B); }

template <typename T>
tGraph<T> tGraph<T>::operator-(const tGraph &B) const { return minus(B); }

template <typename T>
tGraph<T> &tGraph<T>::operator+=(const tGraph &B) { return plus_eq(B); }

// ═══════════════════════════════════════════════════════════════════════════
// Sous-graphe
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
tGraph<T> tGraph<T>::subgraph(const vertex_set &A) const
{
    tGraph G;
    for (typename vertex_set::const_iterator p = A.begin(); p != A.end(); ++p)
    {
        const_iterator t = find(*p);
        if (t != end())
        {
            vertex_set new_in  = (A * in_neighbors(t));
            vertex_set new_out = (A * out_neighbors(t));
            G.insert_new_vertex_inout_list(*p, new_in, new_out);
        }
    }
    return G;
}

template <typename T>
unsigned int tGraph<T>::subgraph_size(const vertex_set &A) const
{
    unsigned int n = 0;
    for (typename vertex_set::const_iterator p = A.begin(); p != A.end(); ++p)
    {
        const_iterator pG = find(*p);
        if (pG != end()) n += intersection_size(A, out_neighbors(pG));
    }
    return n;
}

template <typename T>
double tGraph<T>::subgraph_sparsity(const vertex_set &A) const
{
    double N        = A.size();
    double sparsity = (A.size() == 1 ? 0.0 : subgraph_size(A) / (N * (N - 1)));
    if (is_undirected()) sparsity *= 2.0;
    return sparsity;
}

// ═══════════════════════════════════════════════════════════════════════════
// Absorption (fusion de sommets)
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
void tGraph<T>::absorb(iterator pa, iterator pb)
{
    if (pa == pb) return;

    remove_edge(pa, pb);

    // Redirige toutes les arêtes sortantes de b vers a
    vertex_set b_out = out_neighbors(pb);
    for (typename vertex_set::iterator p = b_out.begin(); p != b_out.end(); ++p)
    {
        iterator pi = find(*p);
        remove_edge(pb, pi);
        insert_edge(pa, pi);
    }

    // Redirige toutes les arêtes entrantes de b vers a
    vertex_set b_in = in_neighbors(pb);
    for (typename vertex_set::iterator p = b_in.begin(); p != b_in.end(); ++p)
    {
        iterator pi = find(*p);
        remove_edge(pi, pb);
        insert_edge(pi, pa);
    }

    remove_vertex(pb);
}

template <typename T>
void tGraph<T>::absorb(vertex a, vertex b)
{
    if (a == b) return;
    iterator pa = find(a);
    if (pa == end()) return;
    iterator pb = find(b);
    if (pb == end()) return;
    absorb(pa, pb);
}

template <typename T>
typename tGraph<T>::iterator
tGraph<T>::smart_absorb(iterator pa, iterator pb)
{
    if (degree(pa) >= degree(pb)) { absorb(pa, pb); return pb; }
    else                          { absorb(pb, pa); return pb; }
}

template <typename T>
T tGraph<T>::smart_absorb(vertex a, vertex b)
{
    iterator pa = find(a);
    if (pa == end()) return b;
    iterator pb = find(b);
    if (pb == end()) return a;
    iterator pc = smart_absorb(pa, pb);
    return node(pc);
}

// ═══════════════════════════════════════════════════════════════════════════
// Affichage
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
void tGraph<T>::print() const
{
    std::cerr << "# sommets : " << num_vertices() << "\n";
    std::cerr << "# arêtes  : " << num_edges()    << "\n";
    for (const_iterator p = G_.begin(); p != G_.end(); ++p)
    {
        const vertex_set &out = out_neighbors(p);
        for (typename vertex_set::const_iterator q = out.begin();
             q != out.end(); ++q)
            std::cerr << p->first << "  -->  " << *q << "\n";
    }
    std::cerr << std::endl;
}

// ═══════════════════════════════════════════════════════════════════════════
// Méthodes statiques sur les itérateurs
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
const T &tGraph<T>::node(const_iterator p)         { return p->first; }

template <typename T>
const T &tGraph<T>::node(iterator p)               { return p->first; }

template <typename T>
const T &tGraph<T>::node(const_vertex_iterator p)  { return *p; }

template <typename T>
const typename tGraph<T>::vertex_set &
tGraph<T>::in_neighbors(const_iterator p) { return p->second.first; }

template <typename T>
typename tGraph<T>::vertex_set &
tGraph<T>::in_neighbors(iterator p)       { return p->second.first; }

template <typename T>
const typename tGraph<T>::vertex_set &
tGraph<T>::out_neighbors(const_iterator p){ return p->second.second; }

template <typename T>
typename tGraph<T>::vertex_set &
tGraph<T>::out_neighbors(iterator p)      { return p->second.second; }

template <typename T>
typename tGraph<T>::const_vertex_iterator
tGraph<T>::in_begin(const_iterator p)  { return in_neighbors(p).begin(); }

template <typename T>
typename tGraph<T>::const_vertex_iterator
tGraph<T>::in_end(const_iterator p)    { return in_neighbors(p).end(); }

template <typename T>
typename tGraph<T>::const_vertex_iterator
tGraph<T>::out_begin(const_iterator p) { return out_neighbors(p).begin(); }

template <typename T>
typename tGraph<T>::vertex_iterator
tGraph<T>::out_begin(iterator p)       { return out_neighbors(p).begin(); }

template <typename T>
unsigned int tGraph<T>::num_edges(const_iterator p)
{ return out_neighbors(p).size(); }

template <typename T>
unsigned int tGraph<T>::num_edges(iterator p)
{ return out_neighbors(p).size(); }

template <typename T>
unsigned int tGraph<T>::out_degree(const_iterator p)
{ return p->second.second.size(); }

template <typename T>
unsigned int tGraph<T>::out_degree(iterator p)
{ return p->second.second.size(); }

template <typename T>
unsigned int tGraph<T>::in_degree(const_iterator p)
{ return p->second.first.size(); }

template <typename T>
unsigned int tGraph<T>::in_degree(iterator p)
{ return p->second.first.size(); }

template <typename T>
unsigned int tGraph<T>::degree(const_iterator p)
{ return in_neighbors(p).size() + out_neighbors(p).size(); }

template <typename T>
unsigned int tGraph<T>::degree(iterator p)
{ return in_neighbors(p).size() + out_neighbors(p).size(); }

template <typename T>
bool tGraph<T>::isolated(const_iterator p)
{ return (in_degree(p) == 0 && out_degree(p) == 0); }

template <typename T>
bool tGraph<T>::isolated(iterator p)
{ return (in_degree(p) == 0 && out_degree(p) == 0); }

// ═══════════════════════════════════════════════════════════════════════════
// Lecture d'une ligne (format texte)
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
std::istream &tGraph<T>::read_line(std::istream &s, T &v1, T &v2,
                                    std::string &line, line_type &t)
{
    while (getline(s, line) && line.size() < 1) ;

    if (s.eof()) { t = EMPTY; return s; }

    if (line[0] == '%' || line[0] == '#') { t = COMMENT; return s; }

    std::istringstream L(line);
    L >> v1;
    if (L.eof()) { t = VERTEX; }
    else         { L >> v2; t = EDGE; }

    return s;
}

// ═══════════════════════════════════════════════════════════════════════════
// Opérateurs de flux (hors classe)
// ═══════════════════════════════════════════════════════════════════════════

template <typename T>
std::istream &operator>>(std::istream &s, tGraph<T> &G)
{
    std::string line;
    T v1, v2;
    typename tGraph<T>::line_type t;

    while (tGraph<T>::read_line(s, v1, v2, line, t))
    {
        if      (t == tGraph<T>::VERTEX) G.insert_vertex(v1);
        else if (t == tGraph<T>::EDGE)   G.insert_edge(v1, v2);
    }
    return s;
}

template <typename T>
std::ostream &operator<<(std::ostream &s, const tGraph<T> &G)
{
    for (typename tGraph<T>::const_node_iterator p = G.begin();
         p != G.end(); ++p)
    {
        const typename tGraph<T>::vertex_set &out = tGraph<T>::out_neighbors(p);
        typename tGraph<T>::vertex v = p->first;
        if (out.empty() && tGraph<T>::in_neighbors(p).empty())
            s << v << "\n";                            // sommet isolé
        else
            for (typename tGraph<T>::vertex_set::const_iterator q = out.begin();
                 q != out.end(); ++q)
                s << v << " " << *q << "\n";
    }
    return s;
}

} // namespace NGraph

#endif // NGRAPH_IMPL_
