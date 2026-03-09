/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_vector.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurele <aurele@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 18:21:58 by aurele            #+#    #+#             */
/*   Updated: 2026/03/09 18:22:16 by aurele           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * matrix_vector.hpp
 * ─────────────────────────────────────────────────────────────────────────────
 * Implémentation de :
 *   • Point   – point dans ℝⁿ (base)
 *   • Vector  – vecteur défini comme différence de deux Point
 *   • Matrix  – matrice définie comme collection de Vector (colonnes)
 *
 * Opérateurs surchargés : +, =, << (et quelques autres utiles).
 * ─────────────────────────────────────────────────────────────────────────────
 */

#ifndef MATRIX_VECTOR_HPP_
#define MATRIX_VECTOR_HPP_

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <iomanip>

// ═══════════════════════════════════════════════════════════════════════════
// Classe Point – coordonnées dans ℝⁿ
// ═══════════════════════════════════════════════════════════════════════════
class Point
{
protected:
    std::vector<double> coords_; ///< coordonnées du point

public:
    // ── Constructeurs ────────────────────────────────────────────────────────
    explicit Point(int dim, double val = 0.0)
        : coords_(dim, val) {}

    Point(std::initializer_list<double> il)
        : coords_(il) {}

    Point(const Point &)            = default;
    Point &operator=(const Point &) = default;

    // ── Accesseurs ───────────────────────────────────────────────────────────
    int    dim()            const { return static_cast<int>(coords_.size()); }
    double operator[](int i) const { return coords_.at(i); }
    double &operator[](int i)      { return coords_.at(i); }

    // ── Distance euclidienne entre deux points ────────────────────────────────
    double distance(const Point &other) const
    {
        if (dim() != other.dim())
            throw std::invalid_argument("Dimensions incompatibles.");
        double s = 0.0;
        for (int i = 0; i < dim(); ++i)
        {
            double d = coords_[i] - other.coords_[i];
            s += d * d;
        }
        return std::sqrt(s);
    }

    // ── Opérateur d'affichage ─────────────────────────────────────────────────
    friend std::ostream &operator<<(std::ostream &os, const Point &p)
    {
        os << "(";
        for (int i = 0; i < p.dim(); ++i)
        {
            os << p.coords_[i];
            if (i + 1 < p.dim()) os << ", ";
        }
        os << ")";
        return os;
    }
};


// ═══════════════════════════════════════════════════════════════════════════
// Classe Vector – vecteur de ℝⁿ exprimé en termes de Point
//
// Un vecteur est la différence de deux points : v = B - A
// Il hérite de Point pour réutiliser les coordonnées.
// ═══════════════════════════════════════════════════════════════════════════
class Vector : public Point
{
public:
    // ── Constructeurs ────────────────────────────────────────────────────────

    /// Vecteur nul de dimension dim
    explicit Vector(int dim, double val = 0.0) : Point(dim, val) {}

    /// Depuis une liste d'initialiseurs
    Vector(std::initializer_list<double> il) : Point(il) {}

    /// Copie depuis Point
    explicit Vector(const Point &p) : Point(p) {}

    /// Construit le vecteur AB = B - A
    Vector(const Point &A, const Point &B) : Point(A.dim())
    {
        if (A.dim() != B.dim())
            throw std::invalid_argument("Dimensions incompatibles pour B - A.");
        for (int i = 0; i < A.dim(); ++i)
            coords_[i] = B[i] - A[i];
    }

    // ── Opérateur d'affectation ───────────────────────────────────────────────
    Vector &operator=(const Vector &other)
    {
        if (this != &other) coords_ = other.coords_;
        return *this;
    }

    // ── Addition de vecteurs ──────────────────────────────────────────────────
    Vector operator+(const Vector &other) const
    {
        if (dim() != other.dim())
            throw std::invalid_argument("Dimensions incompatibles pour +.");
        Vector result(dim());
        for (int i = 0; i < dim(); ++i)
            result.coords_[i] = coords_[i] + other.coords_[i];
        return result;
    }

    Vector &operator+=(const Vector &other)
    {
        *this = *this + other;
        return *this;
    }

    // ── Soustraction ─────────────────────────────────────────────────────────
    Vector operator-(const Vector &other) const
    {
        if (dim() != other.dim())
            throw std::invalid_argument("Dimensions incompatibles pour -.");
        Vector result(dim());
        for (int i = 0; i < dim(); ++i)
            result.coords_[i] = coords_[i] - other.coords_[i];
        return result;
    }

    // ── Multiplication par un scalaire ────────────────────────────────────────
    Vector operator*(double s) const
    {
        Vector result(dim());
        for (int i = 0; i < dim(); ++i)
            result.coords_[i] = coords_[i] * s;
        return result;
    }

    friend Vector operator*(double s, const Vector &v) { return v * s; }

    // ── Produit scalaire ──────────────────────────────────────────────────────
    double dot(const Vector &other) const
    {
        if (dim() != other.dim())
            throw std::invalid_argument("Dimensions incompatibles pour produit scalaire.");
        double s = 0.0;
        for (int i = 0; i < dim(); ++i)
            s += coords_[i] * other.coords_[i];
        return s;
    }

