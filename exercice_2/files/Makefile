# ═══════════════════════════════════════════════════════════════════════════
# Makefile – Compilation de tous les programmes du projet
#
# Structure :
#   include/   → headers (.h, .hpp) et implémentation des templates (.cpp)
#   tests/     → programmes principaux (pp1..pp4)
#
# Usage :
#   make          → compile tous les exécutables
#   make pp1      → compile uniquement pp1
#   make clean    → supprime les exécutables et fichiers objets
#   make run      → compile et exécute tous les programmes
# ═══════════════════════════════════════════════════════════════════════════

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Iinclude

# Répertoires
SRCDIR  := tests
INCDIR  := include
BINDIR  := bin

# Cibles exécutables
TARGETS := $(BINDIR)/pp1 $(BINDIR)/pp2 $(BINDIR)/pp3 $(BINDIR)/pp4

# ─── Cible par défaut ─────────────────────────────────────────────────────────
.PHONY: all clean run pp1 pp2 pp3 pp4

all: $(BINDIR) $(TARGETS)
	@echo ""
	@echo "✓ Compilation terminée. Exécutables dans $(BINDIR)/"

# ─── Création du répertoire bin ───────────────────────────────────────────────
$(BINDIR):
	mkdir -p $(BINDIR)

# ─── PP1 : Graph, SubGraph, Tree ─────────────────────────────────────────────
$(BINDIR)/pp1: $(SRCDIR)/pp1_graph_tree.cpp $(INCDIR)/graph_classes.hpp
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "✓ pp1 compilé"

# ─── PP2 : Point, Vector, Matrix ─────────────────────────────────────────────
$(BINDIR)/pp2: $(SRCDIR)/pp2_matrix_vector.cpp $(INCDIR)/matrix_vector.hpp
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "✓ pp2 compilé"

# ─── PP3 : NGraph::tGraph<T> ─────────────────────────────────────────────────
# ngraph.cpp est inclus via ngraph.h (template) → pas de compilation séparée
$(BINDIR)/pp3: $(SRCDIR)/pp3_ngraph.cpp \
               $(INCDIR)/ngraph.h $(INCDIR)/ngraph.cpp $(INCDIR)/set_ops.hpp
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "✓ pp3 compilé"

# ─── PP4 : Dijkstra ──────────────────────────────────────────────────────────
$(BINDIR)/pp4: $(SRCDIR)/pp4_dijkstra.cpp \
               $(INCDIR)/dijkstra.hpp \
               $(INCDIR)/ngraph.h $(INCDIR)/ngraph.cpp $(INCDIR)/set_ops.hpp
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "✓ pp4 compilé"

# ─── Cibles individuelles ─────────────────────────────────────────────────────
pp1: $(BINDIR) $(BINDIR)/pp1
pp2: $(BINDIR) $(BINDIR)/pp2
pp3: $(BINDIR) $(BINDIR)/pp3
pp4: $(BINDIR) $(BINDIR)/pp4

# ─── Exécution de tous les programmes ────────────────────────────────────────
run: all
	@echo ""
	@echo "══════════════════════════════════════════════════════════"
	@echo "  Exécution PP1 : Graph / SubGraph / Tree"
	@echo "══════════════════════════════════════════════════════════"
	./$(BINDIR)/pp1
	@echo ""
	@echo "══════════════════════════════════════════════════════════"
	@echo "  Exécution PP2 : Point / Vector / Matrix"
	@echo "══════════════════════════════════════════════════════════"
	./$(BINDIR)/pp2
	@echo ""
	@echo "══════════════════════════════════════════════════════════"
	@echo "  Exécution PP3 : NGraph::tGraph<T>"
	@echo "══════════════════════════════════════════════════════════"
	./$(BINDIR)/pp3
	@echo ""
	@echo "══════════════════════════════════════════════════════════"
	@echo "  Exécution PP4 : Algorithme de Dijkstra"
	@echo "══════════════════════════════════════════════════════════"
	./$(BINDIR)/pp4

# ─── Nettoyage ────────────────────────────────────────────────────────────────
clean:
	rm -rf $(BINDIR)
	@echo "✓ Nettoyage terminé"
