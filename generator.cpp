
#include "graph.h"
#include "parser.h"
#include "generator.h"
#include <random>
#include <set>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <queue>
#include <numeric>

size_t random_in_range(size_t min_val, size_t max_val)
{
    if (min_val >= max_val) {
        return min_val;
    }
    size_t range = max_val - min_val + 1;
    return min_val + (rand() % range);
}

// ============================================================================
// ВСПОМОГАТЕЛЬНАЯ: Случайный 2-связный граф (без мостов)
// ============================================================================
Graph generate_2connected_graph_random(size_t n, unsigned int seed)
{
    if (n < 3) {
        // Для n < 3 невозможно создать 2-связный граф
        Graph g;
        g.add_vershiny(n);
        if (n == 2) g.add_rebro(0, 1);
        return g;
    }

    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    Graph g;
    g.add_vershiny(n);

    // Шаг 1: Создаём базовый цикл (гарантирует 2-связность)
    for (size_t i = 0; i < n; i++) {
        g.add_rebro(i, (i + 1) % n);
    }

    // Шаг 2: Добавляем случайные хорды с вероятностью 0.3
    double chord_prob = 0.3;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 2; j < n; j++) {
            // Пропускаем рёбра цикла
            if (i == 0 && j == n - 1) continue;

            if (prob_dist(gen) < chord_prob) {
                g.add_rebro(i, j);
            }
        }
    }

    return g;
}

Graph generate_full(size_t n)
{
    Graph g;
    g.add_vershiny(n);
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = i + 1; j < n; j++)
        {
            g.add_rebro(i, j);
        }
    }
return g;
} // 1

Graph generate_full_twodol(size_t n, size_t m)
{
    Graph g;
    g.add_vershiny(n + m);
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = n; j < m + n; j++)
        {
          g.add_rebro(i, j);
        }
    }
    return g;
} // 2

Graph generate_tree(size_t n)
{
    Graph tree;

    for (size_t i = 0; i < n; i++)
    {
        tree.add_vershina();
    }

    if (n <= 1)
    {
        return tree;
    }
    if (n == 2)
    {
        tree.add_rebro(0, 1);
        return tree;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, n - 1);

    std::vector<size_t> prufer(n - 2);
    for (size_t i = 0; i < n - 2; i++) {
        prufer[i] = dist(gen);
    }


    std::vector<size_t> degree(n, 1);
    for (size_t v : prufer) {
        degree[v]++;
    }


    std::vector<bool> is_leaf(n, false);
    for (size_t i = 0; i < n; i++) {
        if (degree[i] == 1) {
            is_leaf[i] = true;
        }
    }


    for (size_t i = 0; i < n - 2; i++) {

        size_t leaf = 0;
        while (!is_leaf[leaf]) {
            leaf++;
        }


        tree.add_rebro(leaf, prufer[i]);


        is_leaf[leaf] = false;
        degree[leaf]--;


        degree[prufer[i]]--;
        if (degree[prufer[i]] == 1) {
            is_leaf[prufer[i]] = true;
        }
    }

    size_t last1 = 0, last2 = 0;
    for (size_t i = 0; i < n; i++) {
        if (degree[i] == 1) {
            if (last1 == 0) {
                last1 = i;
            } else {
                last2 = i;
                break;
            }
        }
    }

    tree.add_rebro(last1, last2);

    return tree;
} // 3

Graph generate_star(size_t n)
{
    Graph g;
    g.add_vershiny(n);
    for (size_t i = 1; i < n; i++)
    {
        g.add_rebro(0, i);
    }
    return g;
} // 4

Graph generate_cycle(size_t n)
{
    Graph g;
    g.add_vershiny(n);
    for (size_t i = 0; i < n - 1; i++)
    {
        g.add_rebro(i, i + 1);
    }
    g.add_rebro(0,n - 1);
    return g;
} // 5

Graph generate_path(size_t n)
{
    Graph g;
    g.add_vershiny(n);
    for (size_t i = 0; i < n - 1; i++)
    {
        g.add_rebro(i, i + 1);
    }
    return g;
} // 6

Graph generate_wheel(size_t n)
{
    Graph g;
    g.add_vershiny(n);
    size_t m = n - 1;
    for (size_t i = 0; i < m - 1; i++)
    {
        g.add_rebro(i, i + 1);
    }
    g.add_rebro(0,m - 1);

    for (size_t i = 0; i < m; i++)
    {
        g.add_rebro(n - 1, i);
    }
   return g;
} // 7

Graph random_veroyatnost(size_t n, double p) {        //окааааазывается это граф с n вершинами и вероятностью p добавления ребра
    Graph g;

    g.add_vershiny(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if (dist(gen) < p) {
                g.add_rebro(i, j);
            }
        }
    }

    return g;
} // 8

