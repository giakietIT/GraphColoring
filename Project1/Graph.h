#include <iostream>
#include <list>
#include <vector>
#include <functional>
#include <random>
using namespace std;

class Graph {
private:
    int V;
    list<int>* adj;
public:
    void clear() {
        adj = {};
        V = 0; 
    }
    Graph() {}
    Graph(int V) : V(V) {
        adj = new list<int>[V];
    }
    ~Graph() {
        delete[] adj;
    }
    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }
    int getV() const {
        return V;
    }
    void printTotalColorsUsed(const vector<int>& colors);
    //Greedy
    void greedyColoring();
    //BruteForce
    void bruteForceColoring();
    //Genetic
    bool isSafe(int v, int color, vector<int>& result);
    vector<int> geneticColoring(int numColors, int populationSize, int numGenerations);
    static void generateGraph(int numVertices, const std::string& filename);
    void loadFromFile(const std::string& filename);

    /*void DSatur();*/
};

//Graph.cpp


