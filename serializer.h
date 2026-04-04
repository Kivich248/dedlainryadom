#pragma once

#include "graph.h"
#include <string>
#include <vector>
#include <ostream>

using namespace std;

// Флаги для визуализации
struct VizOptions {
    bool show_components = false;       // Компоненты связности (кластеры)
    bool show_biconnected_vertices = false; // Вершинная двусвязность (кластеры)
    bool show_bridges = false;          // Подсветка мостов
    bool show_articulations = false;    // Подсветка точек сочленения
    bool show_random_spanning_tree = false; // Случайный остов
    bool show_random_cycle = false;     // Случайный цикл
};

class GraphSerializer {
public:
    virtual ~GraphSerializer();
    virtual void serialize(const Graph& graph, ostream& output, const VizOptions& options = VizOptions()) = 0;
    
    // Фабрика
    static GraphSerializer* sozdat_serializer(const string& format);
};

// ============================================================================
// GRAPHVIZ SERIALIZER (.dot)
// ============================================================================
class GraphVizSerializer : public GraphSerializer {
public:
    void serialize(const Graph& graph, ostream& output, const VizOptions& options = VizOptions()) override;

private:
    // Вспомогательные методы для генерации структур
    vector<vector<size_t>> get_components(const Graph& g);
    vector<vector<size_t>> get_biconnected_components(const Graph& g); // Упрощенно: через точки сочленения
    vector<pair<size_t, size_t>> get_random_spanning_tree(const Graph& g);
    vector<size_t> get_random_cycle(const Graph& g);
    
    string escape_string(const string& s);
};

// ============================================================================
// EDGES SERIALIZER (.edges)
// Формат Pro g r@m4You
// ============================================================================
class EdgesSerializer : public GraphSerializer {
public:
    void serialize(const Graph& graph, ostream& output, const VizOptions& options = VizOptions()) override;
    // Для .edges опции визуализации не нужны, но интерфейс единый
};