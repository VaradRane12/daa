// relief_knapsack_simple.cpp
// Simple 0/1 Knapsack with DP + reconstruction.
// Compile: g++ -std=c++17 -O2 relief_knapsack_simple.cpp -o relief_knapsack_simple

#include <bits/stdc++.h>
using namespace std;

struct Item {
    string name;
    int weight;
    int value;
    int priority; // 1 = High, 2 = Medium, 3 = Low
    Item() {}
    Item(string n, int w, int v, int p): name(n), weight(w), value(v), priority(p) {}
};

// Build DP table: dp[i][w] = max value using first i items with capacity w
vector<vector<int>> knapsackDP(const vector<Item>& items, int W) {
    int n = items.size();
    vector<vector<int>> dp(n+1, vector<int>(W+1, 0));
    for (int i = 1; i <= n; ++i) {
        const Item &it = items[i-1];
        for (int w = 0; w <= W; ++w) {
            if (it.weight <= w)
                dp[i][w] = max(it.value + dp[i-1][w - it.weight], dp[i-1][w]);
            else
                dp[i][w] = dp[i-1][w];
        }
    }
    return dp;
}

// Reconstruct selected items from DP table
vector<Item> getSelectedItems(const vector<Item>& items, int W, const vector<vector<int>>& dp) {
    vector<Item> selected;
    int n = items.size();
    int w = W;
    for (int i = n; i > 0; --i) {
        if (dp[i][w] != dp[i-1][w]) {
            selected.push_back(items[i-1]);
            w -= items[i-1].weight;
        }
    }
    reverse(selected.begin(), selected.end());
    return selected;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cout << "Enter number of items: ";
    if (!(cin >> n) || n <= 0) return 0;

    vector<Item> items;
    items.reserve(n);
    for (int i = 0; i < n; ++i) {
        string name;
        int weight, value, p;
        cout << "\nItem #" << (i+1) << '\n';
        cout << "Name: "; cin >> name;
        cout << "Weight (kg): "; cin >> weight;
        cout << "Utility Value: "; cin >> value;
        cout << "Priority (1=High,2=Medium,3=Low): "; cin >> p;
        items.emplace_back(name, weight, value, p);
    }

    int W;
    cout << "\nEnter truck capacity (W in kg): ";
    cin >> W;
    if (W < 0) W = 0;

    // Sort by priority (high first). Stable sort keeps input order for same priority.
    stable_sort(items.begin(), items.end(), [](const Item &a, const Item &b){
        return a.priority < b.priority;
    });

    auto dp = knapsackDP(items, W);
    auto selected = getSelectedItems(items, W, dp);

    int totalValue = dp[items.size()][W];
    int totalWeight = 0;
    for (auto &it : selected) totalWeight += it.weight;

    cout << "\n===== Optimal Resource Allocation =====\n";
    for (auto &it : selected) {
        cout << it.name << " | Weight: " << it.weight
             << " | Value: " << it.value
             << " | Priority: " << it.priority << '\n';
    }
    cout << "------------------------------------\n";
    cout << "Total Weight: " << totalWeight << " / " << W << '\n';
    cout << "Total Utility: " << totalValue << '\n';

    return 0;
}
