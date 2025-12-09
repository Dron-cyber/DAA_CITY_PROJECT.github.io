#include <bits/stdc++.h>
using namespace std;

int main() {
    priority_queue<pair<int,int>> maxHeap;
    int n, safeLevel;

    cout << "Enter number of field zones: ";
    cin >> n;

    cout << "Enter safe moisture level (%): ";
    cin >> safeLevel;

    cout << "Enter moisture % for each zone:\n";
    for(int i=1; i<=n; i++){
        int m;
        cin >> m;
        maxHeap.push({100-m, i});

    }

    cout << "\n--- Irrigation Plan ---\n";
    while(!maxHeap.empty()){
        auto zone = maxHeap.top();
        maxHeap.pop();

        int dryness = zone.first;
        int id = zone.second;
        int moisture = 100 - dryness;

        if(moisture < safeLevel){
            cout << "Irrigate Zone " << id
                 << " (Moisture: " << moisture << "%)\n";
        } else {
            cout << "Zone " << id
                 << " is safe. Moisture: "
                 << moisture << "%\n";
        }
    }
    return 0;
}
