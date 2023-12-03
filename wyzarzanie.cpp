#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

struct City {
    string name;
    int x, y;
};

// Function to calculate the Euclidean distance between two cities
double calculateDistance(const City& a, const City& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Function to calculate the total distance of a tour
double calculateTotalDistance(const vector<int>& tour, const vector<City>& cities) {
    double distance = 0.0;
    int n = tour.size();
    for (int i = 0; i < n - 1; ++i) {
        distance += calculateDistance(cities[tour[i]], cities[tour[i + 1]]);
    }
    distance += calculateDistance(cities[tour[n - 1]], cities[tour[0]]); // Return to the starting city
    return distance;
}

// Function to generate a random initial tour
vector<int> generateRandomTour(int n) {
    vector<int> tour(n);
    for (int i = 0; i < n; ++i) {
        tour[i] = i;
    }
    shuffle(tour.begin() + 1, tour.end(), std::default_random_engine(std::random_device()()));
    return tour;
}

// Function to perform Simulated Annealing
vector<int> simulatedAnnealing(const vector<City>& cities, double initialTemperature,
                               double coolingRate, int maxIterations) {
    srand(time(nullptr));

    int n = cities.size();
    vector<int> currentTour = generateRandomTour(n);
    vector<int> bestTour = currentTour;

    double currentDistance = calculateTotalDistance(currentTour, cities);
    double bestDistance = currentDistance;

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        double temperature = initialTemperature * exp(-coolingRate * iteration);

        // Generate a new tour by swapping two cities
        vector<int> newTour = currentTour;
        int pos1 = rand() % (n - 1) + 1; // Avoid swapping the starting city
        int pos2 = rand() % (n - 1) + 1;
        swap(newTour[pos1], newTour[pos2]);

        double newDistance = calculateTotalDistance(newTour, cities);

        // Accept the new tour if it is better or with a certain probability if it is worse
        if (newDistance < currentDistance || (rand() / (RAND_MAX + 1.0)) < exp((currentDistance - newDistance) / temperature)) {
            currentTour = newTour;
            currentDistance = newDistance;
        }

        // Update the best tour if necessary
        if (currentDistance < bestDistance) {
            bestTour = currentTour;
            bestDistance = currentDistance;
        }
    }

    return bestTour;
}

// Function to read cities from a file
vector<City> readCitiesFromFile(const string& filename) {
    vector<City> cities;
    ifstream file(filename);
    if (file.is_open()) {
        int numCities;
        file >> numCities;

        for (int i = 0; i < numCities; ++i) {
            City city;
            file >> city.name >> city.x >> city.y;
            cities.push_back(city);
        }

        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }
    return cities;
}

int main() {
    // Example file format: "cities.txt" with the number of cities and city information
    vector<City> cities = readCitiesFromFile("cities.txt");

    int numCities = cities.size();

    // Set parameters for Simulated Annealing
    double initialTemperature = 1000.0;
    double coolingRate = 0.003;
    int maxIterations = 10000;

    // Find the best tour using Simulated Annealing
    vector<int> bestTour = simulatedAnnealing(cities, initialTemperature, coolingRate, maxIterations);

    // Output the results
    cout << "Best Tour: ";
    for (int cityIndex : bestTour) {
        cout << cities[cityIndex].name << " ";
    }
    cout << "\nBest Distance: " << calculateTotalDistance(bestTour, cities) << endl;

    return 0;
}
