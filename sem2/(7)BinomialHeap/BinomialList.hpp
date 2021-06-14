//
// Created by Bohdan on 30.03.2021.
//

#ifndef BINOMIALHEAP_BINOMIALLIST_HPP
#define BINOMIALHEAP_BINOMIALLIST_HPP

#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <numeric>
#include "BinomialHeap.hpp"
#define Node typename BinomialList<K, D>::BinomialNode
using namespace std;
const string ForOutputArrow = " -> "; //4
const string ForOutputSpace = "    "; //4

template <typename K, typename D>
class BinomialList {
public:
    class BinomialNode {
    public:
        explicit BinomialNode(const K & key = K(), const D & data = D(), size_t d = 0);
        ~BinomialNode();

        BinomialNode * CreateListWithNode ();
        BinomialNode * SearchNodeByKey (const K & targetKey);
        void printNode (vector<vector<string>> & output, size_t w, size_t h = 0);



        BinomialNode * parent;
        BinomialNode * child;
        BinomialNode * sibling;
        size_t        degree;
        K             Key;
        D             Data;
    };

    //---------------for test funtions------------------------------------------------------------------------
    void AddToStart (const K & key = K(), const D & data = D(), size_t d = 0) {
        auto newNode = new BinomialNode(key, data, d);
        newNode->sibling = head;
        head = newNode;
    }
    //--------------------------------------------------------------------------------------------------------

    explicit BinomialList(const K & key = K(), const D & data = D(), size_t d = 0);
    explicit BinomialList(BinomialNode* _head);
    ~BinomialList() {delete head;}

    inline bool isEmpty () {return head == nullptr;}
    void BinomialLink (BinomialNode * y, BinomialNode * z);
    void Consolidate ();
    BinomialNode * GetMinimumNode(bool isEraseMin = false);

    //видаляє L1 & L2
    void MergeList(BinomialList * L1, BinomialList * L2);

    BinomialNode * head;
};


#include "BinomialList.cpp"

#endif //BINOMIALHEAP_BINOMIALLIST_HPP
