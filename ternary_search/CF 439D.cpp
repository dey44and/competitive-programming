#include <iostream>

using namespace std;

int64_t a[100005], b[100005], N, M;

int64_t steps(int64_t k)
{
    int64_t ans = 0;
    for(int i = 1; i <= N; i++)
        if(k > a[i])
            ans += (k - a[i]);
    for(int i = 1; i <= M; i++)
        if(k < b[i])
            ans += (b[i] - k);
    return ans;
}

void solve()
{
    int64_t L = 0, R = 1e9, ans = 1e18;
    for(int i = 1; i <= 100; i++)
    {
        //cout << L << " " << R << "\n";
        int64_t m1 = L + (R - L) / 3;
        int64_t m2 = R - (R - L) / 3;
        int64_t x1 = steps(m1);
        int64_t x2 = steps(m2);
        ans = min(ans, x1);
        ans = min(ans, x2);
        if(x1 >= x2) {
            L = m1;
        } else {
            R = m2;
        }
    }
    cout << ans;
}

int main()
{
    cin >> N >> M;
    for(int i = 1; i <= N; i++)
        cin >> a[i];
    for(int i = 1; i <= M; i++)
        cin >> b[i];
    solve();
}