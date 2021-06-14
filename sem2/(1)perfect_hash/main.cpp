#include <iostream>
#include <vector>

using namespace std;

//для назви міста використовується lowercase English alphabet

const int prm_num_len = 60;
const int prime_nums[prm_num_len] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
                                73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,
                                179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281};


int compute_p (const int m) {
    int r = prm_num_len, l = 13, mid;
    for (int i = 0; i < 3; i++) {
        mid = (r+l)/2;
        if (m < prime_nums[mid]) r = mid;
        else l = mid;
    }
    return prime_nums[mid];
}


int double_hash (const int key ,const int a, const int b, const int i) {
    return (key % a) + i * (key % b);
}


/*int sipmle_hash (const int key ,const int a, const int b) {
    return (a * key + b);
}*/
//a*s[i]+b = s[i]
//s[0]+s[1]*q+s[1]*q^2...
int compute_hash (const string & name, const int m, const int a, const int b){
    const int q = 53;
    int hash_val = 0, q_pow = 1;

    for (int i = 0; i < name.length(); i++) {
        hash_val = (hash_val + double_hash(name[i] - 'a',a,b,i) * q_pow) % m;
        q_pow = (q_pow * q) % m;
    }

    return hash_val;
}


struct obj {
    string name;
    string group;

    obj(string  n = "",string  g = "") : name(move(n)), group(move(g)) {}
};


namespace my_hash {
    struct hash {
        int m, a, b;
        vector<obj> H;

        hash() : m(0), a(0), b(0) {}

        hash(const int p, vector<obj> & arr) {
            const int l = arr.size();
            if (l == 0) {
                a = b = m = 0;
            } else if (l == 1) {
                m = 1;
                a = b = 1;
                H = {arr[0]};
            } else {
                m = l * l;
                H.resize(m);


                int temp;

                for (int pa = 0; pa < prm_num_len; pa++) {
                    for (int pb = 0; pb < prm_num_len; pb++) {
                        vector<int> computed_hashes(l);
                        vector<bool> check(m, true); //true = можна вставити елем.

                        for (int i = 0; i < l; i++) {
                            if (check[temp = compute_hash(arr[i].name, m, prime_nums[pa], prime_nums[pb])]) {
                                check[temp] = false;
                                computed_hashes[i] = temp;
                            } else goto label;
                        }

                        a = prime_nums[pa];
                        b = prime_nums[pb];

                        for(int i = 0; i < l; i++)
                            H[computed_hashes[i]] = arr[i];
                        return;
                        label:;
                    }
                }

                cerr << "can't create perfect hash-map";
            }
        }
    };
}


struct prf_hash {
    int P, m, a, b;
    vector<my_hash::hash> perfect_hash;

    prf_hash(const int _m) : m(_m), P(compute_p(_m)),a(rand()%P),b(rand()%P) {
        perfect_hash.resize(m);
    }
};


void create_perfect_hash_map (prf_hash & H, const vector<obj> & studs) {
    const int m = studs.size(), p = compute_p(m);

    vector<vector<obj>> prime_hash_tbl (m);

    for (const obj & x : studs) {
        prime_hash_tbl[compute_hash(x.name,m,H.a,H.b)].push_back(x);
    }

    for (int i = 0; i < m; i++) {
        H.perfect_hash[i] = my_hash::hash(p, prime_hash_tbl[i]);
    }
}


obj search_obj_by_name (prf_hash & _hash, const string & _name) {
    my_hash::hash temp = _hash.perfect_hash[compute_hash(_name, _hash.m, _hash.a, _hash.b)];
    return temp.H[compute_hash(_name,temp.m,temp.a,temp.b)];
}



int main() {
    vector<obj> studs = {
            {"au", "qwe"},
            {"bm", "qwe"},
            {"cl", "qwe"},

            {"rn", "asd"},
            {"ta", "asd"},
            {"yv", "asd"}
    };

    prf_hash H (studs.size());

    create_perfect_hash_map(H, studs);

    obj test1 = search_obj_by_name(H, "au");

    cout << "bp stop";
    return 0;
}
