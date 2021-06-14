//
// Created by Bohdan on 05.04.2021.
//

#ifndef B__TREE_B_NODE_H
#define B__TREE_B_NODE_H

#include <fstream>
using namespace std;

template <size_t t>
class B_Node {
public:
    explicit B_Node(bool _leaf) {
        n = 0;
        Keys = new int[2 * n - 1];
        Data = new int[2 * n];
        isLeaf = _leaf;
    }

private:
    int   n;
    int * Keys;
    int * Data;
    bool  isLeaf;
};

#include "B_Node.cpp"

#endif //B__TREE_B_NODE_H
