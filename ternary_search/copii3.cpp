#include <iostream> 

using namespace std;

// ifstream cin("copii3.in");
// ofstream cout("copii3.out");

int64_t N, Q;
int64_t bits[320005];
int64_t count_zero[320005], count_sum[320005];

void read_data()
{
    cin >> N >> Q;
    for(int i = 1; i <= N; i++)
    {
        char c; cin >> c;
        bits[i] = c - '0';
    }
}

void precompute()
{       
    // calculez sume partiale spre dreapta, cate valori de 0 am pana la pozitia i, pornind de la 
    if(bits[1] == 0)
    {
        count_zero[1] = 1;
        count_sum[1] = 1;
    }
    for(int i = 2; i <= N; i++)
    {
        count_zero[i] = count_zero[i - 1] + 1 - bits[i];
        count_sum[i] = count_sum[i - 1] + (bits[i] == 0 ? i : 0);
    }
}

inline int64_t count_zero_interval(int64_t l, int64_t r)
{
    return count_zero[r] - count_zero[l - 1];
}

inline int64_t sum_zero_interval(int64_t l, int64_t r)
{
    return count_sum[r] - count_sum[l - 1];
}

inline int64_t search_to_left(int64_t A, int64_t B)
{
    int64_t L = 1, R = A - 1, ans = -1;
    // vreau atatia de 0 cati de 1 exista intre A si B
    int64_t prag = B - A + 1 - count_zero_interval(A, B);
    while(L <= R)
    {
        int64_t M = (L + R) / 2;
        // Verific daca am pozitii de 0 libere intre M SI A
        if(count_zero_interval(M, A - 1) >= prag) {
            ans = M;
            L = M + 1;
        }  else {
            R = M - 1;
        }
    }
    return ans;
}

inline int64_t search_to_right(int64_t A, int64_t B)
{
    int64_t L = B + 1, R = N, ans = -1;
    // vreau atatia de 0 cati de 1 exista intre A si B
    int64_t prag = B - A + 1 - count_zero_interval(A, B);
    while(L <= R)
    {
        int64_t M = (L + R) / 2;
        // Verific daca am pozitii de 0 libere
        if(count_zero_interval(B + 1, M) >= prag) {
            ans = M;
            R = M - 1;
        }  else {
            L = M + 1;
        }
    }
    return ans;
}

inline int64_t sum_one_interval(int64_t l, int64_t r)
{
    return r * (r + 1) / 2 - (l - 1) * l / 2 - sum_zero_interval(l, r);
}

int64_t f(int L, int M, int R)
{
    /* Caut binar pozitia din stanga minima pana la care am atatea zerouri cati de 1 vreau sa mut
    - Avem intervalul [L, M] si incercam sa mutam valorile de 1 in [alfa, L - 1];
    */
    // Cautam alfa
    int64_t alfa = search_to_left(L, M);
    //cout << "alfa = " << alfa << " pentru M = " << M << "\n";
    if(alfa == -1) 
    // Daca nu reusesc sa mut la stanga, incerc sa mut tot la dreapta
    {
        /* Caut binar pozitia din dreapta minima pana la care am atatea zerouri cati de 1 vreau sa mut
            - Avem intervalul [L, R] si incercam sa mutam valorile de 1 in [R + 1, beta];
            */
            // Cautam beta
            int64_t beta = search_to_right(L, R);
            //cout << "beta = " << alfa << " pentru M = " << M << "\n";
            if(beta == -1) return -1;
            // Acum calculez costul de a muta valorile din [L, R] in [R + 1, beta]
            // Costul va fi suma valorilor de 0 din [R + 1, beta] - suma valorilor de 1 din [L, R]
            int64_t ans = sum_zero_interval(R + 1, beta) - sum_one_interval(L, R);
            return ans;
    }
    else 
    {
        /* Caut binar pozitia din dreapta minima pana la care am atatea zerouri cati de 1 vreau sa mut
        - Avem intervalul [M + 1, R] si incercam sa mutam valorile de 1 in [M + 1, beta];
        */
        // Cautam beta
        int64_t beta = search_to_right(M + 1, R);
        //cout << "beta = " << alfa << "\n";
        if(beta == -1) {
            // Daca beta imi da -1, incerc sa mut tot la stanga
            /* Caut binar pozitia din stanga minima pana la care am atatea zerouri cati de 1 vreau sa mut
            - Avem intervalul [L, R] si incercam sa mutam valorile de 1 in [alfa, L - 1];
            */
            // Cautam alfa
            int64_t alfa = search_to_left(L, R);
            //cout << "alfa = " << alfa << " pentru M = " << M << "\n";
            if(alfa == -1) return -1;
            // Acum calculez costul de a muta valorile din [L, R] in [alfa, L - 1]
            // Costul va fi suma valorilor de 1 din [L, R] - suma valorilor de 0 din [afla, L - 1]
            int64_t ans = sum_one_interval(L , R) - sum_zero_interval(alfa, L - 1);
            return ans;
        } else {
            // Acum calculez costul de a muta valorile din [L, M] in [alfa, L - 1]
            // Costul va fi suma valorilor de 1 din [L, M] - suma valorilor de 0 din [afla, L - 1]
            int64_t ans = sum_one_interval(L , M) - sum_zero_interval(alfa, L - 1);
            // Acum calculez costul de a muta valorile din [M + 1, R] in [R + 1, beta]
            // Costul va fi suma valorilor de 0 din [R + 1, beta] - suma valorilor de 1 din [M + 1, R]
            ans += sum_zero_interval(R + 1, beta) - sum_one_interval(M + 1, R);
            return ans;
        }
    }
    
}

