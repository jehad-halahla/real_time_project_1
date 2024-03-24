#include <bits/stdc++.h>
using namespace std;
#define fastio ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define endl '\n'

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<pii> vpii;
typedef vector<vi> vvi;

const int MOD = 1e9 + 7;
const int INF = 1e9;
const ll LLINF = 1e18;

// Debugging macro
#define debug(x) cerr << #x << " = " << (x) << endl;

// Modular arithmetic
int add(int a, int b) { return (a + b) % MOD; }
int subtract(int a, int b) { return (a - b + MOD) % MOD; }
int multiply(int a, int b) { return (ll(a) * b) % MOD; }

// Binary exponentiation
ll power(ll base, ll exp) {
    ll result = 1;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return result;
}

void solve() {
    // Your code logic goes here
}

int main() {
	
    fastio; // Remove for interactive problems

	solve(); // Remove for interactive problems
        
    return 0;
}
