#include "catch_amalgamated.hpp"
#include "generator.h"
#include "graph.h"
#include "graphmetrics.h"

using namespace std;

// Вспомогательная функция подсчета мостов (через удаление ребра)
size_t count_bridges_naive(const Graph& g) {
    size_t count = 0;
    auto edges = g.get_vse_rebra();
    for (const auto& e : edges) {
        Graph temp = g.kopiya();
        // Временное удаление ребра (костыль, но для теста сойдет)
        // В реальной лабе лучше использовать алгоритм поиска мостов
        // Тут мы просто проверяем что генератор создал нужное кол-во через метрику
    }
    return 0; 
}

TEST_CASE("Generator: Components Count", "[generator][invariant]") {
    size_t n = 20;
    size_t k = 4;
    Graph g = generate_graph_with_components(n, k);
    
    CHECK(g.count_vershiny() == n);
    
    auto comps = g.get_components();
    // Инвариант: Количество компонент равно заданному k
    CHECK(comps.size() == k);
    
    // Инвариант: Сумма размеров компонент равна n
    size_t sum_size = 0;
    for (const auto& c : comps) sum_size += c.size();
    CHECK(sum_size == n);
}

TEST_CASE("Generator: Bridges Count", "[generator][invariant]") {
    size_t n = 15;
    size_t m = 5;
    Graph g = generate_graph_with_bridges_path_blobs_random(n, m);
    
    CHECK(g.count_vershiny() == n);
    
    // Используем метрику для проверки количества мостов
    Metric_Mosty metric;
    metric.compute(g);
    
    // Инвариант: Количество мостов равно заданному m
    CHECK(metric.get_count() == m);
}

TEST_CASE("Generator: Articulation Points Count", "[generator][invariant]") {
    size_t n = 20;
    size_t k = 3;
    Graph g = generate_graph_with_articulations(n, k);

    CHECK(g.count_vershiny() == n);

    Metric_Tochki_Sochleneniya metric;
    metric.compute(g);

    // Инвариант: Количество точек сочленения равно заданному k
    // Добавим погрешность, так как распределение может быть неидеальным
    size_t actual = metric.get_count();
    INFO("Expected: " << k << ", Actual: " << actual);
    CHECK(actual == k);
}

TEST_CASE("Generator: Tree Properties", "[generator][invariant]") {
    size_t n = 50;
    Graph g = generate_tree(n);
    
    // Инвариант дерева: |E| = |V| - 1
    CHECK(g.count_rebra() == n - 1);
    
    // Инвариант дерева: связный (1 компонента)
    Metric_Komponenty_Svyaznosti comp_metric;
    comp_metric.compute(g);
    CHECK(comp_metric.get_count() == 1);
    
    // Инвариант дерева: нет циклов (транзитивность ~ 0, мостов n-1)
    Metric_Mosty bridge_metric;
    bridge_metric.compute(g);
    CHECK(bridge_metric.get_count() == n - 1);
}

TEST_CASE("Generator: Random Cubic Degree", "[generator][invariant]") {
    size_t n = 20; // Четное
    Graph g = generate_random_cubic(n);
    
    // Инвариант кубического графа: степень каждой вершины ровно 3
    for (size_t v = 0; v < n; v++) {
        CHECK(g.stepen(v) == 3);
    }
}