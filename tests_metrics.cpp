#include "catch_amalgamated.hpp"
#include "graph.h"
#include "graphmetrics.h"
#include "generator.h"

using namespace std;
using Catch::Detail::Approx;


TEST_CASE("Metric: Density of Complete Graph", "[metrics][invariant]") {
	size_t n = 10;
	Graph g = generate_full(n);

	Metric_Plotnost metric;
	metric.compute(g);

	// Плотность полного графа всегда 1.0
	CHECK(metric.get_value() == Approx(1.0).epsilon(0.001));
}

TEST_CASE("Metric: Diameter of Path vs Cycle", "[metrics][invariant]") {
	size_t n = 10;

	Graph path = generate_path(n);
	Graph cycle = generate_cycle(n);

	Metric_Diametr d_path, d_cycle;
	d_path.compute(path);
	d_cycle.compute(cycle);

	// Диаметр пути Pn = n-1
	CHECK(d_path.get_value() == n - 1);

	// Диаметр цикла Cn = floor(n/2)
	CHECK(d_cycle.get_value() == n / 2);
}

TEST_CASE("Metric: Bipartiteness", "[metrics][invariant]") {
	Graph cycle_even = generate_cycle(6);
	Graph cycle_odd = generate_cycle(5);
	Graph full = generate_full(4); // K4 не двудольный

	Metric_Dvudolnost m_even, m_odd, m_full;
	m_even.compute(cycle_even);
	m_odd.compute(cycle_odd);
	m_full.compute(full);

	CHECK(m_even.get_is_bipartite() == true);
	CHECK(m_odd.get_is_bipartite() == false);
	CHECK(m_full.get_is_bipartite() == false);
}

TEST_CASE("Metric: Chromatic Number Bounds", "[metrics][invariant]") {
	Graph full = generate_full(5); // Хроматическое число = 5
	Graph bipartite = generate_star(10); // Хроматическое число = 2

	Metric_Hromaticheskoe_Chislo m_full, m_bip;
	m_full.compute(full);
	m_bip.compute(bipartite);

	// Жадный алгоритм на полном графе даст точную оценку
	CHECK(m_full.get_upper_bound() == 5);

	// На двудольном (звезде) тоже должен дать 2 (если центр первый)
	CHECK(m_bip.get_upper_bound() <= 2);
}