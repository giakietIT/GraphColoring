#include <iostream>
#include <fstream>
#include "Graph.h"
#include <chrono>
using namespace std;
using namespace std::chrono;



int main() {
    Graph g1;
    g1.generateGraph(600, "graph600.txt");
    std::cout << "Graph generated and saved to graph.txt." << std::endl;

    g1.loadFromFile("graph600.txt");
    cout << "Coloring of graph 1 using Greedy algorithm:\n";
    auto startGreedy = high_resolution_clock::now();
    g1.greedyColoring();
    auto endGreedy = high_resolution_clock::now(); 
    auto durationGreedy = duration_cast<microseconds>(endGreedy - startGreedy); 
    cout << "\nTime taken by Greedy algorithm: " << durationGreedy.count() << " microseconds" << endl;

    cout << "\n----------------------------------------------\n";

    cout << "\nColoring of graph 1 using Brute Force algorithm:\n";
    auto startBruteForce = high_resolution_clock::now();
    g1.bruteForceColoring();
    auto endBruteForce = high_resolution_clock::now();
    auto durationBruteForce = duration_cast<microseconds>(endBruteForce - startBruteForce);
    cout << "\nTime taken by BruteForce algorithm: " << durationBruteForce.count() << " microseconds" << endl;

    cout << "\n----------------------------------------------\n";





    cout << "\nColoring of graph 1 using Genetic algorithm:\n";

    int numColors = 3; 
    int populationSize = 50; 
    int numGenerations = 100; 
    auto startGenetic = high_resolution_clock::now();
    vector<int> solution = g1.geneticColoring(numColors, populationSize, numGenerations);
    auto endGenetic = high_resolution_clock::now();
    auto durationGenetic = duration_cast<microseconds>(endGenetic - startGenetic);
    cout << "Solution: ";
    for (int i = 0; i < 5; i++) {
        cout << solution[i] << " ";
    }
    cout << endl;
    cout << "\nTime taken by Genetic algorithm: " << durationGenetic.count() << " microseconds" << endl;

    

    

    return 0;
}