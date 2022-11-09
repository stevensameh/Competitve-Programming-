#include<bits/stdc++.h>

#define ll long long
#define f first
#define s second
using namespace std;
const int N = 1e2+5; //size of nodes

const int inf = 1000000010;
struct Edge {
    int to, cost, cap, flow, backEdge;
};
struct MCMF {
    int s, t, n;
    vector<Edge> g[N];
    MCMF(int _s, int _t, int _n) {
        s = _s, t = _t, n = _n+1;
    }
    void addEdge(int u, int v, int cost, int cap) {
        Edge e1 = { v, cost, cap, 0, (int)g[v].size() };
        Edge e2 = { u, -cost, 0, 0, (int)g[u].size() };
        g[u].push_back(e1); g[v].push_back(e2);
    }
    pair<int, int> minCostMaxFlow() {
        int flow = 0, cost = 0;
        vector<int> state(n), from(n), from_edge(n), d(n);
        deque<int> q;
        while (true) {
            for (int i = 0; i < n; i++)
                state[i] = 2, d[i] = inf, from[i] = -1;
            state[s] = 1; q.clear(); q.push_back(s); d[s] = 0;
            while (!q.empty()) {
                int v = q.front(); q.pop_front(); state[v] = 0;
                for (int i = 0; i < (int) g[v].size(); i++) {
                    Edge e = g[v][i];
                    if (e.flow >= e.cap || d[e.to] <= d[v] + e.cost)
                        continue;
                    int to = e.to; d[to] = d[v] + e.cost;
                    from[to] = v; from_edge[to] = i;
                    if (state[to] == 1) continue;
                    if (!state[to] || (!q.empty() && d[q.front()] > d[to]))
                        q.push_front(to);
                    else q.push_back(to);
                    state[to] = 1;
                }
            }
            if (d[t] == inf) break;
            int it = t, addflow = inf;
            while (it != s) {
                addflow = min(addflow,
                              g[from[it]][from_edge[it]].cap
                              - g[from[it]][from_edge[it]].flow);
                it = from[it];
            }
            it = t;
            while (it != s) {
                g[from[it]][from_edge[it]].flow += addflow;
                g[it][g[from[it]][from_edge[it]].backEdge].flow -= addflow;
                cost += g[from[it]][from_edge[it]].cost * addflow;
                it = from[it];
            }
            flow += addflow;
        }
        return {cost,flow};
    }
};

int main()
{
    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

    MCMF G(0 , 3 , 3);
    G.addEdge(0 , 1 , 3 , 2); //cost , cap
    G.addEdge(1 , 3 , 2 , 1);
    G.addEdge(0 , 2 , 2 , 2);
    G.addEdge(2 , 3 , 4 , 7);

    pair<ll,ll> pr = G.minCostMaxFlow(); //cost , flow
    cout << pr.f << ' ' << pr.s << '\n';

    return 0;
}
