#pragma once

#include "graph.h"
#include <random>
#include <set>
#include <algorithm>


Graph generate_full(size_t n);                          // 1. Полный граф Kn
Graph generate_full_twodol(size_t n, size_t m);         // 2. Двудольный Knm
Graph generate_tree(size_t n);                          // 3. Дерево Tn
Graph generate_star(size_t n);                          // 4. Звезда Sn
Graph generate_cycle(size_t n);                         // 5. Цикл Cn
Graph generate_path(size_t n);                          // 6. Путь Pn
Graph generate_wheel(size_t n);                         // 7. Колесо Wn
Graph random_veroyatnost(size_t n, double p);           // 8. G(n, p)
Graph generate_random_cubic(size_t n);                  // 9. Кубический граф
Graph generate_graph_with_components(size_t n, size_t k);           // 10. Компоненты связности
Graph generate_graph_with_bridges_path_blobs_random(size_t n, size_t m);  // 11. Мосты
Graph generate_graph_with_articulations_path_blobs_random(size_t n, size_t num_articulations);  // 12. Точки сочленения
Graph generate_graph_with_2bridges(size_t n, size_t k);
// Graph generate_graph_with_2bridges(size_t n, size_t num_2bridges);  // 13. 2-мосты (отложено)
// Graph generate_graph_Halina(size_t variant = 1);  // 14. Граф Халина (отложено)

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ (ОПЦИОНАЛЬНО, ЕСЛИ НУЖНЫ ВНЕ generator.cpp)
// ============================================================================
Graph generate_2connected_graph_random(size_t n, unsigned int seed = 42);  // 2-связный граф
