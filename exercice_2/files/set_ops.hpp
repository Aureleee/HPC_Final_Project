/**
 * set_ops.hpp
 * Opérations ensemblistes utilisées par ngraph.
 * Fournit intersection, union, inclusion et taille d'intersection
 * pour std::set<T>.
 */

#ifndef SET_OPS_HPP_
#define SET_OPS_HPP_

#include <set>
#include <algorithm>
#include <iterator>

// ─── intersection A ∩ B → résultat dans un nouvel ensemble ───────────────────
template <typename T>
std::set<T> operator*(const std::set<T> &A, const std::set<T> &B)
{
    std::set<T> result;
    std::set_intersection(A.begin(), A.end(),
                          B.begin(), B.end(),
                          std::inserter(result, result.begin()));
    return result;
}

// ─── union A ∪ B ─────────────────────────────────────────────────────────────
template <typename T>
std::set<T> operator+(const std::set<T> &A, const std::set<T> &B)
{
    std::set<T> result;
    std::set_union(A.begin(), A.end(),
                   B.begin(), B.end(),
                   std::inserter(result, result.begin()));
    return result;
}

// ─── différence A \ B ────────────────────────────────────────────────────────
template <typename T>
std::set<T> operator-(const std::set<T> &A, const std::set<T> &B)
{
    std::set<T> result;
    std::set_difference(A.begin(), A.end(),
                        B.begin(), B.end(),
                        std::inserter(result, result.begin()));
    return result;
}

// ─── test d'appartenance : est-ce que b ∈ A ? ────────────────────────────────
template <typename T>
bool includes_elm(const std::set<T> &A, const T &b)
{
    return A.find(b) != A.end();
}

// ─── taille de l'intersection |A ∩ B| sans créer un ensemble ─────────────────
template <typename T>
unsigned int intersection_size(const std::set<T> &A, const std::set<T> &B)
{
    unsigned int count = 0;
    typename std::set<T>::const_iterator a = A.begin(), b = B.begin();
    while (a != A.end() && b != B.end())
    {
        if (*a < *b)      ++a;
        else if (*b < *a) ++b;
        else { ++count; ++a; ++b; }
    }
    return count;
}

#endif // SET_OPS_HPP_
