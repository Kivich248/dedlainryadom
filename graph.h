
#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <cassert>

//структура вершинки
struct Vershina {
    std::string color = "black";     // цвет для GraphViz
    std::string label = "";          // метка вершины (можно отображать)
    bool isArticulation = false;     // флаг: точка сочленения?
    int component = -1;              // номер компоненты связности
    int biconnectedComponent = -1;   // номер компоненты вершинной двусвязности

    Vershina() = default;
    explicit Vershina(const std::string& col) : color(col) {}
};

//структура ребрышка (жаль не куриного)
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

//класс графа
class Graph {
private:

    // edgeIds_[v] — список индексов рёбер, инцидентных вершине v
    // Храним индексы, а не вершины, чтобы быстро получать атрибуты ребра
    std::vector<std::vector<size_t>> edgeIds_;

    // Данные о вершинах (индекс в векторе = номер вершины)
    std::vector<Vershina> vertices_;

    // Данные о рёбрах (индекс в векторе = номер ребра)
    std::vector<Rebro> edges_;

    // Матрица для быстрого поиска ребра: edgeIndex_[u][v] = индекс ребра или -1
    std::vector<std::vector<int>> edgeIndex_;

    // Обновляем матрицу edgeIndex_ после добавления вершины
    void expandEdgeIndex() {
        size_t newSize = vertices_.size();

        // Расширяем существующие строки
        for (auto& row : edgeIndex_) {
            row.resize(newSize, -1);
        }

        // Добавляем новую строку для новой вершины
        edgeIndex_.resize(newSize);
        edgeIndex_[newSize - 1].resize(newSize, -1);
    }

public:

    Graph() = default;

    // Граф с n вершинами (без рёбер)
    explicit Graph(size_t n) {
        vertices_.resize(n);
        edgeIds_.resize(n);
        edgeIndex_.resize(n);
        for (auto& row : edgeIndex_) {
            row.resize(n, -1);
        }
    }

    // Копирование
    Graph(const Graph& other)
        : edgeIds_(other.edgeIds_),
          vertices_(other.vertices_),
          edges_(other.edges_),
          edgeIndex_(other.edgeIndex_) {}

    // Присваивание
    Graph& operator=(const Graph& other) {
        if (this != &other) {
            edgeIds_ = other.edgeIds_;
            vertices_ = other.vertices_;
            edges_ = other.edges_;
            edgeIndex_ = other.edgeIndex_;
        }
        return *this;
    }


    // Добавить одну вершину
    void add_Vershina() {
        vertices_.emplace_back();   // добавляем структуру Vershina
        edgeIds_.emplace_back();    // добавляем пустой список рёбер
        expandEdgeIndex();          // обновляем матрицу поиска
    }

    // Добавить несколько вершин
    void add_Vershina_mn(size_t count) {
        for (size_t i = 0; i < count; i++) {
            add_Vershina();
        }
    }

    // Проверить, существует ли вершина
    bool has_Vershina(size_t v) const {
        return v < vertices_.size();
    }

    // Получить количество вершин
    size_t Vershina_Count() const {
        return vertices_.size();
    }

    // Получить список всех вершин (для итерации)
    std::vector<size_t> get_Vershina_mn() const {
        std::vector<size_t> result(vertices_.size());
        for (size_t i = 0; i < vertices_.size(); i++) {
            result[i] = i;
        }
        return result;
    }

    //красим вершину

    void set_Vershina_Color(size_t v, const std::string& color) {
        if (v < vertices_.size()) {
            vertices_[v].color = color;
        }
    }
    //получаем цвет вершины
    std::string get_Vershina_Color(size_t v) const {
        return (v < vertices_.size()) ? vertices_[v].color : "black";
    }
    //меняем состояние вершины(пока хз зачем)
    void set_Vershina_Label(size_t v, const std::string& label) {
        if (v < vertices_.size()) {
            vertices_[v].label = label;
        }
    }
    //получаем состояние вершины
    std::string get_Vershina_Label(size_t v) const {
        return (v < vertices_.size()) ? vertices_[v].label : "";
    }
    //меняем состояние точки сочленения апхапхапхапххап
    void set_ArticulationPoint(size_t v, bool value = true) {
        if (v < vertices_.size()) {
            vertices_[v].isArticulation = value;
        }
    }
    //получаем состояние точки сочленения
    bool is_ArticulationPoint(size_t v) const {
        return (v < vertices_.size()) ? vertices_[v].isArticulation : false;
    }

