#include <bits/stdc++.h> 
using namespace std; 
#define INF 9999999 
int N; // Number of cities 
int cost[10][10]; // Cost matrix 
int bestCost = INF; 
vector<int> bestPath; 
// Function to find the minimum edge cost outgoing from a city 
int firstMin(int i) { 
int minVal = INF; 
for (int k = 0; k < N; k++) 
if (cost[i][k] != 0 && cost[i][k] < minVal) 
minVal = cost[i][k]; 
return minVal; 
} 
// Function to find the second minimum edge cost outgoing from a city 
int secondMin(int i) { 
int first = INF, second = INF; 
for (int j = 0; j < N; j++) { 
if (i == j) continue; 
if (cost[i][j] <= first) { 
second = first; 
first = cost[i][j]; 
} else if (cost[i][j] < second && cost[i][j] != first) 
second = cost[i][j]; 
} 
return second; 
} 
// Recursive function for Branch and Bound TSP 
void TSP(int currBound, int currWeight, int level, vector<int> &currPath, vector<bool> &visited) { 
// Base Case: all cities visited 
if (level == N) { 
// Check final edge back to start 
if (cost[currPath[level - 1]][currPath[0]] != 0) { 
int finalCost = currWeight + cost[currPath[level - 1]][currPath[0]]; 
if (finalCost < bestCost) { 
bestPath = currPath; 
bestPath.push_back(currPath[0]); 
bestCost = finalCost; 
} 
} 
return; 
} 
// Try all possible next cities 
for (int i = 0; i < N; i++) { 
if (cost[currPath[level - 1]][i] != 0 && !visited[i]) { 
int temp = currBound; 
currWeight += cost[currPath[level - 1]][i]; 
// Calculate new lower bound 
if (level == 1) 
currBound -= ((firstMin(currPath[level - 1]) + firstMin(i)) / 2); 
else 
currBound -= ((secondMin(currPath[level - 1]) + firstMin(i)) / 2); 
// If promising, continue recursion 
if (currBound + currWeight < bestCost) { 
currPath[level] = i; 
visited[i] = true; 
TSP(currBound, currWeight, level + 1, currPath, visited); 
} 
// Backtrack 
currWeight -= cost[currPath[level - 1]][i]; 
currBound = temp; 
fill(visited.begin(), visited.end(), false); 
for (int j = 0; j < level; j++) 
visited[currPath[j]] = true; 
} 
} 
} 
int main() { 
cout << "Enter number of cities: "; 
cin >> N; 
cout << "Enter cost matrix (" << N << "x" << N << "):\n"; 
for (int i = 0; i < N; i++) 
for (int j = 0; j < N; j++) 
cin >> cost[i][j]; 
int currBound = 0; 
vector<int> currPath(N + 1, -1); 
vector<bool> visited(N, false); 
// Calculate initial lower bound 
for (int i = 0; i < N; i++) 
currBound += (firstMin(i) + secondMin(i)); 
currBound = (currBound & 1) ? currBound / 2 + 1 : currBound / 2; 
visited[0] = true; 
currPath[0] = 0; 
// Call TSP function 
TSP(currBound, 0, 1, currPath, visited); 
cout << "\nMinimum Cost: " << bestCost; 
cout << "\nOptimal Path: "; 
for (int i : bestPath) 
cout << i << " "; 
return 0; 
}