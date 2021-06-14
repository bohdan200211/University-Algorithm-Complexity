#include <iostream>
#include "d_heap.h"
int main() {
    const int n = 11;
    int input[n] = {8,2,0,1,7,9,3,6,0,5,4};

    d_heap<int> H = d_heap<int> (n,3);

    for (auto & x : input) {
        H.insert(x);
    }

    H.increase(5,15);

    for (int i = 0; i < n; i++) {
        std::cout << H.extract_max() << ' ';
    }
    return 0;
}
