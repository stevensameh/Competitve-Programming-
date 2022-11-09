#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
#define ll long long
#define ld long double
#define el '\n'
#define pi acos(-1)
#define Baba_Sevawy  ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define ordered_set tree<int , null_type,less<int>, rb_tree_tag,tree_order_statistics_node_update>
using namespace std;
const ll N = 6e3 + 5, M = 1e5 + 5, mod = 1e9 + 9, K = 21, inf = 2e18, p = 31;
const ld EPS = 1e-8;
int n, m;
const int MAX = 10005;
vector<int> adj[MAX];
vector<int> adjInv[MAX];
bool visited[MAX];
bool visitedInv[MAX];
stack<int> s;

int scc[MAX];
int counter = 1;
void addEdges(int a, int b) {
    adj[a].push_back(b);
}

void addEdgesInverse(int a, int b) {
    adjInv[b].push_back(a);
}

void dfsFirst(int u) {
    if (visited[u])
        return;
    visited[u] = 1;
    for (int i : adj[u])
        dfsFirst(i);
    s.push(u);
}

void dfsSecond(int u) {
    if (visitedInv[u])
        return;
    visitedInv[u] = 1;
    for (int i : adjInv[u])
        dfsSecond(i);
    scc[u] = counter;
}

bool is2Satisfiable() {
    for (int i = 1; i <= 2 * n; i++)
        if (!visited[i])
            dfsFirst(i);

    while (!s.empty()) {
        int cur = s.top();
        s.pop();

        if (!visitedInv[cur]) {
            dfsSecond(cur);
            counter++;
        }
    }

    for (int i = 1; i <= n; i++) {
        if (scc[i] == scc[i + n]) {
            return false;
        }
    }
    return true;
}
void go() {
    cin >> n >> m;
    int a[m + 2], b[m + 2];
    set<int>st;
    for(int i = 0; i < m; i++) {
        cin >> a[i] >> b[i];
        if (a[i] > 0 && b[i] > 0) {
            addEdges(a[i] + n, b[i]);
            addEdgesInverse(a[i] + n, b[i]);
            addEdges(b[i] + n, a[i]);
            addEdgesInverse(b[i] + n, a[i]);
        } else if (a[i] > 0 && b[i] < 0) {
            addEdges(a[i] + n, n - b[i]);
            addEdgesInverse(a[i] + n, n - b[i]);
            addEdges(-b[i], a[i]);
            addEdgesInverse(-b[i], a[i]);
        } else if (a[i] < 0 && b[i] > 0) {
            addEdges(-a[i], b[i]);
            addEdgesInverse(-a[i], b[i]);
            addEdges(b[i] + n, n - a[i]);
            addEdgesInverse(b[i] + n, n - a[i]);
        } else {
            addEdges(-a[i], n - b[i]);
            addEdgesInverse(-a[i], n - b[i]);
            addEdges(-b[i], n - a[i]);
            addEdgesInverse(-b[i], n - a[i]);
            st.insert(abs(a[i]));
            st.insert(abs(b[i]));
        }
    }
    if(st.empty()) {
        cout << "-1\n";
        return;
    }
    if(!is2Satisfiable()) {
        cout << "0\n";
        return;
    }
    for(auto it : st){
        memset(visited, 0, sizeof visited);
        memset(visitedInv, 0, sizeof visitedInv);
        addEdges(it + n, it);
        addEdgesInverse(it + n, it);
        addEdges(it + n, it);
        addEdgesInverse(it + n,it);
        if(!is2Satisfiable()){
            cout << "1\n";
            return;
        }
        adj[it + n].pop_back();
        adjInv[it].pop_back();
        adj[it + n].pop_back();
        adjInv[it].pop_back();
    }
    cout << "2\n";
}
int32_t main() {
    Baba_Sevawy
    int tc = 1;
    //cin >> tc;
    while (tc--)
        go();
    return 0;
}