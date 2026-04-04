cmake_minimum_required(VERSION 3.10)
project(GraphLab)

set(CMAKE_CXX_STANDARD 20)

# ============================================================================
# 1. БИБЛИОТЕКА (Общий код проекта)
# Собираем весь твой код в статическую библиотеку, чтобы не дублировать его
# ============================================================================
add_library(graph_lib STATIC
        graph.cpp
        generator.cpp
        parser.cpp
        serializer.cpp
        graphmetrics.cpp
)

# Говорим компилятору, что заголовочные файлы (.h) лежат в текущей папке
target_include_directories(graph_lib PUBLIC ${CMAKE_SOURCE_DIR})

# ============================================================================
# 2. ОСНОВНАЯ ПРОГРАММА (lab2.exe)
# ============================================================================
add_executable(lab2 main.cpp)
# Подключаем библиотеку к программе
target_link_libraries(lab2 PRIVATE graph_lib)

# ============================================================================
# 3. ТЕСТЫ (tests.exe)
# ВАЖНО: main.cpp сюда НЕ входит!
# ============================================================================
add_executable(tests
        tests_graph_core.cpp
        tests_generators.cpp
        tests_metrics.cpp
        tests_serializers.cpp
        tests_parsers.cpp
        # catch_amalgamated.hpp подключается через #include в файлах, добавлять сюда не нужно
)

# Подключаем библиотеку к тестам, чтобы они видели функции графа
target_link_libraries(tests PRIVATE graph_lib)

# Убеждаемся, что тесты видят заголовки (включая catch_amalgamated.hpp)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR})