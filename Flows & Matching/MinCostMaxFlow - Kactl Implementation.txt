#include <bits/stdc++.h>
#include <bits/extc++.h>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;

#define Baba_Sevawy  ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define ll long long
#define ull unsigned long long
#define ld long double
#define el '\n'

template<typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template<typename T>
using ordered_multiset = tree<T, null_type, less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;

mt19937_64 gen(chrono::system_clock::now().time_since_epoch().count());

/*
    Min-cost max-flow. cap[i][j] != cap[j][i] is allowed; double edges are not.
    If costs can be negative, call setpi before maxflow, but note that negative cost cycles are not supported.
    To obtain the actual flow, look at positive values only.
	Time: Approximately O(E^2)
*/
const ll N = 5e2 + 5, M = 2e5 + 5, mod = 998244353, lg = 21, inf = 2e9;
const ll INF = numeric_limits<ll>::max() / 4;

struct MCMF {
    int N;
    vector<vector<int>> ed, red;
    vector<vector<ll>> cap, flow, cost;
    vector<int> seen;
    vector<ll> dist, pi;
    vector<pair<int, int>> par;

    MCMF(int N) :
            N(N), ed(N), red(N), cap(N, vector<ll>(N)), flow(cap), cost(cap),
            seen(N), dist(N), pi(N), par(N) {}

    void addEdge(int from, int to, ll _cap, ll _cost) {
        this->cap[from][to] = _cap;
        this->cost[from][to] = _cost;
        ed[from].push_back(to);
        red[to].push_back(from);
    }

    void path(int s) {
        fill(seen.begin(), seen.end(), 0);
        fill(dist.begin(), dist.end(), INF);
        dist[s] = 0; ll di;

        __gnu_pbds::priority_queue<pair<ll, int>> q;
        vector<decltype(q)::point_iterator> its(N);
        q.push({0, s});

        auto relax = [&](int i, ll _cap, ll _cost, int dir) {
            ll val = di - pi[i] + _cost;
            if (_cap && val < dist[i]) {
                dist[i] = val;
                par[i] = {s, dir};
                if (its[i] == q.end()) its[i] = q.push({-dist[i], i});
                else q.modify(its[i], {-dist[i], i});
            }
        };

        while (!q.empty()) {
            s = q.top().second; q.pop();
            seen[s] = 1; di = dist[s] + pi[s];
            for (int i : ed[s]) if (!seen[i])
                    relax(i, cap[s][i] - flow[s][i], cost[s][i], 1);
            for (int i : red[s]) if (!seen[i])
                    relax(i, flow[i][s], -cost[i][s], 0);
        }
        for(int i = 0; i < N; i++) pi[i] = min(pi[i] + dist[i], INF);
    }

    pair<ll, ll> maxflow(int s, int t) {
        ll totflow = 0, totcost = 0;
        while (path(s), seen[t]) {
            ll fl = INF;
            for (int p,r,x = t; tie(p,r) = par[x], x != s; x = p)
                fl = min(fl, r ? cap[p][x] - flow[p][x] : flow[x][p]);
            totflow += fl;
            for (int p,r,x = t; tie(p,r) = par[x], x != s; x = p)
                if (r) flow[p][x] += fl;
                else flow[x][p] -= fl;
        }
        for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) totcost += cost[i][j] * flow[i][j];
        return {totflow, totcost};
    }

    // If some costs can be negative, call this before maxflow:
    void setpi(int s) { // (otherwise, leave this out)
        fill(pi.begin(), pi.end(), INF); pi[s] = 0;
        int it = N, ch = 1; ll v;
        while (ch-- && it--)
            for(int i = 0; i < N; i++) if (pi[i] != INF)
            for (int to : ed[i]) if (cap[i][to])
                    if ((v = pi[i] + cost[i][to]) < pi[to])
                        pi[to] = v, ch = 1;
        assert(it >= 0); // negative cost cycle
    }
};
int c1, c2;
void go() {
    cin >> c1 >> c2;

    MCMF g(c1 + c2 + 2);
    for(int i = 1; i <= c1; i++)
        g.addEdge(0, i, 1, 0);

    while (true){
        int u, v, w;
        cin >> u >> v >> w;
        if(!u && !v && !w) break;
        g.addEdge(u, v + c1, 1, -w);
    }

    for(int i = 1; i <= c2; i++)
        g.addEdge(i + c1, c1 + c2 + 1, 1, 0);

    g.setpi(0);
    auto ret = g.maxflow(0, c1 + c2 + 1);
    cout << -ret.second << el;
}

int32_t main() {
    Baba_Sevawy
    int tc = 1;
    cin >> tc;
    while (tc--)
        go();
}