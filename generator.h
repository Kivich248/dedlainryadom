#include "graph.h"
#include "parser.h"
#include <random>
#include <set>
#include <algorithm>
Graph generate_full(size_t n);

Graph generate_full_twodol(size_t n, size_t m);

Graph generate_path(size_t n);

Graph generate_star(size_t n);

Graph generate_cycle(size_t n);

Graph generate_wheel(size_t m);

Graph generate_random(size_t n, size_t m);

Graph generate_tree(size_t n);



