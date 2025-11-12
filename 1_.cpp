#include <bits/stdc++.h>
using namespace std;


void merge(vector<int> &a, int left, int mid, int right)
{
    int leftsize = mid -left+1;
    int rightsize = right -mid;
    vector<int> leftarr;
    leftarr.resize(leftsize);
    vector<int> rightarr;
    rightarr.resize(rightsize);
    for(int i =0; i< leftsize;i++)
    {
        leftarr[i] = a[left+i];
    }

    for(int i = 0; i<rightsize; i++)
    {
        rightarr[i] = a[mid+i+1];
    }
    int i = 0, j = 0, k = left;
    while(i<leftsize && j < rightsize){
        if(leftarr[i] <= rightarr[j]){
            a[k] = leftarr[i];
            i ++ ;
        }
        else{
            a[k] = rightarr[j];
            j++;
        }
        k++;
    }

    while(i< leftsize){
        a[k] = leftarr[i];
        i++;
        k++;
    }
    while(j<rightsize){
        a[k] = rightarr[j];
        j++;
        k++;
    }
}
void mergesort(vector<int> &a,int left, int right)
{
    if(left>=right) return;

    int mid = (left+right)/2;

    mergesort(a,left,mid);
    mergesort(a,mid+1,right);
    merge(a,left,mid,right);
}
int main(){
    vector<int> a = {4,2,46,1,3,52,32};

    mergesort(a,0,a.size()-1);
    for(int num : a)cout << num << " ";
}