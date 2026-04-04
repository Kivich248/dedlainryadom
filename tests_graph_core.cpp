#include <catch2/catch.hpp>
#include "graph.h"
#include <numeric>

using namespace std;

TEST_CASE("Graph Invariants: Handshaking Lemma", "[graph][core]") {
	Graph g = generate_random_cubic(10); // Кубический граф: у всех степень 3

	size_t sum_degrees = 0;
	for (size_t v = 0; v < g.count_vershiny(); v++) {
		sum_degrees += g.stepen(v);
	}

	// Инвариант: Сумма степеней равна удвоенному количеству ребер
	CHECK(sum_degrees == 2 * g.count_rebra());
}

TEST_CASE("Graph Remove Vertex: Re-indexing and Consistency", "[graph][core]") {
	Graph g = generate_full(5); // K5: 10 ребер, степень 4 у каждой
	size_t n_before = g.count_vershiny();

	g.remove_vershina(2); // Удаляем вершину 2

	// Инвариант: Количество вершин уменьшилось на 1
	CHECK(g.count_vershiny() == n_before - 1);

	// Инвариант: Оставшиеся вершины имеют степень n-2 (было 4, стало 3)
	for (size_t v = 0; v < g.count_vershiny(); v++) {
		CHECK(g.stepen(v) == g.count_vershiny() - 1);
	}

	// Инвариант: Граф остался полным (K4)
	size_t expected_edges = (g.count_vershiny() * (g.count_vershiny() - 1)) / 2;
	CHECK(g.count_rebra() == expected_edges);

	// Проверка валидации внутренней структуры
	CHECK(g.validate() == true);
}

TEST_CASE("Graph Union Operator: Disjoint Components", "[graph][core]") {
	Graph g1 = generate_cycle(3);
	Graph g2 = generate_cycle(4);

	Graph g3 = g1 + g2;

	CHECK(g3.count_vershiny() == 7);
	CHECK(g3.count_rebra() == 3 + 4);

	// Инвариант: Компонент связности должно быть 2
	auto comps = g3.get_components();
	CHECK(comps.size() == 2);
}