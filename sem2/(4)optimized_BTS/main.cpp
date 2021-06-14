#include <iostream>
#include <vector>

#include "BTS.h"
using namespace std;






int main() {
    vector<pair<int, string>> input = {
            {1,"qwe"},
            {2,"rty"},
            {3,"uio"},
            {4,"asd"},
            {5,"fgh"},
            {6,"zxc"},
            {7,"mnv"}
    };

    BTS<int,string> T({1, 1, 1, 1, 1, 1, 1},
                      {1, 1, 1, 1, 1, 1, 1, 1},
                      input);

    T.print_bts();
    return 0;
}
