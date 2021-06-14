#include <iostream>
#include <chrono>
#include <fstream>
using namespace std;
using namespace std::chrono;

bool is_substr_eq (const string & P, const string & T, const int begin) {
    int i = 0, m = P.length();
    while (i < m && T[begin + i] == P[i]) {
        i++;
    }
    return i == m;
}

int mod_pow (int n, int pow, int mod) {
    int i = 0, res = n;
    while (++i < pow) {
        res *= n;
        res %= mod;
    }
    return res;
}

int mod_div (int a, int n) {
    return a > 0 ? a % n : n + a % n;
}

int * get_suffix_arr (const string & P, int * K) {
    const int m = P.length();

    K[0] = 1;
    int c,i,j;
    for (int k = 1; k <= m; k++) {
        if (m - k - K[k - 1] < 0 || P[m - k] == P[m - k - K[k - 1]]) {
            K[k] = K[k - 1];
        } else {
            c = 0;
            for (i = m - k - K[k - 1] - 1, j = m - 1; i > -1 && c != k; i--) {
                if (P[i] == P[j]) {
                    j--;
                    c++;
                } else {
                    j = m - 1;
                    c = 0;
                }
            }
        }
        if (c == k)
            K[k] =  j - i;
        else
            K[k] =  m - c;
    }
}

void algo_rabin_karp_alg (const string & T, const string & P, const int q = 577)
{
    const int n = T.length(), m  = P.length(), FLA = '!';
    int d = 95, p = 0, t = 0, h = mod_pow(d,m - 1,q);

    for (int i = 0; i < m; i++) {
        p = (d*p + P[i] - FLA) % q;
        t = (d*t + T[i] - FLA) % q;
    }

    for (int s = 0; s <= n - m; s++) {
        if (p == t) {
            if (is_substr_eq(P,T,s)) {
                cout << "sample found with offset : " << s << endl;
            }
        }

        if (s < n - m) {
            t = mod_div((d * (t - (T[s] - FLA) * h) + (T[s + m] - FLA)),q);
        }
    }
}

void algo_brute_force (const string & T, const string & P) {
    for (int s = 0; s <= T.length() - P.length(); s++) {
        if (is_substr_eq(P, T, s))
            cout << "sample found with offset : " << s << endl;
    }
}

void algo_horspool (const string & T, const string & P) {
    const int n = T.length(), m  = P.length();
    int A[256] = {0};

    for (int i = m - 2; i > -1; i--) {
        if (!A[P[i]])
            A[P[i]] = m - i - 1;
    }

    int i = m - 1, k;
    while (i < n) {
        k = 0;
        while (k < m && T[i - k] == P[m - k - 1]) {
            k++;
        }
        if (k == m) {
            cout << "sample found with offset : " << i - m + 1 << endl;
            i++;
        }
        else
            i = i + (A[T[i]] ? A[T[i]] : m);
    }
}

void algo_boyer_moore (const string & T, const string & P) {
    const int n = T.length(), m  = P.length();
    int A[256] = {0};

    for (int i = m - 2; i > -1; i--) {
        if (!A[P[i]])
            A[P[i]] = m - i - 1;
    }

    int * K = new int[m];
    get_suffix_arr(P, K);

    int i = m - 1, k;
    while (i < n) {
        k = 0;
        while (k < m && T[i - k] == P[m - k - 1]) {
            k++;
        }
        if (k == m) {
            cout << "sample found with offset : " << i - m + 1 << endl;
            i++;
        }
        else
            i = i + max((A[T[i]] ? A[T[i]] : m), K[k]);
    }
    delete [] K;
}

void prefix_compute_funct(int * prefix, const string & P) {
    const int m = P.length();
    prefix[0] = prefix[1] = 0;

    int k = 0;
    for (int q = 1; q < m; q++) {
        while (k > 0 && P[k] != P[q])
            k = prefix[k];

        if (P[k] == P[q])
            k = k + 1;

        prefix[q + 1] = k;
    }
}

void KMP (const string & T, const string & P) {
    const int n = T.length(), m  = P.length();

    int * prefix = new int[m + 1];
    prefix_compute_funct (prefix, P);

    int q = 0;
    for (int i = 0; i < n; i++) {
        while (q > 0 && P[q] != T[i])
            q = prefix[q];

        if (P[q] == T[i])
            q = q + 1;

        if (q == m) {
            cout << "sample found with offset : " << i - m + 1 << endl;
            q = prefix[m];
        }
    }
    delete [] prefix;
}


int main() {
    ifstream input;
    input.open("algo.txt");

    if (!input.is_open())
        return 1;

    string T,P;
    getline(input,T,'\n');
    getline(input,P);

    input.close();

    auto t1 = std::chrono::high_resolution_clock::now();
    algo_brute_force(T,P);
    auto t2 = std::chrono::high_resolution_clock::now();
    algo_rabin_karp_alg(T,P);
    auto t3 = std::chrono::high_resolution_clock::now();
    algo_horspool(T,P);
    auto t4 = std::chrono::high_resolution_clock::now();
    algo_boyer_moore(T,P);
    auto t5 = std::chrono::high_resolution_clock::now();
    KMP(T,P);
    auto t6 = std::chrono::high_resolution_clock::now();

    auto duration1 = duration_cast<microseconds>( t2 - t1 ).count();
    auto duration2 = duration_cast<microseconds>( t3 - t2 ).count();
    auto duration3 = duration_cast<microseconds>( t4 - t3 ).count();
    auto duration4 = duration_cast<microseconds>( t5 - t4 ).count();
    auto duration5 = duration_cast<microseconds>( t6 - t5 ).count();

    cout << "brute_force : " << duration1 << endl << "rabin_karp : " << duration2 << endl
    << "horspool : " << duration3 << endl << "boyer_moore : " << duration4 << endl
    << "KMP : " << duration5 << endl;

    return 0;
}