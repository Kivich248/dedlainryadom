#include "GraphMetrics.h"

using namespace std;

// ============================================================================
// GraphMetric (базовый класс)
// ============================================================================

GraphMetric::~GraphMetric() {}

GraphMetric* GraphMetric::sozdat_metric(const string& name) {
    if (name == "plotnost") {
        return new Metric_Plotnost();
    }
    if (name == "diametr") {
        return new Metric_Diametr();
    }
    if (name == "transitivnost") {
        return new Metric_Transitivnost();
    }
    if (name == "komponenty_svyaznosti") {
        return new Metric_Komponenty_Svyaznosti();
    }
    if (name == "tochki_sochleneniya") {
        return new Metric_Tochki_Sochleneniya();
    }
    if (name == "mosty") {
        return new Metric_Mosty();
    }
    if (name == "dvudolnost") {
        return new Metric_Dvudolnost();
    }
    if (name == "hromaticheskoe_chislo") {
        return new Metric_Hromaticheskoe_Chislo();
    }
    return nullptr;
}

vector<string> GraphMetric::get_available_metrics() {
    vector<string> metrics;
    metrics.push_back("plotnost");
    metrics.push_back("diametr");
    metrics.push_back("transitivnost");
    metrics.push_back("komponenty_svyaznosti");
    metrics.push_back("tochki_sochleneniya");
    metrics.push_back("mosty");
    metrics.push_back("dvudolnost");
    metrics.push_back("hromaticheskoe_chislo");
    return metrics;
}

// ============================================================================
// Metric_Plotnost
// ============================================================================

Metric_Plotnost::Metric_Plotnost() : value(0.0), computed(false) {}

void Metric_Plotnost::compute(const Graph& graph) {
    if (computed) return;

    size_t n = graph.count_vershiny();
    size_t m = graph.count_rebra();

    if (n <= 1) {
        value = 0.0;
    } else {
        value = (2.0 * static_cast<double>(m)) / (static_cast<double>(n) * (n - 1));
    }

    computed = true;
}

string Metric_Plotnost::get_result() const {
    if (!computed) return "Not computed";

    ostringstream oss;
    oss << fixed << setprecision(4);
    oss << "Plotnost: " << value;
    return oss.str();
}

bool Metric_Plotnost::is_computed() const {
    return computed;
}

void Metric_Plotnost::clear_cache() {
    computed = false;
    value = 0.0;
}

double Metric_Plotnost::get_value() const {
    return value;
}

// ============================================================================
// Metric_Diametr
// ============================================================================

Metric_Diametr::Metric_Diametr() : value(0), computed(false) {}

size_t Metric_Diametr::bfs_max_distance(const Graph& graph, size_t start) const {
    size_t n = graph.count_vershiny();
    vector<int> dist(n, -1);
    queue<size_t> q;

    dist[start] = 0;
    q.push(start);

    size_t max_dist = 0;

    while (!q.empty()) {
        size_t v = q.front();
        q.pop();

        if (dist[v] > static_cast<int>(max_dist)) {
            max_dist = dist[v];
        }

        vector<size_t> sosedi = graph.get_sosedi(v);
        for (size_t i = 0; i < sosedi.size(); i++) {
            size_t u = sosedi[i];
            if (dist[u] == -1) {
                dist[u] = dist[v] + 1;
                q.push(u);
            }
        }
    }

    return max_dist;
}

void Metric_Diametr::compute(const Graph& graph) {
    if (computed) return;

    size_t n = graph.count_vershiny();

    if (n == 0) {
        value = 0;
        computed = true;
        return;
    }

    Metric_Komponenty_Svyaznosti comp_metric;
    comp_metric.compute(graph);

    if (comp_metric.get_count() > 1) {
        value = 0;
    } else {
        size_t max_dist = 0;
        for (size_t start = 0; start < n; start++) {
            size_t dist = bfs_max_distance(graph, start);
            if (dist > max_dist) {
                max_dist = dist;
            }
        }
        value = max_dist;
    }

    computed = true;
}

