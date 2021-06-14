//
// Created by Bohdan on 30.03.2021.
//

#include "BinomialList.hpp"



template <typename K, typename D>
BinomialList<K, D>::BinomialList(BinomialNode* _head):
        head(_head) {}


template<typename K, typename D>
BinomialList<K, D>::BinomialList(const K &key, const D &data, size_t d) :
        head(new BinomialNode(key, data,d)) {}


template <typename K, typename D>
void BinomialList<K, D>::MergeList(BinomialList<K, D>* L1, BinomialList<K, D>* L2) {
    Node * tempNewHead;
    Node * tempHead1 = L1->head;
    Node * tempHead2 = L2->head;

    if (!tempHead1 && !tempHead2) {
        head = nullptr;
        return;
    }

    //введення мінімума
    if (tempHead1 && tempHead2) {
        if (tempHead1->degree < tempHead2->degree) {
            tempNewHead = tempHead1;
            tempHead1 = tempHead1->sibling;
        } else {
            tempNewHead = tempHead2;
            tempHead2 = tempHead2->sibling;
        }
    } else if (!tempHead2) {
        tempNewHead = tempHead1;
        tempHead1 = tempHead1->sibling;
    } else if (!tempHead1){
        tempNewHead = tempHead2;
        tempHead2 = tempHead2->sibling;
    }

    Node * newHead = tempNewHead;

    while (tempHead1 && tempHead2) {
        if (tempHead1->degree < tempHead2->degree) {
            tempNewHead->sibling = tempHead1;
            tempHead1 = tempHead1->sibling;
        } else {
            tempNewHead->sibling = tempHead2;
            tempHead2 = tempHead2->sibling;
        }
        tempNewHead = tempNewHead->sibling;
    }
    if (tempHead1) {
        tempNewHead->sibling = tempHead1;
    } else if (tempHead2) {
        tempNewHead->sibling = tempHead2;
    }

    L1->head = L2->head = nullptr;

    head = newHead;
}


template<typename K, typename D>
void BinomialList<K, D>::BinomialLink(Node * y, Node * z) {
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
}

template<typename K, typename D>
void BinomialList<K, D>::Consolidate() {
    Node * prevTemp = nullptr;
    Node * Temp = head;
    Node * nextTemp = Temp->sibling;

    while (nextTemp) {
        if (Temp->degree != nextTemp->degree ||
            (nextTemp->sibling != nullptr &&
             nextTemp->sibling->degree == Temp->degree)) {
            prevTemp = Temp;
            Temp     = nextTemp;
        } else {
            if (Temp->Key <= nextTemp->Key) {
                Temp->sibling = nextTemp->sibling;
                BinomialLink(nextTemp,Temp);
            } else {
                if (prevTemp == nullptr) {
                    head = nextTemp;
                } else {
                    prevTemp->sibling = nextTemp;
                }
                BinomialLink(Temp, nextTemp);
                Temp = nextTemp;
            }
        }
        //??????
        nextTemp = Temp->sibling;
    }
}

template<typename K, typename D>
Node * BinomialList<K, D>::GetMinimumNode(bool isEraseMin) {
    if (isEmpty()){
        return nullptr;
    }

    Node * prevTemp = nullptr;
    Node * Temp     = head;
    Node * nextTemp = head->sibling;
    K * min         = &(head->Key);

    while (nextTemp) {
        //перегрузити при необхідності operator<()
        if (nextTemp->Key < *min) {
            min = &(nextTemp->Key);

            prevTemp = Temp;
            Temp = nextTemp;
        }
        nextTemp = nextTemp->sibling;
    }

    if (isEraseMin) {
        if (prevTemp) {
            prevTemp->sibling = Temp->sibling;
        } else {
            head = Temp->sibling;
        }
        Temp->sibling = nullptr;
    }

    return Temp;
}


//BinomialNode Member-Functions Definition
template<typename K, typename D>
BinomialList<K, D>::BinomialNode::BinomialNode(const K &key, const D &data, size_t d) :
        Key(key), Data(data), degree(d), parent(nullptr), child(nullptr), sibling(nullptr) {}


template<typename K, typename D>
BinomialList<K, D>::BinomialNode::~BinomialNode() {
    delete child;
    delete sibling;
}


template<typename K, typename D>
Node *BinomialList<K, D>::BinomialNode::CreateListWithNode() {
    if (!child) {
        return nullptr;
    }
    BinomialNode * prevLeftChild = nullptr;
    BinomialNode * LeftChild     = child;
    BinomialNode * nextLeftChild = child->sibling;

    if (!nextLeftChild) {
        child->parent = nullptr;
    }

    while (nextLeftChild) {
        LeftChild->parent  = nullptr;
        LeftChild->sibling = prevLeftChild;

        prevLeftChild = LeftChild;
        LeftChild     = nextLeftChild;
        nextLeftChild = nextLeftChild->sibling;
    }

    LeftChild->parent  = nullptr;
    LeftChild->sibling = prevLeftChild;

    child = nullptr;

    return LeftChild;
}

template<typename K, typename D>
Node * BinomialList<K, D>::BinomialNode::SearchNodeByKey(const K & targetKey) {
    if (Key == targetKey) {
        return this;
    }

    Node * resultChild   = nullptr;
    Node * resultSibling = nullptr;

    //чи буде час пошуку оптимальнішим, якщо почати перевіряти з кінця (спочатку sibling -> child)????

    if (child && Key <= targetKey) {
        resultChild = child->SearchNodeByKey(targetKey);
    }

    if (sibling) {
        resultSibling = sibling->SearchNodeByKey(targetKey);
    }

    return resultChild ? resultChild : resultSibling;//заперевірку можна використита resultSibling
}


template<typename K, typename D>
void BinomialList<K, D>::BinomialNode::printNode(vector<vector<string>> & output, size_t w, size_t h) {
    output[h][w] = std::to_string(Key);

    //4 краще винести в константу
    output[h][w].append(string(4 - output[h][w].size(),' '));
    if (sibling && parent) {
        size_t next_id = (sibling->degree == 0) ? w + 2 : w + 2 * pow(2, sibling->degree - 1);
        for (int i = w + 1; i < next_id; i++) {
            output[h][i] = ForOutputArrow;
        }

        sibling->printNode(output, next_id, h);
    }

    if (degree) {
        child->printNode(output, (degree == 1) ? w : w - 2 * pow(2, degree - 2), h + 1);
    }
}



