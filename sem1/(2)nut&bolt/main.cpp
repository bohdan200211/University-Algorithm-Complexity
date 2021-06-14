#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
using namespace std;

int partition (vector<int> & A, int low, int high, int pivot) {
    int i = low, j = low;
    while (j < high) {
        if (A[j] < pivot) {
           swap(A[i],A[j]);
           i++;
        } else if (A[j] == pivot) {
            swap(A[j], A[high]);
            j--;
        }
        j++;
    }
    swap(A[i], A[high]);
    return i;
}
void sort_bolt_nut (vector<int> & bolt, vector<int> & nut, int low, int high) {
    if (high > low) {
        int pivot = partition(bolt, low, high, nut[high]);
        partition(nut,low, high,bolt[pivot]);
        sort_bolt_nut(bolt,nut,low,pivot-1);
        sort_bolt_nut(bolt,nut,pivot+1,high);
    }
}

vector<int> gen_arr (const int n) {
    vector<int> ans (n);
    for (int i = 0; i < n; i++) {
        ans[i] = i;
    }
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(ans.begin(),ans.end(),default_random_engine(seed));
    return ans;
}
ostream & operator<<(ostream & out, const vector<int> & A) {
    for (auto & x: A) {
        out << x << ' ';
    }
    return out;
}
int main() {
    const int n = 100;

    //type = int
    vector<int> B = gen_arr(n);
    vector<int> N = gen_arr(n);

    sort_bolt_nut(B,N,0,n-1);

    cout << B << "\n\n" << N;
    return 0;
}