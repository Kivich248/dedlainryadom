#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <cassert>

using namespace std;


// СТРУКТУРА ВЕРШИНЫ

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


// КЛАСС ГРАФА

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
    vector<vector<int>> rebro_index;

    // Вспомогательная функция: обновляет матрицу rebro_index после добавления вершины
    void expand_rebro_index();

public:
    // Конструкторы
    Graph();
    explicit Graph(size_t n);

    // Копирование
    Graph(const Graph& other);
    Graph& operator=(const Graph& other);


    // ОПЕРАЦИИ С ВЕРШИНАМИ


    void add_vershina();
    void add_vershiny(size_t count);
    bool has_vershina(size_t v) const;
    size_t count_vershiny() const;
    vector<size_t> get_vershiny() const;

    // Удаление вершины с перенумерацией
    void remove_vershina(size_t m);


    // АТРИБУТЫ ВЕРШИН


    void set_color_vershiny(size_t v, const string& color);
    string get_color_vershiny(size_t v) const;

    void set_label_vershiny(size_t v, const string& label);
    string get_label_vershiny(size_t v) const;

    // Дефолтный аргумент ТОЛЬКО здесь
    void set_tochka_sochleneniya(size_t v, bool value = true);
    bool is_tochka_sochleneniya(size_t v) const;

    void set_component_vershiny(size_t v, int comp);
    int get_component_vershiny(size_t v) const;


    // ОПЕРАЦИИ С РЁБРАМИ


    void add_rebro(size_t u, size_t v);
    bool has_rebro(size_t u, size_t v) const;
    size_t count_rebra() const;

    // Неконстантная версия
    Rebro& get_rebro(size_t u, size_t v);

    // По индексу
    const Rebro& get_rebro_po_id(size_t id) const;
    Rebro& get_rebro_po_id(size_t id);

    // Все рёбра
    const vector<Rebro>& get_vse_rebra() const;


    // АТРИБУТЫ РЁБЕР


    void set_color_rebra(size_t u, size_t v, const string& color);
    string get_color_rebra(size_t u, size_t v) const;

    // Дефолтный аргумент ТОЛЬКО здесь
    void set_most(size_t u, size_t v, bool value = true);
    bool is_most(size_t u, size_t v) const;

    void set_component_rebra(size_t u, size_t v, int comp);
    int get_component_rebra(size_t u, size_t v) const;

    // Получение компонент связности (для тестов и метрик)
    vector<vector<size_t>> get_components() const;


    // ОБХОД ОКРЕСТНОСТЕЙ


    vector<size_t> get_sosedi(size_t v) const;
    const vector<size_t>& get_incid_rebra(size_t v) const;
    size_t stepen(size_t v) const;
    bool is_list(size_t v) const;
    bool is_izolirovannaya(size_t v) const;


    // УПРАВЛЕНИЕ ГРАФОМ


    void clear();
    bool pust() const;
    Graph kopiya() const;
    Graph operator+(const Graph& other) const;

    // Случайная перенумерация
    void random_renumber();


    // ОТЛАДКА И ТЕСТИРОВАНИЕ


    void print() const;
    bool validate() const;
};