#include <bits/stdc++.h>
using namespace std;

struct Order {
    int timestamp;        // smaller = older order
    string customerName;
    string orderInfo;
};

// generate random sample orders
vector<Order> generateOrders(int n) {
    vector<Order> orders;
    orders.reserve(n);

    for(int i = 0; i < n; i++) {
        Order o;
        o.timestamp = rand() % 100000; // random time
        o.customerName = "Customer_" + to_string(i+1);
        o.orderInfo = "Order_" + to_string(i+1);
        orders.push_back(o);
    }
    return orders;
}

// merge two parts
void mergeArrays(vector<Order> &a, int left, int mid, int right) {
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    vector<Order> leftArr(leftSize);
    vector<Order> rightArr(rightSize);

    for(int i = 0; i < leftSize; i++)
        leftArr[i] = a[left + i];

    for(int i = 0; i < rightSize; i++)
        rightArr[i] = a[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while(i < leftSize && j < rightSize) {
        if(leftArr[i].timestamp <= rightArr[j].timestamp) {
            a[k] = leftArr[i];
            i++;
        } else {
            a[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while(i < leftSize) {
        a[k] = leftArr[i];
        i++; k++;
    }
    while(j < rightSize) {
        a[k] = rightArr[j];
        j++; k++;
    }
}

// merge sort
void mergeSort(vector<Order> &a, int left, int right) {
    if(left >= right) return;

    int mid = (left + right) / 2;

    mergeSort(a, left, mid);
    mergeSort(a, mid + 1, right);
    mergeArrays(a, left, mid, right);
}

int main() {
    srand(time(0));

    int n = 10; // change to 1,000,000 for real test
    vector<Order> orders = generateOrders(n);

    cout << "Before sorting:\n";
    for(auto &o : orders)
        cout << o.timestamp << " | " << o.customerName << "\n";

    mergeSort(orders, 0, orders.size() - 1);

    cout << "\nAfter sorting:\n";
    for(auto &o : orders)
        cout << o.timestamp << " | " << o.customerName << "\n";

    return 0;
}
