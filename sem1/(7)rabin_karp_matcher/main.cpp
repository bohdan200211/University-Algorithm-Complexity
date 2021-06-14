#include <iostream>
#include <vector>
using namespace std;

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


void f(const vector<string> & T, const vector<string> & P, const int q = 1973) {
    if (T.empty())
        return;
    const int n = T.size(), m = P.size(), FLA = '0', d = 10, h = mod_pow(d, m - 1, q);

    vector<int> p(m), t(n);
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < m; i++) {
            p[j] = (d * p[j] + P[j][i] - FLA) % q;
        }
    }

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            t[j] = (d * t[j] + T[j][i] - FLA) % q;
        }
    }

    for (int sx = 0; sx <= n - m; sx++) {
        for (int sy = 0; sy <= n - m; sy++) {
            bool is_eq = true;
            for (int a = 0; a < m; a++) {
                is_eq = is_eq & (p[a] == t[a + sy]);
                if (!is_eq) break;
            }

            if (is_eq) {
                cout << "sample found with offset(x,y) : " << sx + 1 << ',' << sy + 1 << endl;
            }
        }
        if (sx < n - m)
            for (int a = 0; a < n; a++)
                t[a] = mod_div((d * (t[a] - (T[a][sx] - FLA) * h) + (T[a][sx + m] - FLA)), q);
    }
}
//    for (int j = 0; j < T.size(); j++) {
//
//        for (int i = 0; i < m; i++) {
//            p = (d * p + P[i] - FLA) % q;
//            t = (d * t + T[j][i] - FLA) % q;
//        }
//
//        for (int s = 0; s <= n - m; s++) {
//            if (p == t) {
//                if (is_substr_eq(P,T[j],s)) {
//                    cout << "sample found with offset(s,x) : " << s << ','<< j << endl;
//                }
//            }
//
//            if (s < n - m) {
//                t = mod_div((d * (t - (T[j][s] - FLA) * h) + (T[j][s + m] - FLA)),q);
//            }
//        }
//    }


int main() {
    f(
            {
                "qkjrhbgw",
                "jtiomori",
                "ptiortbh",
                "nejiyxcm",
                "hcguiowx",
                "uiwctiou",
                "ftrcejib",
                "jabbcgux"
            },
            {
                "tio",
                "eji",
                "cgu"
            }
            );
    return 0;
}