void solve()
{
    while(Q--)
    {
        int64_t L, R, ans = 1e18;
        cin >> L >> R;
        // There are 3 cases
        if(L == R) {
            if(bits[L] == 1) {
                int64_t ans_best = -1;
                // Caut cel mai apropiat 0 in stanga sau in dreapta
                {
                    int64_t A = 1, B = L - 1, ans = -1;
                    // vreau atatia de 0 cati de 1 exista intre A si B
                    int64_t prag = 1;
                    while(A <= B)
                    {
                        int64_t M = (A + B) / 2;
                        // Verific daca am pozitii de 0 libere intre M SI A
                        if(count_zero_interval(M, L - 1) >= prag) {
                            ans = M;
                            A = M + 1;
                        }  else {
                            B = M - 1;
                        }
                    }

                    if(ans != -1)
                        ans_best = L - ans;
                }
                
                // Caut cel mai apropiat 0 in dreapta
                {
                    int64_t A = R + 1, B = N, ans = -1;
                    // vreau atatia de 0 cati de 1 exista intre A si B
                    int64_t prag = 1;
                    while(A <= B)
                    {
                        int64_t M = (A + B) / 2;
                        // Verific daca am pozitii de 0 libere intre M SI A
                        if(count_zero_interval(R + 1, M) >= prag) {
                            ans = M;
                            B = M - 1;
                        }  else {
                            A = M + 1;
                        }
                    }
                    if(ans != -1) {
                        if(ans_best == -1) ans_best = ans - L;
                        else ans_best = min(ans_best, ans - L);
                    }
                }

                cout << ans_best << "\n";

            } else {
                cout << "0\n";
            }
            
        } else
        if(L != 1 && R != N)
        {
            int64_t A = L, B = R;
            for(int i = 1; i <= 50; i++)
            {
                int64_t m1 = L + (R - L) / 3;
                int64_t m2 = R - (R - L) / 3;
                int64_t x1 = f(A, m1, B);
                int64_t x2 = f(A, m2, B);
                if(x1 == -1 && x2 == -1) {
                    L = m1, R = m2;
                } else if(x1 == -1) {
                    L = m1;
                } else if(x2 == -1) {
                    R = m2;
                } else if(x1 >= x2) {
                    L = m1;
                } else {
                    R = m2;
                }
                if(x1 != -1) ans = min(ans, x1);
                if(x2 != -1) ans = min(ans, x2);
            }
            if(ans == 1e18) ans = -1;
            cout << ans << "\n";
        }
        else if(L == 1 && R == N) {
            // Daca nu sunt copii, se afiseaza 0, altfel -1
            if(count_zero_interval(L, R) == N) cout << "0\n";
            else cout << "-1\n";
        } else if(L == 1) {
            // Caut doar sa maresc in dreapta
            /* Caut binar pozitia din dreapta minima pana la care am atatea zerouri cati de 1 vreau sa mut
            - Avem intervalul [L, R] si incercam sa mutam valorile de 1 in [R + 1, beta];
            */
            // Cautam beta
            int64_t beta = search_to_right(L, R);
            if(beta == -1) {
                cout << "-1\n";
                continue;
            }
            // Acum calculez costul de a muta valorile din [L, R] in [R + 1, beta]
            // Costul va fi suma valorilor de 0 din [R + 1, beta] - suma valorilor de 1 din [L, R]
            int64_t ans = sum_zero_interval(R + 1, beta) - sum_one_interval(L, R);
            cout << ans << "\n";
        } else if(R == N) {
            /* Caut binar pozitia din stanga minima pana la care am atatea zerouri cati de 1 vreau sa mut
            - Avem intervalul [L, R] si incercam sa mutam valorile de 1 in [alfa, L - 1];
            */
            // Cautam alfa
            int64_t alfa = search_to_left(L, R);
            if(alfa == -1) {
                cout << "-1\n";
                continue;
            }

            // Acum calculez costul de a muta valorile din [L, M] in [alfa, L - 1]
            // Costul va fi suma valorilor de 1 din [L, M] - suma valorilor de 0 din [afla, L - 1]
            int64_t ans = sum_one_interval(L , R) - sum_zero_interval(alfa, L - 1);
            cout << ans << "\n";
        }
    }
}

int main()
{
    read_data();
    precompute();
    solve();
}