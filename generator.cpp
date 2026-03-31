
#include "graph.h"
#include "parser.h"
#include "generator.h"
#include <random>
#include <set>
#include <algorithm>
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
Graph generate_random(size_t n, size_t m) {
    Graph g;

    // Добавляем вершины
    for (size_t i = 0; i < n; i++) {
        g.add_vershina();
    }

    // Максимальное количество рёбер
    size_t max_edges = n * (n - 1) / 2;
    if (m > max_edges) {
        throw std::invalid_argument("Слишком много рёбер! Максимум: " +
                                     std::to_string(max_edges));
    }

    // Создаём список всех возможных рёбер
    std::vector<std::pair<size_t, size_t>> all_edges;
    all_edges.reserve(max_edges);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            all_edges.push_back({i, j});
        }
    }

    // Перемешиваем список
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(all_edges.begin(), all_edges.end(), gen);

    // Берём первые m рёбер
    for (size_t i = 0; i < m; i++) {
        g.add_rebro(all_edges[i].first, all_edges[i].second);
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

    // 2. Генерируем случайный код Прюфера (n-2 чисел от 0 до n-1)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, n - 1);

    std::vector<size_t> prufer(n - 2);
    for (size_t i = 0; i < n - 2; i++) {
        prufer[i] = dist(gen);
    }

    // Восстанавливаем дерево по коду Прюфера

    // Считаем степени вершин
    std::vector<size_t> degree(n, 1);  // каждая вершина имеет как минимум степень 1
    for (size_t v : prufer) {
        degree[v]++;
    }

    // Множество листьев (вершины со степенью 1)
    // Используем priority_queue или просто ищем каждый раз
    std::vector<bool> is_leaf(n, false);
    for (size_t i = 0; i < n; i++) {
        if (degree[i] == 1) {
            is_leaf[i] = true;
        }
    }

    // Проходим по коду Прюфера
    for (size_t i = 0; i < n - 2; i++) {
        // Находим наименьший лист
        size_t leaf = 0;
        while (!is_leaf[leaf]) {
            leaf++;
        }

        // Добавляем ребро между leaf и prufer[i]
        tree.add_rebro(leaf, prufer[i]);

        // Удаляем leaf из рассмотрения
        is_leaf[leaf] = false;
        degree[leaf]--;

        // Уменьшаем степень вершины prufer[i]
        degree[prufer[i]]--;
        if (degree[prufer[i]] == 1) {
            is_leaf[prufer[i]] = true;
        }
    }

    // Находим две последние вершины со степенью 1
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

    // Добавляем последнее ребро
    tree.add_rebro(last1, last2);

    return tree;
}
