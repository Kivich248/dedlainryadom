//потестила работу графа
#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <cassert>

// ============================================================================
// СТРУКТУРА ВЕРШИНЫ
// ============================================================================
struct Vershina {
    std::string color = "black";     // цвет для GraphViz
    std::string label = "";          // метка вершины (можно отображать)
    bool isArticulation = false;     // флаг: точка сочленения?
    int component = -1;              // номер компоненты связности
    int biconnectedComponent = -1;   // номер компоненты вершинной двусвязности

    Vershina() = default;
    explicit Vershina(const std::string& col) : color(col) {}
};

// ============================================================================
// СТРУКТУРА РЕБРА
// ============================================================================
struct Rebro {
    size_t from;                     // откуда
    size_t to;                       // куда
    std::string color = "black";     // цвет для GraphViz
    bool isBridge = false;           // флаг: мост?
    int edgeBiconnectedComponent = -1; // компонента рёберной двусвязности

    Rebro(size_t u, size_t v) : from(u), to(v) {}
    Rebro(size_t u, size_t v, const std::string& col) : from(u), to(v), color(col) {}

    // Получить другой конец ребра
    size_t other(size_t v) const {
        return (v == from) ? to : from;
    }

    // Проверить, инцидентно ли ребро вершине v
    bool incidentTo(size_t v) const {
        return v == from || v == to;
    }
};

// ============================================================================
// КЛАСС ГРАФА
// ============================================================================
class Graph {
private:
    // ===== ВНУТРЕННЕЕ ПРЕДСТАВЛЕНИЕ =====

    // edgeIds_[v] — список ИНДЕКСОВ рёбер, инцидентных вершине v
    // Храним индексы, а не вершины, чтобы быстро получать атрибуты ребра
    std::vector<std::vector<size_t>> inchedent_rebra;

    // Данные о вершинах (индекс в векторе = номер вершины)
    std::vector<Vershina> vershini;

    // Данные о рёбрах (индекс в векторе = номер ребра)
    std::vector<Rebro> rebro;

    // Матрица для быстрого поиска ребра: edgeIndex_[u][v] = индекс ребра или -1
    // Позволяет проверять существование ребра за O(1)
    std::vector<std::vector<int>> rebra_index;

    // ===== ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ =====

    // Обновить матрицу edgeIndex_ после добавления вершины
    void expand_rebra_index() {
        size_t newSize = vershini.size();

        // Расширяем существующие строки
        for (auto& row : rebra_index) {
            row.resize(newSize, -1);
        }

        // Добавляем новую строку для новой вершины
        rebra_index.resize(newSize);
        rebra_index[newSize - 1].resize(newSize, -1);
    }

public:
    // ==================== КОНСТРУКТОРЫ ====================

    // Пустой граф
    Graph() = default;

    // Граф с n вершинами (без рёбер)
    explicit Graph(size_t n) {
        vershini.resize(n);
        inchedent_rebra.resize(n);
        rebra_index.resize(n);
        for (auto& row : rebra_index) {
            row.resize(n, -1);
        }
    }

    // Копирование
    Graph(const Graph& other)
        : inchedent_rebra(other.inchedent_rebra),
          vershini(other.vershini),
          rebro(other.rebro),
          rebra_index(other.rebra_index) {}

    // Присваивание
    Graph& operator=(const Graph& other) {
        if (this != &other) {
            inchedent_rebra = other.inchedent_rebra;
            vershini = other.vershini;
            rebro = other.rebro;
            rebra_index = other.rebra_index;
        }
        return *this;
    }

    // ==================== РАБОТА С ВЕРШИНАМИ ====================

    // Добавить одну вершину
    void add_Vershina() {
        vershini.emplace_back();   // добавляем структуру Vershina
        inchedent_rebra.emplace_back();    // добавляем пустой список рёбер
        expand_rebra_index();          // обновляем матрицу поиска
    }

    // Добавить несколько вершин
    void add_Vershina_mn(size_t count) {
        for (size_t i = 0; i < count; i++) {
            add_Vershina();
        }
    }

    // Проверить, существует ли вершина
    bool has_Vershina(size_t v) const {
        return v < vershini.size();
    }

    // Получить количество вершин
    size_t Vershina_Count() const {
        return vershini.size();
    }

    // Получить список всех вершин (для итерации)
    std::vector<size_t> get_Vershina_mn() const {
        std::vector<size_t> result(vershini.size());
        for (size_t i = 0; i < vershini.size(); i++) {
            result[i] = i;
        }
        return result;
    }

    // ===== АТРИБУТЫ ВЕРШИН =====

    void set_Vershina_Color(size_t v, const std::string& color) {
        if (v < vershini.size()) {
            vershini[v].color = color;
        }
    }

