/*
	Nodes must be given 0-based.
	if there is an edge between i and j, push j into adj[i] and i into adj[j].
	Time Complexity: O(VELogV).
*/
#include <bits/stdc++.h>
using namespace std;
struct edge {
    int to;
    int label;
};


vector<pair<int, int>> gabow_edmonds(const vector<vector<int>>& gra) {
    int N = (int)gra.size();
    vector<vector<edge>> g(N + 1);
    vector<pair<int, int>> edges;
    {
        int cnt = N + 1;
        for (int i = 0; i < N; i++) {
            for (auto to: gra[i]) {
                if (i < to) {
                    g[to + 1].push_back({i + 1, cnt});
                    g[i + 1].push_back({to + 1, cnt++});
                    edges.emplace_back(i + 1, to + 1);
                }
            }
        }
    }
    vector<int> mate(N + 1, 0);
    vector<int> label(N + 1, -1);
    vector<int> first(N + 1, 0);
    queue<int> que;

    function<int(int)> eval_first = [&](int x) {
        if (label[first[x]] < 0) return first[x];
        first[x] = eval_first(first[x]);
        return first[x];
    };

    function<void(int, int)> rematch = [&](int v, int w) {
        int t = mate[v];
        mate[v] = w;
        if (mate[t] != v) return;
        if (label[v] <= N) {
            mate[t] = label[v];
            rematch(label[v], t);
        }
        else {
            int x = edges[label[v] - N - 1].first;
            int y = edges[label[v] - N - 1].second;
            rematch(x, y);
            rematch(y, x);
        }
    };

    function<void(int, int, int)> assignLabel = [&](int x, int y, int num) {
        int r = eval_first(x);
        int s = eval_first(y);
        int join = 0;
        if (r == s) return;
        label[r] = -num;
        label[s] = -num;
        while (true) {
            if (s != 0) ::std::swap(r, s);
            r = eval_first(label[mate[r]]);
            if (label[r] == -num) {
                join = r;
                break;
            }
            label[r] = -num;
        }
        int v = first[x];
        while (v != join) {
            que.push(v);
            label[v] = num;
            first[v] = join;
            v = first[label[mate[v]]];
        }
        v = first[y];
        while (v != join) {
            que.push(v);
            label[v] = num;
            first[v] = join;
            v = first[label[mate[v]]];
        }
        return;
    };

    function<bool(int)> augment_check = [&](int u) {
        first[u] = 0;
        label[u] = 0;
        que.push(u);
        while (!que.empty()) {
            int x = que.front();
            que.pop();
            for (auto e: g[x]) {
                int y = e.to;
                if (mate[y] == 0 && y != u) {
                    mate[y] = x;
                    rematch(x, y);
                    return true;
                }
                else if (label[y] >= 0) {
                    assignLabel(x, y, e.label);
                }
                else if (label[mate[y]] < 0) {
                    label[mate[y]] = x;
                    first[mate[y]] = y;
                    que.push(mate[y]);
                }
            }
        }
        return false;
    };

    for (int i = 1; i <= N; i++) {
        que = queue<int>();
        if (mate[i] != 0) continue;
        if (augment_check(i)) {
            fill(label.begin(), label.end(), -1);
        }
    }

    vector<pair<int, int>> ans;
    for (int i = 1; i <= N; i++) {
        if (i < mate[i]) {
            ans.emplace_back(i, mate[i]);
        }
    }
    return ans;
}
int main(){
    int u, v, n;
    cin >> n;
    vector<vector<int>>g(n);
    while (cin >> u >> v) {
        u--, v--;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    auto ans = gabow_edmonds(g);
    cout << 2 * ans.size() << "\n";
    for(auto i : ans)
        cout << i.first << " " << i.second << "\n";
}