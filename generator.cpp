
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
Graph generate_full(size_t n)
{
    Graph g;
    g.add_vershiny(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = i+1; j < n; j++)
        {
            g.add_rebro(i, j);
        }
    }
return g;
}

Graph generate_full_twodol(size_t n, size_t m)
{
    Graph g;
    g.add_vershiny(n+m);
    for (int i = 0; i < n; i++)
    {
        for (int j = n; j < m+n; j++)
        {
          g.add_rebro(i, j);
        }
    }
    return g;
}

Graph generate_star(size_t n)
{
    Graph g;
    g.add_vershiny(n);
    for (int i = 1; i < n; i++)
    {
        g.add_rebro(0, i);
    }
    return g;
}

Graph generate_path(size_t n)
{
    Graph g;
    g.add_vershiny(n);
    for (int i = 0; i < n - 1; i++)
    {
        g.add_rebro(i, i+1);
    }
    return g;
}

Graph generate_cycle(size_t n)
{
    Graph g;
    g.add_vershiny(n);
    for (int i = 0; i < n - 1; i++)
    {
        g.add_rebro(i, i+1);
    }
    g.add_rebro(0,n-1);
    return g;
}

Graph generate_wheel(size_t m)
{
    Graph g;
    g.add_vershiny(m);
    int n = m-1;
    for (int i = 0; i < n - 1; i++)
    {
        g.add_rebro(i, i+1);
    }
    g.add_rebro(0,n-1);

    for (int i = 0; i < n; i++)
    {
        g.add_rebro(m-1, i);
    }
   return g;
}


Graph generate_tree(size_t n) {
    Graph tree;


    for (size_t i = 0; i < n; i++) {
        tree.add_vershina();
    }


    if (n <= 1) {
        return tree;
    }
    if (n == 2) {
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
}

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
    }




Graph generate_random_cubic(size_t n) {             //кубический граф как ты и сказал, делим вершины на 3 и соединяем

    if (n % 2 != 0) {
        throw std::invalid_argument("Кубический граф требует чётное количество вершин!");
    }
    if (n < 4) {
        throw std::invalid_argument("Кубический граф должен иметь хотя бы 4 вершины!");
    }
    Graph g;

    g.add_vershiny(n);

    std::vector<size_t> stubs;
    for (size_t v = 0; v < n; v++) {
        for (size_t i = 0; i < 3; i++) {
            stubs.push_back(v);
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(stubs.begin(), stubs.end(), gen);

    for (size_t i = 0; i < stubs.size(); i += 2) {
        size_t u = stubs[i];
        size_t v = stubs[i + 1];

        if (u == v) {

            return generate_random_cubic(n);
        }


        if (g.has_rebro(u, v)) {
            return generate_random_cubic(n);
        }

        g.add_rebro(u, v);
    }

    return g;
}

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

    for (size_t i = 0; i < remaining; i++) {
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
}


