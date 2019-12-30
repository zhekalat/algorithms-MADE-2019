// Латышев Евгений. Задача 14. Вариант 1
//
// «MST». Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
// Вариант 1. С помощью алгоритма Прима.

#include <iostream>
#include <vector>
#include <queue>

class Edge {
public:
    Edge(int _start, int _end, int _weight): start(_start), end(_end), weight(_weight) {}
    int start;
    int end;
    int weight;
};

class EdgeComparator {
public:
    bool operator() (Edge p1, Edge p2)
    {
        return p1.weight > p2.weight;
    }
};

class Prim {
public:
    Prim(int _v, int _n): v(_v), n(_n) {edges = new std::vector<Edge>[v];}
    ~Prim() {delete[] edges;}
    Prim(const Prim& other) = delete;
    Prim(Prim&& other) = delete;
    Prim& operator=(const Prim& other) = delete;
    Prim& operator=(Prim&& other) = delete;
    void FindMST();  // поиск минимального остовного дерева
    void AddEdge(int start, int end, int weight);  // добавление нового ребра в граф
private:
    int v; // количество вершин
    int n; // количество рёбер
    std::vector<Edge> *edges;  // массив список рёбер, исходящих из каждой вершины
};

void Prim::AddEdge(int start, int end, int weight) {
    // поскольку граф неориентированный, для каждого ребра добавляем обратное
    Edge newEdge(start, end, weight);
    Edge reverseEdge(end, start, weight);
    edges[start].push_back(newEdge);
    edges[end].push_back(reverseEdge);
}

void Prim::FindMST()
{
    std::priority_queue< Edge, std::vector <Edge> , EdgeComparator > edgeQueue;
    std::vector<int> weights(v, std::numeric_limits<int>::max());  // расстояния до вершин от текущего остовного дерева
    std::vector<bool> added(v, false);  // флаги включения узла в остовное дерево
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
            // добавляем в кучу рёбра добавленного в остовное дерево узла
            for (Edge adjEdge : edges[shortestEdge.end])
            {
                int end = adjEdge.end;
                int weight = adjEdge.weight;
                // если узел еще не содержится в остовном дереве и вес рассматриваемого ребра меньше текущего веса узла
                if ((!added[end]) and weights[end] > weight and adjEdge.start != adjEdge.end) {
                    edgeQueue.push(Edge(adjEdge.start, adjEdge.end, adjEdge.weight));
                }
            }
        }
    }
    // считаем сумму весов всех рёбер остовного дерева
    int sum = 0;
    for (int weight: weights) {
        sum += weight;
    }
    std::cout << sum;
}

int main() {
    int v;
    int n;
    std::cin>> v >> n;
    Prim prim(v, n);
    for (int i = 0; i < n; i++) {
        int start;
        int end;
        int weight;
        std::cin>> start >> end >> weight;
        prim.AddEdge(start-1, end-1, weight);
    }
    prim.FindMST();
    return 0;
}
