#include <bits/stdc++.h>
using namespace std;

int parent[100], rnk[100];

int findp(int x){
    if(parent[x]==x) return x;
    return parent[x]=findp(parent[x]);
}

bool unite(int a,int b){
    a=findp(a);
    b=findp(b);
    if(a==b) return false;
    if(rnk[a]<rnk[b]) swap(a,b);
    parent[b]=a;
    if(rnk[a]==rnk[b]) rnk[a]++;
    return true;
}

int main(){
    int n,m;
    cin>>n>>m;
    for(int i
