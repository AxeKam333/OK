#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

const int Maxsize = 100;

using namespace std;

struct City
{
    int name;
    int x, y;
};

double calculateDistance(const City &a, const City &b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void calculateDistanceTable(const vector<City> &cities, double **DistanceTable, int size)
{

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            DistanceTable[i][j] = calculateDistance(cities[i], cities[j]);
        }
    }
}

double calculateDistance(const City &a, const City &b, double DistanceTable[][Maxsize])
{
    return DistanceTable[a.name - 1][b.name - 1];
}

double calculateTotalDistance(const vector<int> &tour, const vector<City> &cities)
{
    double distance = 0.0;
    int n = tour.size();
    for (int i = 0; i < n - 1; ++i)
    {
        distance += calculateDistance(cities[tour[i]], cities[tour[i + 1]]);
    }
    distance += calculateDistance(cities[tour[n - 1]], cities[tour[0]]); // Return to the starting city
    return distance;
}

double calculateDistanceDifference(const vector<int> &tour, const vector<int> &newTour, const vector<City> &cities, int swaps[], int swapNumber)
{
    double distance = 0.0;

    for (int i = 0; i < swapNumber; i++)
    {
        distance -= calculateDistance(cities[tour[swaps[i] - 1]], cities[tour[swaps[i]]]);
        distance -= calculateDistance(cities[tour[swaps[i]]], cities[tour[swaps[i] + 1]]);
    }

    for (int i = 0; i < swapNumber; i++)
    {
        distance += calculateDistance(cities[newTour[swaps[i] - 1]], cities[newTour[swaps[i]]]);
        distance += calculateDistance(cities[newTour[swaps[i]]], cities[newTour[swaps[i] + 1]]);
    }

    return distance;
}

vector<int> generateRandomTour(int n)
{
    vector<int> tour(n);
    for (int i = 0; i < n; ++i)
    {
        tour[i] = i;
    }
    shuffle(tour.begin() + 1, tour.end(), std::default_random_engine(std::random_device()()));
    return tour;
}

vector<int> generateGreedyTour(const vector<City> &cities)
{
    int n = cities.size();
    vector<bool> visited(n, false);
    vector<int> tour;
    tour.reserve(n);

    int currentCityIndex = 0;
    tour.push_back(currentCityIndex);
    visited[currentCityIndex] = true;

    while (tour.size() < n)
    {
        double minDistance = numeric_limits<double>::max();
        int nearestCityIndex = -1;

        for (int i = 0; i < n; ++i)
        {
            if (!visited[i])
            {
                double distance = calculateDistance(cities[currentCityIndex], cities[i]);
                if (distance < minDistance)
                {
                    minDistance = distance;
                    nearestCityIndex = i;
                }
            }
        }

        currentCityIndex = nearestCityIndex;
        tour.push_back(currentCityIndex);
        visited[currentCityIndex] = true;
    }

    return tour;
}

vector<int> simulatedAnnealing(const vector<City> &cities, double initialTemperature,
                               double coolingRate, int maxIteration, int maxTimeSeconds)
{
    srand(time(nullptr));
    clock_t startTime = clock();
    int n = cities.size();
    vector<int> currentTour = generateGreedyTour(cities);
    vector<int> bestTour = currentTour;

    double currentDistance = calculateTotalDistance(currentTour, cities);
    double bestDistance = currentDistance;
    int swap_number = 2;
    // for (int swap_number = 2; swap_number < n; swap_number++)
    // {
        cout << "swap_number: " << swap_number << endl;
        int swaps[swap_number];
        for (int iteration = 0; iteration < maxIteration; ++iteration)
        {
            cout<<"iteration: "<<iteration<<endl;
            double temperature = initialTemperature * exp(-coolingRate * iteration);

            vector<int> newTour = currentTour;
            for (int i = 0; i < swap_number; i++)
                swaps[i] = rand() % (n - 1) + 1;

            for (int i = 0; i < swap_number - 1; i++)
            {
                swap(newTour[swaps[i]], newTour[swaps[i + 1]]);
            }
            swap(newTour[0], newTour[swap_number]);

            double newDistance = calculateDistanceDifference(currentTour, newTour, cities, swaps, swap_number) + currentDistance; // policzyć zmienione krawędzie, zamiast od nowa

            if (newDistance < currentDistance || (rand() / (RAND_MAX + 1.0)) < exp((currentDistance - newDistance) / temperature))
            {
                currentTour = newTour;
                currentDistance = newDistance;
            }

            if (currentDistance < bestDistance)
            {
                // cout << currentDistance << " " << bestDistance << endl;
                bestTour = currentTour;
                bestDistance = currentDistance;
            }
            clock_t currentTime = clock();
            double elapsedTime = static_cast<double>(currentTime - startTime) / CLOCKS_PER_SEC;
            if (elapsedTime >= maxTimeSeconds)
            {
                cout << "Time limit exceeded" << endl;
                return bestTour;
            }
        }
    // }

    return bestTour;
}

vector<City> readCitiesFromFile(const string &filename)
{
    vector<City> cities;
    ifstream file(filename);
    if (file.is_open())
    {
        int numCities;
        file >> numCities;

        for (int i = 0; i < numCities; ++i)
        {
            City city;
            file >> city.name >> city.x >> city.y;
            cities.push_back(city);
        }

        file.close();
    }
    else
    {
        cerr << "Unable to open file: " << filename << endl;
    }
    return cities;
}

int main(int argc, char *argsv[])
{

    string name = "grupa7_1";
    if (argc > 1)
        name = argsv[1];

    vector<City> cities = readCitiesFromFile("../tests/" + name + ".txt");

    int numCities = cities.size();

    double **DistanceTable = new double* [Maxsize];
    for (int i = 0; i < Maxsize; i++)
        DistanceTable[i] = new double[Maxsize];

    // double DistanceTable[Maxsize][Maxsize] = {};
    calculateDistanceTable(cities, DistanceTable, numCities);

    cout << "origin" << endl;

    double initialTemperature = 1000000.0;
    double coolingRate = 0.000003;
    int maxIterations = 10000000;

    int timeSeconds = 30;

    cout<<"initialTemperature: "<<initialTemperature<<endl;
    cout<<"coolingRate: "<<coolingRate<<endl;
    cout<<"maxIterations: "<<maxIterations<<endl;
    cout<<"timeSeconds: "<<timeSeconds<<endl;

    vector<int> bestTour = simulatedAnnealing(cities, initialTemperature, coolingRate, maxIterations, timeSeconds);

    cout << "Best Tour: ";
    for (int cityIndex : bestTour)
    {
        cout << cities[cityIndex].name << " ";
    }
    cout << "\nBest Distance: " << calculateTotalDistance(bestTour, cities) << endl;

    return 0;
}
