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

struct City
{
    string name;
    int x, y;
};

double calculateDistance(const City &a, const City &b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
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
    shuffle(tour.begin() + 1, tour.end(), std::default_random_engine(std::random_device()()));
    return tour;
}

vector<int> simulatedAnnealing(const vector<City> &cities, double initialTemperature,
                               double coolingRate, int maxIterations)
{
    srand(time(nullptr));

    int n = cities.size();
    vector<int> currentTour = generateRandomTour(n);
    vector<int> bestTour = currentTour;

    double currentDistance = calculateTotalDistance(currentTour, cities);
    double bestDistance = currentDistance;

    for (int iteration = 0; iteration < maxIterations; ++iteration)
    {
        double temperature = initialTemperature * exp(-coolingRate * iteration);

        vector<int> newTour = currentTour;
        int pos1 = rand() % (n - 1) + 1; 
        int pos2 = rand() % (n - 1) + 1;
        swap(newTour[pos1], newTour[pos2]);

        double newDistance = calculateTotalDistance(newTour, cities); //policzyć zmienione krawędzie, zamiast od nowa

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

    string name = "cities";
    if (argc > 1)
        name = argsv[1];

    vector<City> cities = readCitiesFromFile("./tests/"+name+".txt");

    int numCities = cities.size();

    double initialTemperature = 100000.0;
    double coolingRate = 0.00003;
    int maxIterations = 10000000;

    vector<int> bestTour = simulatedAnnealing(cities, initialTemperature, coolingRate, maxIterations);

    cout << "Best Tour: ";
    for (int cityIndex : bestTour)
    {
        cout << cities[cityIndex].name << " ";
    }
    cout << "\nBest Distance: " << calculateTotalDistance(bestTour, cities) << endl;

    return 0;
}
