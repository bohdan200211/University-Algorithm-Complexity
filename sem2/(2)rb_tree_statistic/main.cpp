#include <iostream>
#include <vector>
#include "RBtree.h"

using namespace my_rbtree;

int main() {
    std::vector<std::pair<int,std::string>> input = {
            {1,"one"},
            {2,"two"},
            {3,"three"},
            {4,"four"},
            {5,"five"},
            {6,"six"},
            {7,"seven"},
            {8,"eight"},
            {9,"nine"},
    };


    RBtree<int,std::string> t;

    for (auto & i : input)
        t.insert(i.first, i.second);

    t.delet(2);

    //std::cout << t.search(5);

    for (int i = 1; i < input.size(); i++) {
        std::cout << t.os_select(i) << ' ';
    }

    //std::cout << "bp stop!";
    return 0;
}
