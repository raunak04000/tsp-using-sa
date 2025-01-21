#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

struct City {
    double x, y;
};

double euclideanDistance(const City& a, const City& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double calculateTourCost(const vector<City>& cities, const vector<int>& tour) {
    double cost = 0;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        cost += euclideanDistance(cities[tour[i]], cities[tour[i + 1]]);
    }
    cost += euclideanDistance(cities[tour.back()], cities[tour.front()]); // Return to start
    return cost;
}

vector<int> generateInitialSolution(int n) {
    vector<int> tour(n);
    for (int i = 0; i < n; ++i) tour[i] = i;
    random_shuffle(tour.begin(), tour.end());
    return tour;
}

vector<int> getNeighbor(const vector<int>& currentTour) {
    vector<int> newTour = currentTour;
    int i = rand() % newTour.size();
    int j = rand() % newTour.size();
    swap(newTour[i], newTour[j]);
    return newTour;
}

bool acceptSolution(double currentCost, double newCost, double temperature) {
    if (newCost < currentCost) return true;
    double probability = exp((currentCost - newCost) / temperature);
    return (rand() / (double)RAND_MAX) < probability;
}

void simulatedAnnealing(vector<City>& cities, int iterations, double initialTemp, double coolingRate) {
    int n = cities.size();
    vector<int> currentSolution = generateInitialSolution(n);
    double currentCost = calculateTourCost(cities, currentSolution);

    vector<int> bestSolution = currentSolution;
    double bestCost = currentCost;

    double temperature = initialTemp;

    for (int iter = 0; iter < iterations; ++iter) {
        vector<int> newSolution = getNeighbor(currentSolution);
        double newCost = calculateTourCost(cities, newSolution);

        if (acceptSolution(currentCost, newCost, temperature)) {
            currentSolution = newSolution;
            currentCost = newCost;

            if (currentCost < bestCost) {
                bestSolution = currentSolution;
                bestCost = currentCost;
            }
        }

        temperature *= coolingRate;
    }

    cout << "Best tour cost: " << bestCost << endl;
    cout << "Best tour: ";
    for (int city : bestSolution) {
        cout << city << " ";
    }
    cout << endl;
}

int main() {
    srand(time(0));

    // Example: Define cities
    vector<City> cities = {
        {0, 0}, {1, 3}, {4, 3}, {6, 1}, {3, 0}
    };

    int iterations = 10000;
    double initialTemp = 1000.0;
    double coolingRate = 0.995;

    simulatedAnnealing(cities, iterations, initialTemp, coolingRate);

    return 0;
}
