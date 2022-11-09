//Works for negative costs, but does not work for negative cycles
//Complexity: O(min(E^2 *V log V, E logV * flow))
struct edge {
    int to;
    ll flow, cap, cost;
    int rev;
};

struct MCMF {
    int nodes;
    vector<ll> prio, curflow, pot;
    vector<int> prevedge, prevnode, Q;
    vector<bool> inqueue;
    vector<vector<edge>> graph;

    MCMF() {}

    MCMF(int n) : nodes(n), prio(n, 0), curflow(n, 0),
                  prevedge(n, 0), prevnode(n, 0), Q(n, 0), pot(n, 0), inqueue(n, 0), graph(n) {}

    void addEdge(int source, int to, ll capacity, ll cost) {
        edge a = {to, 0, capacity, cost, (int) graph[to].size()};
        edge b = {source, 0, 0, -cost, (int) graph[source].size()};
        graph[source].push_back(a);
        graph[to].push_back(b);
    }

    void bellman_ford(int source, vector<ll> &dist) {
        fill(dist.begin(), dist.end(), INF);
        dist[source] = 0;
        int qt = 0;
        Q[qt++] = source;
        for (int qh = 0; (qh - qt) % nodes != 0; qh++) {
            int u = Q[qh % nodes];
            inqueue[u] = false;
            for (auto &e: graph[u]) {
                if (e.flow >= e.cap)
                    continue;
                int v = e.to;
                ll newDist = dist[u] + e.cost;
                if (dist[v] > newDist) {
                    dist[v] = newDist;
                    if (!inqueue[v]) {
                        inqueue[v] = true;
                        Q[qt++ % nodes] = v;
                    }
                }
            }
        }
    }

    pair<ll, ll> minCostFlow(int source, int dest, ll maxflow = INF) {
        bellman_ford(source, pot);
        ll flow = 0;
        ll flow_cost = 0;
        while (flow < maxflow) {
            priority_queue<pair<ll, int>, vector<pair<ll, int> >, greater<>> q;
            q.push({0ll, source});
            fill(prio.begin(), prio.end(), INF);
            prio[source] = 0;
            curflow[source] = INF;
            while (!q.empty()) {
                ll d = q.top().first;
                int u = q.top().second;
                q.pop();
                if (d != prio[u])
                    continue;
                for (int i = 0; i < graph[u].size(); i++) {
                    edge &e = graph[u][i];
                    int v = e.to;
                    if (e.flow >= e.cap)
                        continue;
                    ll newPrio = prio[u] + e.cost + pot[u] - pot[v];
                    if (prio[v] > newPrio) {
                        prio[v] = newPrio;
                        q.push({newPrio, v});
                        prevnode[v] = u;
                        prevedge[v] = i;
                        curflow[v] = min(curflow[u], e.cap - e.flow);
                    }
                }
            }
            if (prio[dest] == INF)
                break;
            for (int i = 0; i < nodes; i++)
                pot[i] += prio[i];
            ll df = min(curflow[dest], maxflow - flow);
            flow += df;
            for (int v = dest; v != source; v = prevnode[v]) {
                edge &e = graph[prevnode[v]][prevedge[v]];
                e.flow += df;
                graph[v][e.rev].flow -= df;
                flow_cost += df * e.cost;
            }
        }
        return {flow, flow_cost};
    }
};