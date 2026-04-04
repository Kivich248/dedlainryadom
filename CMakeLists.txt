cmake_minimum_required(VERSION 3.10)
project(GraphLab)

set(CMAKE_CXX_STANDARD 20)

# ============================================================================
# 1. БИБЛИОТЕКА (только код графа, без тестов)
# ============================================================================
add_library(graph_lib STATIC
        graph.cpp
        generator.cpp
        parser.cpp
        serializer.cpp
        graphmetrics.cpp
        # tests_main.cpp - НЕ ДОЛЖЕН БЫТЬ ЗДЕСЬ!
)

target_include_directories(graph_lib PUBLIC ${CMAKE_SOURCE_DIR})

# ============================================================================
# 2. ОСНОВНАЯ ПРОГРАММА
# ============================================================================
add_executable(lab2 main.cpp)
target_link_libraries(lab2 PRIVATE graph_lib)

# ============================================================================
# 3. ТЕСТЫ (отдельный исполняемый файл)
# ============================================================================
add_executable(tests
        tests_main.cpp          # <-- теперь здесь
        tests_graph_core.cpp
        tests_generators.cpp
        tests_metrics.cpp
        tests_serializers.cpp
        tests_parsers.cpp
)

target_link_libraries(tests PRIVATE graph_lib)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR})