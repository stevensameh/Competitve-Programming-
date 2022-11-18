mt19937 gen(chrono::system_clock::now().time_since_epoch().count());
ll rand(ll l, ll r){
    return uniform_int_distribution<ll>(l, r)(gen);
}