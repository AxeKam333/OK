#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <tuple>

using namespace std;

struct Point
{
    int id, x, y;
};

double calculateDistance(const Point &p1, const Point &p2)
{
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}

tuple<vector<Point>, double> greedyTSP(vector<Point> points)
{
    vector<Point> tour;

    // Start with the first point as the initial city
    tour.push_back(points[0]);
    points.erase(points.begin());

    double cost = 0;

    while (!points.empty())
    {
        Point currentCity = tour.back();
        double minDistance = calculateDistance(currentCity, points[0]);
        int closestCityIndex = 0;

        // Find the closest unvisited city
        for (int i = 1; i < points.size(); i++)
        {
            double distance = calculateDistance(currentCity, points[i]);
            if (distance < minDistance)
            {
                minDistance = distance;
                closestCityIndex = i;
            }
        }

        cost += minDistance;

        // Move to the closest city
        tour.push_back(points[closestCityIndex]);
        points.erase(points.begin() + closestCityIndex);
    }

    // Return to the starting city to complete the tour (would double with the couts at the end)
    // tour.push_back(tour[0]);

    cost += calculateDistance(tour.back(), tour[0]);

    return {tour, cost};
}

int main(int argc, char *argsv[])
{
    vector<Point> cities;

    // Read cities from a file
    string name="grupa7_1";
    if (argc > 1)
        name = argsv[1];
    ifstream inputFile("tests/"+name+".txt");
    // ifstream inputFile("tests/olek123.txt");
    if (!inputFile.is_open())
    {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    int n, s;
    inputFile >> n;

    int x, y;
    for (int i = 1; i <= n; i++)
    {
        inputFile >> s >> x >> y;
        Point point = {s, x, y};
        cities.push_back(point);
    }
    inputFile.close();

    double cost;
    vector<Point> tour;

    tie(tour, cost) = greedyTSP(cities);

    // cout << "trasa po koordach: ";
    // for (const Point &city : tour)
    // {
    //     cout << "(" << city.x << ", " << city.y << ") -> ";
    // }
    // cout << "(" << tour[0].x << ", " << tour[0].y << ")" << endl;

    // cout << "trasa po id: ";
    // for (const Point &city : tour)
    // {
    //     cout << "(" << city.id << ") -> ";
    // }
    // cout << "(" << tour[0].id << ")" << endl;

    cout << "koszt: " << cost << endl;

    return 0;
}
