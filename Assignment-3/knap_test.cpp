#include <bits/stdc++.h>
using namespace std;
struct Item
{
double weight, value;
bool divisible;
};
bool compare(const Item &a, const Item &b){
 double r1 = a.value/a.weight;
 double r2 = b.value/b.weight;
 return r1>r2;
}
double franctionalKnapsack(double W, vector<Item> &items){
    sort(items.begin(),items.end(),compare);

    double totalValue = 0.0;
    double currectWeight = 0.0;

    for (auto &item :items)
    {
        if(currectWeight + item.weight <= W){
            currectWeight += item.weight;
            totalValue += item.value;
        }
        else{
            double remain = W-currectWeight;
            if (remain <= 0) break;
            if(item.divisible){
                double fraction = (remain/item.weight);
                totalValue += item.value * fraction;
                currectWeight += remain;
                break;
            } else{continue;}
        }
    }
    return totalValue;
}
int main(){
    vector<Item> item = {
        {10, 60, false},  // Medicine kit
        {20, 100, true},  // Food
        {30, 120, true},  // Water
        {25, 180, false}, // Another medical kit
        {15, 90, true}    // More food
    };
    cout << franctionalKnapsack(50,item);
}