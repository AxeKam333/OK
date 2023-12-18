#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

const int Maxsize = 1000;

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

void calculateDistanceTable(const vector<City> &cities, double DistanceTable[][Maxsize], int size)
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

vector<int> generateRandomTour(int n)
{
    vector<int> tour(n);
    for (int i = 0; i < n; ++i)
    {
        tour[i] = i;
    }
    shuffle(tour.begin() + 1, tour.end(), default_random_engine(random_device()()));
    return tour;
}

pair<vector<int>, double> swapCities(const vector<int> &tour, double currentDistance, const vector<City> &cities, int n, int swapNumber)
{
    vector<int> newTour = tour;
    int swaps[swapNumber];

    for (int i = 0; i < swapNumber / 2; i++)
    {
        swaps[i * 2] = rand() % (n - 1) + 1;
        swaps[i * 2 + 1] = rand() % (n - 1) + 1;
        swap(newTour[swaps[i * 2]], newTour[swaps[i * 2 + 1]]);
    }
    swaps[swapNumber - 1] = rand() % (n - 1) + 1;
    swap(newTour[swaps[swapNumber - 1]], newTour[swaps[0]]);

    double newDistance = 0.0;

    for (int i = 0; i < swapNumber; i++)
    {
        if (swaps[i] == 0)
        {
            newDistance -= calculateDistance(cities[tour[n - 1]], cities[tour[0]]);
            newDistance += calculateDistance(cities[newTour[n - 1]], cities[newTour[0]]);
        }
        else
        {
            newDistance -= calculateDistance(cities[tour[swaps[i] - 1]], cities[tour[swaps[i]]]);
            newDistance += calculateDistance(cities[newTour[swaps[i] - 1]], cities[newTour[swaps[i]]]);
            if (swaps[i] + 1 == n)
            {
                newDistance -= calculateDistance(cities[tour[swaps[i]]], cities[tour[0]]);
                newDistance += calculateDistance(cities[newTour[swaps[i]]], cities[newTour[0]]);
            }
            else
            {
                newDistance -= calculateDistance(cities[tour[swaps[i]]], cities[tour[swaps[i] + 1]]);
                newDistance += calculateDistance(cities[newTour[swaps[i]]], cities[newTour[swaps[i] + 1]]);
            }
        }
    }

    return make_pair(newTour, newDistance);
}

vector<int> simulatedAnnealing(const vector<City> &cities, double initialTemperature,
                               double coolingRate, int maxIterations)
{
    time_t startTime = time(nullptr);
    srand(time(nullptr));

    int n = cities.size();
    vector<int> currentTour = generateRandomTour(n);
    vector<int> bestTour = currentTour;

    double currentDistance = calculateTotalDistance(currentTour, cities);
    double bestDistance = currentDistance;

    for (int swapNumber = 2; swapNumber <= n / 2; swapNumber++)
    {
        for (int iteration = 0; iteration < maxIterations; ++iteration)
        {
            double temperature = initialTemperature * exp(-coolingRate * iteration);

            vector<int> newTour;
            double newDistance;

            tie(newTour, newDistance) = swapCities(currentTour, currentDistance, cities, n, swapNumber);

            if (newDistance < currentDistance || (rand() / (RAND_MAX + 1.0)) < exp((currentDistance - newDistance) / temperature))
            {
                currentTour = newTour;
                currentDistance = newDistance;
            }

            if (currentDistance < bestDistance)
            {
                bestTour = currentTour;
                bestDistance = currentDistance;
            }

            time_t currentTime = time(nullptr);
            double elapsedMinutes = difftime(currentTime, startTime) / 60.0;
            if (elapsedMinutes >= 1)
            {
                cout << "Czas upłynął." << endl;
                return bestTour;
            }
        }
    }

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

    string name = "berlin52";
    if (argc > 1)
        name = argsv[1];

    vector<City> cities = readCitiesFromFile("./tests/" + name + ".txt");

    int numCities = cities.size();

    double DistanceTable[Maxsize][Maxsize] = {};
    calculateDistanceTable(cities, DistanceTable, numCities);

    double initialTemperature = 100000.0;
    double coolingRate = 0.00003;
    int maxIterations = 100000;

    vector<int> bestTour = simulatedAnnealing(cities, initialTemperature, coolingRate, maxIterations);

    cout << "Best Tour: ";
    for (int cityIndex : bestTour)
    {
        cout << cities[cityIndex].name << " ";
    }
    cout << "\nBest Distance: " << calculateTotalDistance(bestTour, cities) << endl;

    return 0;
}
