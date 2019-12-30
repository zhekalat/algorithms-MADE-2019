#pragma once

#include <vector>
#include <cmath>

// Ребро графа
struct Edge {
    Edge(int _start, int _end, double _weight): start(_start), end(_end), weight(_weight) {}
    int start;
    int end;
    double weight;
};

// Компаратор для сравнения рёбер
class EdgeComparator {
public:
    bool operator() (Edge p1, Edge p2)
    {
        return p1.weight > p2.weight;
    }
};

// Вершина графа
struct Point {
    Point();
    double x,y;
    double operator-(Point& second) { return std::sqrt(std::pow(x-second.x, 2) + std::pow(y-second.y, 2));}
};

class Graph {
public:
    explicit Graph(int _v);
    ~Graph() = default;
    Graph(const Graph& other) = delete;
    Graph(Graph&& other) = delete;
    Graph& operator=(const Graph& other) = delete;
    Graph& operator=(Graph&& other) = delete;
    std::vector<Edge> FindMST();  // поиск минимального остовного дерева методом Прима
    std::vector<int> TSPSolutionFromMST(const std::vector<Edge>& MST);  // поиск гамильтонова цикла на основе минимального остовного дерева
    std::vector<int> ExhaustiveSearchTSP();  // поиск оптимального решения задачи коммивояжера полным перебором
    double PathDistance(const std::vector<int>& solution);  // расчёт длины пути
private:
    int v; // количество вершин
    std::vector<std::vector<Edge>> edges;  // список списков рёбер, исходящих из каждой вершины
    // рекурсивный Pre-обход минимального остовного дерева
    void RecursiveTraverseMST(int u, std::vector<std::vector<bool>> inMST, std::vector<int>& solution, std::vector<bool>& visited);
};
