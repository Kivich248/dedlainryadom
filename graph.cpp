#include "graph.h"
#include <random>
#include <queue>
#include <utility> // для move

using namespace std;


// РЕАЛИЗАЦИЯ СТРУКТУРЫ VERSHINA (к сожалению мой напарник плохо знает английский) (за то всем все сразу понятно)
// это конструкторы структуры

Vershina::Vershina() : color("black"), label(""), isArticulation(false), component(-1), biconnectedComponent(-1) {}

Vershina::Vershina(const string& col) : color(col), label(""), isArticulation(false), component(-1), biconnectedComponent(-1) {}


// РЕАЛИЗАЦИЯ СТРУКТУРЫ REBRO (очень плохо) (очень хорошо на самом деле)
// конструкторы ребер

Rebro::Rebro(size_t u, size_t v) : from(u), to(v), color("black"), isBridge(false), edgeBiconnectedComponent(-1) {}

Rebro::Rebro(size_t u, size_t v, const string& col) : from(u), to(v), color(col), isBridge(false), edgeBiconnectedComponent(-1) {}

size_t Rebro::other(size_t v) const
{
    return (v == from) ? to : from;
}

bool Rebro::incidentTo(size_t v) const
{
    return v == from || v == to;
}


// всякие методы


void Graph::expand_rebro_index()
{
    size_t newSize = vershini.size();
    vector<int> new_row;

    for (size_t i = 0; i < rebro_index.size(); i++)
    {
        rebro_index[i].push_back(-1);
    }

    for (size_t j = 0; j < newSize; j++)
    {
        new_row.push_back(-1);
    }

    rebro_index.push_back(new_row);
}


// конструкторы графа


Graph::Graph() = default;

Graph::Graph(size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        vershini.push_back(Vershina());
        inchedent_rebra.push_back(vector<size_t>());
    }

    for (size_t i = 0; i < n; i++)
    {
        vector<int> new_row;
        for (size_t j = 0; j < n; j++)
        {
            new_row.push_back(-1);
        }
        rebro_index.push_back(new_row);
    }
}

Graph::Graph(const Graph& other)
    : inchedent_rebra(other.inchedent_rebra),
      vershini(other.vershini),
      rebro(other.rebro),
      rebro_index(other.rebro_index) {}

Graph& Graph::operator=(const Graph& other)
{
    if (this != &other)
    {
        inchedent_rebra = other.inchedent_rebra;
        vershini = other.vershini;
        rebro = other.rebro;
        rebro_index = other.rebro_index;
    }
    return *this;
}


// всякое делаем с вершинами


void Graph::add_vershina()
{
    vershini.emplace_back();
    inchedent_rebra.emplace_back();
    expand_rebro_index();
}

void Graph::add_vershiny(size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        add_vershina();
    }
}

bool Graph::has_vershina(size_t v) const
{
    return v < vershini.size();
}

size_t Graph::count_vershiny() const
{
    return vershini.size();
}

vector<size_t> Graph::get_vershiny() const
{
    vector<size_t> result(vershini.size());
    for (size_t i = 0; i < vershini.size(); i++)
    {
        result[i] = i;
    }
    return result;
}

void Graph::remove_vershina(size_t m)
{
    if (m >= vershini.size())
    {
        throw out_of_range("Vershina s indexom " + to_string(m) + " ne suschestvuet");
    }

    // кикаем инцидентные ребра
    vector<Rebro> filtered_rebro;
    for (size_t i = 0; i < rebro.size(); i++)
    {
        const Rebro& e = rebro[i];
        if (e.from != m && e.to != m)
        {
            filtered_rebro.push_back(e);
        }
    }
    rebro = filtered_rebro;

    // сдвигаем индексы в оставшихся рёбрах
    for (size_t i = 0; i < rebro.size(); i++)
    {
        Rebro& e = rebro[i];
        if (e.from > m) e.from--;
        if (e.to > m) e.to--;
    }

    size_t newSize = vershini.size() - 1;

    // перестраиваем inchedent_rebra
    inchedent_rebra.clear();
    for (size_t i = 0; i < newSize; i++)
    {
        inchedent_rebra.push_back(vector<size_t>());
    }

    for (size_t i = 0; i < rebro.size(); i++)
    {
        const Rebro& e = rebro[i];
        inchedent_rebra[e.from].push_back(i);
        inchedent_rebra[e.to].push_back(i);
    }

    // перестраиваем rebro_index
    rebro_index.clear();
    for (size_t i = 0; i < newSize; i++)
    {
        vector<int> new_row(newSize, -1);
        rebro_index.push_back(new_row);
    }

    for (size_t i = 0; i < rebro.size(); i++)
    {
        const Rebro& e = rebro[i];
        rebro_index[e.from][e.to] = static_cast<int>(i);
        rebro_index[e.to][e.from] = static_cast<int>(i);
    }

    // удаляем вершинку
    vershini.erase(vershini.begin() + static_cast<long>(m));
}


// поля вершин


