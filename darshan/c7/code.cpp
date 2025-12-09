#include <bits/stdc++.h>
using namespace std;

const int N = 100000;
int seg[4*N], arr[N];


void build(int i, int l, int r){
    if(l == r){
        seg[i] = arr[l];
        return;
    }
    int mid = (l+r)/2;
    build(i*2, l, mid);
    build(i*2+1, mid+1, r);
    seg[i] = min(seg[i*2], seg[i*2+1]);
}


int query(int i, int l, int r, int ql, int qr){
    if(qr < l || ql > r) return INT_MAX;
    if(ql <= l && r <= qr) return seg[i];
    int mid = (l+r)/2;
    return min(query(i*2, l, mid, ql, qr),
               query(i*2+1, mid+1, r, ql, qr));
}


void update(int i, int l, int r, int idx, int val){
    if(l == r){
        seg[i] = val;
        arr[idx] = val;
        return;
    }
    int mid = (l+r)/2;
    if(idx <= mid) update(i*2, l, mid, idx, val);
    else update(i*2+1, mid+1, r, idx, val);
    seg[i] = min(seg[i*2], seg[i*2+1]);
}

int main(){
    int n;
    cout << "Enter number of pipeline segments: ";
    cin >> n;

    cout << "Enter pressure of each segment: ";
    for(int i=0;i<n;i++) cin >> arr[i];

    build(1,0,n-1);

    int threshold = 30; 

    int q;
    cout << "Enter number of checks: ";
    cin >> q;

    while(q--){
        int type;
        cout << "\n1.Update Pressure  2.Check Leakage\n";
        cin >> type;

        if(type == 1){
            int idx, val;
            cout << "Enter segment index & new pressure: ";
            cin >> idx >> val;
            update(1,0,n-1,idx,val);
        }
        else {
            int L, R;
            cout << "Check zone (L R): ";
            cin >> L >> R;
            int minPressure = query(1,0,n-1,L,R);
            if(minPressure < threshold)
                cout << "Leak Detected in Zone!" << endl;
            else
                cout << "✓ Zone Safe" << endl;
        }
    }
    return 0;
}