string Metric_Diametr::get_result() const {
    if (!computed) return "Not computed";

    ostringstream oss;
    oss << "Diametr: " << value;
    return oss.str();
}

bool Metric_Diametr::is_computed() const {
    return computed;
}

void Metric_Diametr::clear_cache() {
    computed = false;
    value = 0;
}

size_t Metric_Diametr::get_value() const {
    return value;
}

// ============================================================================
// Metric_Transitivnost
// ============================================================================

Metric_Transitivnost::Metric_Transitivnost() : value(0.0), computed(false) {}

void Metric_Transitivnost::compute(const Graph& graph) {
    if (computed) return;

    size_t closed_triads = 0;
    size_t all_triads = 0;
    size_t n = graph.count_vershiny();

    for (size_t v = 0; v < n; v++) {
        vector<size_t> sosedi = graph.get_sosedi(v);
        size_t degree = sosedi.size();

        if (degree < 2) continue;

        size_t possible_pairs = degree * (degree - 1) / 2;
        all_triads += possible_pairs;

        for (size_t i = 0; i < sosedi.size(); i++) {
            for (size_t j = i + 1; j < sosedi.size(); j++) {
                if (graph.has_rebro(sosedi[i], sosedi[j])) {
                    closed_triads++;
                }
            }
        }
    }

    if (all_triads == 0) {
        value = 0.0;
    } else {
        value = static_cast<double>(closed_triads) / static_cast<double>(all_triads);
    }

    computed = true;
}

string Metric_Transitivnost::get_result() const {
    if (!computed) return "Not computed";

    ostringstream oss;
    oss << fixed << setprecision(4);
    oss << "Transitivnost: " << value;
    return oss.str();
}

bool Metric_Transitivnost::is_computed() const {
    return computed;
}

void Metric_Transitivnost::clear_cache() {
    computed = false;
    value = 0.0;
}

double Metric_Transitivnost::get_value() const {
    return value;
}

// ============================================================================
// Metric_Komponenty_Svyaznosti
// ============================================================================

Metric_Komponenty_Svyaznosti::Metric_Komponenty_Svyaznosti()
    : value(0), computed(false) {}

void Metric_Komponenty_Svyaznosti::compute(const Graph& graph) {
    if (computed) return;

    size_t n = graph.count_vershiny();
    component_ids.assign(n, -1);
    value = 0;

    for (size_t start = 0; start < n; start++) {
        if (component_ids[start] == -1) {
            queue<size_t> q;
            q.push(start);
            component_ids[start] = static_cast<int>(value);

            while (!q.empty()) {
                size_t v = q.front();
                q.pop();

                vector<size_t> sosedi = graph.get_sosedi(v);
                for (size_t i = 0; i < sosedi.size(); i++) {
                    size_t u = sosedi[i];
                    if (component_ids[u] == -1) {
                        component_ids[u] = static_cast<int>(value);
                        q.push(u);
                    }
                }
            }

            value++;
        }
    }

    computed = true;
}

string Metric_Komponenty_Svyaznosti::get_result() const {
    if (!computed) return "Not computed";

    ostringstream oss;
    oss << "Komponenty svyaznosti: " << value;
    return oss.str();
}

bool Metric_Komponenty_Svyaznosti::is_computed() const {
    return computed;
}

void Metric_Komponenty_Svyaznosti::clear_cache() {
    computed = false;
    value = 0;
    component_ids.clear();
}

size_t Metric_Komponenty_Svyaznosti::get_count() const {
    return value;
}

const vector<int>& Metric_Komponenty_Svyaznosti::get_component_ids() const {
    return component_ids;
}

// ============================================================================
// Metric_Tochki_Sochleneniya
// ============================================================================

Metric_Tochki_Sochleneniya::Metric_Tochki_Sochleneniya()
    : value(0), computed(false) {}