void Graph::set_color_vershiny(size_t v, const string& color)
{
    if (v >= vershini.size()) throw out_of_range("Index vershiny out of range");
    vershini[v].color = color;
}

string Graph::get_color_vershiny(size_t v) const
{
    if (v >= vershini.size()) throw out_of_range("Index vershiny out of range");
    return vershini[v].color;
}

void Graph::set_label_vershiny(size_t v, const string& label)
{
    if (v >= vershini.size()) throw out_of_range("Index vershiny out of range");
    vershini[v].label = label;
}

string Graph::get_label_vershiny(size_t v) const
{
    if (v >= vershini.size()) throw out_of_range("Index vershiny out of range");
    return vershini[v].label;
}


void Graph::set_tochka_sochleneniya(size_t v, bool value)
{
    if (v >= vershini.size()) throw out_of_range("Index vershiny out of range");
    vershini[v].isArticulation = value;
}

bool Graph::is_tochka_sochleneniya(size_t v) const
{
    if (v >= vershini.size()) throw out_of_range("Index vershiny out of range");
    return vershini[v].isArticulation;
}

void Graph::set_component_vershiny(size_t v, int comp)
{
    if (v >= vershini.size()) throw out_of_range("Index vershiny out of range");
    vershini[v].component = comp;
}

int Graph::get_component_vershiny(size_t v) const
{
    if (v >= vershini.size()) throw out_of_range("Index vershiny out of range");
    return vershini[v].component;
}


// делаем всякое с ребрами


void Graph::add_rebro(size_t u, size_t v)
{
    if (u >= vershini.size() || v >= vershini.size())
        throw out_of_range("Index vershiny out of range");

    if (has_rebro(u, v)) return;

    size_t edgeId = rebro.size();
    rebro.emplace_back(u, v);

    inchedent_rebra[u].push_back(edgeId);
    inchedent_rebra[v].push_back(edgeId);

    rebro_index[u][v] = static_cast<int>(edgeId);
    rebro_index[v][u] = static_cast<int>(edgeId);
}

bool Graph::has_rebro(size_t u, size_t v) const
{
    if (u >= vershini.size() || v >= vershini.size()) return false;
    return rebro_index[u][v] != -1;
}

size_t Graph::count_rebra() const
{
    return rebro.size();
}

Rebro& Graph::get_rebro(size_t u, size_t v)
{
    if (!has_rebro(u, v)) throw runtime_error("Rebro ne suschestvuet");
    return rebro[rebro_index[u][v]];
}

const Rebro& Graph::get_rebro_po_id(size_t id) const
{
    if (id >= rebro.size()) throw out_of_range("Index rebra out of range");
    return rebro[id];
}

Rebro& Graph::get_rebro_po_id(size_t id)
{
    if (id >= rebro.size()) throw out_of_range("Index rebra out of range");
    return rebro[id];
}

const vector<Rebro>& Graph::get_vse_rebra() const
{
    return rebro;
}


// поля ребер


void Graph::set_color_rebra(size_t u, size_t v, const string& color)
{
    if (has_rebro(u, v)) rebro[rebro_index[u][v]].color = color;
}

string Graph::get_color_rebra(size_t u, size_t v) const
{
    if (has_rebro(u, v)) return rebro[rebro_index[u][v]].color;
    return "black";
}

// В .cpp БЕЗ дефолтного значения
void Graph::set_most(size_t u, size_t v, bool value)
{
    if (has_rebro(u, v)) rebro[rebro_index[u][v]].isBridge = value;
}

bool Graph::is_most(size_t u, size_t v) const
{
    if (has_rebro(u, v)) return rebro[rebro_index[u][v]].isBridge;
    return false;
}

void Graph::set_component_rebra(size_t u, size_t v, int comp)
{
    if (has_rebro(u, v)) rebro[rebro_index[u][v]].edgeBiconnectedComponent = comp;
}

int Graph::get_component_rebra(size_t u, size_t v) const
{
    if (has_rebro(u, v)) return rebro[rebro_index[u][v]].edgeBiconnectedComponent;
    return -1;
}

