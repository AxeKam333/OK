#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>

using namespace std;

struct Node {
    int id;
};

double calculateDistance(const Node& p1, const Node& p2, int *matrix[]) {
    return matrix[p1.id][p2.id];
}

vector<Node> greedyTSP(vector<Node> points, int *matrix[]) {
    vector<Node> tour;

    // Start with the first point as the initial city
    tour.push_back(points[0]);
    points.erase(points.begin());

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
        tour.push_back(points[closestCityIndex]);
        points.erase(points.begin() + closestCityIndex);
    }

    // Return to the starting city to complete the tour (would double with the couts at the end)
    // tour.push_back(tour[0]);

    return tour;
}

int main() {
    vector<Node> cities;

    ifstream inputFile("plik2.txt");
    if (!inputFile.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    //adjency matrix should be symmetric and have 0 on diagonal

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

    vector<Node> tour = greedyTSP(cities, matrix);

    // Print the tour
    cout << "trasa: ";
    for (const Node& city : tour) {
        cout << "(" << city.id  << ") -> ";
    }
    cout << "(" << tour[0].id << ")" << endl;

    for(int i=0; i<n; i++){
        delete [] matrix[i];
    }
    delete [] matrix;

    return 0;
}
