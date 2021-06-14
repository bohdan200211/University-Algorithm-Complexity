//
// Created by Bohdan on 30.03.2021.
//

#ifndef BINOMIALHEAP_BINOMIALHEAP_H
#define BINOMIALHEAP_BINOMIALHEAP_H

#include "BinomialList.hpp"

template <typename K, typename D>
class BinomialHeap {
public:
    BinomialHeap();
    ~BinomialHeap() {delete m_HeadList;}
    void Insert(const K & key, const D & data);
    void DecreaseKey(const K & targetKey, const K & newKey);
    void Delete (const K & targetKey);
    const K & Minimum() const;
    void ExtractMinimum();
    void PrintHeap ();
    bool isEmpty();


    //видаляє L1 & L2
    void Union (BinomialHeap * H1, BinomialHeap * H2);

private:
    BinomialHeap(BinomialList<K, D> * lst);

private:
    BinomialList<K, D> * m_HeadList;
};

#include "BinomialHeap.cpp"

#endif //BINOMIALHEAP_BINOMIALHEAP_H
