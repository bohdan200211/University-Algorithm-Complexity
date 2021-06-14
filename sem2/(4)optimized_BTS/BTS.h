/*template<typename K, typename D>
void BTS<K, D>::delet (const K & key) {
    node * z = _find(key);
    node *y = z,*x;


    if(z->left == nullptr) {
        transplant(z,z->right);
    }
    else if(z->right == nullptr) {
        transplant(z,z->left);
    }
    else {
        y = tree_minimum(z->right);
        if(y->p != z) {
            transplant(y,y->right);
            y->right = z->right;
            y->right->p = y;
        }
        transplant(z,y);
        y->left = z->left;
        y->left->p = y;
    }
}*/
//
// Created by Bohdan on 08.03.2021.
//

#ifndef OPTIMIZED_BTS_BTS_H
#define OPTIMIZED_BTS_BTS_H
#include <vector>
using std::vector;

template <typename K, typename D>
class BTS {
public:
    BTS() : root(nullptr) {}

    BTS(const vector<double> & p, const vector<double> & q, const vector<std::pair<K,D>> & indata) {
        const int n = p.size();
        vector<vector<double>> E(n + 1, vector<double>(n + 1)), W(n + 1, vector<double>(n + 1));
        vector<vector<int>> Root(n, vector<int>(n));

        //innit
        for (int i = 0; i < n; ++i) {
            E[i][i]    = q[i];
            W[i][i]    = q[i];
            Root[i][i] = i;
        }


        E[n][n] = q[n]; W[n][n] = q[n];
        for (int i = 0; i < n; ++i) {
            int j = i + 1;

            W[i][j] = W[i][j - 1] + p[j - 1] + q[j];
            E[i][j] = E[i][i] + E[i + 1][j] + W[i][j];
            //Root[i][j - 1] = i;
        }


        for (int l = 1; l < n; ++l) {
            for (int i = 0; i < n - l; ++i) {
                int j = i + l + 1;
                E[i][j] = INT32_MAX;
                W[i][j] = W[i][j - 1] + p[j - 1] + q[j];

                //for (int r = i; r < j; ++r) {
                for (int r = Root[i][j - 2]; r <= Root[i + 1][j - 1]; ++r) {
                    double t = E[i][r] + E[r + 1][j] + W[i][j];
                    if (t < E[i][j]) {
                        E[i][j] = t;
                        Root[i][j - 1] = r;
                    }
                }
            }
        }

        root = f(0,n-1, Root, indata);

    }

    void print_bts () {
        _printST(root);
    }

    virtual ~BTS() {
        delete root;
    }

private:
    class node {
    public:
        const K & key;
        const D & data;
        node * left, *right, *p;

        node(const K & _key = K(), const D & _data = D()) :
                key(_key), data(_data),left(nullptr), right(nullptr),p(nullptr){}

        ~node () {
            //std::cout << "del:"<< data << std::endl;
            delete left;
            delete right;
        }
    };

    node * root;

    node * f(int l, int r, const vector<vector<int>> & inroot, const vector<std::pair<K,D>> & indata) {
        node * res = nullptr;
        if (l <= r) {
            int temp = inroot[l][r];
            const std::pair<K,D>& dat = indata[temp];

            res = new node(dat.first, dat.second);

            res->left = f(l, temp - 1, inroot, indata);
            res->right = f(temp + 1, r, inroot, indata);
        }

        return res;
    }

    void _printST(node * _r, const std::string& prefix = "", bool isLeft = false) {
        if(_r != nullptr ) {
            std::cout << prefix;

            std::cout << (isLeft ? "├──" : "└──" );

            std::cout << _r->key<<" : "<<_r->data << std::endl; // custom data output

            _printST(_r->left,prefix + (isLeft ? "│   " : "    "), true);
            _printST(_r->right, prefix + (isLeft ? "│   " : "    "),false);
        }
    }

};

#endif //OPTIMIZED_BTS_BTS_H
