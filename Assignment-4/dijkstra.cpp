#include <bits/stdc++.h>
using namespace std;

void dijkstra(int source, vector<vector<pair<int,int>>> &graph, vector<int> &dist) {
    int V = graph.size();
    dist.assign(V, INT_MAX);
    dist[source] = 0;

    // Min-heap priority queue: (distance, node)
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto &edge : graph[u]) {
            int v = edge.first;
            // cout<<"edge_first"<<v<<endl;
            int w = edge.second;
            // cout << "edge_second"<<w<<endl;


            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

// Debug function to print adjacency list
void printGraph(const vector<vector<pair<int,int>>> &graph) {
    cout << "Graph adjacency list:\n";
    for (int u = 0; u < graph.size(); u++) {
        cout << u << " -> ";
        for (auto &edge : graph[u]) {
            cout << "(" << edge.first << "," << edge.second << ") ";
        }
        cout << "\n";
    }
    cout << "--------------------------\n";
}

int main() {
    int V = 5; // number of intersections
    int E = 6; // number of roads

    vector<vector<pair<int,int>>> graph(V);

    // (u, v, w) format: road from u to v with weight w
    vector<tuple<int,int,int>> edges = {
        make_tuple(0, 1, 2),
        make_tuple(0, 2, 4),
        make_tuple(1, 2, 1),
        make_tuple(1, 3, 7),
        make_tuple(2, 4, 3),
        make_tuple(3, 4, 2)
    };

    // Build graph (undirected)
    for (auto &edge : edges) {
        int u, v, w;
        tie(u, v, w) = edge;   // unpack tuple
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }

    // Print adjacency list (debug)
    printGraph(graph);

    int source = 0; // ambulance start location
    vector<int> hospitals = {3, 4}; // hospital nodes

    // Run Dijkstra
    vector<int> dist;
    dijkstra(source, graph, dist);

    // Find nearest hospital
    int minTime = INT_MAX, nearestHospital = -1;
    for (int h : hospitals) {
        if (dist[h] < minTime) {
            minTime = dist[h];
            nearestHospital = h;
        }
    }

    if (nearestHospital == -1)
        cout << "No hospital reachable.\n";
    else
        cout << "Nearest hospital is at node " << nearestHospital 
             << " with travel time " << minTime << " minutes.\n";

    return 0;
}
