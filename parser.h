#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Graph; // Forward declaration

class GraphParser {
public:
	virtual ~GraphParser();
	virtual void parse(istream& input, Graph& graph) = 0;
	static GraphParser* sozdat_parser(const string& format);
};

class EdgeListParser : public GraphParser {
public:
	void parse(istream& input, Graph& graph) override;
};

class MatrixParser : public GraphParser {
public:
	void parse(istream& input, Graph& graph) override;
};

class DIMACSParser : public GraphParser {
public:
	void parse(istream& input, Graph& graph) override;
};

class SNAPParser : public GraphParser {
public:
	void parse(istream& input, Graph& graph) override;
};