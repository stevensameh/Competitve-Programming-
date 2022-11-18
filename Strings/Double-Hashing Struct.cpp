const ll N = 1e5 + 5, M = 1e6 + 5, MOD1 = 1e9 + 7, MOD2 = 1e9 + 9, lg = 21, INF  = 1e15, P1 = 29, P2 = 31;
int pw1[N], inv1[N], pw2[N], inv2[N];

int add(int freq, int b, int mod) {
    return (freq + b) % mod;
}

int sub(int freq, int b, int mod) {
    return ((freq - b) % mod + mod) % mod;
}

int mul(int freq, int b, int mod) {
    return 1ll * freq * b % mod;
}

int power(int b, int p, int mod) {
    int x = 1;
    for (; p; p /= 2) {
        if (p & 1)
            x = mul(x, b, mod);
        b = mul(b, b, mod);
    }
    return x;
}

struct Hash {
    vector<pair<int, int>> prefix;
    Hash(const string &s) {
        prefix.resize(s.size() + 1);
        for (int i = 0; i < int(s.size()); ++i)
            prefix[i + 1] = make_pair(add(prefix[i].first, mul(pw1[i], s[i] - 'a' + 1, MOD1), MOD1),
                                      add(prefix[i].second, mul(pw2[i], s[i] - 'a' + 1, MOD2), MOD2));
    }
    int size() const {
        return prefix.size() - 1;
    }
    pair<int, int> getHash() const {
        return prefix.back();
    }
    pair<int, int> getRange(int l, int r) const {
        return make_pair(mul(inv1[l], sub(prefix[r + 1].first, prefix[l].first, MOD1), MOD1),
                         mul(inv2[l], sub(prefix[r + 1].second, prefix[l].second, MOD2), MOD2));
    }
    static void prepare() {
        pw1[0] = inv1[0] = pw2[0] = inv2[0] = 1;
        int iv1 = power(P1, MOD1 - 2, MOD1);
        int iv2 = power(P2, MOD2 - 2, MOD2);
        for (int i = 1; i < N; ++i) {
            pw1[i] = mul(pw1[i - 1], P1, MOD1);
            inv1[i] = mul(inv1[i - 1], iv1, MOD1);
            pw2[i] = mul(pw2[i - 1], P2, MOD2);
            inv2[i] = mul(inv2[i - 1], iv2, MOD2);
        }
    }
};