    std::string get_Vershina_Color(size_t v) const {
        return (v < vershini.size()) ? vershini[v].color : "black";
    }

    void set_Vershina_Label(size_t v, const std::string& label) {
        if (v < vershini.size()) {
            vershini[v].label = label;
        }
    }

    std::string get_Vershina_Label(size_t v) const {
        return (v < vershini.size()) ? vershini[v].label : "";
    }

    void set_ArticulationPoint(size_t v, bool value = true) {
        if (v < vershini.size()) {
            vershini[v].isArticulation = value;
        }
    }

    bool is_ArticulationPoint(size_t v) const {
        return (v < vershini.size()) ? vershini[v].isArticulation : false;
    }

    void set_Vershina_Component(size_t v, int comp) {
        if (v < vershini.size()) {
            vershini[v].component = comp;
        }
    }

    int get_Vershina_Component(size_t v) const {
        return (v < vershini.size()) ? vershini[v].component : -1;
    }

    // ==================== РАБОТА С РЁБРАМИ ====================

    // Добавить ребро между u и v
    void add_Rebro(size_t u, size_t v) {
        // Проверяем, что вершины существуют
        if (u >= vershini.size() || v >= vershini.size()) {
            throw std::out_of_range("Vertex index out of range");
        }

        // Не добавляем существующее ребро
        if (has_Rebro(u, v)) {
            return;
        }

        // Создаём новое ребро
        size_t edgeId = rebro.size();
        rebro.emplace_back(u, v);

        // Добавляем индекс ребра в списки смежности вершин
        inchedent_rebra[u].push_back(edgeId);
        inchedent_rebra[v].push_back(edgeId);

        // Запоминаем индекс в матрице для быстрого доступа
        rebra_index[u][v] = static_cast<int>(edgeId);
        rebra_index[v][u] = static_cast<int>(edgeId);
    }

    // Проверить, существует ли ребро (O(1))
    bool has_Rebro(size_t u, size_t v) const {
        if (u >= vershini.size() || v >= vershini.size()) {
            return false;
        }
        return rebra_index[u][v] != -1;
    }

    // Получить количество рёбер
    size_t rebro_Count() const {
        return rebro.size();
    }

    // Получить ребро по его концам (константная версия)
    const Rebro& get_Rebro(size_t u, size_t v) const {
        if (!has_Rebro(u, v)) {
            throw std::runtime_error("Edge does not exist");
        }
        return rebro[rebra_index[u][v]];
    }

    // Получить ребро по его концам (неконстантная версия)
    Rebro& get_Rebro(size_t u, size_t v) {
        if (!has_Rebro(u, v)) {
            throw std::runtime_error("Edge does not exist");
        }
        return rebro[rebra_index[u][v]];
    }

    // Получить ребро по его индексу
    const Rebro& get_Rebro_By_Id(size_t id) const {
        if (id >= rebro.size()) {
            throw std::out_of_range("Edge index out of range");
        }
        return rebro[id];
    }

    Rebro& get_Rebro_By_Id(size_t id) {
        if (id >= rebro.size()) {
            throw std::out_of_range("Edge index out of range");
        }
        return rebro[id];
    }

    // Получить все рёбра (для алгоритмов обхода)
    const std::vector<Rebro>& get_All_Rebra() const {
        return rebro;
    }

    // ===== АТРИБУТЫ РЁБЕР =====

    void set_Rebro_Color(size_t u, size_t v, const std::string& color) {
        if (has_Rebro(u, v)) {
            rebro[rebra_index[u][v]].color = color;
        }
    }

    std::string get_Rebro_Color(size_t u, size_t v) const {
        if (has_Rebro(u, v)) {
            return rebro[rebra_index[u][v]].color;
        }
        return "black";
    }

    void set_Bridge(size_t u, size_t v, bool value = true) {
        if (has_Rebro(u, v)) {
            rebro[rebra_index[u][v]].isBridge = value;
        }
    }

    bool is_Bridge(size_t u, size_t v) const {
        if (has_Rebro(u, v)) {
            return rebro[rebra_index[u][v]].isBridge;
        }
        return false;
    }

    void set_EdgeBiconnectedComponent(size_t u, size_t v, int comp) {
        if (has_Rebro(u, v)) {
            rebro[rebra_index[u][v]].edgeBiconnectedComponent = comp;
        }
    }

    int get_EdgeBiconnectedComponent(size_t u, size_t v) const {
        if (has_Rebro(u, v)) {
            return rebro[rebra_index[u][v]].edgeBiconnectedComponent;
        }
        return -1;
    }

    // ==================== ОБХОД ОКРЕСТНОСТЕЙ ====================

    // Получить всех соседей вершины v (список вершин)
    std::vector<size_t> get_Neighbors(size_t v) const {
        std::vector<size_t> neighbors;
        if (v >= inchedent_rebra.size()) {
            return neighbors;
        }

        neighbors.reserve(inchedent_rebra[v].size());
        for (size_t edgeId : inchedent_rebra[v]) {
            const Rebro& e = rebro[edgeId];
            neighbors.push_back(e.other(v));
        }
        return neighbors;
    }

    // Получить список индексов рёбер, инцидентных вершине v
    const std::vector<size_t>& get_EdgeIds(size_t v) const {
        static const std::vector<size_t> empty;
        if (v >= inchedent_rebra.size()) {
            return empty;
        }
        return inchedent_rebra[v];
    }

    // Получить степень вершины (количество инцидентных рёбер)
    size_t degree(size_t v) const {
        return (v < inchedent_rebra.size()) ? inchedent_rebra[v].size() : 0;
    }

    // Проверить, является ли вершина листом (степень 1)
    bool is_Leaf(size_t v) const {
        return degree(v) == 1;
    }

    // Проверить, является ли вершина изолированной (степень 0)
    bool is_Isolated(size_t v) const {
        return degree(v) == 0;
    }

    // ==================== ОПЕРАЦИИ НАД ГРАФОМ ====================

    // Очистить граф (удалить все вершины и рёбра)
    void clear() {
        inchedent_rebra.clear();
        vershini.clear();
        rebro.clear();
        rebra_index.clear();
    }

    // Проверить, пустой ли граф
    bool isEmpty() const {
        return vershini.empty();
    }

    // Создать глубокую копию
    Graph copy() const {
        return Graph(*this);
    }

    // ==================== ВИЗУАЛИЗАЦИЯ И ОТЛАДКА ====================

    // Вывести граф в консоль (для отладки)
    void print() const {
        std::cout << "========================================\n";
        std::cout << "Graph: " << Vershina_Count() << " vertices, "
                  << rebro_Count() << " edges\n";
        std::cout << "========================================\n";

        // Вывод вершин
        std::cout << "\nVertices:\n";
        for (size_t i = 0; i < vershini.size(); i++) {
            std::cout << "  " << i << ": ";
            std::cout << "color=" << vershini[i].color;
            if (!vershini[i].label.empty()) {
                std::cout << ", label=" << vershini[i].label;
            }
            if (vershini[i].isArticulation) {
                std::cout << " [ARTICULATION]";
            }
            if (vershini[i].component != -1) {
                std::cout << " (comp=" << vershini[i].component << ")";
            }
            std::cout << "\n";
        }

        // Вывод рёбер
        std::cout << "\nEdges:\n";
        for (const auto& e : rebro) {
            std::cout << "  " << e.from << " -- " << e.to;
            if (e.color != "black") {
                std::cout << " [color=" << e.color << "]";
            }
            if (e.isBridge) {
                std::cout << " [BRIDGE]";
            }
            if (e.edgeBiconnectedComponent != -1) {
                std::cout << " (bcomp=" << e.edgeBiconnectedComponent << ")";
            }
            std::cout << "\n";
        }

        // Вывод списка смежности (для проверки)
        std::cout << "\nAdjacency (by vertex):\n";
        for (size_t i = 0; i < inchedent_rebra.size(); i++) {
            std::cout << "  " << i << ": ";
            for (size_t eid : inchedent_rebra[i]) {
                const Rebro& e = rebro[eid];
                std::cout << e.other(i) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "========================================\n";
    }

    // Проверить корректность внутреннего состояния (для тестов)
    bool validate() const {
        // Проверяем, что количество рёбер в edgeIds_ совпадает с edges_.size() * 2
        size_t totalEdgeRefs = 0;
        for (const auto& list : inchedent_rebra) {
            totalEdgeRefs += list.size();
        }
        if (totalEdgeRefs != rebro.size() * 2) {
            std::cerr << "Validation failed: edge ref count mismatch\n";
            return false;
        }

        // Проверяем, что edgeIndex_ симметричен и соответствует edges_
        for (size_t i = 0; i < vershini.size(); i++) {
            for (size_t j = 0; j < vershini.size(); j++) {
                int idx = rebra_index[i][j];
                if (idx != -1) {
                    if (idx >= static_cast<int>(rebro.size())) {
                        std::cerr << "Validation failed: invalid edge index\n";
                        return false;
                    }
                    const Rebro& e = rebro[idx];
                    if (!((e.from == i && e.to == j) || (e.from == j && e.to == i))) {
                        std::cerr << "Validation failed: edge index mismatch\n";
                        return false;
                    }
                }
            }
        }

        return true;
    }
};
int main() {
    Graph g;
    g.add_Vershina_mn(6);
    for (int i = 1; i<=5; i++)
    {
        g.add_Rebro(0, i);
    }
    g.print();
    return 0;
}