#include "parser.h"
#include "Graph.h"
#include <stdexcept>

GraphParser::~GraphParser() {} //хуйня для корректной очистки памяти в мэйне иначе все плохо, называется деструктор, внутри кода быть не должно, за это отвечает ~

GraphParser* GraphParser::sozdat_parser(const string& format) //выбираем формат парсера по строке, дальше непосредственно уже парсим
{
    string type = format;
    transform(type.begin(), type.end(), type.begin(), ::tolower); //понизили регистр

    if (type == "edgelist" || type == "edge")
    {
        return new EdgeListParser(); //new что-то вроде выделения памяти
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

void EdgeListParser::parse(istream& input, Graph& graph)
{
    vector<size_t> vse_chisla;  // Храним все числа
    size_t chislo; //переменная числа
    size_t max_vershina = -1; //первая вершина точно будет больше

    while (input >> chislo)
    {
        vse_chisla.push_back(chislo + 1); // пока что просто +1 чтобы 0 -> 1, можно потом просто запретить ввод 0
        max_vershina = max(max_vershina, chislo);
    }

    // Шаг 2: Проверяем четность количества чисел
    if (vse_chisla.size() % 2 != 0)
    {
        throw invalid_argument("Ошибка формата: нечетное количество чисел (" + to_string(vse_chisla.size()) + "). Ожидаются пары u v.");
    }

    // Шаг 3: Создаем все вершины (до добавления ребер)
    for (size_t i = 0; i <= max_vershina; i++)
    {
        graph.add_vershina(); //надо прописать код в файле с классом графа
    }

    // Шаг 4: Добавляем ребра (цикл с шагом 2)
    for (size_t i = 0; i < vse_chisla.size(); i += 2)
    {
        size_t u = vse_chisla[i];
        size_t v = vse_chisla[i + 1];
        graph.add_rebro(u + 1, v + 1);  //0 -> 1
    }
}

void MatrixParser::parse(istream& input, Graph& graph)
{
    size_t n;
    input >> n; // размер таблицы = число вершин
    
    // создаем вершины
    for (size_t i = 0; i < n; i++)
    {
        graph.add_vershina();
    }
    
    // читаем и добавляем
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            int value;
            input >> value;
            // Если в матрице 1 - значит есть ребро из i в j
            if (value == 1)
            {
                graph.add_rebro(i, j);
            }
        }
    }
}


void DIMACSParser::parse(istream& input, Graph& graph)
{
    string stroka;  //сюда получаем одну строку
    size_t chislo_vershin = 0;
    vector<pair<size_t, size_t>> rebra;
    bool zagolovok_nayden = false;  // Флаг: нашли ли строку "p"

    while (getline(input, stroka))
    {
        if (stroka.empty()) continue;   // Пропускаем пустые строки

        size_t first_char_pos = stroka.find_first_not_of(" \t");   // Пропускаем ведущие пробелы для корректной проверки первого символа
        if (first_char_pos == string::npos) continue;  // Строка только из пробелов

        char tip = stroka[first_char_pos];

        if (tip == 'c')
        {
            continue;
        }
        if (tip == 'p')            // Строка проблемы: p edge <вершины> <ребра>
        {
            istringstream iss(stroka); //позволяет разбивать строку с помощью >>
            string p_word, edge_word;
            size_t chislo_reber;

            iss >> p_word >> edge_word >> chislo_vershin >> chislo_reber;

            string ostatok;
            if (iss >> ostatok)
            {
                throw invalid_argument("Ошибка формата: лишние данные в строке p заголовка.");
            }

            if (edge_word != "edge")
            {
                throw invalid_argument("Ошибка формата: ожидается 'edge' после 'p'.");
            }

            // Создаем вершины заранее
            for (size_t i = 0; i < chislo_vershin; i++)
            {
                graph.add_vershina();
            }

            zagolovok_nayden = true;
        }
        if (tip == 'n')            // Информация о вершине: n <id> <color>
        {
            istringstream iss(stroka);
            string n_word;
            size_t nomer, color;

            iss >> n_word >> nomer >> color;

            string ostatok;
            if (iss >> ostatok)
            {
                throw invalid_argument("Ошибка формата: лишние данные в строке вершины.");
            }
            //пока что ничего не делаю с цветом
            continue;
        }
        if (tip == 'e')            // Ребро: e <u> <v>
        {
            istringstream iss(stroka);
            string e_word;
            size_t u, v;

            iss >> e_word >> u >> v;

            string ostatok;
            if (iss >> ostatok)
            {
                throw invalid_argument("Ошибка формата: лишние данные в строке ребра.");
            }

            rebra.push_back({u - 1, v - 1});
        }
        else //возможно этот иф елс работает некорректно, он как будто будет ток к последнему ифу
        {
            throw invalid_argument("Ошибка формата: неизвестный тип строки '" + string(1, tip) + "'.");
        }
    }

    // Проверка что заголовок был найден
    if (!zagolovok_nayden)
    {
        throw invalid_argument("Ошибка формата: отсутствует строка заголовка 'p edge'.");
    }

    // Добавляем все ребра после создания вершин
    for (size_t i = 0; i < rebra.size(); i++)
    {
        graph.add_rebro(rebra[i].first, rebra[i].second);
    }
}

void SNAPParser::parse(istream& input, Graph& graph)
{
    string stroka;
    size_t max_vershina = 0;
    vector<pair<size_t, size_t>> rebra;
    size_t stroka_count = 0;  // Для статистики

    while (getline(input, stroka))
    {
        if (stroka.empty()) continue;

        // Пропускаем ведущие пробелы для проверки первого символа
        size_t first_char_pos = stroka.find_first_not_of(" \t");
        if (first_char_pos == string::npos) continue;  // Строка только из пробелов

        // Пропускаем комментарии
        if (stroka[first_char_pos] == '#') continue;

        // Парсинг ребра
        istringstream iss(stroka);
        long long temp_u, temp_v;  // long long для проверки отрицательных

        if (!(iss >> temp_u >> temp_v))
        {
            // Ошибка чтения - пропускаем строку или выбрасываем ошибку
            throw invalid_argument("Ошибка формата SNAP: не удалось прочитать ребро в строке " + to_string(stroka_count));
        }

        // Проверка на лишние данные в строке
        string ostatok;
        if (iss >> ostatok)
        {
            throw invalid_argument("Ошибка формата SNAP: лишние данные в строке ребра.");
        }

        // Проверка на отрицательные номера вершин
        if (temp_u < 0 || temp_v < 0)
        {
            throw invalid_argument("Ошибка формата SNAP: номера вершин не могут быть отрицательными. ""Получено: " + to_string(temp_u) + " " + to_string(temp_v));
        }

        // Проверка на слишком большие значения (защита от переполнения)
        if (temp_u > 100000000 || temp_v > 100000000)
        {
            throw invalid_argument("Ошибка формата SNAP: слишком большие номера вершин.");
        }

        rebra.push_back({static_cast<size_t>(temp_u), static_cast<size_t>(temp_v)});
        max_vershina = max(max_vershina, max(static_cast<size_t>(temp_u), static_cast<size_t>(temp_v)));
        stroka_count++;
    }

    // Создаем все вершины от 0 до max_vershina
    for (size_t i = 0; i <= max_vershina; i++)
    {
        graph.add_vershina();
    }

    // Добавляем все ребра
    for (size_t i = 0; i < rebra.size(); i++)
    {
        graph.add_rebro(rebra[i].first, rebra[i].second);
    }
}