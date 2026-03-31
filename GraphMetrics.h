#pragma once

#include "Graph.h"
#include <string>
#include <vector>

using namespace std;

// ============================================================================
// БАЗОВЫЙ КЛАСС ДЛЯ МЕТРИКИ (как GraphParser для парсеров)
// ============================================================================
class GraphMetric {
public:
    virtual ~GraphMetric();                      // 1. Деструктор
    virtual void compute(const Graph& graph) = 0; // 2. Виртуальная функция вычисления
    virtual string get_result() const = 0;        // Результат (строка)
    
    // 3. Статическая фабрика (выбирает нужную по строке)
    static GraphMetric* sozdat_metric(const string& name);
};

// ============================================================================
// МЕТРИКА: ПЛОТНОСТЬ
// ============================================================================
class Metric_Plotnost : public GraphMetric {
private:
    double value;
    bool computed;
public:
    Metric_Plotnost();
    void compute(const Graph& graph) override;
    string get_result() const override;
};

// ============================================================================
// МЕТРИКА: ДИАМЕТР
// ============================================================================
class Metric_Diametr : public GraphMetric {
private:
    size_t value;
    bool computed;
public:
    Metric_Diametr();
    void compute(const Graph& graph) override;
    string get_result() const override;
};

// ============================================================================
// МЕТРИКА: КОМПОНЕНТЫ СВЯЗНОСТИ
// ============================================================================
class Metric_Komponenty : public GraphMetric {
private:
    size_t value;
    bool computed;
public:
    Metric_Komponenty();
    void compute(const Graph& graph) override;
    string get_result() const override;
};

// ... ещё 5 метрик по аналогии ...