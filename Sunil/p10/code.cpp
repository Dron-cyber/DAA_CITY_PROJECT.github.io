
#include <iostream>
#include <vector>
using namespace std;

// Build Longest Prefix Suffix (LPS) array for KMP
vector<int> buildLPS(const vector<int>& pattern) {
    int m = pattern.size();
    vector<int> lps(m, 0);

    int len = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

// KMP algorithm to search pattern in text
bool KMPSearch(const vector<int>& text, const vector<int>& pattern) {
    int n = text.size();
    int m = pattern.size();

    if (m > n) return false;

    vector<int> lps = buildLPS(pattern);

    int i = 0;  // index for text
    int j = 0;  // index for pattern

    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
        }

        if (j == m) {
            return true;     // Pattern found
        }

        else if (i < n && text[i] != pattern[j]) {
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }
    return false;
}

int main() {
    vector<int> historicalData = {20, 30, 40, 60, 80, 90, 100};
    vector<int> realtimeData   = {60, 80, 90};

    cout << "Historical Traffic Data: ";
    for (int x : historicalData) cout << x << " ";
    cout << "\nReal-Time Pattern: ";
    for (int x : realtimeData) cout << x << " ";
    cout << "\n\n";

    bool congestion = KMPSearch(historicalData, realtimeData);

    if (congestion)
        cout << "Prediction: Heavy traffic incoming → Rerouting recommended.\n";
    else
        cout << "Prediction: No congestion expected.\n";

    return 0;
}
