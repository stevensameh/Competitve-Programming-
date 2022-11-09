/*
	Hungarian algorithm:
	Given a weighted bipartite graph, matches every node on 
	the left with a node on the right such that no
    nodes are in two matchings and the sum of the edge weights is minimal. Takes
    cost[N][M], where cost[i][j] = cost for L[i] to be matched with R[j] and
    returns (min cost, match), where L[i] is matched with
    R[match[i]].
    
    Notes: 1- Left must be less than or equal to right nodes count.
    	   2- Negate costs for max cost.
    	   3- ans vector carries the matching we have done for every node one the left
    	   4- matrix a carrires the cost between every two nodes in both sides.
    	   5- output of ans is zero based.
    Time: O(N * N * M)
*/

const ll N = 5e2 + 5, M = 2e5 + 5, mod = 998244353, lg = 21, inf = 4e18;
int c1, c2, cost[N][N];
pair<int, vector<int>> hungarian(vector<vector<int>> &a) {
    if (a.empty()) return {0, {}};
    int n = (int)a.size() + 1, m = (int)a[0].size() + 1;
    vector<int> u(n), v(m), p(m), ans(n - 1);
    for (int i = 1; i < n; i++) {
        p[0] = i;
        int j0 = 0; // add "dummy" worker 0
        vector<int> dist(m, INT_MAX), pre(m, -1);
        vector<bool> done(m + 1);
        do { // dijkstra
            done[j0] = true;
            int i0 = p[j0], j1, delta = INT_MAX;
            for (int j = 1; j < m; j++)
                if (!done[j]) {
                    auto cur = a[i0 - 1][j - 1] - u[i0] - v[j];
                    if (cur < dist[j]) dist[j] = cur, pre[j] = j0;
                    if (dist[j] < delta) delta = dist[j], j1 = j;
                }
            for (int j = 0; j < m; j++) {
                if (done[j]) u[p[j]] += delta, v[j] -= delta;
                else dist[j] -= delta;
            }
            j0 = j1;
        } while (p[j0]);
        while (j0) { // update alternating path
            int j1 = pre[j0];
            p[j0] = p[j1], j0 = j1;
        }
    }
    for (int j = 1; j < m; j++) if (p[j]) ans[p[j] - 1] = j - 1;
    return {-v[0], ans}; // min cost
}
void go() {
    cin >> c1 >> c2;
    for(auto & i : cost)
        for(int & j : i)
            j = 0;

    while (true){
        int u, v, w;
        cin >> u >> v >> w;
        if(!u && !v && !w) break;
        cost[u][v] = w;
    }
    vector<vector<int>>a;
    for(int i = 1; i <= max(c1, c2); i++) {
        vector<int> v;
        for (int j = 1; j <= max(c1, c2); j++)
            v.push_back(cost[i][j]);
        a.emplace_back(v);
    }
    auto ret = hungarian(a);
    cout << -ret.first << el;
}