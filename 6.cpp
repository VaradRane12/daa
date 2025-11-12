#include <bits/stdc++.h>
using namespace std;

struct Item {
    string name;
    int weight, value, priority; // smaller = higher priority
};

vector<vector<int>> knapsackDP(const vector<Item>& items, int W) {
    int n = items.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
    for (int i = 1; i <= n; ++i) {
        const Item& it = items[i - 1];
        for (int w = 0; w <= W; ++w) {
            if (it.weight <= w)
                dp[i][w] = max(it.value + dp[i - 1][w - it.weight], dp[i - 1][w]);
            else
                dp[i][w] = dp[i - 1][w];
        }
    }
    return dp;
}

vector<Item> getSelectedItems(const vector<Item>& items, int W, const vector<vector<int>>& dp) {
    vector<Item> selected;
    int n = items.size(), w = W;
    for (int i = n; i > 0; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            const Item& it = items[i - 1];
            selected.push_back(it);
            w -= it.weight;
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
    if (!(cin >> n)) return 0;

    vector<Item> items;
    items.reserve(n);
    for (int i = 0; i < n; ++i) {
        cout << "\nItem #" << (i + 1) << "\n";
        cout << "Name: ";
        string name; cin >> name; // token like Java's next()
        cout << "Weight (kg): ";
        int weight; cin >> weight;
        cout << "Utility Value: ";
        int value; cin >> value;
        cout << "Priority (1=High, 2=Medium, 3=Low): ";
        int p; cin >> p;
        items.push_back({name, weight, value, p});
    }

    cout << "\nEnter truck capacity (W in kg): ";
    int W; cin >> W;

    sort(items.begin(), items.end(), [](const Item& a, const Item& b){
        return a.priority < b.priority;
    });

    auto dp = knapsackDP(items, W);
    auto selected = getSelectedItems(items, W, dp);

    int totalValue = dp[(int)items.size()][W];
    int totalWeight = 0;
    for (const auto& it : selected) totalWeight += it.weight;

    cout << "\n===== Optimal Resource Allocation =====\n";
    for (const auto& it : selected) {
        cout << it.name << " | Weight: " << it.weight
             << " | Value: " << it.value
             << " | Priority: " << it.priority << "\n";
    }
    cout << "------------------------------------\n";
    cout << "Total Weight: " << totalWeight << " / " << W << "\n";
    cout << "Total Utility: " << totalValue << "\n";

    return 0;
}
