int lg[N], st[N][K + 1], a[N];
void pre(){
    lg[1] = 0;
    for (int i = 2; i <= N; i++)
        lg[i] = lg[i / 2] + 1;
}
void build(int n){
    for (int i = 0; i < n; i++)
        st[i][0] = a[i];

    for (int j = 1; j <= K; j++)
        for (int i = 0; i + (1 << j) <= n; i++)
            st[i][j] = __gcd(st[i][j-1], st[i + (1 << (j - 1))][j - 1]);
}
int query(int L, int R){
    int j = lg[R - L + 1];
    return __gcd(st[L][j], st[R - (1 << j) + 1][j]);
}