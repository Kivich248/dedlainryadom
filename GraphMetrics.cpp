#include "GraphMetrics.h"

// ============================================================================
// БАЗОВЫЙ КЛАСС
// ============================================================================

GraphMetric::~GraphMetric() {}

GraphMetric* GraphMetric::sozdat_metric(const string& name)
{
	// 3. Функция выбирающая нужную по строке (как sozdat_parser)
	if (name == "plotnost")
	{
		return new Metric_Plotnost();
	}
	else if (name == "diametr")
	{
		return new Metric_Diametr();
	}
	else if (name == "komponenty")
	{
		return new Metric_Komponenty();
	}
	// ... остальные метрики ...
	return nullptr;
}

// ============================================================================
// МЕТРИКА: ПЛОТНОСТЬ
// ============================================================================
Metric_Plotnost::Metric_Plotnost() : value(0.0), computed(false) {}

void Metric_Plotnost::compute(const Graph& graph)
{
	size_t n = graph.count_vershiny();
	size_t m = graph.count_rebra();
	if (n <= 1)
	{
		value = 0.0;
	} else
	{
		value = (2.0 * m) / (static_cast<double>(n) * (n - 1));
	}
	computed = true;
}

string Metric_Plotnost::get_result() const
{
	return to_string(value);
}

// ============================================================================
// МЕТРИКА: ДИАМЕТР
// ============================================================================

Metric_Diametr::Metric_Diametr() : value(0), computed(false) {}

void Metric_Diametr::compute(const Graph& graph) {
	// ... твой BFS алгоритм ...
	computed = true;
}

string Metric_Diametr::get_result() const {
	return "Diametr: " + to_string(value);
}


// ============================================================================
// МЕТРИКА: КОМПОНЕНТЫ СВЯЗНОСТИ
// ============================================================================

Metric_Komponenty::Metric_Komponenty() : value(0), computed(false) {}  // ← Инициализация!

void Metric_Komponenty::compute(const Graph& graph) {
	if (computed) {
		return;
	}
	// ... твой алгоритм ...
	computed = true;
}


string Metric_Komponenty::get_result() const {
	if (!computed) {
		return "Not computed";
	}
	return "Komponenty svyaznosti: " + to_string(value);
}

// ... остальные метрики по аналогии ...