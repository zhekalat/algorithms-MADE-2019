// Латышев Евгений. Задача 18. Вариант 1
//
// Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого множества и вычислите ее периметр.
// Вариант 1. С помощью алгоритма Грэхема.

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

const double epsilon = 1e-6;

// Структура для хранения точек с перегруженными операторами
struct Point {
    Point(double _x, double _y): x(_x), y(_y) {}
    double x, y;
    Point operator-(Point& second) { return {x - second.x, y - second.y};}
    bool operator>(Point& second) { return vector_product(second) > epsilon;}
    bool operator==(Point& second) { return std::abs(vector_product(second)) < epsilon;}
    double vector_product(Point second) {return x * second.y - second.x * y;} // Векторное произведение в 2D
};

// Компаратор для сортировки точек по полярным координатам. При их равенстве обрабатываем сначала ближайшую точку
bool comparePoints(Point p1, Point p2) {
    double distance1 = std::sqrt(p1.x*p1.x + p1.y*p1.y);
    double distance2 = std::sqrt(p2.x*p2.x + p2.y*p2.y);
    return p1 > p2 or (p1 == p2 and distance1 < distance2);
}

// Поиск нижней из самых левых точек в качестве базовой
Point findStartingPoint(const std::vector<Point>& points) {
    std::vector<Point> potential_starting_points;
    Point minPoint(1001.0, 1001.0);
    for (auto point : points) {
        if ((point.x < minPoint.x) or (point.x == minPoint.x and point.y < minPoint.y)) {
            minPoint = point;
        }
    }
    return minPoint;
}

// Поиск минимальной выпуклой оболочки точек с помощью алгоритма Грэхема
std::vector<Point> grahamConvexHull(std::vector<Point> points) {
    Point startingPoint = findStartingPoint(points);
    // Для удобства сместим начало координат в стартовую точку
    for (Point &point : points) {
        point.x -= startingPoint.x;
        point.y -= startingPoint.y;
    }
    sort(points.begin(), points.end(), comparePoints);
    std::vector<Point> hull;
    for (Point p : points) {
        // Пока получаем не выпуклый угол и в стеке остались точки, выкидываем предыдущую точку из стека
        while (hull.size() > 1 and (p - hull.back()).vector_product(hull[hull.size() - 2] - hull.back()) <= epsilon) {
            hull.pop_back();
        }
        hull.push_back(p);
    }
    // вернём координаты в исходное состояние (для подсчёта периметра можно этого не делать, но так получим универсальную функцию)
    for (Point &point : points) {
        point.x += startingPoint.x;
        point.y += startingPoint.y;
    }
    return hull;
}

// Подсчёт периметра фигуры по точкам
double hull_perimeter(std::vector<Point> points) {
    double perimeter = 0;
    Point startingPoint = points.back();
    Point previousPoint = points.back();
    points.pop_back();
    for (Point point: points) {
        double distance = std::sqrt(std::pow(point.x - previousPoint.x, 2) + std::pow(point.y - previousPoint.y, 2));
        perimeter += distance;
        previousPoint = point;
    }
    perimeter += std::sqrt(std::pow(startingPoint.x - previousPoint.x, 2) + std::pow(startingPoint.y - previousPoint.y, 2));
    return perimeter;
}

int main(const int argc, const char* argv[]) {
    int pointCount;
    double x, y;
    double perimeter;
    std::vector<Point> points;
    std::cin >> pointCount;
    for (int i = 0; i < pointCount; i++) {
        std::cin >> x >> y;
        Point point(x, y);
        points.push_back(point);
    }
    std::vector<Point> hull;
    hull = grahamConvexHull(points);
    perimeter = hull_perimeter(hull);
    std::cout.precision(9);
    std::cout << perimeter;
    return 0;
}
