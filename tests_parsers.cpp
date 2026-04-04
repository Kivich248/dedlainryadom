#include <sstream>

#include "catch_amalgamated.hpp"
#include "parser.h"
#include "graph.h"
#include "generator.h"
#include "graphmetrics.h"


using namespace std;

TEST_CASE("Parser: DIMACS Round-Trip", "[parser][invariant]") {
	Graph original = generate_full(5);

	// Сериализуем вручную в DIMACS формат
	ostringstream oss;
	oss << "p edge " << original.count_vershiny() << " " << original.count_rebra() << "\n";
	for (const auto& e : original.get_vse_rebra()) {

		oss << "e " << (e.from + 1) << " " << (e.to + 1) << "\n";
	}

	// Парсим обратно
	istringstream iss(oss.str());
	Graph restored;
	DIMACSParser parser;
	parser.parse(iss, restored);

	// Инвариант: Графы изоморфны (совпадают V и E)
	CHECK(restored.count_vershiny() == original.count_vershiny());
	CHECK(restored.count_rebra() == original.count_rebra());

	// Проверка связности
	Metric_Komponenty_Svyaznosti m_orig, m_rest;
	m_orig.compute(original);
	m_rest.compute(restored);
	CHECK(m_rest.get_count() == m_orig.get_count());
}

TEST_CASE("Parser: SNAP Comments Handling", "[parser][format]") {
	string input_data =
		"# Это комментарий\n"
		"0 1\n"
		"# Еще комментарий\n"
		"1 2\n"
		"2 0\n";

	istringstream iss(input_data);
	Graph g;
	SNAPParser parser;
	parser.parse(iss, g);

	CHECK(g.count_vershiny() == 3); // Вершины 0, 1, 2
	CHECK(g.count_rebra() == 3);    // Треугольник
}