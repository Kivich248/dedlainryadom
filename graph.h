#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <cassert>

using namespace std;

// ============================================================================
// СТРУКТУРА ВЕРШИНЫ
// ============================================================================
struct Vershina
{
    string color;                      // цвет для GraphViz
    string label;                      // метка вершины
    bool isArticulation;               // флаг: точка сочленения?
    int component;                     // номер компоненты связности
    int biconnectedComponent;          // номер компоненты вершинной двусвязности

    Vershina();
    explicit Vershina(const string& col);
};

// ============================================================================
// СТРУКТУРА РЕБРА
// ============================================================================
struct Rebro
{
    size_t from;                       // откуда
    size_t to;                         // куда
    string color;                      // цвет для GraphViz
    bool isBridge;                     // флаг: мост?
    int edgeBiconnectedComponent;      // компонента рёберной двусвязности

    Rebro(size_t u, size_t v);
    Rebro(size_t u, size_t v, const string& col);

    size_t other(size_t v) const;
    bool incidentTo(size_t v) const;
};

// ============================================================================
// КЛАСС ГРАФА
// ============================================================================
class Graph
{
private:
    // inchedent_rebra[v] — список индексов рёбер, инцидентных вершине v
    vector<vector<size_t>> inchedent_rebra;

    // Данные о вершинах (индекс = номер вершины)
    vector<Vershina> vershini;

    // Данные о рёбрах (индекс = номер ребра)
    vector<Rebro> rebro;

    // Матрица для быстрого поиска ребра: rebro_index[u][v] = индекс ребра или -1
    vector<vector<int>> rebra_index;

    // Вспомогательная функция: обновляет матрицу rebro_index после добавления вершины
    void expand_rebra_index();

public:
    // Конструкторы
    Graph();
    explicit Graph(size_t n);

    // Копирование
    Graph(const Graph& other);
    Graph& operator=(const Graph& other);

    // =========================================================================
    // ОПЕРАЦИИ С ВЕРШИНАМИ
    // =========================================================================

    // Добавить одну вершину
    void add_vershina();

    // Добавить несколько вершин
    void add_vershiny(size_t count);

    // Проверить существование вершины
    bool has_vershina(size_t v) const;

    // Получить количество вершин
    size_t count_vershiny() const;

    // Получить список всех вершин (для итерации)
    vector<size_t> get_vershiny() const;

    void remove_vershina(size_t m);
    // =========================================================================
    // АТРИБУТЫ ВЕРШИН (цвета, метки, флаги)
    // =========================================================================

    void set_color_vershiny(size_t v, const string& color);
    string get_color_vershiny(size_t v) const;

    void set_label_vershiny(size_t v, const string& label);
    string get_label_vershiny(size_t v) const;

    void set_tochka_sochleneniya(size_t v, bool value = true);
    bool is_tochka_sochleneniya(size_t v) const;

    void set_component_vershiny(size_t v, int comp);
    int get_component_vershiny(size_t v) const;

    // =========================================================================
    // ОПЕРАЦИИ С РЁБРАМИ
    // =========================================================================

    // Добавить ребро между u и v
    void add_rebro(size_t u, size_t v);

    // Проверить существование ребра
    bool has_rebro(size_t u, size_t v) const;

    // Получить количество рёбер
    size_t count_rebra() const;

    // Получить ребро по концам (константная версия)
    // const Rebro& get_rebro(size_t u, size_t v) const;

    // Получить ребро по концам (неконстантная версия)
    Rebro& get_rebro(size_t u, size_t v);

    // Получить ребро по индексу
    const Rebro& get_rebro_po_id(size_t id) const;
    Rebro& get_rebro_po_id(size_t id);

    // Получить все рёбра
    const vector<Rebro>& get_vse_rebra() const;

    // =========================================================================
    // АТРИБУТЫ РЁБЕР
    // =========================================================================

    void set_color_rebra(size_t u, size_t v, const string& color);
    string get_color_rebra(size_t u, size_t v) const;

    void set_most(size_t u, size_t v, bool value = true);
    bool is_most(size_t u, size_t v) const;

    void set_component_rebra(size_t u, size_t v, int comp);
    int get_component_rebra(size_t u, size_t v) const;

    // =========================================================================
    // ОБХОД ОКРЕСТНОСТЕЙ
    // =========================================================================

    // Получить всех соседей вершины v
    vector<size_t> get_sosedi(size_t v) const;

    // Получить список индексов рёбер, инцидентных вершине v
    const vector<size_t>& get_incid_rebra(size_t v) const;

    // Получить степень вершины
    size_t stepen(size_t v) const;

    // Проверить, является ли вершина листом (степень 1)
    bool is_list(size_t v) const;

    // Проверить, является ли вершина изолированной (степень 0)
    bool is_izolirovannaya(size_t v) const;

    // =========================================================================
    // УПРАВЛЕНИЕ ГРАФОМ
    // =========================================================================

    // Очистить граф
    void clear();

    // Проверить, пустой ли граф
    bool pust() const;

    // Создать глубокую копию
    Graph kopiya() const;

    // Объединение графов (оператор +)
    Graph operator+(const Graph& other) const;

    // =========================================================================
    // ОТЛАДКА И ТЕСТИРОВАНИЕ
    // =========================================================================

    // Вывести граф в консоль
    void print() const;

    // Проверить корректность внутреннего состояния (инварианты)
    bool validate() const;
};