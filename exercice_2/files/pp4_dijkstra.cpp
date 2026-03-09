/**
 * pp4_dijkstra.cpp
 * ─────────────────────────────────────────────────────────────────────────────
 * Programme principal 4 : implémentation et test de l'algorithme de Dijkstra
 * sur NGraph::tGraph<T>.
 *
 * Deux graphes de test :
 *   1. Graphe numéroté classique (sommets = unsigned int)
 *   2. Graphe de villes (sommets = std::string)
 * ─────────────────────────────────────────────────────────────────────────────
 */

#include <iostream>
#include "ngraph.h"
#include "dijkstra.hpp"

using namespace NGraph;

static void sep(const std::string &title)
{
    std::cout << "\n" << std::string(62, '═') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(62, '═') << "\n";
}

// ═══════════════════════════════════════════════════════════════════════════
// Test 1 – graphe numéroté avec poids explicites
// ═══════════════════════════════════════════════════════════════════════════
void test_dijkstra_int()
{
    sep("Dijkstra – Graphe orienté pondéré (sommets = uint)");

    //   Graphe :
    //
    //       2      3
    //   1 ───→ 2 ───→ 4
    //   │             ↑
    //   │ 6     1     │
    //   └────→ 3 ────→┘  (1→3 poids 6, 3→4 poids 1, 2→3 poids 1)
    //               ↘
    //            5   5
    //         3 ───→ 5

    Graph g;
    g.insert_edge(1, 2);
    g.insert_edge(1, 3);
    g.insert_edge(2, 3);
    g.insert_edge(2, 4);
    g.insert_edge(3, 4);
    g.insert_edge(3, 5);

    WeightMap<unsigned int> w;
    w[{1u, 2u}] = 2.0;
    w[{1u, 3u}] = 6.0;
    w[{2u, 3u}] = 1.0;  // meilleur chemin vers 3 : 1→2→3 (coût 3)
    w[{2u, 4u}] = 3.0;
    w[{3u, 4u}] = 1.0;  // meilleur chemin vers 4 : 1→2→3→4 (coût 4)
    w[{3u, 5u}] = 5.0;

    std::cout << "Graphe G :\n" << g;

    auto result = dijkstra(g, 1u, w);
    result.print_distances();

    std::cout << "\n";
    result.print_path(4u);
    result.print_path(5u);
    result.print_path(1u);
}

// ═══════════════════════════════════════════════════════════════════════════
// Test 2 – graphe de villes avec poids = distances réelles (km)
// ═══════════════════════════════════════════════════════════════════════════
void test_dijkstra_cities()
{
    sep("Dijkstra – Graphe de villes françaises (sommets = string)");

    sGraph g;
    g.insert_edge("Paris",     "Lyon");
    g.insert_edge("Paris",     "Bordeaux");
    g.insert_edge("Lyon",      "Marseille");
    g.insert_edge("Lyon",      "Nice");
    g.insert_edge("Bordeaux",  "Toulouse");
    g.insert_edge("Toulouse",  "Marseille");
    g.insert_edge("Marseille", "Nice");

    WeightMap<std::string> w;
    w[{"Paris",     "Lyon"}]      = 465.0;
    w[{"Paris",     "Bordeaux"}]  = 580.0;
    w[{"Lyon",      "Marseille"}] = 315.0;
    w[{"Lyon",      "Nice"}]      = 470.0;
    w[{"Bordeaux",  "Toulouse"}]  = 245.0;
    w[{"Toulouse",  "Marseille"}] = 405.0;
    w[{"Marseille", "Nice"}]      = 205.0;

    std::cout << "Graphe G (distances en km) :\n" << g;

    auto result = dijkstra(g, std::string("Paris"), w);
    result.print_distances();

    std::cout << "\n";
    result.print_path("Nice");
    result.print_path("Toulouse");
    result.print_path("Marseille");
}

// ═══════════════════════════════════════════════════════════════════════════
// Test 3 – cas limite : sommet inaccessible
// ═══════════════════════════════════════════════════════════════════════════
void test_dijkstra_unreachable()
{
    sep("Dijkstra – Sommet inaccessible");

    Graph g;
    // Deux composantes connexes : {1,2} et {3,4}
    g.insert_edge(1, 2);
    g.insert_edge(3, 4);

    auto result = dijkstra(g, 1u);
    result.print_distances();
    result.print_path(4u); // doit afficher « inaccessible »
}

// ═══════════════════════════════════════════════════════════════════════════
// Test 4 – graphe non-pondéré (poids uniformes = 1)
// ═══════════════════════════════════════════════════════════════════════════
void test_dijkstra_unweighted()
{
    sep("Dijkstra – Graphe non-pondéré (BFS-like, poids=1)");

    Graph g;
    // Graphe en forme de grille :
    //  1→2→3
    //  ↓   ↓
    //  4→5→6
    g.insert_edge(1,2); g.insert_edge(2,3);
    g.insert_edge(1,4); g.insert_edge(3,6);
    g.insert_edge(4,5); g.insert_edge(5,6);

    // Pas de WeightMap → tous les poids valent 1.0
    auto result = dijkstra(g, 1u);
    result.print_distances();
    result.print_path(6u); // chemin le plus court en nb d'arêtes
}

// ═══════════════════════════════════════════════════════════════════════════
// main
// ═══════════════════════════════════════════════════════════════════════════
int main()
{
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║     PP4 – Algorithme de Dijkstra sur NGraph::tGraph<T>       ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";

    test_dijkstra_int();
    test_dijkstra_cities();
    test_dijkstra_unreachable();
    test_dijkstra_unweighted();

    std::cout << "\n[PP4] Tous les tests terminés.\n";
    return 0;
}
