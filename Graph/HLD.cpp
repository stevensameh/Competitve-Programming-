// Problem : USACO 2019 February Contest, Gold Problem 1. Cow Land
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

const int N = 2e5 + 5, M = 1e6 + 5, mod = 1e9 + 7, INF  = 2e9;
template<int size, int lg, typename seg_t = long long>
struct hld {
    vector<int> edges[size];
    int bigchild[size], sz[size], depth[size], chain[size], label[size], par[size], lca_lift[size][lg];
    int N, label_time;

    seg_t seg_tree[size << 2], seg_lazy[size << 2];


    /* ----------- segment tree ----------- */

    /* CHANGE THIS SECTION BY PROBLEM */
    inline seg_t seg_combine(seg_t a, seg_t b) {
        return a ^ b;
    }

    inline seg_t seg_lazy_apply(seg_t lazy_val, seg_t new_val) {
        return new_val;
    }

    inline seg_t seg_lazy_func(seg_t cur_val, seg_t lazy_val, int l, int r) {
        return lazy_val;
    }

    const seg_t seg_sentinel = 0;
    const seg_t seg_lazy_sentinel = -1;
    const seg_t seg_init_val = 0;
    /* END SECTION */

    seg_t seg_query_header(int l, int r) {
        return seg_query_rec(0, 0, N - 1, l, r);
    }

    seg_t seg_query_rec(int i, int tl, int tr, int ql, int qr) {
        seg_eval_lazy(i, tl, tr);

        if (ql <= tl && tr <= qr) return seg_tree[i];
        if (tl > tr || tr < ql || qr < tl) return seg_sentinel;

        int mid = tl + tr >> 1;
        seg_t a = seg_query_rec(2 * i + 1, tl, mid, ql, qr);
        seg_t b = seg_query_rec(2 * i + 2, mid + 1, tr, ql, qr);
        return seg_combine(a, b);
    }

    void seg_update_header(int l, int r, seg_t v) {
        seg_update_rec(0, 0, N - 1, l, r, v);
    }

    seg_t seg_update_rec(int i, int tl, int tr, int ql, int qr, seg_t v) {
        seg_eval_lazy(i, tl, tr);

        if (tl > tr || tr < ql || qr < tl) return seg_tree[i];
        if (ql <= tl && tr <= qr) {
            seg_lazy[i] = seg_lazy_apply(seg_lazy[i], v);
            seg_eval_lazy(i, tl, tr);
            return seg_tree[i];
        }
        if (tl == tr) return seg_tree[i];

        int mid = tl + tr >> 1;
        seg_t a = seg_update_rec(2 * i + 1, tl, mid, ql, qr, v);
        seg_t b = seg_update_rec(2 * i + 2, mid + 1, tr, ql, qr, v);
        return seg_tree[i] = seg_combine(a, b);
    }

    void seg_eval_lazy(int i, int l, int r) {
        if (seg_lazy[i] == seg_lazy_sentinel) return;

        seg_tree[i] = seg_lazy_func(seg_tree[i], seg_lazy[i], l, r);

        if (l != r) {
            seg_lazy[i * 2 + 1] = seg_lazy_apply(seg_lazy[i * 2 + 1], seg_lazy[i]);
            seg_lazy[i * 2 + 2] = seg_lazy_apply(seg_lazy[i * 2 + 2], seg_lazy[i]);
        }

        seg_lazy[i] = seg_lazy_sentinel;
    }

    /* ----------- init phase 1 ----------- */
    /* initialize segtree, clear edges, etc. */

    void init_arrays(int n) {
        // everything not initialized doesn't need to be
        N = n;
        for (int i = 0; i < N; i++) {
            edges[i].clear();
            chain[i] = i;
        }

        for (int i = 0; i < 4 * N; i++) {
            seg_tree[i] = seg_init_val;
            seg_lazy[i] = seg_lazy_sentinel;
        }
    }

    /* ----------- init phase 2 ----------- */
    /* put edges in */

    void add_edge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    /* ----------- init phase 3 ----------- */
    /* build the lca and hld stuff */

    void init_tree(seg_t* arr, int root = 0) {
        // lca
        lca_dfs(root, -1);

        // size, compute biggest children
        dfs_size(root, -1, 0);

        // compute chains
        dfs_chains(root, -1);

        // label nodes
        label_time = 0;
        dfs_labels(arr, root, -1);
    }