void Metric_Tochki_Sochleneniya::dfs_articulation(const Graph& graph, size_t v,
                                                   size_t parent, vector<int>& disc,
                                                   vector<int>& low,
                                                   vector<bool>& is_art,
                                                   int& time, size_t& root_children) {
    disc[v] = low[v] = ++time;

    vector<size_t> sosedi = graph.get_sosedi(v);
    for (size_t i = 0; i < sosedi.size(); i++) {
        size_t u = sosedi[i];

        if (u == parent) {
            continue;
        }

        if (disc[u] != -1) {
            if (disc[u] < low[v]) {
                low[v] = disc[u];
            }
        } else {
            if (parent == static_cast<size_t>(-1)) {
                root_children++;
            }

            dfs_articulation(graph, u, v, disc, low, is_art, time, root_children);

            if (low[u] >= disc[v] && parent != static_cast<size_t>(-1)) {
                is_art[v] = true;
            }

            if (low[u] < low[v]) {
                low[v] = low[u];
            }
        }
    }
}

void Metric_Tochki_Sochleneniya::compute(const Graph& graph) {
    if (computed) return;

    size_t n = graph.count_vershiny();
    vector<int> disc(n, -1);
    vector<int> low(n, -1);
    is_articulation.assign(n, false);
    value = 0;

    int time = 0;

    for (size_t i = 0; i < n; i++) {
        if (disc[i] == -1) {
            size_t root_children = 0;
            dfs_articulation(graph, i, static_cast<size_t>(-1), disc, low,
                           is_articulation, time, root_children);

            if (root_children > 1) {
                is_articulation[i] = true;
            }
        }
    }

    for (size_t i = 0; i < n; i++) {
        if (is_articulation[i]) {
            value++;
        }
    }

    computed = true;
}

string Metric_Tochki_Sochleneniya::get_result() const {
    if (!computed) return "Not computed";

    ostringstream oss;
    oss << "Tochki sochleneniya: " << value;
    return oss.str();
}

bool Metric_Tochki_Sochleneniya::is_computed() const {
    return computed;
}

void Metric_Tochki_Sochleneniya::clear_cache() {
    computed = false;
    value = 0;
    is_articulation.clear();
}

size_t Metric_Tochki_Sochleneniya::get_count() const {
    return value;
}

const vector<bool>& Metric_Tochki_Sochleneniya::get_articulation_flags() const {
    return is_articulation;
}

// ============================================================================
// Metric_Mosty
// ============================================================================

Metric_Mosty::Metric_Mosty() : value(0), computed(false) {}

void Metric_Mosty::dfs_bridges(const Graph& graph, size_t v, size_t parent,
                               vector<int>& disc, vector<int>& low,
                               vector<pair<size_t, size_t>>& bridges, int& time) {
    disc[v] = low[v] = ++time;

    vector<size_t> sosedi = graph.get_sosedi(v);
    for (size_t i = 0; i < sosedi.size(); i++) {
        size_t u = sosedi[i];

        if (u == parent) {
            continue;
        }

        if (disc[u] != -1) {
            if (disc[u] < low[v]) {
                low[v] = disc[u];
            }
        } else {
            dfs_bridges(graph, u, v, disc, low, bridges, time);

            if (low[u] > disc[v]) {
                if (v < u) {
                    bridges.push_back(make_pair(v, u));
                } else {
                    bridges.push_back(make_pair(u, v));
                }
            }

            if (low[u] < low[v]) {
                low[v] = low[u];
            }
        }
    }
}

void Metric_Mosty::compute(const Graph& graph) {
    if (computed) return;

    size_t n = graph.count_vershiny();
    vector<int> disc(n, -1);
    vector<int> low(n, -1);
    bridge_list.clear();
    int time = 0;

    for (size_t i = 0; i < n; i++) {
        if (disc[i] == -1) {
            dfs_bridges(graph, i, static_cast<size_t>(-1), disc, low, bridge_list, time);
        }
    }

    value = bridge_list.size();
    computed = true;
}

string Metric_Mosty::get_result() const {
    if (!computed) return "Not computed";

    ostringstream oss;
    oss << "Mosty: " << value;
    return oss.str();
}

bool Metric_Mosty::is_computed() const {
    return computed;
}

