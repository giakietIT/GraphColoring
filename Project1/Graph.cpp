#include "Graph.h"
#include <iostream>
#include <list>
#include <vector>
#include <functional>
#include <random>
#include <algorithm>
#include <fstream>
#include <unordered_set>
#include <set>
using namespace std;

void Graph::generateGraph(int numVertices, const std::string& filename) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 1);

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    file << numVertices << std::endl;
    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            int edge = distribution(generator);
            if (edge == 1) {
                file << i << " " << j << std::endl;
            }
        }
    }

    file.close();
    std::cout << "Graph data generated and saved to: " << filename << std::endl;

    Graph randomGraph(numVertices);
    randomGraph.loadFromFile(filename);
    
}

void Graph::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return;
    }

    file >> V;
    adj = new std::list<int>[V];

    int u, v;
    while (file >> u >> v) {
        addEdge(u, v);
    }

    file.close();
    std::cout << "Graph data loaded from: " << filename << std::endl;
}

void Graph::printTotalColorsUsed(const vector<int>& colors) {
    unordered_set<int> usedColors;
    for (int color : colors) {
        usedColors.insert(color);
    }
    int totalColorsUsed = usedColors.size();
    cout << "Tong so mau da dung: " << totalColorsUsed << endl;
}
void Graph::greedyColoring() {
    vector<int> result(V, -1);
    result[0] = 0;

    bool* available = new bool[V];
    for (int cr = 0; cr < V; cr++)
        available[cr] = true;

    for (int u = 1; u < V; u++) {
        for (int i : adj[u]) {
            if (result[i] != -1)
                available[result[i]] = false;
        }

        int cr;
        for (cr = 0; cr < V; cr++) {
            if (available[cr])
                break;
        }

        result[u] = cr;

        for (int i : adj[u]) {
            if (result[i] != -1)
                available[result[i]] = true;
        }
    }

    cout << "Solution: ";
    for (int i = 0; i < V; i++) {
        cout << result[i] << " ";
    }
    cout << endl;
    printTotalColorsUsed(result);

    delete[] available;
}

void Graph::bruteForceColoring() {
    vector<int> result(V, -1);
    function<bool(int)> colorVertex = [&](int u) {
        for (int c = 0; c < V; c++) {
            bool isSafe = true;
            for (int v : adj[u])
                if (result[v] == c) {
                    isSafe = false;
                    break;
                }
            if (isSafe) {
                result[u] = c;
                if (u == V - 1 || colorVertex(u + 1))
                    return true;
                result[u] = -1;
            }
        }
        return false;
        };
    colorVertex(0);

    cout << "Solution: ";
    for (int i = 0; i < V; i++) {
        cout << result[i] << " ";
    }
    cout << endl;
    printTotalColorsUsed(result);

}

bool Graph::isSafe(int v, int color, vector<int>& result) {
    for (const auto& i : adj[v]) {
        if (result[i] == color) {
            return false;
        }
    }
    return true;
}




vector<int> Graph::geneticColoring(int numColors, int populationSize, int numGenerations) {
    vector<vector<int>> population(populationSize, vector<int>(V));
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(0, numColors - 1);

    // Khoi tao quan the ban dau
    for (int i = 0; i < populationSize; i++) {
        for (int j = 0; j < V; j++) {
            population[i][j] = distribution(generator);
        }
    }

    for (int generation = 0; generation < numGenerations; generation++) {
        vector<pair<int, int>> fitnessScores;

        // Danh gia fitness cho tung ca the trong quan the
        for (int i = 0; i < populationSize; i++) {
            int fitness = 0;
            for (int j = 0; j < V; j++) {
                if (isSafe(j, population[i][j], population[i])) {
                    fitness++;
                }
            }
            fitnessScores.push_back(make_pair(fitness, i));
        }

        // Sap xep theo thu tu giam dan cua fitness
        sort(fitnessScores.rbegin(), fitnessScores.rend());

        // NNeu tra ve loi giai tot nhat, tra ve ket qua
        if (fitnessScores[0].first == V) {
            return population[fitnessScores[0].second];
        }

        vector<vector<int>> newPopulation(populationSize, vector<int>(V));

        // Chon loc
        int eliteCount = static_cast<int>(0.1 * populationSize);
        for (int i = 0; i < eliteCount; i++) {
            newPopulation[i] = population[fitnessScores[i].second];
        }

        // Lai ghep va dot bien
        for (int i = eliteCount; i < populationSize; i++) {
            int parent1Idx = i % (populationSize / 2);
            int parent2Idx = (i + 1) % (populationSize / 2);

            // Lai ghep
            int crossoverPoint = distribution(generator);

            for (int j = 0; j < crossoverPoint; j++) {
                newPopulation[i][j] = population[parent1Idx][j];
            }

            for (int j = crossoverPoint; j < V; j++) {
                newPopulation[i][j] = population[parent2Idx][j];
            }

            // Dot bien
            int mutationPoint = distribution(generator);
            newPopulation[i][mutationPoint] = distribution(generator);
        }

        population.swap(newPopulation);
    }
    printTotalColorsUsed(population[0]);
    // Tra ve ca the tot nhat trong quan the cuoi cung
    return population[0];
}

//
//void Graph::DSatur() {
//    vector<bool> used(V, false);
//    vector<int> result(V, -1);
//    vector<int> saturationDegree(V, 0);
//    set<nodeInfo, maxSat> priorityQueue;
//
//    // Initialize saturation degree for each vertex
//    for (int u = 0; u < V; ++u) {
//        saturationDegree[u] = adj[u].size();
//        priorityQueue.emplace(nodeInfo{ 0, saturationDegree[u], u });
//    }
//
//    while (!priorityQueue.empty()) {
//        auto maxNode = *priorityQueue.begin();
//        priorityQueue.erase(priorityQueue.begin());
//        int u = maxNode.vertex;
//
//        // Find the lowest feasible color for vertex u
//        int i = 0;
//        for (; i < V; ++i) {
//            if (!used[i]) {
//                break;
//            }
//        }
//        result[u] = i;
//
//        // Mark u as colored
//        used[u] = true;
//    }
//
//    cout << "Solution: ";
//    for (int i = 0; i < V; i++) {
//        cout << result[i] << " ";
//    }
//    cout << endl;
//}