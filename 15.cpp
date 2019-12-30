// Латышев Евгений. Задача 15
//
// Найдите приближенное решение метрической неориентированной задачи коммивояжера в полном графе (на плоскости) с помощью минимального остовного дерева.
// Оцените качество приближения на случайном наборе точек, нормально распределенном на плоскости с дисперсией 1.
// Нормально распределенный набор точек получайте с помощью преобразования Бокса-Мюллера.
// При фиксированном N, количестве вершин графа, несколько раз запустите оценку качества приближения.
// Вычислите среднее значение и среднеквадратичное отклонение качества приближения для данного N.
// Запустите данный эксперимент для всех N в некотором диапазоне, например, [2, 10]. Автоматизируйте запуск экспериментов.

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include "Graph.h"

std::pair<double, double> Experiment(int v, int n) {
    std::vector<double> qualities;
    for (int i = 0; i < n; i++) {
        Graph graph(v);
        std::vector<Edge> edgesMST = graph.FindMST();
        std::vector<int> solutionMST = graph.TSPSolutionFromMST(edgesMST);
        double distanceMST = graph.PathDistance(solutionMST);
        std::vector<int> solutionExhaustive = graph.ExhaustiveSearchTSP();
        double distanceExhaustive = graph.PathDistance(solutionExhaustive);
        double quality = distanceMST / distanceExhaustive;
        qualities.push_back(quality);
    }
    double mean = std::accumulate(std::begin(qualities), std::end(qualities), 0.0)  / qualities.size();
    double accum = 0.0;
    std::for_each (std::begin(qualities), std::end(qualities), [&](const double d) {
        accum += (d-mean) * (d-mean);
    });
    double std = std::sqrt(accum / (qualities.size()-1));
    std::pair<double, double> result = std::make_pair(mean, std);
    return result;
}

int main() {
    std::vector<int> vertices = {2,3,4,5,6,7,8,9,10,11,12};
    int numSamples = 100;
    for (int v: vertices) {
        std::pair<double, double> result = Experiment(v, numSamples);
        std::cout <<  "V = " << v << ": mean = " << result.first << ", std = " << result.second << '\n';
    }
    return 0;
}
