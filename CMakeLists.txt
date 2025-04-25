cmake_minimum_required(VERSION 3.31)
project(projekt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") # pozwala na optymalizacje kodu na najwyzszym poziomie

add_executable(projekt
        main.cpp
        InsertionSort.cpp
        HeapSort.cpp
        ShellSort.cpp
        QuickSort.cpp
)