    void set_Vershina_Component(size_t v, int comp) {
        if (v < vertices_.size()) {
            vertices_[v].component = comp;
        }
    }

    int get_Vershina_Component(size_t v) const {
        return (v < vertices_.size()) ? vertices_[v].component : -1;
    }


    // Добавить ребро между u и v
    void add_Rebro(size_t u, size_t v) {
        // Проверяем, что вершины существуют
        if (u >= vertices_.size() || v >= vertices_.size()) {
            throw std::out_of_range("Vertex index out of range");
        }

        // Не добавляем существующее ребро
        if (has_Rebro(u, v)) {
            return;
        }

        // Создаём новое ребро
        size_t edgeId = edges_.size();
        edges_.emplace_back(u, v);

        // Добавляем индекс ребра в списки смежности вершин
        edgeIds_[u].push_back(edgeId);
        edgeIds_[v].push_back(edgeId);

        // Запоминаем индекс в матрице для быстрого доступа
        edgeIndex_[u][v] = static_cast<int>(edgeId);
        edgeIndex_[v][u] = static_cast<int>(edgeId);
    }

    //проверочка на существование ребра
    bool has_Rebro(size_t u, size_t v) const {
        if (u >= vertices_.size() || v >= vertices_.size()) {
            return false;
        }
        return edgeIndex_[u][v] != -1;
    }

    // Получить количество рёбер
    size_t rebro_Count() const {
        return edges_.size();
    }

    // Получить ребро по его концам (константная версия)
    const Rebro& get_Rebro(size_t u, size_t v) const {
        if (!has_Rebro(u, v)) {
            throw std::runtime_error("Edge does not exist");
        }
        return edges_[edgeIndex_[u][v]];
    }

    // Получить ребро по его концам (неконстантная версия)
    Rebro& get_Rebro(size_t u, size_t v) {
        if (!has_Rebro(u, v)) {
            throw std::runtime_error("Edge does not exist");
        }
        return edges_[edgeIndex_[u][v]];
    }

    // Получить ребро по его индексу
    const Rebro& get_Rebro_By_Id(size_t id) const {
        if (id >= edges_.size()) {
            throw std::out_of_range("Edge index out of range");
        }
        return edges_[id];
    }

    Rebro& get_Rebro_By_Id(size_t id) {
        if (id >= edges_.size()) {
            throw std::out_of_range("Edge index out of range");
        }
        return edges_[id];
    }

    // Получить все рёбра (для алгоритмов обхода)
    const std::vector<Rebro>& get_All_Rebra() const {
        return edges_;
    }

    //меняем цвет ребрышка
    void set_Rebro_Color(size_t u, size_t v, const std::string& color) {
        if (has_Rebro(u, v)) {
            edges_[edgeIndex_[u][v]].color = color;
        }
    }

    //получаем цвет ребрышка
    std::string get_Rebro_Color(size_t u, size_t v) const {
        if (has_Rebro(u, v)) {
            return edges_[edgeIndex_[u][v]].color;
        }
        return "black";
    }

    //ээээ мне дальше лень комментить
    void set_Bridge(size_t u, size_t v, bool value = true) {
        if (has_Rebro(u, v)) {
            edges_[edgeIndex_[u][v]].isBridge = value;
        }
    }

    bool is_Bridge(size_t u, size_t v) const {
        if (has_Rebro(u, v)) {
            return edges_[edgeIndex_[u][v]].isBridge;
        }
        return false;
    }

    void set_EdgeBiconnectedComponent(size_t u, size_t v, int comp) {
        if (has_Rebro(u, v)) {
            edges_[edgeIndex_[u][v]].edgeBiconnectedComponent = comp;
        }
    }

    int get_EdgeBiconnectedComponent(size_t u, size_t v) const {
        if (has_Rebro(u, v)) {
            return edges_[edgeIndex_[u][v]].edgeBiconnectedComponent;
        }
        return -1;
    }

    // Получить всех соседей вершины v (список вершин)
    std::vector<size_t> get_Neighbors(size_t v) const {
        std::vector<size_t> neighbors;
        if (v >= edgeIds_.size()) {
            return neighbors;
        }

        neighbors.reserve(edgeIds_[v].size());
        for (size_t edgeId : edgeIds_[v]) {
            const Rebro& e = edges_[edgeId];
            neighbors.push_back(e.other(v));
        }
        return neighbors;
    }

