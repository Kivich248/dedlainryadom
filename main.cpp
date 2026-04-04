#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "graph.h"
#include "generator.h"
#include "parser.h"
#include "serializer.h"
#include "graphmetrics.h"

using namespace std; // да мы беспомощные и не хотим писать std::

void print_help() {
    cout << "Usage: ./lab2 [options]\n";
    cout << "Options:\n";
    cout << "  -g, --generate <type> <args>   Generate a graph (e.g., full 10)\n";
    cout << "  -p, --parse <format> <file>    Parse a graph\n";
    cout << "  -m, --metrics <list>           Compute metrics (comma-separated or 'all')\n";
    cout << "  -s, --save <format> <file>     Save graph\n";
    cout << "  --viz-components               Show components\n";
    cout << "  --viz-bridges                  Highlight bridges\n";
    cout << "  --viz-articulations            Highlight articulation points\n";
    cout << "  --viz-spanning-tree            Show random spanning tree\n";
    cout << "  --viz-cycle                    Show random cycle\n";
    cout << "  -h, --help                     Show help\n";
} //для беспомощных

int main(int argc, char* argv[]) {
    Graph g;
    bool graph_loaded = false;
    VizOptions viz_opts;

    vector<string> metrics_list;
    string save_format, save_file;
    string parse_format, parse_file;
    string gen_type;
    vector<string> gen_args;

    // парсинг уры
    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            print_help();
            return 0;
        }
        else if (arg == "-g" || arg == "--generate")
        {
            if (i + 1 >= argc) { cerr << "Error: -g needs type\n"; return 1; }
            gen_type = argv[++i];
            // аргументы генератора до следующего флага
            while (i + 1 < argc && argv[i+1][0] != '-')
            {
                gen_args.push_back(argv[++i]);
            }
        }
        else if (arg == "-p" || arg == "--parse") {
            if (i + 2 >= argc) { cerr << "Error: -p needs format and file\n"; return 1; }
            parse_format = argv[++i];
            parse_file = argv[++i];
        }
        else if (arg == "-m" || arg == "--metrics") {
            if (i + 1 >= argc) { cerr << "Error: -m needs metrics list\n"; return 1; }
            string m_str = argv[++i];
            if (m_str == "all") {
                metrics_list = GraphMetric::get_available_metrics();
            } else {
                stringstream ss(m_str);
                string item;
                while (getline(ss, item, ',')) {
                    metrics_list.push_back(item);
                }
            }
        }
        else if (arg == "-s" || arg == "--save") {
            if (i + 2 >= argc) { cerr << "Error: -s needs format and file\n"; return 1; }
            save_format = argv[++i];
            save_file = argv[++i];
        }
        else if (arg == "--viz-components") viz_opts.show_components = true;
        else if (arg == "--viz-bridges") viz_opts.show_bridges = true;
        else if (arg == "--viz-articulations") viz_opts.show_articulations = true;
        else if (arg == "--viz-spanning-tree") viz_opts.show_random_spanning_tree = true;
        else if (arg == "--viz-cycle") viz_opts.show_random_cycle = true;
        else {
            cerr << "Unknown option: " << arg << "\n";
            return 1;
        }
    }

    try {
        // выбор функции
        if (!gen_type.empty()) {
            if (gen_type == "full") g = generate_full(stoul(gen_args[0]));
            else if (gen_type == "twodol") g = generate_full_twodol(stoul(gen_args[0]), stoul(gen_args[1]));
            else if (gen_type == "tree") g = generate_tree(stoul(gen_args[0]));
            else if (gen_type == "star") g = generate_star(stoul(gen_args[0]));
            else if (gen_type == "cycle") g = generate_cycle(stoul(gen_args[0]));
            else if (gen_type == "path") g = generate_path(stoul(gen_args[0]));
            else if (gen_type == "wheel") g = generate_wheel(stoul(gen_args[0]));
            else if (gen_type == "random") g = random_veroyatnost(stoul(gen_args[0]), stod(gen_args[1]));
            else if (gen_type == "cubic") g = generate_random_cubic(stoul(gen_args[0]));
            else if (gen_type == "components") g = generate_graph_with_components(stoul(gen_args[0]), stoul(gen_args[1]));
            else if (gen_type == "bridges") g = generate_graph_with_bridges_path_blobs_random(stoul(gen_args[0]), stoul(gen_args[1]));
            else if (gen_type == "articulations") g = generate_graph_with_articulations_path_blobs_random(stoul(gen_args[0]), stoul(gen_args[1]));
            else { cerr << "Unknown generator: " << gen_type << "\n"; return 1; }
            graph_loaded = true;
            cout << "Generated: " << g.count_vershiny() << " V, " << g.count_rebra() << " E\n";
        }
        else if (!parse_file.empty()) {
            ifstream file(parse_file);
            if (!file.is_open()) throw runtime_error("Cannot open " + parse_file);
            GraphParser* parser = GraphParser::sozdat_parser(parse_format);
            if (!parser) throw runtime_error("Unknown parser: " + parse_format);
            parser->parse(file, g);
            delete parser;
            graph_loaded = true;
            cout << "Parsed: " << g.count_vershiny() << " V, " << g.count_rebra() << " E\n";
        }

        if (!graph_loaded && (!metrics_list.empty() || !save_file.empty())) {
            cerr << "Error: No graph loaded. Use -g or -p first.\n";
            return 1;
        }

        // метрики мои любимые(нет)
        if (!metrics_list.empty()) {
            cout << "\n--- Metrics ---\n";
            for (const string& name : metrics_list) {
                GraphMetric* m = GraphMetric::sozdat_metric(name);
                if (m) {
                    m->compute(g);
                    cout << m->get_result() << "\n";
                    delete m;
                } else {
                    cerr << "Unknown metric: " << name << "\n";
                }
            }
        }

        // сохраняем граф(наконец-то)
        if (!save_file.empty()) {
            ofstream file(save_file);
            if (!file.is_open()) throw runtime_error("Cannot create " + save_file);
            GraphSerializer* ser = GraphSerializer::sozdat_serializer(save_format);
            if (!ser) throw runtime_error("Unknown serializer: " + save_format);
            ser->serialize(g, file, viz_opts);
            delete ser;
            cout << "Saved to " << save_file << "\n";
        }

    } catch (exception& e) {
        cerr << "Fatal Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}