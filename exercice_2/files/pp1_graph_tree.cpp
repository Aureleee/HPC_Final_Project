/**
 * pp1_graph_tree.cpp
 * ─────────────────────────────────────────────────────────────────────────────
 * Programme principal 1 : test des classes Graph, SubGraph, Tree.
 * ─────────────────────────────────────────────────────────────────────────────
 */

#include <iostream>
#include <set>
#include "graph_classes.hpp"

// ─── Séparateur d'affichage ───────────────────────────────────────────────────
static void sep(const std::string &title)
{
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

// ═══════════════════════════════════════════════════════════════════════════
// Tests de la classe Graph
// ═══════════════════════════════════════════════════════════════════════════
void test_graph()
{
    sep("TEST : classe Graph (dirigé)");

    // Création d'un graphe dirigé
    Graph g(true);

    // Ajout de sommets et d'arêtes
    // Représente le graphe :  1 → 2 → 4
    //                         1 → 3 → 4
    //                         3 → 5
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 4);
    g.add_edge(3, 4);
    g.add_edge(3, 5);
    g.add_vertex(6); // sommet isolé

    std::cout << g;

    // Requêtes
    std::cout << "has_edge(1,2) = " << g.has_edge(1, 2) << "\n"; // 1
    std::cout << "has_edge(2,1) = " << g.has_edge(2, 1) << "\n"; // 0 (dirigé)
    std::cout << "degree(3)     = " << g.degree(3)      << "\n"; // 2

    // Suppression d'une arête
    g.remove_edge(3, 5);
    std::cout << "\nAprès remove_edge(3,5) :\n" << g;

    sep("TEST : classe Graph (non-dirigé)");

    Graph h(false);
    h.add_edge(1, 2);
    h.add_edge(2, 3);
    h.add_edge(3, 1);
    std::cout << h;
    // En non-dirigé, has_edge(2,1) == has_edge(1,2)
    std::cout << "has_edge(2,1) = " << h.has_edge(2, 1) << "\n"; // 1
}

// ═══════════════════════════════════════════════════════════════════════════
// Tests de la classe SubGraph
// ═══════════════════════════════════════════════════════════════════════════
void test_subgraph()
{
    sep("TEST : classe SubGraph");

    Graph g(true);
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 4);
    g.add_edge(3, 4);
    g.add_edge(4, 5);

    std::cout << "Graphe parent :\n" << g;

    // Sous-graphe induit par {1, 2, 3, 4}
    std::set<int> subset = {1, 2, 3, 4};
    SubGraph sg(g, subset);
    std::cout << "\n" << sg;

    // Vérifie que l'arête (4,5) n'est pas dans le sous-graphe
    // (car 5 n'est pas dans le sous-ensemble)
    std::cout << "has_edge(4,5) dans SubGraph = " << sg.has_edge(4, 5) << "\n"; // 0
    std::cout << "has_edge(1,2) dans SubGraph = " << sg.has_edge(1, 2) << "\n"; // 1
}

// ═══════════════════════════════════════════════════════════════════════════
// Tests de la classe Tree
// ═══════════════════════════════════════════════════════════════════════════
void test_tree()
{
    sep("TEST : classe Tree");

    //        1
    //      / | \
    //     2  3  4
    //    / \    |
    //   5   6   7

    Tree t(1);          // racine = 1
    t.add_child(1, 2);
    t.add_child(1, 3);
    t.add_child(1, 4);
    t.add_child(2, 5);
    t.add_child(2, 6);
    t.add_child(4, 7);

    std::cout << t;

    // Propriétés
    std::cout << "num_nodes  = " << t.num_nodes() << "\n"; // 7
    std::cout << "num_edges  = " << t.num_edges() << "\n"; // 6
    std::cout << "depth(5)   = " << t.depth(5)   << "\n"; // 2
    std::cout << "depth(1)   = " << t.depth(1)   << "\n"; // 0
    std::cout << "parent(6)  = " << t.parent(6)  << "\n"; // 2
    std::cout << "is_root(1) = " << t.is_root(1) << "\n"; // 1
    std::cout << "is_root(3) = " << t.is_root(3) << "\n"; // 0

    std::cout << "\nEnfants de 2 : ";
    for (int c : t.children(2)) std::cout << c << " ";
    std::cout << "\n";
}

// ═══════════════════════════════════════════════════════════════════════════
// main
// ═══════════════════════════════════════════════════════════════════════════
int main()
{
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║     PP1 – Test des classes Graph, SubGraph, Tree         ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";

    test_graph();
    test_subgraph();
    test_tree();

    std::cout << "\n[PP1] Tous les tests terminés.\n";
    return 0;
}
