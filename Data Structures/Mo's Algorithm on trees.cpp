// Problem: SPOJ COT2 - Count on a tree II
#include <bits/stdc++.h>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;


#define Baba_Sevawy  ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define ll unsigned long long
#define ull unsigned long long
#define ld long double
#define el '\n'
#define pi acos(-1)
#define F first
#define S second
#define sz(x) (int)(x).size()

template<typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template<typename T>
using ordered_multiset = tree<T, null_type, less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;

mt19937 gen(chrono::system_clock::now().time_since_epoch().count());
ll rand(ll l, ll r){
    return uniform_int_distribution<ll>(l, r)(gen);
}

const int N = 1e5 + 5, M = 1e6 + 5, mod = 1e9 + 7, INF  = 2e9, lg = 20;
vector <int> adj[N];
int l[N], r[N], cur, ID[N], sp[N][lg], lvl[N], n, q, a[N];
inline int64_t gilbertOrder(int x, int y, int pow, int rotate) {
    if (pow == 0) {
        return 0;
    }
    int hpow = 1 << (pow - 1);
    int seg = (x < hpow) ? ((y < hpow) ? 0 : 3) : ((y < hpow) ? 1 : 2);
    seg = (seg + rotate) & 3;
    const int rotateDelta[4] = {3, 0, 0, 1};
    int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
    int nrot = (rotate + rotateDelta[seg]) & 3;
    int64_t subSquareSize = int64_t(1) << (2 * pow - 2);
    int64_t ans = seg * subSquareSize;
    int64_t add = gilbertOrder(nx, ny, pow - 1, nrot);
    ans += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
    return ans;
}
struct Query {
    int l, r, idx, lc;
    int64_t ord;

    inline void calcOrder() {
        ord = gilbertOrder(l, r, 21, 0);
    }
};
inline bool operator<(const Query &a, const Query &b) {
    return a.ord < b.ord;
}
void dfs(int node, int par){
    l[node] = ++cur;
    lvl[node] = lvl[par] + 1;
    sp[node][0] = par;
    ID[cur] = node;
    for (int i = 1; i < lg; i++){
        int tmp = sp[node][i - 1];
        sp[node][i] = sp[tmp][i - 1];
    }
    for (auto i : adj[node]) {
        if (i == par) continue;
        dfs(i, node);
    }
    r[node] = ++cur;
    ID[cur] = node;
}
int get_kth(int node , int k) {
    int tmpNode = node;
    for (int i = 0; i < lg; i++)
        if (k >> i & 1)
            tmpNode = sp[tmpNode][i];
    return tmpNode;
}
int lca(int u, int v){
    if (lvl[u] > lvl[v]) swap(u, v);
    v = get_kth(v, lvl[v] - lvl[u]);

    if (u == v) return u;
    for (int i = lg - 1; i >= 0; i--){
        if (sp[u][i] != sp[v][i]){
            u = sp[u][i];
            v = sp[v][i];
        }
    }
    return sp[u][0];
}

int freq[N], cnt;
bool vis[N];
void calc(int L){
    if(vis[L]) {if(freq[a[L]] - 1 == 0) cnt--; freq[a[L]]--;}
    else if(!vis[L]) {if(!freq[a[L]]) cnt++; freq[a[L]]++;}
    vis[L] = !vis[L];
}

int get(){
    return cnt;
}
vector<int> MO(const vector<Query>& qu) {
    vector<int> answers(qu.size());
    int cur_l = 0;
    int cur_r = -1;
    for (Query qq: qu) {
        while (cur_l > qq.l) {
            cur_l--;
            calc(ID[cur_l]);
        }
        while (cur_r < qq.r) {
            cur_r++;
            calc(ID[cur_r]);
        }
        while (cur_l < qq.l) {
            calc(ID[cur_l]);
            cur_l++;
        }
        while (cur_r > qq.r) {
            calc(ID[cur_r]);
            cur_r--;
        }
        if(qq.lc != ID[qq.l] && qq.lc != ID[qq.r]) calc(qq.lc);
        answers[qq.idx] = get();
        if(qq.lc != ID[qq.l] && qq.lc != ID[qq.r]) calc(qq.lc);
    }
    return answers;
}

unordered_map<int, int>id;
int c;
int get(int x){
    if(id.count(x)) return id[x];
    return id[x] = ++c;
}
void go() {
    while (cin >> n >> q) {
        for (int i = 1; i <= n; i++) cin >> a[i], a[i] = get(a[i]);
        for (int i = 1; i <= n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].emplace_back(v);
            adj[v].emplace_back(u);
        }
        lvl[0] = -1;
        dfs(1, 0);
        vector<Query> Q(q);
        for (int i = 0; i < q; i++) {
            int u, v;
            cin >> u >> v;
            Q[i].lc = lca(u, v);
            if (l[u] > l[v]) swap(u, v);
            Q[i].l = (Q[i].lc == u ? l[u] : r[u]), Q[i].r = l[v], Q[i].idx = i;
            Q[i].calcOrder();
        }
        sort(Q.begin(), Q.end());
        auto ret = MO(Q);
        for (auto i: ret) cout << i << endl;

        for (int i = 0; i <= n; i++) adj[i].clear(), freq[i] = 0, vis[i] = false;
        id.clear();
        c = cnt = 0;
    }
}
int32_t main() {
    Baba_Sevawy
    int tc = 1;
    //cin >> tc;
    while (tc--)
        go();
}