void Metric_Mosty::clear_cache() {
    computed = false;
    value = 0;
    bridge_list.clear();
}

size_t Metric_Mosty::get_count() const {
    return value;
}

const vector<pair<size_t, size_t>>& Metric_Mosty::get_bridge_list() const {
    return bridge_list;
}

// ============================================================================
// Metric_Dvudolnost
// ============================================================================

Metric_Dvudolnost::Metric_Dvudolnost() : is_bipartite(true), computed(false) {}

void Metric_Dvudolnost::compute(const Graph& graph) {
    if (computed) return;

    size_t n = graph.count_vershiny();
    coloring.assign(n, -1);
    is_bipartite = true;

    for (size_t start = 0; start < n; start++) {
        if (coloring[start] == -1) {
            queue<size_t> q;
            q.push(start);
            coloring[start] = 0;

            while (!q.empty() && is_bipartite) {
                size_t v = q.front();
                q.pop();

                vector<size_t> sosedi = graph.get_sosedi(v);
                for (size_t i = 0; i < sosedi.size(); i++) {
                    size_t u = sosedi[i];

                    if (coloring[u] == -1) {
                        coloring[u] = 1 - coloring[v];
                        q.push(u);
                    } else if (coloring[u] == coloring[v]) {
                        is_bipartite = false;
                        break;
                    }
                }
            }
        }

        if (!is_bipartite) {
            break;
        }
    }

    computed = true;
}

string Metric_Dvudolnost::get_result() const {
    if (!computed) return "Not computed";

    ostringstream oss;
    oss << "Dvudolnost: " << (is_bipartite ? "da" : "net");
    return oss.str();
}

bool Metric_Dvudolnost::is_computed() const {
    return computed;
}

void Metric_Dvudolnost::clear_cache() {
    computed = false;
    is_bipartite = true;
    coloring.clear();
}

bool Metric_Dvudolnost::get_is_bipartite() const {
    return is_bipartite;
}

const vector<int>& Metric_Dvudolnost::get_coloring() const {
    return coloring;
}

// ============================================================================
// Metric_Hromaticheskoe_Chislo
// ============================================================================

Metric_Hromaticheskoe_Chislo::Metric_Hromaticheskoe_Chislo()
    : upper_bound(0), computed(false) {}

void Metric_Hromaticheskoe_Chislo::compute(const Graph& graph) {
    if (computed) return;

    size_t n = graph.count_vershiny();
    coloring.assign(n, -1);
    upper_bound = 0;

    vector<bool> available_colors(n, false);

    for (size_t v = 0; v < n; v++) {
        for (size_t i = 0; i < n; i++) {
            available_colors[i] = true;
        }

        vector<size_t> sosedi = graph.get_sosedi(v);
        for (size_t i = 0; i < sosedi.size(); i++) {
            size_t u = sosedi[i];
            if (coloring[u] != -1 && coloring[u] < static_cast<int>(n)) {
                available_colors[coloring[u]] = false;
            }
        }

        int cr;
        for (cr = 0; cr < static_cast<int>(n); cr++) {
            if (available_colors[cr]) {
                break;
            }
        }

        coloring[v] = cr;
        if (cr + 1 > static_cast<int>(upper_bound)) {
            upper_bound = cr + 1;
        }
    }

    computed = true;
}

string Metric_Hromaticheskoe_Chislo::get_result() const {
    if (!computed) return "Not computed";

    ostringstream oss;
    oss << "Hromaticheskoe chislo (upper bound): " << upper_bound;
    return oss.str();
}

bool Metric_Hromaticheskoe_Chislo::is_computed() const {
    return computed;
}

void Metric_Hromaticheskoe_Chislo::clear_cache() {
    computed = false;
    upper_bound = 0;
    coloring.clear();
}

size_t Metric_Hromaticheskoe_Chislo::get_upper_bound() const {
    return upper_bound;
}

const vector<int>& Metric_Hromaticheskoe_Chislo::get_coloring() const {
    return coloring;
}