#pragma once  //хуйня чтобы не было двойного включения потом

#include <iostream>  //пусть они тут будут пока
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Graph; //объявляем его существование чисто, мб в инклудах нужно написать откуда брали


class GraphParser {
public:
    virtual ~GraphParser();

    virtual void parse(istream& input, Graph& graph) = 0; //эта функция определяется 4мя способами в зависимости от результата sozdat_parcer

    static GraphParser* sozdat_parser(const string& format); //выбираем парсер и создаем
};

// ПАРСЕР СПИСКА РЕБЕР (Edge List)
// Формат: каждая строка содержит два числа u v (ребро между вершинами u и v)

class EdgeListParser : public GraphParser {
public:
    void parse(istream& input, Graph& graph) override;
};

// ============================================================================
// ПАРСЕР МАТРИЦЫ СМЕЖНОСТИ (Adjacency Matrix)
// Формат: первая строка - размер N, затем N строк по N чисел (0 или 1)

class MatrixParser : public GraphParser {
public:
    void parse(istream& input, Graph& graph) override;
};

// ============================================================================
// ПАРСЕР DIMACS (Coloring Instances)
// Формат: строки начинаются с c (коммент), p (проблема), e (ребро), n (вершина)
// Пример:
// c Это комментарий
// p edge 5 6
// e 1 2
// e 2 3
// ============================================================================
class DIMACSParser : public GraphParser {
public:
    void parse(istream& input, Graph& graph) override;
};

// ============================================================================
// ПАРСЕР SNAP (Stanford Network Analysis Project)
// Формат: комментарии начинаются с #, остальное - ребра u v
// Пример:
// # Social network
// 0 1
// 0 2
// 1 2
// ============================================================================
class SNAPParser : public GraphParser {
public:
    void parse(istream& input, Graph& graph) override;
};