    // ── Norme euclidienne ─────────────────────────────────────────────────────
    double norm() const { return std::sqrt(dot(*this)); }

    // ── Affichage ─────────────────────────────────────────────────────────────
    friend std::ostream &operator<<(std::ostream &os, const Vector &v)
    {
        os << "[";
        for (int i = 0; i < v.dim(); ++i)
        {
            os << v.coords_[i];
            if (i + 1 < v.dim()) os << ", ";
        }
        os << "]";
        return os;
    }
};


// ═══════════════════════════════════════════════════════════════════════════
// Classe Matrix – matrice m×n exprimée en termes de Vector (colonnes)
//
// Représentation : liste de n vecteurs colonnes de dimension m.
// ═══════════════════════════════════════════════════════════════════════════
class Matrix
{
private:
    int                 rows_; ///< nombre de lignes (m)
    int                 cols_; ///< nombre de colonnes (n)
    std::vector<Vector> cols_data_; ///< colonnes, chacune est un Vector de ℝᵐ

public:
    // ── Constructeurs ────────────────────────────────────────────────────────

    /// Matrice nulle m×n
    Matrix(int rows, int cols)
        : rows_(rows), cols_(cols),
          cols_data_(cols, Vector(rows, 0.0)) {}

    /// Depuis une liste de vecteurs colonnes
    Matrix(std::initializer_list<Vector> columns)
        : cols_data_(columns)
    {
        cols_ = static_cast<int>(cols_data_.size());
        rows_ = (cols_ > 0) ? cols_data_[0].dim() : 0;
        for (auto &c : cols_data_)
            if (c.dim() != rows_)
                throw std::invalid_argument("Les colonnes doivent avoir la même dimension.");
    }

    // ── Accesseurs ───────────────────────────────────────────────────────────
    int rows() const { return rows_; }
    int cols() const { return cols_; }

    double operator()(int r, int c) const { return cols_data_.at(c)[r]; }
    double &operator()(int r, int c)      { return cols_data_.at(c)[r]; }

    const Vector &col(int c) const { return cols_data_.at(c); }
          Vector &col(int c)       { return cols_data_.at(c); }

    // ── Opérateur d'affectation ───────────────────────────────────────────────
    Matrix &operator=(const Matrix &other)
    {
        if (this != &other)
        {
            rows_      = other.rows_;
            cols_      = other.cols_;
            cols_data_ = other.cols_data_;
        }
        return *this;
    }

    // ── Addition de matrices ──────────────────────────────────────────────────
    Matrix operator+(const Matrix &B) const
    {
        if (rows_ != B.rows_ || cols_ != B.cols_)
            throw std::invalid_argument("Dimensions incompatibles pour l'addition.");
        Matrix result(rows_, cols_);
        for (int c = 0; c < cols_; ++c)
            result.cols_data_[c] = cols_data_[c] + B.cols_data_[c];
        return result;
    }

    Matrix &operator+=(const Matrix &B)
    {
        *this = *this + B;
        return *this;
    }

    // ── Multiplication matricielle ────────────────────────────────────────────
    Matrix operator*(const Matrix &B) const
    {
        if (cols_ != B.rows_)
            throw std::invalid_argument(
                "Dimensions incompatibles pour la multiplication (A.cols != B.rows).");
        Matrix result(rows_, B.cols_);
        for (int r = 0; r < rows_; ++r)
            for (int c = 0; c < B.cols_; ++c)
            {
                double s = 0.0;
                for (int k = 0; k < cols_; ++k)
                    s += (*this)(r, k) * B(k, c);
                result(r, c) = s;
            }
        return result;
    }

    // ── Produit Matrice × Vecteur ─────────────────────────────────────────────
    Vector operator*(const Vector &v) const
    {
        if (cols_ != v.dim())
            throw std::invalid_argument("Dimensions incompatibles pour M*v.");
        Vector result(rows_, 0.0);
        for (int r = 0; r < rows_; ++r)
            for (int c = 0; c < cols_; ++c)
                result[r] += (*this)(r, c) * v[c];
        return result;
    }

    // ── Transposée ────────────────────────────────────────────────────────────
    Matrix transpose() const
    {
        Matrix T(cols_, rows_);
        for (int r = 0; r < rows_; ++r)
            for (int c = 0; c < cols_; ++c)
                T(c, r) = (*this)(r, c);
        return T;
    }

    // ── Affichage ─────────────────────────────────────────────────────────────
    friend std::ostream &operator<<(std::ostream &os, const Matrix &M)
    {
        os << "Matrix " << M.rows_ << "×" << M.cols_ << " :\n";
        for (int r = 0; r < M.rows_; ++r)
        {
            os << "  [ ";
            for (int c = 0; c < M.cols_; ++c)
                os << std::setw(8) << std::fixed << std::setprecision(2)
                   << M(r, c) << " ";
            os << "]\n";
        }
        return os;
    }
};

#endif // MATRIX_VECTOR_HPP_