    void lca_dfs(int v, int par) {
        lca_lift[v][0] = par;

        for (int i = 1; i < lg; i++) {
            if (lca_lift[v][i - 1] == -1) lca_lift[v][i] = -1;
            else lca_lift[v][i] = lca_lift[lca_lift[v][i - 1]][i - 1];
        }

        for (int x: edges[v])
            if (x != par)
                lca_dfs(x, v);
    }

    void dfs_size(int v, int p, int d) {
        sz[v] = 1;
        depth[v] = d;
        par[v] = p;
        int bigc = -1, bigv = -1;

        for (int x: edges[v]) {
            if (x != p) {
                dfs_size(x, v, d + 1);
                sz[v] += sz[x];
                if (sz[x] > bigv) {
                    bigc = x;
                    bigv = sz[x];
                }
            }
        }

        bigchild[v] = bigc;
    }

    void dfs_chains(int v, int p) {
        if (bigchild[v] != -1)
            chain[bigchild[v]] = chain[v];

        for (int x: edges[v])
            if (x != p)
                dfs_chains(x, v);
    }

    void dfs_labels(seg_t* arr, int v, int p) {
        label[v] = label_time++;
        seg_update_header(label[v], label[v], arr[v]);

        if (bigchild[v] != -1)
            dfs_labels(arr, bigchild[v], v);

        for (int x: edges[v])
            if (x != p && x != bigchild[v])
                dfs_labels(arr, x, v);
    }

    /* ----------- init phase 4 ----------- */
    /* usage */

    int lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        int d = depth[a] - depth[b];
        int v = get_kth_ancestor(a, d);
        if (v == b)
            return v;
        for (int i = lg - 1; i >= 0; i--) {
            if (lca_lift[v][i] != lca_lift[b][i]) {
                v = lca_lift[v][i];
                b = lca_lift[b][i];
            }
        }
        return lca_lift[b][0];
    }

    int get_kth_ancestor(int v, int k) {
        for (int i = lg - 1; i >= 0; i--) {
            if (v == -1) return v;
            if ((1 << i) <= k) {
                v = lca_lift[v][i];
                k -= (1 << i);
            }
        }
        return v;
    }

    /* excludes p */
    seg_t query_chain(int v, int p) {
        seg_t val = seg_sentinel;
        while (depth[p] < depth[v]) {
            int top = chain[v];
            if (depth[top] <= depth[p]) {
                int diff = depth[v] - depth[p];
                top = get_kth_ancestor(v, diff - 1);
            }
            val = seg_combine(val, seg_query_header(label[top], label[v]));
            v = par[top];
        }
        return val;
    }

    seg_t query(int u, int v) {
        int lc = lca(u, v);
        seg_t val = seg_combine(query_chain(u, lc), query_chain(v, lc));
        return seg_combine(val, seg_query_header(label[lc], label[lc]));
    }

    /* excludes p */
    void update_chain(int v, int p, seg_t val) {
        while (depth[p] < depth[v]) {
            int top = chain[v];
            if (depth[top] <= depth[p]) {
                int diff = depth[v] - depth[p];
                top = get_kth_ancestor(v, diff - 1);
            }
            seg_update_header(label[top], label[v], val);
            v = par[top];
        }
    }

    void update(int u, int v, seg_t val) {
        int lc = lca(u, v);
        update_chain(u, lc, val);
        update_chain(v, lc, val);
        seg_update_header(label[lc], label[lc], val);
    }
};

hld<N, 20, ll> h;
int n, q;
ll a[N];
void go() {
    cin >> n >> q;
    for(int i = 0; i < n; i++) cin >> a[i];
    h.init_arrays(n);
    for(int i = 1; i <= n - 1; i++){
        int u, v;
        cin >> u >> v;
        u--, v--;
        h.add_edge(u, v);
    }
    h.init_tree(a);
    while (q--){
        int type, x, y;
        cin >> type >> x >> y;
        if(type == 1){
            x--;
            a[x] = y;
            h.update(x, x, y);
        }else{
            x--, y--;
            cout << h.query(x, y) << el;
        }
    }
}
int32_t main() {
    Baba_Sevawy
    ::freopen("cowland.in", "r", stdin);
    ::freopen("cowland.out", "w", stdout);
    int tc = 1;
    //cin >> tc;
    while (tc--){
        go();
    }
}