Graph generate_random_cubic(size_t n) {             //кубический граф как ты и сказал, делим вершины на 3 и соединяем

    if (n % 2 != 0)
    {
        throw std::invalid_argument("Кубический граф требует чётное количество вершин!");
    }
    if (n < 4)
    {
        throw std::invalid_argument("Кубический граф должен иметь хотя бы 4 вершины!");
    }
    Graph g;

    g.add_vershiny(n);

    std::vector<size_t> stubs;
    for (size_t v = 0; v < n; v++)
    {
        for (size_t i = 0; i < 3; i++)
        {
            stubs.push_back(v);
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(stubs.begin(), stubs.end(), gen);

    for (size_t i = 0; i < stubs.size(); i += 2)
    {
        size_t u = stubs[i];
        size_t v = stubs[i + 1];

        if (u == v)
        {
            return generate_random_cubic(n);
        }

        if (g.has_rebro(u, v))
        {
            return generate_random_cubic(n);
        }

        g.add_rebro(u, v);
    }

    return g;
} // 9

Graph generate_graph_with_components(size_t n, size_t k) {     //проблема - генерит ребра с фиксированной вероятностью
    if (k > n) k = n;
    if (k == 0) k = 1;
    double extra_edge_probability = 0.3;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);
    std::uniform_int_distribution<size_t> comp_dist(0, k - 1);

    // 1. Случайно разбиваем n на k частей (размеры компонент)
    std::vector<size_t> component_sizes(k, 1);  // каждой компоненте минимум 1 вершина
    size_t remaining = n - k;

    for (size_t i = 0; i < remaining; i++)
    {
        component_sizes[comp_dist(gen)]++;
    }

    // 2. Создаём компоненты (пути)
    std::vector<Graph> components;

    for (size_t comp_idx = 0; comp_idx < k; comp_idx++) {
        size_t comp_size = component_sizes[comp_idx];

        if (comp_size == 1) {
            // Компонента из одной вершины — просто добавляем вершину
            Graph single;
            single.add_vershina();
            components.push_back(single);
        } else {
            // Создаём путь на comp_size вершинах
            Graph path = generate_path(comp_size);

            // Добавляем случайные дополнительные рёбра (делаем компоненту интереснее)
            for (size_t i = 0; i < comp_size; i++) {
                for (size_t j = i + 2; j < comp_size; j++) {  // j = i+2 чтобы не дублировать путь
                    if (prob_dist(gen) < extra_edge_probability) {
                        // Проверяем, нет ли уже ребра
                        if (!path.has_rebro(i, j)) {
                            path.add_rebro(i, j);
                        }
                    }
                }
            }

            components.push_back(path);
        }
    }

    // 3. Объединяем все компоненты в один граф (оператор +)
    Graph result;
    for (size_t i = 0; i < k; i++) {
        result = result + components[i];
    }

    // 4. Случайно перенумеровываем вершины (чтобы компоненты не шли подряд)
    result.random_renumber();

    return result;
}  // 10

Graph generate_graph_with_bridges_path_blobs_random(size_t n, size_t m)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> prob_dist(0.0, 1.0);
    uniform_int_distribution<size_t> vertex_dist;

    if (n < 3) {
        throw invalid_argument("Minimalnoe kolichestvo vershin: 3");
    }
    if (m > n - 1) {
        throw invalid_argument(
            "Maksimum mostov: " + to_string(n - 1) +
            ", polucheno: " + to_string(m)
        );
    }

    // Специальные случаи
    if (m == 0) {
        return generate_2connected_graph_random(n, rd());
    }
    if (m == n - 1) {
        Graph path = generate_path(n);
        // Помечаем все рёбра пути как мосты
        for (size_t i = 0; i < n - 1; i++) {
            path.set_most(i, i + 1, true);
        }
        return path;
    }
    // Случай m = n - 2: путь с одним "блобом" на 2 вершинах
    if (m == n - 2) {
        Graph g = generate_path(n - 1);  // Путь на n-1 вершинах = n-2 моста
        g.add_vershina();  // Добавляем последнюю вершину
        // Присоединяем её к случайной вершине пути (не создаёт новый мост)
        size_t attach_to = rand() % (n - 1);
        g.add_rebro(n - 1, attach_to);
        // Помечаем рёбра пути как мосты
        for (size_t i = 0; i < n - 2; i++) {
            g.set_most(i, i + 1, true);
        }
        return g;
    }

    Graph g;
    g.add_vershiny(n);

    size_t vershini_pyti = m + 1;
    for (size_t i = 0; i < m; i++) {
        g.add_rebro(i, i + 1);
        // Помечаем рёбра пути как мосты
        g.set_most(i, i + 1, true);
    }

    size_t ost_vershini = n - vershini_pyti;
    size_t vershina_now = vershini_pyti;
    const size_t MIN_VERTICES_PER_BLOB = 2;

    while (ost_vershini >= MIN_VERTICES_PER_BLOB) {
        vertex_dist = uniform_int_distribution<size_t>(0, vershini_pyti - 1);
        size_t vershina_pyti = vertex_dist(gen);

        vertex_dist = uniform_int_distribution<size_t>(MIN_VERTICES_PER_BLOB, ost_vershini);
        size_t blob_size = vertex_dist(gen);

        if (blob_size == ost_vershini - 1) {
            blob_size = ost_vershini;
        }

        vector<size_t> vershini_bloba;
        vershini_bloba.push_back(vershina_pyti);
        for (size_t i = 0; i < blob_size; i++) {
            vershini_bloba.push_back(vershina_now + i);
        }

        // Создаём цикл в блоке
        size_t k = vershini_bloba.size() - 1;
        if (k >= 2) {
            for (size_t i = 1; i < k; i++) {
                g.add_rebro(vershini_bloba[i], vershini_bloba[i + 1]);
            }
            g.add_rebro(vershini_bloba[1], vershina_pyti);
            g.add_rebro(vershini_bloba[k], vershina_pyti);
        }

        // Случайные рёбра внутри блока
        double edge_prob = prob_dist(gen);
        for (size_t i = 1; i < vershini_bloba.size(); i++) {
            for (size_t j = i + 1; j < vershini_bloba.size(); j++) {
                size_t u = vershini_bloba[i];
                size_t v = vershini_bloba[j];
                if (!g.has_rebro(u, v)) {
                    if (prob_dist(gen) < edge_prob) {
                        g.add_rebro(u, v);
                    }
                }
            }
        }

        // Помечаем вершину пути как точку сочленения (если к ней присоединён блок)
        g.set_tochka_sochleneniya(vershina_pyti, true);

        vershina_now += blob_size;
        ost_vershini -= blob_size;
    }

    return g;
}

