#include "parser.h"
#include "graph.h" // Подключаем определение Graph
#include <stdexcept>
#include <climits>
#include <utility>

using namespace std;

// ============================================================================
// БАЗОВЫЙ КЛАСС
// ============================================================================

GraphParser::~GraphParser() {}

GraphParser* GraphParser::sozdat_parser(const string& format)
{
    string type = format;
    transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "edgelist" || type == "edge")
    {
        return new EdgeListParser();
    }
    if (type == "matrix" || type == "adjacency")
    {
        return new MatrixParser();
    }
    if (type == "dimacs" || type == "col")
    {
        return new DIMACSParser();
    }
    if (type == "snap" || type == "networkx")
    {
        return new SNAPParser();
    }

    return nullptr;
}

// ============================================================================
// EDGE LIST PARSER
// Предполагаем формат: u v (0-based или 1-based? Сделаем универсально: если встречаем 0, то 0-based, иначе 1-based)
// Но для простоты лабы часто делают строго 1-based вход -> 0-based внутри.
// Исправление: считаем вход 1-based (как в DIMACS), вычитаем 1.
// ============================================================================

void EdgeListParser::parse(istream& input, Graph& graph)
{
    vector<pair<size_t, size_t>> rebra;
    size_t u_in, v_in;
    size_t max_vershina = 0;

    // Читаем пары чисел
    while (input >> u_in >> v_in)
    {
        // Конвертация 1-based (файл) -> 0-based (граф)
        // Если в файле могут быть 0, то логику нужно усложнить.
        // Для лабы обычно предполагаем 1-based ввод для всех форматов кроме SNAP (там 0-based).
        // Но EdgeList часто 0-based. Давай сделаем так: если встречаем 0, считаем 0-based.
        // НО в твоем коде было +1, значит ты ожидал 0-based вход?
        // Давай сделаем строго как в DIMACS: вход 1-based -> вычитаем 1.

        if (u_in == 0 || v_in == 0) {
             // Если вдруг 0, то оставляем как есть (0-based)
             // Но лучше кидать ошибку или предупреждение.
             // Для унификации: пусть вход всегда 1-based для EdgeList в этой лабе.
             throw invalid_argument("EdgeList parser ozhidaet nomera vershin s 1. Nayden 0.");
        }

        size_t u = u_in - 1;
        size_t v = v_in - 1;

        rebra.push_back({u, v});
        if (u > max_vershina) max_vershina = u;
        if (v > max_vershina) max_vershina = v;
    }

    // Создаем вершины
    for (size_t i = 0; i <= max_vershina; i++)
    {
        graph.add_vershina(); // Исправлено имя метода
    }

    // Добавляем ребра
    for (const auto& edge : rebra)
    {
        graph.add_rebro(edge.first, edge.second); // Исправлено имя метода
    }
}

// ============================================================================
// MATRIX PARSER
// Вход: N, затем NxN матрица (0/1). Индексы неявные (0..N-1).
// ============================================================================

void MatrixParser::parse(istream& input, Graph& graph)
{
    size_t n;
    if (!(input >> n)) {
        throw invalid_argument("Ne udalos prochitat razmer matritsy");
    }

    for (size_t i = 0; i < n; i++)
    {
        graph.add_vershina();
    }

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            int value;
            input >> value;
            if (value == 1)
            {
                graph.add_rebro(i, j);
            }
        }
    }
}

// ============================================================================
// DIMACS PARSER
// Вход 1-based. Конвертируем в 0-based.
// ============================================================================

void DIMACSParser::parse(istream& input, Graph& graph)
{
    string stroka;
    size_t chislo_vershin = 0;
    vector<pair<size_t, size_t>> rebra;
    bool zagolovok_nayden = false;

    while (getline(input, stroka))
    {
        if (stroka.empty()) continue;

        size_t first_char_pos = stroka.find_first_not_of(" \t");
        if (first_char_pos == string::npos) continue;

        char tip = stroka[first_char_pos];

        if (tip == 'c')
        {
            continue;
        }
        else if (tip == 'p')
        {
            istringstream iss(stroka);
            string p_word, edge_word;
            size_t chislo_reber;

            iss >> p_word >> edge_word >> chislo_vershin >> chislo_reber;

            if (edge_word != "edge")
            {
                throw invalid_argument("Oshibka formata DIMACS: ozhidaetsya 'edge'.");
            }

            for (size_t i = 0; i < chislo_vershin; i++)
            {
                graph.add_vershina();
            }
            zagolovok_nayden = true;
        }
        else if (tip == 'n')
        {
            // Пропускаем информацию о цветах вершин (можно добавить позже)
            continue;
        }
        else if (tip == 'e')
        {
            istringstream iss(stroka);
            string e_word;
            size_t u_in, v_in;

            iss >> e_word >> u_in >> v_in;

            // Конвертация 1-based -> 0-based
            if (u_in == 0 || v_in == 0) {
                 throw invalid_argument("DIMACS: nomera vershin dolzhny byt >= 1");
            }
            rebra.push_back({u_in - 1, v_in - 1});
        }
        else
        {
            // Неизвестный символ в начале строки
            throw invalid_argument("Oshibka formata DIMACS: neizvestny tip stroki '" + string(1, tip) + "'.");
        }
    }

    if (!zagolovok_nayden)
    {
        throw invalid_argument("Oshibka formata DIMACS: otsutstvuyet stroka 'p edge'.");
    }

    for (const auto& edge : rebra)
    {
        graph.add_rebro(edge.first, edge.second);
    }
}

// ============================================================================
// SNAP PARSER
// Вход 0-based (обычно). Оставляем как есть.
// ============================================================================

void SNAPParser::parse(istream& input, Graph& graph)
{
    string stroka;
    size_t max_vershina = 0;
    vector<pair<size_t, size_t>> rebra;

    while (getline(input, stroka))
    {
        if (stroka.empty()) continue;

        size_t first_char_pos = stroka.find_first_not_of(" \t");
        if (first_char_pos == string::npos) continue;

        if (stroka[first_char_pos] == '#') continue;

        istringstream iss(stroka);
        long long temp_u, temp_v;

        if (!(iss >> temp_u >> temp_v))
        {
            continue; // Пропускаем битые строки, а не крашим весь парсер
        }

        if (temp_u < 0 || temp_v < 0)
        {
            throw invalid_argument("SNAP: otritsatelnye nomera vershin.");
        }

        size_t u = static_cast<size_t>(temp_u);
        size_t v = static_cast<size_t>(temp_v);

        rebra.push_back({u, v});
        if (u > max_vershina) max_vershina = u;
        if (v > max_vershina) max_vershina = v;
    }

    // Создаем вершины (SNAP может иметь дырки в нумерации, но мы создаем до max)
    for (size_t i = 0; i <= max_vershina; i++)
    {
        graph.add_vershina();
    }

    for (const auto& edge : rebra)
    {
        graph.add_rebro(edge.first, edge.second);
    }
}