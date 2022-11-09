int lazy[4 * N], tr[4 * N], a[N];
void build(int node, int start, int end) {
    if (start == end) {
        tr[node] = a[start];
    } else {
        int mid = (start + end) / 2;
        build(2 * node, start, mid);
        build(2 * node + 1, mid + 1, end);
        tr[node] = tr[2 * node] + tr[2 * node + 1];
    }
}
void updateRange(int node, int start, int end, int l, int r, int val) {
    if (lazy[node] != -1) {
        tr[node] += (end - start + 1) * lazy[node];
        if (start != end) {
            lazy[node * 2] += lazy[node];
            lazy[node * 2 + 1] += lazy[node];
        }
        lazy[node] = -1;
    }
    if (start > end or start > r or end < l)
        return;
    if (start >= l and end <= r) {
        lazy[node] = val;
        tr[node] += (end - start + 1) * lazy[node];
        if (start != end) {
            lazy[node * 2] += lazy[node];
            lazy[node * 2 + 1] += lazy[node];
        }
        lazy[node] = -1;
        return;
    }
    int mid = (start + end) / 2;
    updateRange(node * 2, start, mid, l, r, val);
    updateRange(node * 2 + 1, mid + 1, end, l, r, val);
    tr[node] = tr[node * 2] + tr[node * 2 + 1];
}

int queryRange(int node, int start, int end, int l, int r) {
    if (start > end or start > r or end < l)
        return 0;
    if (lazy[node] != -1) {
        tr[node] += (end - start + 1) * lazy[node];
        if (start != end) {
            lazy[node * 2] += lazy[node];
            lazy[node * 2 + 1] += lazy[node];
        }
        lazy[node] = -1;
    }
    if (start >= l and end <= r)
        return tr[node];
    int mid = (start + end) / 2;
    int p1 = queryRange(node * 2, start, mid, l, r);
    int p2 = queryRange(node * 2 + 1, mid + 1, end, l, r);
    return (p1 + p2);
}