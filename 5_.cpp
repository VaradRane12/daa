#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
using namespace std;

// O(V^2) DP on a multistage DAG (logistics graph)
const int INF = numeric_limits<int>::max();

static void printAdjMatrix(const vector<vector<int>>& a) {
    int n = (int)a.size();
    cout << "\nAdjacency cost matrix (INF = no edge):\n     ";
    for (int j = 0; j < n; ++j) cout << setw(7) << j;
    cout << "\n";
    for (int i = 0; i < n; ++i) {
        cout << setw(3) << i << " |";
        for (int j = 0; j < n; ++j) {
            if (a[i][j] == INF) cout << setw(7) << "INF";
            else                cout << setw(7) << a[i][j];
        }
        cout << "\n";
    }
    cout << "\n";
}

static void printVectorINF(const string& name, const vector<int>& v) {
    cout << name << " = [";
    for (int i = 0; i < (int)v.size(); ++i) {
        if (v[i] == INF) cout << (i ? ", " : "") << "INF";
        else             cout << (i ? ", " : "") << v[i];
    }
    cout << "]\n";
}

void findShortestPath(int numNodes, const vector<vector<int>>& adjMatrix) {
    vector<int> minCost(numNodes, INF);
    vector<int> route(numNodes, -1);

    cout << "\n========== DP Initialization ==========\n";
    cout << "Destination node: " << (numNodes - 1) << "\n";
    minCost[numNodes - 1] = 0;
    printVectorINF("minCost (start)", minCost);
    cout << "route (start)     = [-1 repeated]\n";
    cout << "======================================\n";

    printAdjMatrix(adjMatrix);

    // DP from second-last node down to 0
    cout << "========== DP Iterations ==========\n";
    for (int i = numNodes - 2; i >= 0; --i) {
        cout << "\n-- Processing node i = " << i << " --\n";
        for (int j = i + 1; j < numNodes; ++j) {
            if (adjMatrix[i][j] == INF) {
                cout << "  skip j=" << j << " (no edge i->j)\n";
                continue;
            }
            if (minCost[j] == INF) {
                cout << "  consider j=" << j
                     << " edge=" << adjMatrix[i][j]
                     << " but minCost[j]=INF, cannot improve yet\n";
                continue;
            }
            long long currentCost = (long long)adjMatrix[i][j] + (long long)minCost[j];
            cout << "  check j=" << j
                 << " edge=" << adjMatrix[i][j]
                 << " + minCost[" << j << "]=" << minCost[j]
                 << " => candidate=" << currentCost << "\n";

            if (currentCost < minCost[i]) {
                cout << "    UPDATE: minCost[" << i << "] "
                     << (minCost[i] == INF ? string("INF") : to_string(minCost[i]))
                     << " -> " << currentCost
                     << ", route[" << i << "]=" << j << "\n";
                minCost[i] = (int)currentCost;
                route[i] = j;
            } else {
                cout << "    no improvement (current best="
                     << (minCost[i] == INF ? string("INF") : to_string(minCost[i]))
                     << ")\n";
            }
        }
        printVectorINF("minCost after i="+to_string(i), minCost);
        cout << "route   after i=" << i << " = [";
        for (int k = 0; k < numNodes; ++k) {
            cout << (k ? ", " : "") << route[k];
        }
        cout << "]\n";
    }
    cout << "====================================\n";

    cout << "\n========== Route Optimization Report ==========\n";
    if (minCost[0] == INF) {
        cout << "No valid route exists from the source to the destination.\n";
    } else {
        cout << "Minimum Cost: " << minCost[0] << "\n";
        cout << "Optimal Path: ";
        int currentNode = 0;
        while (currentNode != -1) {
            cout << currentNode;
            if (route[currentNode] != -1) cout << " -> ";
            currentNode = route[currentNode];
        }
        cout << "\n";
    }
    cout << "==============================================\n";
}

int main() {
    int nodeCount, edgeCount;

    cout << "Enter the number of nodes (warehouses/hubs): ";
    cin >> nodeCount;

    cout << "Enter the number of routes (edges): ";
    cin >> edgeCount;

    vector<vector<int>> costGraph(nodeCount, vector<int>(nodeCount, INF));
    vector<tuple<int,int,int>> edges;

    cout << "\nEnter " << edgeCount << " routes (format: source destination cost):\n";
    for (int i = 0; i < edgeCount; ++i) {
        int source, dest, cost;
        cin >> source >> dest >> cost;
        if (source >= 0 && source < nodeCount && dest >= 0 && dest < nodeCount) {
            costGraph[source][dest] = cost;
            edges.emplace_back(source, dest, cost);
        } else {
            cout << "Warning: Invalid node number entered. Skipping this route.\n";
        }
    }

    cout << "\nRaw edge list:\n";
    for (auto &e : edges) {
        int s, d, c;
        tie(s, d, c) = e;
        cout << "  " << s << " -> " << d << "  cost=" << c << "\n";
    }

    findShortestPath(nodeCount, costGraph);
    return 0;
}
