#pragma once

#include "Graph.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>

using namespace std;


// АБСТРАКТНЫЙ КЛАСС ДЛЯ МЕТРИКИ

class GraphMetric {
public:
    virtual ~GraphMetric();
    virtual void compute(const Graph& graph) = 0;
    virtual string get_result() const = 0;
    virtual bool is_computed() const = 0;
    virtual void clear_cache() = 0;

    // Фабрика: создаёт метрику по имени
    static GraphMetric* sozdat_metric(const string& name);

    // Список всех доступных метрик
    static vector<string> get_available_metrics();
};


// МЕТРИКА 1: ПЛОТНОСТЬ ГРАФА

class Metric_Plotnost : public GraphMetric {
private:
    double value;
    bool computed;
public:
    Metric_Plotnost();
    void compute(const Graph& graph) override;
    string get_result() const override;
    bool is_computed() const override;
    void clear_cache() override;
    double get_value() const;
};


// МЕТРИКА 2: ДИАМЕТР ГРАФА

class Metric_Diametr : public GraphMetric {
private:
    size_t value;
    bool computed;
public:
    Metric_Diametr();
    void compute(const Graph& graph) override;
    string get_result() const override;
    bool is_computed() const override;
    void clear_cache() override;
    size_t get_value() const;
private:
    size_t bfs_max_distance(const Graph& graph, size_t start) const;
};


// МЕТРИКА 3: ТРАНЗИТИВНОСТЬ ГРАФА

class Metric_Transitivnost : public GraphMetric {
private:
    double value;
    bool computed;
public:
    Metric_Transitivnost();
    void compute(const Graph& graph) override;
    string get_result() const override;
    bool is_computed() const override;
    void clear_cache() override;
    double get_value() const;
};


// МЕТРИКА 4: КОЛИЧЕСТВО КОМПОНЕНТ СВЯЗНОСТИ

class Metric_Komponenty_Svyaznosti : public GraphMetric {
private:
    size_t value;
    vector<int> component_ids;
    bool computed;
public:
    Metric_Komponenty_Svyaznosti();
    void compute(const Graph& graph) override;
    string get_result() const override;
    bool is_computed() const override;
    void clear_cache() override;
    size_t get_count() const;
    const vector<int>& get_component_ids() const;
};


// МЕТРИКА 5: КОЛИЧЕСТВО ТОЧЕК СОЧЛЕНЕНИЯ

class Metric_Tochki_Sochleneniya : public GraphMetric {
private:
    size_t value;
    vector<bool> is_articulation;
    bool computed;
public:
    Metric_Tochki_Sochleneniya();
    void compute(const Graph& graph) override;
    string get_result() const override;
    bool is_computed() const override;
    void clear_cache() override;
    size_t get_count() const;
    const vector<bool>& get_articulation_flags() const;
private:
    void dfs_articulation(const Graph& graph, size_t v, size_t parent,
                         vector<int>& disc, vector<int>& low,
                         vector<bool>& is_art, int& time, size_t& root_children);
};


// МЕТРИКА 6: КОЛИЧЕСТВО МОСТОВ

class Metric_Mosty : public GraphMetric {
private:
    size_t value;
    vector<pair<size_t, size_t>> bridge_list;
    bool computed;
public:
    Metric_Mosty();
    void compute(const Graph& graph) override;
    string get_result() const override;
    bool is_computed() const override;
    void clear_cache() override;
    size_t get_count() const;
    const vector<pair<size_t, size_t>>& get_bridge_list() const;
private:
    void dfs_bridges(const Graph& graph, size_t v, size_t parent,
                    vector<int>& disc, vector<int>& low,
                    vector<pair<size_t, size_t>>& bridges, int& time);
};


// МЕТРИКА 7: ПРОВЕРКА НА ДВУДОЛЬНОСТЬ

class Metric_Dvudolnost : public GraphMetric {
private:
    bool is_bipartite;
    vector<int> coloring;
    bool computed;
public:
    Metric_Dvudolnost();
    void compute(const Graph& graph) override;
    string get_result() const override;
    bool is_computed() const override;
    void clear_cache() override;
    bool get_is_bipartite() const;
    const vector<int>& get_coloring() const;
};


// МЕТРИКА 8: ХРОМАТИЧЕСКОЕ ЧИСЛО (ВЕРХНЯЯ ОЦЕНКА)

class Metric_Hromaticheskoe_Chislo : public GraphMetric {
private:
    size_t upper_bound;
    vector<int> coloring;
    bool computed;
public:
    Metric_Hromaticheskoe_Chislo();
    void compute(const Graph& graph) override;
    string get_result() const override;
    bool is_computed() const override;
    void clear_cache() override;
    size_t get_upper_bound() const;
    const vector<int>& get_coloring() const;
};