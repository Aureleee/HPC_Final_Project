/**
 * dijkstra.hpp
 * ─────────────────────────────────────────────────────────────────────────────
 * Implémentation de l'algorithme de Dijkstra sur NGraph::tGraph<T>.
 *
 * Principe :
 *   Dijkstra calcule les plus courts chemins depuis un sommet source s vers
 *   tous les autres sommets dans un graphe à poids positifs.
 *
 * Complexité : O((V + E) log V) avec un tas min (priority_queue).
 *
 * Hypothèse ici : les poids des arêtes sont stockés dans une map externe
 *   weight_map[(u,v)] = w   (poids de l'arête u→v).
 *   Si une arête n'est pas dans la map, son poids par défaut est 1.0.
 * ─────────────────────────────────────────────────────────────────────────────
 */

#ifndef DIJKSTRA_HPP_
#define DIJKSTRA_HPP_

#include "ngraph.h"
#include <map>
#include <queue>
#include <vector>
#include <limits>
#include <stdexcept>
#include <iostream>

namespace NGraph
{

// ─── Type raccourci pour la carte des poids ───────────────────────────────────
template <typename T>
using WeightMap = std::map<std::pair<T, T>, double>;

// ═══════════════════════════════════════════════════════════════════════════
// Résultat de Dijkstra
// ═══════════════════════════════════════════════════════════════════════════
template <typename T>
struct DijkstraResult
{
    std::map<T, double> dist;   ///< dist[v] = distance minimale depuis source
    std::map<T, T>      prev;   ///< prev[v] = prédécesseur de v sur le chemin
    T                   source; ///< sommet source

    /**
     * @brief Reconstitue le chemin de source à target.
     * @return vecteur de sommets du chemin, vide si inaccessible.
     */
    std::vector<T> path_to(const T &target) const
    {
        const double INF = std::numeric_limits<double>::infinity();
        if (dist.find(target) == dist.end() || dist.at(target) == INF)
            return {}; // sommet inaccessible

        std::vector<T> path;
        T cur = target;
        while (cur != source)
        {
            path.push_back(cur);
            cur = prev.at(cur);
        }
        path.push_back(source);
        std::reverse(path.begin(), path.end());
        return path;
    }

    /// Affiche toutes les distances depuis la source
    void print_distances() const
    {
        const double INF = std::numeric_limits<double>::infinity();
        std::cout << "Distances depuis " << source << " :\n";
        for (auto &[v, d] : dist)
        {
            std::cout << "  " << source << " -> " << v << " : ";
            if (d == INF) std::cout << "∞ (inaccessible)\n";
            else          std::cout << d << "\n";
        }
    }

    /// Affiche le plus court chemin vers target
    void print_path(const T &target) const
    {
        std::vector<T> p = path_to(target);
        const double INF = std::numeric_limits<double>::infinity();
        if (p.empty())
        {
            std::cout << "Pas de chemin de " << source << " à " << target << "\n";
            return;
        }
        std::cout << "Chemin " << source << " -> " << target
                  << " (distance=" << dist.at(target) << ") : ";
        for (int i = 0; i < (int)p.size(); ++i)
        {
            std::cout << p[i];
            if (i + 1 < (int)p.size()) std::cout << " → ";
        }
        std::cout << "\n";
    }
};


// ═══════════════════════════════════════════════════════════════════════════
// Algorithme de Dijkstra
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Lance Dijkstra depuis le sommet `source` dans le graphe `G`.
 *
 * @param G          Le graphe NGraph::tGraph<T>
 * @param source     Sommet de départ
 * @param weights    Carte des poids (optionnel, défaut = 1.0 par arête)
 * @return DijkstraResult<T> contenant distances et prédécesseurs
 */
template <typename T>
DijkstraResult<T> dijkstra(const tGraph<T> &G,
                             const T &source,
                             const WeightMap<T> &weights = WeightMap<T>())
{
    const double INF = std::numeric_limits<double>::infinity();

    DijkstraResult<T> result;
    result.source = source;

    // Initialise toutes les distances à ∞
    for (typename tGraph<T>::const_iterator p = G.begin(); p != G.end(); ++p)
        result.dist[tGraph<T>::node(p)] = INF;

    if (result.dist.find(source) == result.dist.end())
        throw std::runtime_error("Sommet source absent du graphe.");

    result.dist[source] = 0.0;

    // File de priorité : (distance, sommet)
    // std::priority_queue est un max-heap → on utilise negative distances
    using PQ_Pair = std::pair<double, T>;
    std::priority_queue<PQ_Pair, std::vector<PQ_Pair>, std::greater<PQ_Pair>> pq;
    pq.push({0.0, source});

    while (!pq.empty())
    {
        auto [d, u] = pq.top(); pq.pop();

        // Si on a déjà trouvé un meilleur chemin, on ignore cette entrée
        if (d > result.dist[u]) continue;

        // Parcours des voisins sortants de u
        typename tGraph<T>::const_iterator pu = G.find(u);
        if (pu == G.end()) continue;

        const typename tGraph<T>::vertex_set &out = tGraph<T>::out_neighbors(pu);
        for (const T &v : out)
        {
            // Récupère le poids de l'arête u→v (1.0 par défaut)
            auto key = std::make_pair(u, v);
            double w = 1.0;
            auto wit = weights.find(key);
            if (wit != weights.end()) w = wit->second;

            if (w < 0.0)
                throw std::runtime_error(
                    "Dijkstra ne supporte pas les poids négatifs.");

            double new_dist = result.dist[u] + w;
            if (new_dist < result.dist[v])
            {
                result.dist[v] = new_dist;
                result.prev[v] = u;
                pq.push({new_dist, v});
            }
        }
    }

    return result;
}

} // namespace NGraph

#endif // DIJKSTRA_HPP_
