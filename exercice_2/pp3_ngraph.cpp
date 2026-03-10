/**
 * pp3_ngraph.cpp
 * ─────────────────────────────────────────────────────────────────────────────
 * Programme principal 3 : test de NGraph::tGraph<T>
 *   – création, sous-graphe, opérations entre graphes, méthodes diverses.
 * ─────────────────────────────────────────────────────────────────────────────
 */

#include <iostream>
#include <sstream>
#include <set>
#include "ngraph.h"

using namespace NGraph;

static void sep(const std::string &title)
{
    std::cout << "\n" << std::string(62, '─') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(62, '─') << "\n";
}

// ═══════════════════════════════════════════════════════════════════════════
// Création et affichage
// ═══════════════════════════════════════════════════════════════════════════
void test_creation()
{
    sep("Création d'un graphe orienté d'entiers");

    Graph g;
    // Graphe :  1→2  1→3  2→4  3→4  4→5  2→5
    g.insert_edge(1, 2);
    g.insert_edge(1, 3);
    g.insert_edge(2, 4);
    g.insert_edge(3, 4);
    g.insert_edge(4, 5);
    g.insert_edge(2, 5);
    g.insert_vertex(6); // sommet isolé

    std::cout << "Graphe G :\n" << g;
    std::cout << "Sommets : " << g.num_vertices()
              << "  Arêtes : " << g.num_edges() << "\n";
}

// ═══════════════════════════════════════════════════════════════════════════
// Requêtes sur les sommets et arêtes
// ═══════════════════════════════════════════════════════════════════════════
void test_queries()
{
    sep("Requêtes sur le graphe");

    Graph g;
    g.insert_edge(1, 2); g.insert_edge(1, 3);
    g.insert_edge(2, 4); g.insert_edge(3, 4);

    std::cout << "includes_vertex(3)   = " << g.includes_vertex(3)   << "\n"; // 1
    std::cout << "includes_vertex(99)  = " << g.includes_vertex(99)  << "\n"; // 0
    std::cout << "includes_edge(1,2)   = " << g.includes_edge(1,2)   << "\n"; // 1
    std::cout << "includes_edge(2,1)   = " << g.includes_edge(2,1)   << "\n"; // 0
    std::cout << "out_degree(1)        = " << g.out_degree(1)        << "\n"; // 2
    std::cout << "in_degree(4)         = " << g.in_degree(4)         << "\n"; // 2
    std::cout << "isolated(99)         = " << g.isolated(99)         << "\n"; // 0
}

// ═══════════════════════════════════════════════════════════════════════════
// Sous-graphe
// ═══════════════════════════════════════════════════════════════════════════
void test_subgraph()
{
    sep("Sous-graphe induit par un ensemble de sommets");

    Graph g;
    g.insert_edge(1, 2); g.insert_edge(1, 3);
    g.insert_edge(2, 4); g.insert_edge(3, 4);
    g.insert_edge(4, 5);

    std::cout << "Graphe G :\n" << g;

    // Sous-graphe sur {1,2,3,4}
    Graph::vertex_set A = {1u, 2u, 3u, 4u};
    Graph sg = g.subgraph(A);
    std::cout << "\nSous-graphe sur {1,2,3,4} :\n" << sg;
    std::cout << "  Sommets: " << sg.num_vertices()
              << "  Arêtes: "  << sg.num_edges() << "\n";
    // L'arête 4→5 disparaît car 5 ∉ A
}

// ═══════════════════════════════════════════════════════════════════════════
// Opérations entre graphes : union (+), intersection (*), différence (-)
// ═══════════════════════════════════════════════════════════════════════════
void test_operations()
{
    sep("Opérations entre graphes : G1 + G2, G1 * G2, G1 - G2");

    Graph G1, G2;
    // G1 : 1→2, 1→3, 2→4
    G1.insert_edge(1, 2); G1.insert_edge(1, 3); G1.insert_edge(2, 4);
    // G2 : 1→2, 3→4, 4→5
    G2.insert_edge(1, 2); G2.insert_edge(3, 4); G2.insert_edge(4, 5);

    std::cout << "G1 :\n" << G1;
    std::cout << "G2 :\n" << G2;

    Graph U = G1 + G2;
    std::cout << "\nG1 + G2 (union) :\n" << U;
    std::cout << "Arêtes : " << U.num_edges() << "\n"; // 5 arêtes uniques

    Graph I = G1 * G2;
    std::cout << "\nG1 * G2 (intersection) :\n" << I;
    std::cout << "Arêtes : " << I.num_edges() << "\n"; // 1 arête commune : 1→2

    Graph D = G1 - G2;
    std::cout << "\nG1 - G2 (différence) :\n" << D;
    std::cout << "Arêtes : " << D.num_edges() << "\n"; // arêtes de G1 absentes de G2
}

// ═══════════════════════════════════════════════════════════════════════════
// Suppression de sommets et d'arêtes
// ═══════════════════════════════════════════════════════════════════════════
void test_removal()
{
    sep("Suppression de sommets et d'arêtes");

    Graph g;
    g.insert_edge(1, 2); g.insert_edge(1, 3);
    g.insert_edge(2, 4); g.insert_edge(3, 4);
    std::cout << "Avant suppression :\n" << g;

    g.remove_edge(1, 3);
    std::cout << "Après remove_edge(1,3) :\n" << g;

    g.remove_vertex(2);
    std::cout << "Après remove_vertex(2) :\n" << g;
}

// ═══════════════════════════════════════════════════════════════════════════
// Lecture depuis un flux texte (opérateur >>)
// ═══════════════════════════════════════════════════════════════════════════
void test_stream_io()
{
    sep("Lecture depuis un flux texte (opérateur >>)");

    // Format : chaque ligne « u v » représente une arête u→v
    //          une ligne « u » seule représente un sommet isolé
    std::string data =
        "# Ceci est un commentaire\n"
        "10 20\n"
        "10 30\n"
        "20 40\n"
        "30\n";         // sommet isolé 30 (dans ce cas sans arêtes supplémentaires)

    std::istringstream iss(data);
    Graph g(iss);
    std::cout << "Graphe lu depuis flux :\n" << g;
}

// ═══════════════════════════════════════════════════════════════════════════
// Test avec sGraph (sommets = std::string)
// ═══════════════════════════════════════════════════════════════════════════
void test_string_graph()
{
    sep("Graphe avec sommets de type std::string (sGraph)");

    sGraph sg;
    sg.insert_edge("Paris",   "Lyon");
    sg.insert_edge("Paris",   "Bordeaux");
    sg.insert_edge("Lyon",    "Marseille");
    sg.insert_edge("Bordeaux","Toulouse");
    sg.insert_vertex("Strasbourg"); // ville isolée

    std::cout << sg;
    std::cout << "Voisins de Paris : { ";
    for (const std::string &v : sg.out_neighbors("Paris"))
        std::cout << v << " ";
    std::cout << "}\n";
}

// ═══════════════════════════════════════════════════════════════════════════
// main
// ═══════════════════════════════════════════════════════════════════════════
int main()
{
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║     PP3 – Test de NGraph::tGraph<T>                          ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";

    test_creation();
    test_queries();
    test_subgraph();
    test_operations();
    test_removal();
    test_stream_io();
    test_string_graph();

    std::cout << "\n[PP3] Tous les tests terminés.\n";
    return 0;
}
