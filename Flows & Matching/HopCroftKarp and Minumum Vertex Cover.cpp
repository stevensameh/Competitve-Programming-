/*
     Hopcroft-Karp algorithm
     Time : O(E * sqrt(V))
     Note : the verticies are 1-based. if the graph is 0-based change int bfs() and maximumMatching().
 */
 /*
	 Minimum Vertex Cover
	 Finds a minimum vertex cover in a bipartite graph.
     The size is the same as the size of a maximum matching, and
     the complement is a maximum independent set.
     Time : O(E + V)
 */
 
struct HopCroft{
    int L;
    vector<vector<int>> adj;
    vector<int>mt, lvl;
    HopCroft(int l, int r) : L(l), adj(l + 1), mt(l + r + 1, -1), lvl(l + 1) {}
    bool bfs(){
        queue<int>q;
        for(int i = 1; i <= L; i++){
            lvl[i] = -1;
            if(mt[i] < 0)
                q.push(i), lvl[i] = 0;
        }
        while (!q.empty()){
            int node = q.front();
            q.pop();
            for(auto i : adj[node]){
                int v = mt[i];
                if(v == -1) return true;
                if(lvl[v] == -1)
                    lvl[v] = lvl[node] + 1, q.push(v);
            }
        }
        return false;
    }
    bool dfs(int node){
        for(auto i : adj[node]){
            int v = mt[i];
            if(v == -1 || (lvl[v] > lvl[node] && dfs(v))){
                mt[i] = node;
                mt[node] = i;
                return true;
            }
        }
        return false;
    }
    int maximumMatching(){
        int mat = 0;
        while (bfs()){
            for(int i = 1; i <= L; i++)
                mat += (mt[i] == -1 && dfs(i));
        }
        return mat;
    }
};
int n;
vector<pair<int, int>>v;
vector<pair<int, int>>L, R;
vector<int> MinimumVertexCover(){
    HopCroft h(L.size(), R.size());
    for(int i = 0; i < L.size(); i++)
        for(int j = 0; j < R.size(); j++)
            if(abs(L[i].first - R[j].first) + abs(L[i].second - R[j].second) == 1)
                h.adj[i + 1].push_back(j + 1 + (int)L.size());

    int res = h.maximumMatching();
    auto mt = h.mt;
    vector<bool>found(L.size() + 1, true), seen(R.size() + 1, false);
    vector<int>cover;
    queue<int>q;
    
    for(int i = 1; i <= R.size(); i++) if(mt[i + L.size()] != -1) found[mt[i + L.size()]] = false;
    for(int i = 1; i <= L.size(); i++) if(found[i]) q.push(i);

    while (!q.empty()){
        int cur = q.front();
        q.pop();
        found[cur] = true;
        for(auto i : h.adj[cur]){
            if(!seen[i - L.size()] && mt[i] != -1){
                seen[i - L.size()] = true;
                q.push(mt[i]);
            }
        }
    }
    for(int i = 1; i <= L.size(); i++) if(!found[i]) cover.push_back(i);
    for(int i = 1; i <= R.size(); i++) if(seen[i]) cover.push_back(i + (int)L.size());
    
    if(cover.size() != res){
        cout << "OOPS!";
        exit(0);
    }
    return cover;
}