#include <iostream>
#include <vector>
using namespace std;




// Запуск с аргументами:
// ./lab2 --source keyboard --format edgelist
// ./lab2 --source file --format dimacs --filename graph.col

int main(int argc, char* argv[]) {
	string source = "keyboard";
	string format = "edgelist";
	string filename = "";

	// Парсинг аргументов
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if (arg == "--source" && i + 1 < argc) {
			source = argv[++i];
		}
		else if (arg == "--format" && i + 1 < argc) {
			format = argv[++i];
		}
		else if (arg == "--filename" && i + 1 < argc) {
			filename = argv[++i];
		}
	}
}