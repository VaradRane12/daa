#include <bits/stdc++.h>
using namespace std;

void dijkstra(int source, vector<vector<pair<int,int>>> &graph, vector<int> &dist) {
    int V = graph.size();
    dist.assign(V, INT_MAX);
    dist[source] = 0;

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto &edge : graph[u]) {
            int v = edge.first;
            int w = edge.second;

            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

// Function to update dynamic weights
void updateWeight(vector<vector<pair<int,int>>> &graph, int u, int v, int newW) {
    // Update u -> v
    for (auto &edge : graph[u]) {
        if (edge.first == v) {
            edge.second = newW;
            break;
        }
    }
    // Update v -> u (since undirected)
    for (auto &edge : graph[v]) {
        if (edge.first == u) {
            edge.second = newW;
            break;
        }
    }
}

int main() {
    int V, E;
    cout << "Enter number of intersections (vertices): ";
    cin >> V;
    cout << "Enter number of roads (edges): ";
    cin >> E;

    vector<vector<pair<int,int>>> graph(V);

    cout << "Enter edges (u v w):\n";
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w}); // undirected
    }

    int source;
    cout << "Enter ambulance start location (source): ";
    cin >> source;

    int H;
    cout << "Enter number of hospitals: ";
    cin >> H;
    vector<int> hospitals(H);
    cout << "Enter hospital nodes: ";
    for (int i = 0; i < H; i++) {
        cin >> hospitals[i];
    }

    vector<int> dist;
    dijkstra(source, graph, dist);

    auto findNearestHospital = [&](vector<int> &dist, vector<int> &hospitals) {
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
    };

    findNearestHospital(dist, hospitals);

    int Q;
    cout << "\nEnter number of updates (dynamic traffic changes): ";
    cin >> Q;
    while (Q--) {
        int u, v, newW;
        cout << "Enter road to update (u v newWeight): ";
        cin >> u >> v >> newW;
        updateWeight(graph, u, v, newW);

        dijkstra(source, graph, dist);
        findNearestHospital(dist, hospitals);
    }

    return 0;
}