    // Получить список индексов рёбер, инцидентных вершине v
    const std::vector<size_t>& get_EdgeIds(size_t v) const {
        static const std::vector<size_t> empty;
        if (v >= edgeIds_.size()) {
            return empty;
        }
        return edgeIds_[v];
    }

    // Получить степень вершины (количество инцидентных рёбер)
    size_t degree(size_t v) const {
        return (v < edgeIds_.size()) ? edgeIds_[v].size() : 0;
    }

    // Проверить, является ли вершина листом (степень 1)
    bool is_Leaf(size_t v) const {
        return degree(v) == 1;
    }

    // Проверить, является ли вершина изолированной (степень 0)
    bool is_Isolated(size_t v) const {
        return degree(v) == 0;
    }

    // Очистить граф (удалить все вершины и рёбра)
    void clear() {
        edgeIds_.clear();
        vertices_.clear();
        edges_.clear();
        edgeIndex_.clear();
    }

    // Проверить, пустой ли граф
    bool isEmpty() const {
        return vertices_.empty();
    }

    // Создать глубокую копию
    Graph copy() const {
        return Graph(*this);
    }

    //оператор плюсик для графов. берет 2 графа, объединяет в 1, увеличивая количество компонентов связности
    Graph operator+(const Graph& other) const {
        Graph result = *this;  // копируем текущий граф

        // Получаем количество вершин в текущем графе
        size_t current_vertices = Vershina_Count();

        // Добавляем вершины из другого графа
        size_t other_vertices = other.Vershina_Count();
        for (size_t i = 0; i < other_vertices; i++) {
            result.add_Vershina();
        }

        // Добавляем рёбра из другого графа (с учётом сдвига индексов)
        for (const auto& e : other.get_All_Rebra()) {
            // Сдвигаем индексы вершин на current_vertices
            size_t new_from = e.from + current_vertices;
            size_t new_to = e.to + current_vertices;
            result.add_Rebro(new_from, new_to);

            // Копируем атрибуты ребра
            result.set_Rebro_Color(new_from, new_to, e.color);
            if (e.isBridge) {
                result.set_Bridge(new_from, new_to, true);
            }
        }

        // Копируем атрибуты вершин из другого графа
        for (size_t i = 0; i < other_vertices; i++) {
            size_t new_index = current_vertices + i;
            result.set_Vershina_Color(new_index, other.get_Vershina_Color(i));
            result.set_Vershina_Label(new_index, other.get_Vershina_Label(i));
            if (other.is_ArticulationPoint(i)) {
                result.set_ArticulationPoint(new_index, true);
            }
        }

        return result;
    }

    // Вывести граф в консоль (для отладки)
    void print() const {
        std::cout << "========================================\n";
        std::cout << "Graph: " << Vershina_Count() << " vertices, "
                  << rebro_Count() << " edges\n";
        std::cout << "========================================\n";

        // Вывод вершин
        std::cout << "\nVertices:\n";
        for (size_t i = 0; i < vertices_.size(); i++) {
            std::cout << "  " << i << ": ";
            std::cout << "color=" << vertices_[i].color;
            if (!vertices_[i].label.empty()) {
                std::cout << ", label=" << vertices_[i].label;
            }
            if (vertices_[i].isArticulation) {
                std::cout << " [ARTICULATION]";
            }
            if (vertices_[i].component != -1) {
                std::cout << " (comp=" << vertices_[i].component << ")";
            }
            std::cout << "\n";
        }

        // Вывод рёбер
        std::cout << "\nEdges:\n";
        for (const auto& e : edges_) {
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
        for (size_t i = 0; i < edgeIds_.size(); i++) {
            std::cout << "  " << i << ": ";
            for (size_t eid : edgeIds_[i]) {
                const Rebro& e = edges_[eid];
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
        for (const auto& list : edgeIds_) {
            totalEdgeRefs += list.size();
        }
        if (totalEdgeRefs != edges_.size() * 2) {
            std::cerr << "Validation failed: edge ref count mismatch\n";
            return false;
        }

        // Проверяем, что edgeIndex_ симметричен и соответствует edges_
        for (size_t i = 0; i < vertices_.size(); i++) {
            for (size_t j = 0; j < vertices_.size(); j++) {
                int idx = edgeIndex_[i][j];
                if (idx != -1) {
                    if (idx >= static_cast<int>(edges_.size())) {
                        std::cerr << "Validation failed: invalid edge index\n";
                        return false;
                    }
                    const Rebro& e = edges_[idx];
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