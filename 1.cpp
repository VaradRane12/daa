#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdlib>

#define NUM_ORDERS 1000000 // Can reduce for testing
using namespace std;

struct Order {
    string order_id;
    time_t timestamp;
};

// Generate random sample orders
void generate_sample_orders(vector<Order>& orders, int n) {
    tm base_time = {};
    base_time.tm_year = 2025 - 1900;
    base_time.tm_mon = 5; // June (0-based)
    base_time.tm_mday = 24;
    base_time.tm_hour = 12;

    time_t base = mktime(&base_time);

    for (int i = 0; i < n; i++) {
        int random_minutes = rand() % 100000; // up to ~70 days
        time_t ts = base + (random_minutes * 60);

        Order o;
        o.order_id = "ORD" + to_string(i + 1);
        o.timestamp = ts;

        orders.push_back(o);
    }
}

// Merge function for merge sort
void merge(vector<Order>& orders, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Order> L(n1);
    vector<Order> R(n2);

    for (int i = 0; i < n1; i++) L[i] = orders[left + i];
    for (int j = 0; j < n2; j++) R[j] = orders[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].timestamp <= R[j].timestamp)
            orders[k++] = L[i++];
        else
            orders[k++] = R[j++];
    }

    while (i < n1) orders[k++] = L[i++];
    while (j < n2) orders[k++] = R[j++];
}

// Merge Sort recursive function
void merge_sort(vector<Order>& orders, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(orders, left, mid);
        merge_sort(orders, mid + 1, right);
        merge(orders, left, mid, right);
    }
}

// Print first N orders
void print_first_n_orders(const vector<Order>& orders, int n) {
    char buffer[30];
    for (int i = 0; i < n; i++) {
        tm* tm_info = gmtime(&orders[i].timestamp);
        strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", tm_info);
        cout << "Order ID: " << orders[i].order_id
             << ", Timestamp: " << buffer << endl;
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    vector<Order> orders;
    orders.reserve(NUM_ORDERS);

    cout << "Generating orders..." << endl;
    generate_sample_orders(orders, NUM_ORDERS);

    cout << "Sorting orders by timestamp..." << endl;
    clock_t start = clock();
    merge_sort(orders, 0, NUM_ORDERS - 1);
    clock_t end = clock();

    double time_taken = double(end - start) / CLOCKS_PER_SEC;

    cout << "Done! Sorted " << NUM_ORDERS
         << " orders in " << fixed << setprecision(2)
         << time_taken << " seconds." << endl;

    cout << "\n✅ All Sorted Orders:\n";
    print_first_n_orders(orders, min(NUM_ORDERS, 20)); // print first 20 to avoid long output

    return 0;
}