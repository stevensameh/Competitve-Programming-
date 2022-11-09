/*  Mo's algorithm for offline queires 
	this code computes number of distinct elements in range l to r. (0-based)
*/

inline int64_t gilbertOrder(int x, int y, int pow, int rotate) {
    if (pow == 0) {
        return 0;
    }
    int hpow = 1 << (pow - 1);
    int seg = (x < hpow) ? (
            (y < hpow) ? 0 : 3
    ) : (
                      (y < hpow) ? 1 : 2
              );
    seg = (seg + rotate) & 3;
    const int rotateDelta[4] = {3, 0, 0, 1};
    int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
    int nrot = (rotate + rotateDelta[seg]) & 3;
    int64_t subSquareSize = int64_t(1) << (2 * pow - 2);
    int64_t ans = seg * subSquareSize;
    int64_t add = gilbertOrder(nx, ny, pow - 1, nrot);
    ans += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
    return ans;
}

struct Query {
    int l, r, idx;
    int64_t ord;

    inline void calcOrder() {
        ord = gilbertOrder(l, r, 21, 0);
    }
};
inline bool operator<(const Query &a, const Query &b) {
    return a.ord < b.ord;
}
int freq[M], a[N], n, cnt;
void add(int l){
    freq[a[l]]++;
    if(freq[a[l]] == 1) cnt++;
}

void remove(int l){
    freq[a[l]]--;
    if(!freq[a[l]]) cnt--;
}

int get(){
    return cnt;
}
vector<int> MO(const vector<Query>& qu) {
    vector<int> answers(qu.size());
    int cur_l = 0;
    int cur_r = -1;
    for (Query q: qu) {
        while (cur_l > q.l) {
            cur_l--;
            add(cur_l);
        }
        while (cur_r < q.r) {
            cur_r++;
            add(cur_r);
        }
        while (cur_l < q.l) {
            remove(cur_l);
            cur_l++;
        }
        while (cur_r > q.r) {
            remove(cur_r);
            cur_r--;
        }
        answers[q.idx] = get();
    }
    return answers;
}
int q;
vector<Query>qu;
void go() {
    cin >> n;
    for(int i = 0; i < n; i++) cin >> a[i];
    cin >> q;
    qu.resize(q);
    for(int i = 0; i < q; i++){
        cin >> qu[i].l >> qu[i].r;
        qu[i].l--;
        qu[i].r--;
        qu[i].idx = i;
        qu[i].calcOrder();
    }
    sort(qu.begin(), qu.end());
    auto ret = MO(qu);
    for(auto i : ret) cout << i << el;
}