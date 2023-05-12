struct SuffixArray
{
    string S;
    vector<int> logs, sa, lcp;
    vector<vector<int>>table;
    SuffixArray(string& s, int lim = 256)
    {
        S = s;
        int n = s.size() + 1, k = 0, a, b;
        vector<int> x(s.begin(), s.end() + 1), y(n), ws(max(n, lim)), rank(n);
        sa = lcp = y, iota(sa.begin(), sa.end(), 0);
        for (int j = 0, p = 0; p < n; j = max(1, j * 2), lim = p)
        {
            p = j, iota(y.begin(), y.end(), n - j);
            for(int i = 0; i < n; i++)
            {
                if (sa[i] >= j)
                    y[p++] = sa[i] - j;
            }

            fill(ws.begin(), ws.end(), 0);

            for(int i = 0; i < n; i++) ws[x[i]]++;
            for(int i = 1; i < lim; i++) ws[i] += ws[i - 1];
            for (int i = n; i--;) sa[--ws[x[y[i]]]] = y[i];

            swap(x, y), p = 1, x[sa[0]] = 0;
            for(int i = 1; i < n; i++)
                a = sa[i - 1], b = sa[i], x[b] = (y[a] == y[b] && y[a + j] == y[b + j]) ? p - 1 : p++;
        }

        for(int i = 1; i < n; i++) rank[sa[i]] = i;
        for (int i = 0, j; i < n - 1; lcp[rank[i++]] = k)
            for (k && k--, j = sa[rank[i] - 1];
                    s[i + k] == s[j + k]; k++);
    }

    void prepLcp()
    {
        int n = S.size() + 1;
        logs = vector<int>(n + 5, 0);
        for(int i = 2; i < n + 5; i++)
            logs[i] = logs[i / 2] + 1;

        table = vector<vector<int>>(n, vector<int>(20));

        for(int i = 0; i < n; i++)
            table[i][0] = lcp[i];

        for(int j = 1; j <= logs[n]; j++)
            for(int i = 0; i <= n - (1 << j); i++)
                table[i][j] = min(table[i][j - 1], table[i + (1 << (j - 1))][j - 1]);
    }

    int queryLcp(int i, int j)
    {
        if(i > j)
            swap(i, j);

        i++;
        int len = logs[j - i + 1];
        return min(table[i][len], table[j - (1 << len) + 1][len]);
    }
};