#include <bits/stdc++.h>
using namespace std;

int main() {
    priority_queue<int, vector<int>, greater<int>> minHeap;
    int n, threshold;

    cout << "Enter number of sensors: ";
    cin >> n;

    cout << "Enter gas safety threshold (ppm): ";
    cin >> threshold;

    cout << "Enter gas readings (ppm) from sensors:\n";

    for(int i = 0; i < n; i++) {
        int gas;
        cin >> gas;
        minHeap.push(gas);
    }

    int currentMin = minHeap.top();

    cout << "\nMinimum Gas Concentration Detected: "
         << currentMin << " ppm\n";

    if(currentMin > threshold) {
        cout << " ALERT! Dangerous Gas Leak Detected!\n";
    } else {
        cout << "Safe Environment. No Leak.\n";
    }

    return 0;
}