vector<vector<size_t>> Graph::get_components() const
{
    vector<vector<size_t>> components;
    vector<bool> visited(count_vershiny(), false);

    for (size_t v = 0; v < count_vershiny(); v++)
    {
        if (!visited[v])
        {
            vector<size_t> component;
            queue<size_t> q;
            visited[v] = true;
            q.push(v);

            while (!q.empty())
            {
                size_t current = q.front();
                q.pop();
                component.push_back(current);

                for (size_t neighbor : get_sosedi(current))
                {
                    if (!visited[neighbor])
                    {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
            components.push_back(component);
        }
    }
    return components;
}


// а что вокруг


vector<size_t> Graph::get_sosedi(size_t v) const
{
    vector<size_t> neighbors;
    if (v >= inchedent_rebra.size()) return neighbors;

    neighbors.reserve(inchedent_rebra[v].size());
    for (size_t edgeId : inchedent_rebra[v])
    {
        const Rebro& e = rebro[edgeId];
        neighbors.push_back(e.other(v));
    }
    return neighbors;
}

const vector<size_t>& Graph::get_incid_rebra(size_t v) const
{
    static const vector<size_t> empty;
    if (v >= inchedent_rebra.size()) return empty;
    return inchedent_rebra[v];
}

size_t Graph::stepen(size_t v) const
{
    return (v < inchedent_rebra.size()) ? inchedent_rebra[v].size() : 0;
}

bool Graph::is_list(size_t v) const { return stepen(v) == 1; }
bool Graph::is_izolirovannaya(size_t v) const { return stepen(v) == 0; }


// УПРАВЛЕНИЕ ГРАФОМ


void Graph::clear()
{
    inchedent_rebra.clear();
    vershini.clear();
    rebro.clear();
    rebro_index.clear();
}

bool Graph::pust() const { return vershini.empty(); }

Graph Graph::kopiya() const { return Graph(*this); }

Graph Graph::operator+(const Graph& other) const
{
    Graph result = *this;
    size_t current_vertices = count_vershiny();
    size_t other_vertices = other.count_vershiny();

    for (size_t i = 0; i < other_vertices; i++) result.add_vershina();

    for (const auto& e : other.get_vse_rebra())
    {
        size_t new_from = e.from + current_vertices;
        size_t new_to = e.to + current_vertices;
        result.add_rebro(new_from, new_to);
        result.set_color_rebra(new_from, new_to, e.color);
        if (e.isBridge) result.set_most(new_from, new_to, true);
    }

    for (size_t i = 0; i < other_vertices; i++)
    {
        size_t new_index = current_vertices + i;
        result.set_color_vershiny(new_index, other.get_color_vershiny(i));
        result.set_label_vershiny(new_index, other.get_label_vershiny(i));
        if (other.is_tochka_sochleneniya(i)) result.set_tochka_sochleneniya(new_index, true);
    }

    return result;
}

void Graph::random_renumber()
{
    if (vershini.empty()) return;

    size_t n = vershini.size();
    vector<size_t> permutation(n);
    for (size_t i = 0; i < n; i++) permutation[i] = i;

    random_device rd;
    mt19937 gen(rd());
    shuffle(permutation.begin(), permutation.end(), gen);

    vector<size_t> old_to_new(n);
    for (size_t i = 0; i < n; i++) old_to_new[permutation[i]] = i;

    vector<Vershina> new_vershini(n);
    for (size_t old_v = 0; old_v < n; old_v++)
    {
        size_t new_v = old_to_new[old_v];
        new_vershini[new_v] = vershini[old_v];
    }
    vershini = move(new_vershini);

    vector<Rebro> new_rebro;
    vector<vector<size_t>> new_inchedent_rebra(n);
    vector<vector<int>> new_rebro_index(n, vector<int>(n, -1));

    for (const auto& e : rebro)
    {
        size_t new_from = old_to_new[e.from];
        size_t new_to = old_to_new[e.to];

        size_t new_edge_id = new_rebro.size();
        new_rebro.emplace_back(new_from, new_to);
        new_rebro.back().color = e.color;
        new_rebro.back().isBridge = e.isBridge;
        new_rebro.back().edgeBiconnectedComponent = e.edgeBiconnectedComponent;

        new_inchedent_rebra[new_from].push_back(new_edge_id);
        new_inchedent_rebra[new_to].push_back(new_edge_id);
        new_rebro_index[new_from][new_to] = static_cast<int>(new_edge_id);
        new_rebro_index[new_to][new_from] = static_cast<int>(new_edge_id);
    }

    rebro = move(new_rebro);
    inchedent_rebra = move(new_inchedent_rebra);
    rebro_index = move(new_rebro_index);
}


// ОТЛАДКА


void Graph::print() const
{
    cout << "========================================\n";
    cout << "Graph: " << count_vershiny() << " vertices, " << count_rebra() << " edges\n";
    cout << "========================================\n";

    cout << "\nVertices:\n";
    for (size_t i = 0; i < vershini.size(); i++)
    {
        cout << "  " << i << ": color=" << vershini[i].color;
        if (vershini[i].isArticulation) cout << " [ARTICULATION]";
        cout << "\n";
    }

    cout << "\nEdges:\n";
    for (const auto& e : rebro)
    {
        cout << "  " << e.from << " -- " << e.to;
        if (e.isBridge) cout << " [BRIDGE]";
        cout << "\n";
    }
    cout << "========================================\n";
}

bool Graph::validate() const
{
    size_t totalEdgeRefs = 0;
    for (const auto& list : inchedent_rebra) totalEdgeRefs += list.size();
    if (totalEdgeRefs != rebro.size() * 2) return false;

    for (size_t i = 0; i < vershini.size(); i++)
    {
        for (size_t j = 0; j < vershini.size(); j++)
        {
            int idx = rebro_index[i][j];
            if (idx != -1)
            {
                if (idx >= static_cast<int>(rebro.size())) return false;
                const Rebro& e = rebro[idx];
                if (!((e.from == i && e.to == j) || (e.from == j && e.to == i))) return false;
            }
        }
    }
    return true;
}