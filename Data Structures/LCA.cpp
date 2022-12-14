const int N = 1e5 + 5, lg = 21;
int level[N], sp[N][lg], spmn[N][lg];
vector<pair<int, int>> adj[N];

void dfs(int node, int par, int cost) {
    level[node] = level[par] + 1;
    sp[node][0] = par;
    spmn[node][0] = cost;
    for (int i = 1; i < lg; i++) {
        int tmp = sp[node][i - 1];
        sp[node][i] = sp[tmp][i - 1];
        spmn[node][i] = min(spmn[node][i - 1], spmn[tmp][i - 1]);
    }
    for (auto i: adj[node]) {
        if (i.first == par) continue;
        dfs(i.first, node, i.second);
    }
}
pair<int,int> get_kth(int node , int k) {
    int tmpNode = node, mn = 2e9;
    for (int i = 0; i < lg; i++) {
        if (k >> i & 1) {
            mn = min(mn, spmn[tmpNode][i]);
            tmpNode = sp[tmpNode][i];
        }
    }
    return {tmpNode, mn};
}

pair<int, int> lca(int  u, int v) {
    if (level[u] > level[v]) swap(u, v);

    pair<int, int> pr = get_kth(v, level[v] - level[u]);

    if (pr.first == u) return {u, pr.second};
    v = pr.first;
    int mn = pr.second;

    for (int i = lg - 1; i >= 0; i--) {
        if (sp[u][i] != sp[v][i]) {
            mn = min({mn, spmn[u][i], spmn[v][i]});
            u = sp[u][i], v = sp[v][i];
        }
    }

    mn = min({mn, spmn[u][0], spmn[v][0]});
    return {sp[u][0], mn};
}