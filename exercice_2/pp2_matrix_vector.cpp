/**
 * pp2_matrix_vector.cpp
 * ─────────────────────────────────────────────────────────────────────────────
 * Programme principal 2 : test des classes Point, Vector, Matrix
 * avec surcharge des opérateurs +, =, <<.
 * ─────────────────────────────────────────────────────────────────────────────
 */

#include <iostream>
#include <cmath>
#include "matrix_vector.hpp"

static void sep(const std::string &title)
{
    std::cout << "\n" << std::string(60, '-') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(60, '-') << "\n";
}

// ═══════════════════════════════════════════════════════════════════════════
// Tests de la classe Point
// ═══════════════════════════════════════════════════════════════════════════
void test_point()
{
    sep("TEST : classe Point");

    Point A({1.0, 2.0, 3.0});
    Point B({4.0, 6.0, 3.0});

    std::cout << "A = " << A << "\n";
    std::cout << "B = " << B << "\n";
    std::cout << "Distance(A, B) = " << A.distance(B) << "\n"; // √(9+16+0)=5
}

// ═══════════════════════════════════════════════════════════════════════════
// Tests de la classe Vector
// ═══════════════════════════════════════════════════════════════════════════
void test_vector()
{
    sep("TEST : classe Vector – opérateur =");

    Vector v1({1.0, 2.0, 3.0});
    Vector v2({4.0, 5.0, 6.0});

    // Test opérateur =
    Vector v3 = v1;
    std::cout << "v1 = " << v1 << "\n";
    std::cout << "v2 = " << v2 << "\n";
    std::cout << "v3 = v1 = " << v3 << "\n";

    sep("TEST : classe Vector – opérateur +");

    // Test opérateur +
    Vector v4 = v1 + v2;
    std::cout << "v1 + v2 = " << v4 << "\n"; // [5, 7, 9]

    sep("TEST : classe Vector – opérateur <<");
    std::cout << "Affichage de v2 via << : " << v2 << "\n";

    sep("TEST : classe Vector – construction depuis deux Point");

    Point A({0.0, 0.0});
    Point B({3.0, 4.0});
    Vector AB(A, B);
    std::cout << "A = " << A << "  B = " << B << "\n";
    std::cout << "AB = B - A = " << AB << "\n";   // [3, 4]
    std::cout << "||AB|| = " << AB.norm() << "\n"; // 5

    sep("TEST : produit scalaire et scalaire × vecteur");

    Vector u({1.0, 0.0, 0.0});
    Vector v({0.0, 1.0, 0.0});
    std::cout << "u . v = " << u.dot(v) << "\n"; // 0 (orthogonaux)

    Vector w = 3.0 * u;
    std::cout << "3 * u = " << w << "\n"; // [3, 0, 0]
}

// ═══════════════════════════════════════════════════════════════════════════
// Tests de la classe Matrix
// ═══════════════════════════════════════════════════════════════════════════
void test_matrix()
{
    sep("TEST : classe Matrix – opérateur <<");

    // Matrice 3×2 :  col0=[1,3,5]  col1=[2,4,6]
    Matrix M({
        Vector({1.0, 3.0, 5.0}),
        Vector({2.0, 4.0, 6.0})
    });
    std::cout << M;

    sep("TEST : classe Matrix – opérateur =");

    Matrix N = M; // copie via opérateur =
    std::cout << "N = M :\n" << N;

    sep("TEST : classe Matrix – opérateur +");

    Matrix P = M + N;
    std::cout << "M + N :\n" << P;

    sep("TEST : classe Matrix – multiplication M × Mᵀ");

    Matrix Mt = M.transpose();
    std::cout << "Mᵀ :\n" << Mt;

    // M est 3×2, Mᵀ est 2×3 → M*Mᵀ est 3×3
    Matrix MMt = M * Mt;
    std::cout << "M × Mᵀ :\n" << MMt;

    sep("TEST : classe Matrix – produit M × vecteur");

    // Matrice 2×2 d'identité
    Matrix I({
        Vector({1.0, 0.0}),
        Vector({0.0, 1.0})
    });
    Vector x({7.0, 3.0});
    Vector Ix = I * x;
    std::cout << "I × [7, 3] = " << Ix << "\n"; // [7, 3]
}

// ═══════════════════════════════════════════════════════════════════════════
// main
// ═══════════════════════════════════════════════════════════════════════════
int main()
{
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║   PP2 – Test des classes Point, Vector, Matrix           ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";

    test_point();
    test_vector();
    test_matrix();

    std::cout << "\n[PP2] Tous les tests terminés.\n";
    return 0;
}
