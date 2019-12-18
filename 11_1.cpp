// Латышев Евгений. Задача 11. Вариант 1
// https://contest.yandex.ru/contest/16167/run-report/29015257/
// Дан невзвешенный неориентированный граф. Найдите цикл минимальной длины.
// Ввод: v: кол-во вершин (макс. 50000), n: кол-во ребер (макс. 200000), n пар реберных вершин.
// Вывод: одно целое число равное длине минимального цикла. Если цикла нет, то вывести -1.

#include <iostream>
#include <vector>
#include <queue>

class ShortestCycleFinder {
public:
    ShortestCycleFinder(int _v, int _n): v(_v), n(_n) {edges = new std::vector<int>[v];}
    ~ShortestCycleFinder() {delete[] edges;}
    ShortestCycleFinder(const ShortestCycleFinder& other) = delete;
    ShortestCycleFinder(ShortestCycleFinder&& other) = delete;
    ShortestCycleFinder& operator=(const ShortestCycleFinder& other) = delete;
    ShortestCycleFinder& operator=(ShortestCycleFinder&& other) = delete;

    int FindShortestCycleLen(); // Поиск цикла минимальной длины
    void AddEdge(int start, int end); // Добавление ребра в граф
private:
    int v; // количество вершин
    int n; // количество рёбер
    std::vector<int> *edges; // массив списков рёбер, исходящих из каждой вершины
};

void ShortestCycleFinder::AddEdge(int start, int end) {
    edges[start].push_back(end);
    edges[end].push_back(start);
}

int ShortestCycleFinder::FindShortestCycleLen() {
    int minLen = n+2;  // Недостижимая максимальная длина цикла в графе
    for (int i = 0; i < v; i++) {
        std::vector<int> distance(v, (int)(n+2)); // расстояния от текущей вершины до каждой вершины графа
        distance[i] = 0;
        std::vector<int> parent(v, -1);  // храним, откуда мы пришли в вершину
        std::queue<int> nodesToVisit;
        nodesToVisit.push(i);
        while (!nodesToVisit.empty()) { // идём по всем достижимым узлам
            int currNode = nodesToVisit.front();
            nodesToVisit.pop();
            for (int adj : edges[currNode]) { // если еще не заходили в смежную с текущей вершину, добавляем её в очередь
                if (distance[adj] == (int)(n+2)) {
                    distance[adj] = 1 + distance[currNode];
                    parent[adj] = currNode;
                    nodesToVisit.push(adj);
                }
                // если мы уже были в смежной вершине и не тем путём, которым идем сейчас, мы нашли цикл
                else if (parent[currNode] != adj and parent[adj] != currNode) {
                    minLen = std::min(minLen, distance[currNode] + distance[adj] + 1);
                }
            }
        }
    }
    if (minLen == n+2)
        return -1;
    else
        return minLen;
}

int main() {
    int v;
    int n;
    std::cin>> v >> n;
    ShortestCycleFinder finder(v, n);
    for (int i = 0; i < n; i++) {
        int start;
        int end;
        std::cin>> start >> end;
        finder.AddEdge(start, end);
    }
    int minLen;
    minLen = finder.FindShortestCycleLen();
    std::cout<< minLen;
    return 0;
}