// ============================================================================
// ГЕНЕРАТОР 12: Граф на n вершинах с заданным количеством точек сочленения
// ============================================================================
Graph generate_graph_with_articulations(size_t n, size_t num_articulations)
{
    if (n < 3) {
        throw std::invalid_argument("Minimalnoe kolichestvo vershin: 3");
    }
    if (num_articulations > n - 2) {
        throw std::invalid_argument(
            "Maksimum tochek sochleneniya: " + std::to_string(n - 2)
        );
    }

    // Создаём граф
    Graph result;
    result.add_vershiny(n);

    // Количество двусвязных компонент
    size_t num_components = num_articulations + 1;

    // Минимальный размер компоненты для двусвязности = 3
    if (n < 3 * num_components) {
        // Не хватает вершин — уменьшаем количество компонент
        num_components = n / 3;
        if (num_components < 1) num_components = 1;
        num_articulations = num_components - 1;
    }

    // Размеры компонент (минимально по 3)
    std::vector<size_t> comp_sizes(num_components, 3);
    size_t remaining = n - 3 * num_components;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, num_components - 1);

    for (size_t i = 0; i < remaining; i++) {
        comp_sizes[dist(gen)]++;
    }

    // Создаём компоненты и соединяем их
    size_t offset = 0;

    for (size_t comp = 0; comp < num_components; comp++) {
        size_t size = comp_sizes[comp];

        if (size == 1) {
            // Одна вершина — изолированная
            offset++;
            continue;
        }
        else if (size == 2) {
            // Две вершины — просто ребро
            result.add_rebro(offset, offset + 1);
            offset += 2;
        }
        else {
            // Три и более — создаём цикл (двусвязный граф)
            for (size_t i = 0; i < size - 1; i++) {
                result.add_rebro(offset + i, offset + i + 1);
            }
            result.add_rebro(offset + size - 1, offset);
            offset += size;
        }

        // Соединяем с предыдущей компонентой через точку сочленения
        if (comp > 0) {
            // Точка сочленения — последняя вершина предыдущей компоненты
            size_t articulation_point = offset - comp_sizes[comp] - 1;

            // Соединяем с первой вершиной текущей компоненты
            result.add_rebro(articulation_point, offset - comp_sizes[comp]);

            // Можно пометить точку сочленения (но не обязательно)
            result.set_tochka_sochleneniya(articulation_point, true);
        }
    }

    return result;
}
// 12