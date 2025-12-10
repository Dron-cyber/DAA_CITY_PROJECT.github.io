
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Lane {
    int id;
    int queueLength;
};

// Function to choose lane with longest queue (Greedy)
int getNextGreenLane(const vector<Lane> &lanes) {
    return max_element(
        lanes.begin(),
        lanes.end(),
        [](const Lane &a, const Lane &b) {
            return a.queueLength < b.queueLength;
        }
    )->id;
}

int main() {
    int n;
    cout << "Enter number of lanes: ";
    cin >> n;

    vector<Lane> lanes(n);

    cout << "Enter queue lengths for " << n << " lanes:\n";
    for (int i = 0; i < n; i++) {
        lanes[i].id = i + 1;
        cout << "Lane " << lanes[i].id << ": ";
        cin >> lanes[i].queueLength;
    }

    int nextLane = getNextGreenLane(lanes);

    cout << "\nNext green light should be given to Lane " << nextLane << endl;
    return 0;
}
