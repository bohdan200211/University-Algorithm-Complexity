//
// Created by Bohdan on 30.03.2021.
//


#include "BinomialHeap.hpp"

template<typename K, typename D>
BinomialHeap<K, D>::BinomialHeap():
        m_HeadList(new BinomialList<K, D>(nullptr))  {}

template<typename K, typename D>
BinomialHeap<K, D>::BinomialHeap(BinomialList<K, D> *lst):
        m_HeadList(lst) {}


template<typename K, typename D>
const K & BinomialHeap<K, D>::Minimum() const {
    Node * minimumNode = m_HeadList->GetMinimumNode();;
    return minimumNode->Key;
}

template<typename K, typename D>
void BinomialHeap<K, D>::Union(BinomialHeap<K, D> * H1, BinomialHeap<K, D> * H2) {
    auto * resultList = new BinomialList<K, D>;
    resultList->MergeList(H1->m_HeadList, H2->m_HeadList);//(H1&H2)->m_HeadList=NUL

    if (resultList->isEmpty()) {
         m_HeadList = nullptr;
        return;
    }
    m_HeadList = resultList;
    m_HeadList->Consolidate();
}

template<typename K, typename D>
void BinomialHeap<K, D>::Insert(const K & key, const D & data) {
    auto * newHeap = new BinomialHeap<K, D>(new BinomialList<K, D>(key, data));
    Union(this, newHeap);
}

template<typename K, typename D>
void BinomialHeap<K, D>::ExtractMinimum() {
    Node * minimumNode = m_HeadList->GetMinimumNode(true);
    Node * newHead     = minimumNode->CreateListWithNode();

    auto * newHeap = new BinomialHeap<K, D>(new BinomialList<K, D>(newHead));

    Union(this, newHeap);

    //можна поветати Node*
    //K tempKey = minimumNode->Key;
    delete minimumNode;
}

template<typename K, typename D>
void BinomialHeap<K, D>::PrintHeap() {
    vector<size_t> w_heads;

    Node * prevTemp = nullptr;
    Node * Temp     = m_HeadList->head;
    while (Temp) {
        if (Temp->degree) {
            w_heads.push_back(pow(2, Temp->degree - 1));
        } else {
            w_heads.push_back(1);
        }

        prevTemp = Temp;
        Temp = Temp->sibling;
    }
    size_t h = prevTemp->degree + 1;
    size_t w = std::accumulate(w_heads.begin(), w_heads.end(), 0);

    vector<vector<string>> output(h, vector<string>(2 * w, ForOutputSpace));

    Temp = m_HeadList->head;
    w = 2 * w_heads[0] - 1;
    for (int i = 1;Temp;i++) {
        Temp->printNode(output, w);
        w += 2 * w_heads[i];
        Temp = Temp->sibling;
    }

    for (auto & str : output[0]) {
        if (str == ForOutputSpace) {
            str = ForOutputArrow;
        }
    }

    for (auto & row : output) {
        for (auto & str : row)
            std::cout << str;

        std::cout << std::endl;
    }
}

template<typename K, typename D>
bool BinomialHeap<K, D>::isEmpty() {
    return m_HeadList->isEmpty();
}



template<typename K, typename D>
void BinomialHeap<K, D>::DecreaseKey(const K & targetKey, const K & newKey) {
    if (newKey > targetKey) {
        cerr << "ERROR: newKey > targer!!!\n";
        return;
    }
    Node * targetNode = m_HeadList->head->SearchNodeByKey(targetKey);
    targetNode->Key = newKey;

    Node * Temp     = targetNode;
    Node * parentTemp = Temp->parent;

    while (parentTemp && Temp->Key < parentTemp->Key) {
        //swap можна зробити без копіювання за О(1) !!!
        swap(Temp->Key, parentTemp->Key);
        swap(Temp->Data, parentTemp->Data);

        Temp = parentTemp;
        parentTemp = parentTemp->parent;
    }
}



template<typename K, typename D>
void BinomialHeap<K, D>::Delete(const K & targetKey) {
    DecreaseKey(targetKey, /*повинно бути мінімальне значення для типу K!!!!!!*/ INT32_MIN); //працює лише для INT32
    ExtractMinimum();
}
