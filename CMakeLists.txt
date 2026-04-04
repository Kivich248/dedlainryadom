cmake_minimum_required(VERSION 4.0)
project(dedlainryadom)

set(CMAKE_CXX_STANDARD 17)

add_executable(dedlainryadom main.cpp
        parser.cpp
        parser.cpp
        parser.h
        test.cpp
        generator.cpp
        generator.h
        graph.cpp
        GraphMetrics.h
        GraphMetrics.cpp
        serializer.h
        serializer.h
        serializer.cpp
        tests_main.cpp
        tests_graph_core.cpp
        tests_generators.cpp
        tests_metrics.cpp
        tests_serializers.cpp
        tests_parsers.cpp
)
