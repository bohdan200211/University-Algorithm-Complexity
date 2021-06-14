#include <iostream>
using namespace std;

bool f(const string & T, const string & t) {
    const int n = T.size();
    int i = 0, j;

    while (i < n) {
        j = 0;
        while (j < n && T[j] == t[i % n]) {
            i++;
            j++;
        }

        if (j == n)
            return true;

        i++;
    }
    return false;
}

int main() {
    cout << f("abababc", "ababcab");
    return 0;
}