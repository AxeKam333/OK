#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <limits>
#include <tuple>

using namespace std;

struct Node {
    int id;
};

double calculateDistance(const Node& p1, const Node& p2, int *matrix[]) {
    return matrix[p1.id][p2.id] == -1 ? numeric_limits<double>::max() : matrix[p1.id][p2.id];
}

tuple<vector<Node>,int> greedyTSP(vector<Node> points, int *matrix[]) {
    vector<Node> tour;

    // Start with the first point as the initial city
    tour.push_back(points[0]);
    points.erase(points.begin());

    int cost = 0;

    while (!points.empty()) {
        Node currentCity = tour.back();
        double minDistance = calculateDistance(currentCity, points[0], matrix);
        int closestCityIndex = 0;

        // Find the closest unvisited city
        for (int i = 1; i < points.size(); i++) {
            double distance = calculateDistance(currentCity, points[i], matrix);
            if (distance < minDistance) {
                minDistance = distance;
                closestCityIndex = i;
            }
        }

        // Move to the closest city
        cost += minDistance;
        tour.push_back(points[closestCityIndex]);
        points.erase(points.begin() + closestCityIndex);
    }

    // Return to the starting city to complete the tour (would double with the couts at the end)
    // tour.push_back(tour[0]);
    cost += calculateDistance(tour.back(), tour[0], matrix);

    return {tour,cost};
}

int main() {
    vector<Node> cities;

    ifstream inputFile("plik2.txt");
    if (!inputFile.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    // adjency matrix should be symmetric and have 0 on diagonal. must be Hamiltonian
    // it should have -1 if there is no connection between cities

    //reading adjacency matrix
    int n;
    inputFile >> n;

    int **matrix = new int*[n];
    for(int i=0; i<n; i++){
        cities.push_back({i});
        matrix[i] = new int[n];
        for(int j=0; j<n; j++){
            inputFile >> matrix[i][j];
        }
    }

    inputFile.close();

    vector<Node> tour;
    int cost;
    
    tie(tour, cost) = greedyTSP(cities, matrix);

    // Print the tour
    cout << "trasa: ";
    for (const Node& city : tour) {
        cout << "(" << city.id  << ") -> ";
    }
    cout << "(" << tour[0].id << ")" << endl;

    cout << "koszt: " << cost << endl;

    for(int i=0; i<n; i++){
        delete [] matrix[i];
    }
    delete [] matrix;

    return 0;
}
