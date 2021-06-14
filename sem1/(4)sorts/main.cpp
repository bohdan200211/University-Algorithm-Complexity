#include <iostream>
#include <vector>
using namespace std;

void generate_arr(vector<pair<int,int>> & A, int n) {
    A.resize(n);
    srand(time(nullptr));

    for (int i = 0; i < n; i++) {
        A[i] = {rand() % 2, i};
    }
}

void counting_sort(const vector<pair<int,int>> & A, vector<pair<int,int>> & B)
{
    const int n = A.size();

    vector<int> C = {-1,0};

    for (int j = 0; j < n; j++)
    {
        C[A[j].first] = C[A[j].first] + 1;
    }

    C[1] = C[0] + C[1];

    for (int j = n - 1; j > -1; j--) {
        B[C[A[j].first]] = A[j];
        C[A[j].first] = C[A[j].first] - 1;
    }
}

void double_point_sort (vector<pair<int,int>> & A) {
    int l = 0, r = A.size() - 1;

    while (l < r) {
        if (A[l].first > A[r].first)
            swap(A[l++],A[r++]);
        else if (A[l].first < A[r].first) {
            l++;r--;
        } else if (A[l].first == 0)
            l++;
        else
            r--;
    }
}

void merge(vector<pair<int,int>> & A, int l, int mid, int r)
{
    if (A[mid - 1].first <= A[mid].first)
        return;
    mid--;

    while (l <= mid && A[mid].first == 1) {
        int temp = mid;

        do {
            swap(A[temp],A[temp + 1]);
            temp++;
        } while (temp + 1 <= r && A[temp + 1].first == 0);
        mid--;
    }
}

void merge_sort(vector<pair<int,int>> & A, int l, int r)
{
    if (l + 1 < r) {
        int m = (r + l) / 2;

        merge_sort(A, l, m);
        merge_sort(A, m, r);

        merge(A, l, m, r - 1);
    }
}

int main() {
    const int n = 12;

    vector<pair<int,int>> res(n);
    vector<pair<int,int>> A;

    generate_arr(A,n);
    A[0].first = 1;
    A[1].first = 1;
    A[2].first = 1;
    A[3].first = 1;
    A[4].first = 0;
    A[5].first = 0;
    A[6].first = 0;


    //counting_sort(A,res);
    //double_point_sort(A);

    for (int i = 0; i < n; i++) {
        cout << "key : " << A[i].first << "\t" << "val : " << A[i].second << endl;
    }

    cout << endl;
    merge_sort(A,0,n);

    for (int i = 0; i < n; i++) {
        cout << "key : " << A[i].first << "\t" << "val : " << A[i].second << endl;
    }
    return 0;
}