/**
 * Description: Fast bipartite matching algorithm. Graph $g$ should be a list
 * of neighbors of the left partition, and $btoa$ should be a vector full of
 * -1's of the same size as the right partition. Returns the size of
 * the matching. $btoa[i]$ will be the match for vertex $i$ on the right side,
 * or $-1$ if it's not matched.
 * Usage: vi btoa(m, -1); hopcroftKarp(g, btoa);
 * Time: O(\sqrt{V}E)
 */

bool dfs(int node, int L, vector<int> &mtch, vector<int> &A, vector<int> &B) {
    if (A[node] != L) return false;
    A[node] = -1;
    for (int ch: adj[node])
        if (B[ch] == L + 1) {
            B[ch] = 0;
            if (mtch[ch] == -1 || dfs(mtch[ch], L + 1, mtch, A, B))
                return mtch[ch] = node, 1;
        }
    return false;
}

int hopcroftKarp(int sz, vector<int> &mtch) {
    int res = 0;
    vector<int> A(sz + 1), B(sz + 1), cur, next;
    while (true) {
        fill(A.begin(), A.end(), 0);
        fill(B.begin(), B.end(), 0);
        /// Find the starting nodes for BFS (i.e. layer 0).
        cur.clear();
        for (int i: mtch) if (i != -1) A[i] = -1;
        for (int i = 1; i <= sz; i++) if (!A[i]) cur.push_back(i);
        /// Find all layers using bfs.
        for (int lay = 1;; lay++) {
            bool islast = false;
            next.clear();
            for (int ch: cur)
                for (int j: adj[ch]) {
                    if (mtch[j] == -1) {
                        B[j] = lay;
                        islast = true;
                    } else if (mtch[j] != ch && !B[j]) {
                        B[j] = lay;
                        next.push_back(mtch[j]);
                    }
                }
            if (islast) break;
            if (next.empty()) return res;
            for (int i : next) A[i] = lay;
            cur.swap(next);
        }
        /// Use DFS to scan for augmenting paths.
        for (int i = 1; i <= sz; i++) res += dfs(i, 0, mtch, A, B);
    }
}