#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <set>

using namespace std;

int main(int argc, char *argsv[])
{
    srand(time(NULL));

    int numberOfFiles = 1, SIZE = 1000;
    if (argc > 1)
        numberOfFiles = atoi(argsv[1]);
    if (argc > 2)
        SIZE = atoi(argsv[2]);

    cout << "number of files:" << numberOfFiles << "\nsize:" << SIZE << endl;
    for (int i = 0; i < numberOfFiles; i++)
    {
        string name = "file" + to_string(i);
        ofstream inputFile("./tests/" +name + ".txt");
        if (!inputFile.is_open())
        {
            cerr << "Failed to open the file." << endl;
            return 1;
        }

        int n = rand() % 70 + 5;
        int x, y;
        set<pair<int, int>> points;

        while (points.size() < n)
        {
            x = rand() % SIZE;
            y = rand() % SIZE;
            points.insert({x, y});
        }

        int id = 1;
        inputFile << n << endl;
        
        for( auto point = points.begin(); point != points.end(); ++point ){
            inputFile << id << " " << point->first << " " << point->second << endl;
            id++;
        }
    }
    return 0;
}