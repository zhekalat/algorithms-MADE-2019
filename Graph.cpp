#pragma once

#include <vector>
#include <queue>
#include <cmath>
#include <random>
#include "Graph.h"

// Конструктор вершины, генерирующий координаты преобразованием Бокса-Мюллера
Point::Point() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    double x_rand, y_rand, s;
    do {
        x_rand = dis(gen);
        y_rand = dis(gen);
        s = x_rand*x_rand + y_rand*y_rand;
    } while (s == 0.0 or s > 1);
    x = x_rand * std::sqrt(-2 * std::log(s) / s);
    y = y_rand * std::sqrt(-2 * std::log(s) / s);
}

// Конструктор, инициализирующий полный граф с заданым количеством вершин
Graph::Graph(int _v): v(_v) {
    std::vector<Point> coordinates;
    for (int i = 0; i < v; i++) {
        Point point;
        coordinates.push_back(point);
    }
    for (int i = 0; i < v; i++) {
        std::vector<Edge> row;
        for (int j = 0; j < v; j++) {
            Edge newEdge(i, j, coordinates[j]-coordinates[i]);
            row.push_back(newEdge);
        }
        edges.push_back(row);
    }
}

// поиск минимального остовного дерева методом Прима
std::vector<Edge> Graph::FindMST()
{
    std::priority_queue< Edge, std::vector <Edge> , EdgeComparator > edgeQueue;
    std::vector<double> weights(v, std::numeric_limits<double>::max());  // расстояния до вершин от текущего остовного дерева
    std::vector<bool> added(v, false);  // флаги включения узла в остовное дерево
    std::vector<Edge> MST;  // флаги включения узла в остовное дерево
    added[0] = true;
    weights[0] = 0;
    for (Edge adjEdge : edges[0]) {
        edgeQueue.push(Edge(adjEdge.start, adjEdge.end, adjEdge.weight));
    }
    // пока остались необработанные рёбра, берём ребро с минимальным весом из кучи и добавляем его в остовное дерево
    while (!edgeQueue.empty())
    {
        Edge shortestEdge = edgeQueue.top();
        edgeQueue.pop();
        if (!added[shortestEdge.end]) {
            added[shortestEdge.end] = true;
            weights[shortestEdge.end] = shortestEdge.weight;
            MST.push_back(shortestEdge);
            // добавляем в кучу рёбра добавленного в остовное дерево узла
            for (Edge adjEdge : edges[shortestEdge.end])
            {
                int end = adjEdge.end;
                double weight = adjEdge.weight;
                // если узел еще не содержится в остовном дереве и вес рассматриваемого ребра меньше текущего веса узла
                if ((!added[end]) and weights[end] > weight and adjEdge.start != adjEdge.end) {
                    edgeQueue.push(Edge(adjEdge.start, adjEdge.end, adjEdge.weight));
                }
            }
        }
    }
    return MST;
}

// поиск гамильтонова цикла на основе минимального остовного дерева
std::vector<int> Graph::TSPSolutionFromMST(const std::vector<Edge>& MST) {
    std::vector<int> solution;
    std::vector<bool> visited(v, false);
    std::vector<std::vector<bool>> inMST;
    // инициализируем маску рёбер, входящих в состав MST
    for (int i = 0; i < v; i++) {
        std::vector<bool> row(v, false);
        inMST.push_back(row);
    }
    for (Edge edge: MST) {
        inMST[edge.start][edge.end] = true;
    }
    // Обойдём остовное дерево в глубину, чтобы получить порядок вершин гамильтонова графа
    RecursiveTraverseMST(0, inMST, solution, visited);
    return solution;
}

// рекурсивный Pre-обход минимального остовного дерева
void Graph::RecursiveTraverseMST(int u, std::vector<std::vector<bool>> inMST, std::vector<int>& solution, std::vector<bool>& visited)
{
    solution.push_back(u);
    visited[u] = true;
    for (Edge edge : edges[u]) {
        if(inMST[edge.start][edge.end]) {
            RecursiveTraverseMST(edge.end, inMST, solution, visited);
        }
    }
}

// поиск оптимального решения задачи коммивояжера полным перебором
std::vector<int> Graph::ExhaustiveSearchTSP() {
    std::vector<int> vertexPermutation;
    std::vector<int> optimalSolution;
    double optimalSolutionWeight = std::numeric_limits<double>::max();
    for (int i = 1; i < v; i++)
        vertexPermutation.push_back(i);
    do {
        double currWeight = 0;
        int lastIndex = 0;
        // считаем путь текущей перестановки
        for (int currIndex: vertexPermutation) {
            currWeight += edges[lastIndex][currIndex].weight;
            lastIndex = currIndex;
        }
        currWeight += edges[lastIndex][0].weight;
        // сохраняем минимальный путь
        if (currWeight < optimalSolutionWeight) {
            optimalSolutionWeight = currWeight;
            optimalSolution = vertexPermutation;
        }
    } while (std::next_permutation(vertexPermutation.begin(), vertexPermutation.end()));
    return optimalSolution;
}

// расчёт длины пути
double Graph::PathDistance(const std::vector<int>& solution) {
    double distance = 0;
    int k = 0;
    for (int i : solution) {
        distance += edges[k][i].weight;
        k = i;
    }
    distance += edges[k][0].weight;
    return distance;
}
