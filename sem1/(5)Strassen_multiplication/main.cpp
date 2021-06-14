#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>

using namespace std;
using namespace std::chrono;

void _print_m (vector<vector<int>> & A) {
    const int n = A.size();
    for (const auto & X : A) {
        for (const auto & x : X) {
            cout << x << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

void _add_m(vector<vector<int>> & A, vector<vector<int>> & B, vector<vector<int>> & C) {
    const int n = A.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void _sub_m(vector<vector<int>> & A, vector<vector<int>> & B, vector<vector<int>> & C) {
    const int n = A.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

void bf_mul_m(vector<vector<int>> & A, vector<vector<int>> & B, vector<vector<int>> & C) {
    const int n = A.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n ; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void strassen_mul_m(vector<vector<int>> & A, vector<vector<int>> & B, vector<vector<int>> & C) {
    const int n = A.size(), h = n / 2;


    if (n <= 64) {
        bf_mul_m(A, B, C);
    }
    else {
        vector<vector<int>> A11(h,vector<int>(h)),A12(h,vector<int>(h)),A21(h,vector<int>(h)),A22(h,vector<int>(h));
        vector<vector<int>> B11(h,vector<int>(h)),B12(h,vector<int>(h)),B21(h,vector<int>(h)),B22(h,vector<int>(h));
        vector<vector<int>> C11(h,vector<int>(h)),C12(h,vector<int>(h)),C21(h,vector<int>(h)),C22(h,vector<int>(h));
        vector<vector<int>> M1(h,vector<int>(h)),M2(h,vector<int>(h)),M3(h,vector<int>(h)),M4(h,vector<int>(h)),
        M5(h,vector<int>(h)),M6(h,vector<int>(h)),M7(h,vector<int>(h));


        vector<vector<int>> A_res(h,vector<int>(h));
        vector<vector<int>> B_res(h,vector<int>(h));

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < h; j++) {
                A11[i][j] = A[i][j];
                A12[i][j] = A[i][j + h];
                A21[i][j] = A[i + h][j];
                A22[i][j] = A[i + h][j + h];

                B11[i][j] = B[i][j];
                B12[i][j] = B[i][j + h];
                B21[i][j] = B[i + h][j];
                B22[i][j] = B[i + h][j + h];
            }
        }


        //M7
        _sub_m(A12, A22, A_res);
        _add_m(B21, B22, B_res);
        strassen_mul_m(A_res, B_res, M7);

        //M2
        _add_m(A21, A22, B_res);
        strassen_mul_m(B_res, B11, M2);

        //M3
        _sub_m(B12, B22, A_res);
        strassen_mul_m(A11, A_res, M3);

        //M4
        _sub_m(B21, B11, B_res);
        strassen_mul_m(A22, B_res, M4);

        //M1
        _add_m(A11, A22, A_res);
        _add_m(B11, B22, B_res);
        strassen_mul_m(A_res, B_res, M1);

        //M5
        _add_m(A11, A12, A_res);
        strassen_mul_m(A_res, B22, M5);

        //M6
        _sub_m(A21, A11, A_res);
        _add_m(B11, B12, B_res);
        strassen_mul_m(A_res, B_res, M6);





        //C11 = M1 + M4 - M5 + M7;
        _add_m(M1, M4, A_res);
        _sub_m(M7, M5, B_res);
        _add_m(A_res, B_res, C11);

        //C12 = M3 + M5;
        _add_m(M3, M5, C12);

        //C21 = M2 + M4;
        _add_m(M2, M4, C21);

        //C22 = M1 + M3 - M2 + M6;
        _add_m(M1, M3, A_res);
        _sub_m(M6, M2, B_res);
        _add_m(A_res, B_res, C22);

        for (int i = 0; i < h ; i++) {
            for (int j = 0 ; j < h ; j++) {
                C[i][j] = C11[i][j];
                C[i][j + h] = C12[i][j];
                C[i + h][j] = C21[i][j];
                C[i + h][j + h] = C22[i][j];
            }
        }
    }
}

void strassen_entrence_funct (vector<vector<int>> & A, vector<vector<int>> & B, vector<vector<int>> & C) {
    const int n = A.size();
    int t = n;
    while ((t / 2) * 2 == t) {
        t = t / 2;
    }
    if (t == 1) {
        strassen_mul_m(A,B,C);
    } else {
        int s = pow(2,int(log2(n)) + 1), N = n * n;
        vector<vector<int>> A1(s,vector<int>(s)),B1(s,vector<int>(s));
        C.resize(s);
        for (auto & x : C) x.resize(s);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A1[i][j] = A[i][j];
                B1[i][j] = B[i][j];
            }
        }

        strassen_mul_m(A1,B1,C);
    }
}

bool is_matrix_eq(vector<vector<int>> & C1,vector<vector<int>> & C2) {
    const int n = C1.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (C1[i][j] != C2[i][j])
                return false;
        }
    }
    return true;
}

void generate_mtx(vector<vector<int>> & A, int (*f) (int,int)) {
    const int n = A.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = f(i, j);
        }
    }
}

int f1(int i, int j) {
    return i + j;
}
int f2(int i, int j) {
    return i*i + j*j;
}

int main() {
    const int n = 512;

    vector<vector<int>> A(n,vector<int>(n)),B(n,vector<int>(n)),C1(n,vector<int>(n)),C2(n,vector<int>(n));

    generate_mtx(A, f1);
    //_print_m(A);
    generate_mtx(B, f1);
    //_print_m(B);
    //bf_mul_m(A,B,C2);


    //_print_m(C1);

    auto t1 = std::chrono::high_resolution_clock::now();
    strassen_entrence_funct(A,B,C1);
    auto t2 = std::chrono::high_resolution_clock::now();
//
    auto t3 = std::chrono::high_resolution_clock::now();
    bf_mul_m(A,B,C2);
    auto t4 = std::chrono::high_resolution_clock::now();

    auto duration1 = duration_cast<milliseconds>( t2 - t1 ).count();
    auto duration2 = duration_cast<milliseconds>( t4 - t3 ).count();
//
//
    cout << "Duration_strassen : " << duration1 << endl;
    cout << "Duration_brute_force : " << duration2 << endl;
    cout << "n : " << n << endl;

    if(is_matrix_eq(C2,C1)) {
        cout << "t";
    } else {
        cout << "f";
    }
    return 0;
}