#include <iostream>
#include <vector>
#include "splay_tree.h"

using namespace std;
using my_splay_tree::splay_tree;

int main() {
    vector<pair<int,string>> input = {
            {0,"aaa"},
            {1,"bbb"},
            {2,"ccc"},
            {3,"ddd"},
            {4,"eee"},
    };

    splay_tree<int, string> T;

    for (auto & x : input) {
        T.insert(x.first, x.second);
    }

    cout << "del \n";
    T.del(1);

    cout << "search \n";
    cout << T.search(3);*/

    return 0;
}
