struct AhoCorasick {
    enum {
        alpha = 26, first = 'a'
    };

    struct Node {
        int back, next[alpha], start = -1, end = -1, nmatches = 0;

        Node(int v) { memset(next, v, sizeof(next)); }
    };

    vector<Node> N;
    vector<int> backp;

    void insert(string &s, int j) {
        assert(!s.empty());
        int n = 0;
        for (char c: s) {
            int &m = N[n].next[c - first];
            if (m == -1) {
                n = m = sz(N);
                N.emplace_back(-1);
            } else n = m;
        }
        if (N[n].end == -1) N[n].start = j;
        backp.push_back(N[n].end);
        N[n].end = j;
        N[n].nmatches++;
    }

    AhoCorasick(vector<string> &pat) : N(1, -1) {
        for (int i = 0; i < sz(pat); i++) insert(pat[i], i);
        N[0].back = sz(N);
        N.emplace_back(0);

        queue<int> q;
        for (q.push(0); !q.empty(); q.pop()) {
            int n = q.front(), prev = N[n].back;
            for (int i = 0; i < alpha; i++) {
                int &ed = N[n].next[i], y = N[prev].next[i];
                if (ed == -1) ed = y;
                else {
                    N[ed].back = y;
                    (N[ed].end == -1 ? N[ed].end : backp[N[ed].start])
                            = N[y].end;
                    N[ed].nmatches += N[y].nmatches;
                    q.push(ed);
                }
            }
        }
    }

    vector<int> find(const string& word) {
        int n = 0;
        vector<int> res; ll count = 0;
        for (char c: word) {
            n = N[n].next[c - first];
            res.push_back(N[n].end);
            count += N[n].nmatches;
        }
        cout << count << el;
        return res;
    }

    vector<vector<int>> findAll(vector<string> &pat, const string& word) {
        vector<int> r = find(word);
        vector<vector<int>> res(sz(word));
        for (int i = 0; i < sz(word); i++) {
            int ind = r[i];
            while (ind != -1) {
                res[i - sz(pat[ind]) + 1].push_back(ind);
                ind = backp[ind];
            }
        }
        return res;
    }
};
void go() {

    string s = "ahishers";
    vector<string> v = {"he", "she", "hers", "his"};

    AhoCorasick ac(v);
    auto res = ac.find(s);
    cout << el;

    for(auto i : res) cout << i << el;

    auto res2 = ac.findAll(v, s);
    cout << el;
    for(int i = 0; i < sz(res2); i++){
        cout << i << ": ";
        for(auto j : res2[i]) cout << j << " ";
        cout << el;
    }
}