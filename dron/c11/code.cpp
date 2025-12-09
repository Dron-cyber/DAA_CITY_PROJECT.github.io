#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Fenwick Tree (Binary Indexed Tree) for range sum queries and point updates
class FenwickTree {
private:
    vector<long long> tree;
    int n;

public:
    FenwickTree(int _n) : n(_n), tree(_n + 1, 0) {}

    // Update the value at index idx by adding val (O(log n))
    void update(int idx, long long val) {
        while (idx <= n) {
            tree[idx] += val;
            idx += idx & -idx;
        }
    }

    // Query the prefix sum from 1 to idx (O(log n))
    long long query(int idx) {
        long long sum = 0;
        while (idx > 0) {
            sum += tree[idx];
            idx -= idx & -idx;
        }
        return sum;
    }

    // Query the range sum from l to r (O(log n))
    long long query(int l, int r) {
        return query(r) - query(l - 1);
    }
};

int main() {
    // Dataset: MonthlyChange values (indices 1 to 20 correspond to S001 to S020)
    vector<long long> initial_changes = {
        500,   // S001 - Market Circle
        -300,  // S002 - Market Circle
        700,   // S003 - Market Circle
        200,   // S004 - Shopping Lane A
        -150,  // S005 - Shopping Lane A
        300,   // S006 - Shopping Lane B
        1200,  // S007 - IT Park Shops
        900,   // S008 - IT Park Shops
        -500,  // S009 - IT Park Shops
        100,   // S010 - Residential Market A
        150,   // S011 - Residential Market A
        -100,  // S012 - Residential Market B
        800,   // S013 - Airport Road Shops
        500,   // S014 - Airport Road Shops
        300,   // S015 - Industrial Market Zone
        -200,  // S016 - Industrial Market Zone
        200,   // S017 - Temple Street Shops
        250,   // S018 - Temple Street Shops
        400,   // S019 - University Road Market
        -150   // S020 - University Road Market
    };

    // Current changes (to handle updates by computing differences)
    vector<long long> current_changes(21, 0); // 1-based indexing
    for (int i = 1; i <= 20; ++i) {
        current_changes[i] = initial_changes[i - 1];
    }

    // Initialize Fenwick Tree with initial changes
    FenwickTree ft(20);
    for (int i = 1; i <= 20; ++i) {
        ft.update(i, current_changes[i]);
    }

    // Map of market segments to their shop index ranges (1-based)
    map<string, pair<int, int>> segments = {
        {"Market Circle", {1, 3}},
        {"Shopping Lane A", {4, 5}},
        {"Shopping Lane B", {6, 6}},
        {"IT Park Shops", {7, 9}},
        {"Residential Market A", {10, 11}},
        {"Residential Market B", {12, 12}},
        {"Airport Road Shops", {13, 14}},
        {"Industrial Market Zone", {15, 16}},
        {"Temple Street Shops", {17, 18}},
        {"University Road Market", {19, 20}}
    };

    cout << "=== Market Shop Rent Analysis (Fenwick Tree) ===\n";
    cout << "Initial data loaded. 20 shops across 10 market segments.\n\n";

    cout << "Available Commands:\n";
    cout << "  update <shop_index> <new_monthly_change>\n";
    cout << "    Example: update 7 1500\n";
    cout << "  query <market_segment_name>\n";
    cout << "    Example: query IT Park Shops\n\n";
    cout << "Market Segments: Market Circle, Shopping Lane A, Shopping Lane B, IT Park Shops,\n";
    cout << "                 Residential Market A, Residential Market B, Airport Road Shops,\n";
    cout << "                 Industrial Market Zone, Temple Street Shops, University Road Market\n\n";

    // User input for operations (updates and queries)
    cout << "Enter the number of operations: ";
    int q;
    cin >> q;
    cin.ignore(); // Ignore the newline after reading q

    for (int qi = 0; qi < q; ++qi) {
        cout << "\nOperation " << (qi + 1) << " (update <shop> <value> OR query <segment>): ";
        string line;
        getline(cin, line);
        stringstream ss(line);
        string type;
        ss >> type;

        if (type == "update") {
            // Format: update <shop_index> <new_monthly_change>
            int idx;
            long long new_val;
            if (!(ss >> idx >> new_val)) {
                cout << "❌ Invalid update format. Use: update <shop_index> <new_monthly_change>\n";
                cout << "   Example: update 5 200\n";
                continue;
            }
            if (idx < 1 || idx > 20) {
                cout << "❌ Invalid shop index. Must be between 1 and 20 (S001-S020).\n";
                continue;
            }
            long long diff = new_val - current_changes[idx];
            current_changes[idx] = new_val;
            ft.update(idx, diff);
            cout << "✅ Updated shop S" << (idx < 10 ? "00" : "0") << idx << " to monthly change " << new_val << endl;
        } else if (type == "query") {
            // Format: query <market_segment>
            string seg;
            getline(ss, seg);
            // Remove leading and trailing whitespace
            size_t start = seg.find_first_not_of(" \t\n\r\f\v");
            if (start == string::npos) seg = "";
            else seg = seg.substr(start, seg.find_last_not_of(" \t\n\r\f\v") - start + 1);
            
            if (segments.count(seg)) {
                pair<int, int> range = segments[seg];
                int l = range.first;
                int r = range.second;
                long long sum = ft.query(l, r);
                cout << "💰 Cumulative change for '" << seg << "': " << sum << endl;
            } else {
                cout << "❌ Invalid market segment. Available segments listed above.\n";
            }
        } else {
            cout << "❌ Invalid operation type. Use 'update' or 'query' only.\n";
            cout << "   Examples:\n";
            cout << "     update 7 1500\n";
            cout << "     query IT Park Shops\n";
        }
    }

    cout << "\n=== Session Complete ===\n";

    return 0;
}

