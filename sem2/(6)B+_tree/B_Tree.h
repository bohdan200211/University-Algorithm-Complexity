//
// Created by Bohdan on 05.04.2021.
//

#ifndef B__TREE_B_TREE_H
#define B__TREE_B_TREE_H

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <algorithm>


using namespace std;

template <size_t t, typename D>
class B_Tree {
public:
    explicit B_Tree(const string & _file_name, bool isNewTree = false);
    ~B_Tree() {
        Data.close();
        Tree.close();
    }

    int Search(int key);
    void Insert(int, D*);
    void Delete(int);
    void Print();
private:
    //***********************************************************************************************
    //change to int
    static const int sizeNode = /*1 + */sizeof(int) * (2 * t + 5);

    class B_Node {
    public:
        explicit B_Node(bool _leaf, int place) {
            n = 0;
            Keys.resize(t);
            Childs.resize(t + 1);
            isLeaf    = _leaf;
            selfPlace = place;
            isExist   = true;
        }

        int n;
        vector<int> Keys;
        vector<int> Childs;
        bool        isLeaf;
        int         selfPlace;
        bool        isExist;
    };

    void _insert(int, D*);
    void _split(stack<pair<B_Node*,int>> & st);
    void _merge(stack<pair<B_Node*,int>> & st);

    void _write(fstream & file, B_Node * node, int pos);
    void _read(fstream & file, B_Node * node, int pos);

private:
    fstream Data;
    fstream Tree;
};


template<size_t t, typename D>
B_Tree<t, D>::B_Tree(const string &_file_name, bool isNewTree) {
    if (isNewTree) {
        Data.open(_file_name + "Data.dat", ios::binary | ios::out);
        Tree.open(_file_name + "Tree.dat", ios::binary | ios::out);
        _write(Tree, new B_Node(true,0), 0);
        Data.close();
        Tree.close();
    }

    Data.open(_file_name + "Data.dat", ios::binary | ios::in | ios::out);
    Tree.open(_file_name + "Tree.dat", ios::binary | ios::in | ios::out);
}


template<size_t t, typename D>
void B_Tree<t, D>::Insert(int Key, D *data) {
//    B_Node * x = new B_Node(false, 0);
//
//    _read(Tree, x, 0);//читає з позиції 0
//
//    stack<pair<B_Node*, int>> st;
//    st.push({x, 0});
    _insert(Key, data);
}

template<size_t t, typename D>
void B_Tree<t, D>::_insert(int key, D * data) {
    B_Node * x = new B_Node(false, 0);

    _read(Tree, x, 0);//читає з позиції 0

    stack<pair<B_Node*, int>> st;
    st.push({x, 0});

    int i;
    while (!x->isLeaf) {
        i = 0;
        while (i < x->n && key >= x->Keys[i]) {
            i++;
        }

        auto * temp = new B_Node(false, 0);
        _read(Tree, temp, x->Childs[i]);

        x = temp;

        st.push({x, i});
    }
    Data.seekg(0, ios_base::end);
    int pos = Data.tellg();
    Data.write((char*)data, sizeof(D));
    Data.write((char*)new int(1), sizeof(int));

    x->Keys[x->n]   = key;
    x->Childs[x->n] = pos;
    x->n++;

    if (x->n == t) {
        _split(st);
    } else {
        _write(Tree, x, x->selfPlace);
    }
}

template<size_t t, typename D>
void B_Tree<t, D>::_split(stack<pair<B_Node *, int>> &st) {
    const int half = t / 2;

    Tree.seekg(0, ios_base::end);
    int endPos = Tree.tellg();

    B_Node * node;
    int index;

    while (!st.empty() && st.top().first->n == t) {
        node  = st.top().first;
        index = st.top().second;

        st.pop();

        B_Node * newRight = new B_Node(node->isLeaf, endPos);
        endPos += sizeNode;


        for (int i = 0; i < t - half; i++) {
            newRight->Keys[i] = node->Keys[i + half];
        }

        if (!node->isLeaf) {
            for (int i = 0; i < t - half; i++) {
                newRight->Childs[i] = node->Childs[i + half + 1];
            }
        } else {
            for (int i = 0; i < t - half; i++) {
                newRight->Childs[i] = node->Childs[i + half];
            }
        }
        newRight->n = t - half;
        node->n = half;

        if (st.empty()) {
            B_Node * newRoot = new B_Node(false, endPos);

            newRoot->n = 1;
            newRoot->Keys[0] = newRight->Keys[0];
            newRoot->Childs[0] = node->selfPlace;
            newRoot->Childs[1] = newRight->selfPlace;

            //поставити root на 0-ву позицію
            swap(newRoot->selfPlace, node->selfPlace);
            auto it = find(newRoot->Childs.begin(), newRoot->Childs.begin() + t + 1, newRoot->selfPlace);
            *it = node->selfPlace;

            _write(Tree, newRoot, newRoot->selfPlace);
        } else {
            B_Node * temp = st.top().first;

            temp->Keys.insert(temp->Keys.begin() + index, newRight->Keys[0]);
            temp->Childs.insert(temp->Childs.begin() + index + 1, newRight->selfPlace);
            temp->n++;

            _write(Tree, temp, temp->selfPlace);
        }

        if (!node->isLeaf) {
            newRight->Keys.erase(newRight->Keys.begin());
            newRight->Keys.push_back(0);
            newRight->n--;
        }

        _write(Tree, node,     node->selfPlace);
        _write(Tree, newRight, newRight->selfPlace);
    }
}

template<size_t t, typename D>
void B_Tree<t, D>::_write(fstream &file, B_Tree::B_Node *node, int pos) {
    file.seekg(pos);
    //***********************************************************************************************
    //change to int
    file.write((char *)&node->isLeaf,    sizeof(int));//bool
    file.write((char *)&node->n,         sizeof(int));
    file.write((char *)&node->Keys[0],   t * sizeof(int));
    file.write((char *)&node->Childs[0], (t + 1) * sizeof(int));
    file.write((char *)&node->selfPlace, sizeof(int));
    file.write((char *)&node->isExist,   sizeof(int));//bool
}


template<size_t t, typename D>
void B_Tree<t, D>::_read(fstream &file, B_Tree::B_Node *node, int pos) {
    file.seekg(pos);
    //***********************************************************************************************
    //change to int
    file.read((char *)&node->isLeaf,    sizeof(int));//bool
    file.read((char *)&node->n,         sizeof(int));
    file.read((char *)&node->Keys[0],   t * sizeof(int));
    file.read((char *)&node->Childs[0], (t + 1) * sizeof(int));
    file.read((char *)&node->selfPlace, sizeof(int));
    file.read((char *)&node->isExist,   sizeof(int));//bool
}


template<size_t t, typename D>
void B_Tree<t, D>::Print() {
    Tree.seekg(0, ios::beg);


    auto * temp = new B_Node(false, 0);
    while (true) {

        if (!(Tree.read((char *)&temp->isLeaf,    sizeof(int)))) {
            break;
        }
        Tree.read((char *)&temp->n,         sizeof(int));
        Tree.read((char *)&temp->Keys[0],   t * sizeof(int));
        Tree.read((char *)&temp->Childs[0], (t + 1) * sizeof(int));
        Tree.read((char *)&temp->selfPlace, sizeof(int));
        Tree.read((char *)&temp->isExist, sizeof(int));

        cout << "isLeaf:    " << (temp->isLeaf?"true":"false") << endl;
        cout << "n:         " << temp->n <<endl;
        cout << "Keys:      ";
        for (int i = 0; i < temp->n; i++)
            cout << temp->Keys[i] << ", ";

        cout << endl;

        cout << "Childs:    ";
        for (int i = 0; i < temp->n; i++)
            cout << temp->Childs[i] << ", ";

        if(!temp->isLeaf) {
            cout << temp->Childs[temp->n];
        }

        cout << endl;
        cout << "selfPlace: " << temp->selfPlace << endl;
        cout << "isExist:   " << temp->isExist << endl;

        cout << "----------------------------------------------------------------------------------------------------\n";
    }

    cout << "****************************************************************************************************\n";
    cout << "----------------------------------------------------------------------------------------------------\n";
    D *   nTemp = new D;
    int * isE   = new int;
    int  pos    = 0;
    Data.seekg(0, ios::beg);
    while (true) {
        if (!(Data.read((char *)nTemp, sizeof(D)))) {
            break;
        }

        Data.read((char *)nTemp, sizeof(D));
        Data.read((char *)isE, sizeof(int));

        if(!(*isE)) {
            pos += sizeof(D);
            continue;
        }

        cout << "Data:      " << *nTemp << endl;
        cout << "Position:  " << pos << endl;
        cout << "----------------------------------------------------------------------------------------------------\n";
        pos += sizeof(D);
    }
    delete nTemp;
    delete isE;
    delete temp;
}


template<size_t t, typename D>
void B_Tree<t, D>::Delete(int key) {
    B_Node * x = new B_Node(false, 0);

    _read(Tree, x, 0);//читає з позиції 0

    stack<pair<B_Node*, int>> st;
    st.push({x, 0});

    int i;
    while (!x->isLeaf) {
        i = 0;
        while (i < x->n && key >= x->Keys[i]) {
            i++;
        }

        auto * temp = new B_Node(false, 0);
        _read(Tree, temp, x->Childs[i]);

        x = temp;

        st.push({x, i});
    }
    int xIndex = st.top().second;
    st.pop();

    i = 0;
    while (i < x->n && x->Keys[i] != key) {
        i++;
    }

    if (x->Keys[i] == key) {
        D *   nTemp = new D;
        int * isE   = new int;

        Data.seekg(x->Childs[i]);
        Data.read((char *)nTemp, sizeof(D));
        Data.read((char *)isE, sizeof(int));

        *isE = 0;

        Data.seekg(x->Childs[i]);
        Data.write((char *)nTemp, sizeof(D));
        Data.write((char *)isE, sizeof(int));

        //deletion from leaf
        x->Keys.erase(x->Keys.begin() + i);
        x->Childs.erase(x->Childs.begin() + i);
        x->n--;

        if (x->n < t / 2) {
            if (!st.empty()) {
                B_Node * parent = st.top().first;
                B_Node * xSibling = new B_Node(false, 0);
                /*  if (xIndex == t) {
                      centr = t - 2;
                      _read(Tree,r,p->Childs[xIndex - 1]);
                      swap(x,r);
                      r->n--;
                      x->Keys[x->n] = r->Keys[r->n];
                      x->Childs[x->n] = r->Childs[r->n];
                      r->Keys.erase(r->Keys.begin() + r->n);
                      r->Childs.erase(r->Childs.begin() + r->n + 1);
                      x->n++;


                  } else {
                      centr = xIndex;
                      _read(Tree,r,p->Childs[xIndex + 1]);
                  }*/

                if (xIndex - 1 > -1) {
                    _read(Tree,xSibling,parent->Childs[xIndex - 1]);

                    if (xSibling->n - 1 < t / 2) {
                        //злиття
                        for (int j = 0; j < x->n; j++) {
                            xSibling->Keys[j + xSibling->n]   = x->Keys[j];
                            xSibling->Childs[j + xSibling->n] = x->Childs[j];
                        }
                        parent->Keys.erase(parent->Keys.begin() + xIndex - 1);
                        parent->Childs.erase(parent->Childs.begin() + xIndex);
                        parent->n--;
                        x->isExist = false;
                    } else {
                        xSibling->n--;
                        x->Keys.insert(x->Keys.begin(), xSibling->Keys[xSibling->n]);
                        x->Childs.insert(x->Childs.begin(), xSibling->Childs[xSibling->n]);
                        x->n++;

                        parent->Keys[xIndex - 1] = x->Keys[0];
                    }
                } else {
                    _read(Tree,xSibling,parent->Childs[xIndex + 1]);

                    if (xSibling->n - 1 < t / 2) {
                        //злиття
                        for (int j = 0; j < xSibling->n; j++) {
                            x->Keys[j + x->n]   = xSibling->Keys[j];
                            x->Childs[j + x->n] = xSibling->Childs[j];
                        }
                        parent->Keys.erase(parent->Keys.begin() + xIndex);
                        parent->Childs.erase(parent->Childs.begin() + xIndex + 1);
                        parent->n--;
                        xSibling->isExist = false;
                    } else {
                        xSibling->n--;
                        x->Keys[x->n]   = xSibling->Keys[xSibling->n];
                        x->Childs[x->n] = xSibling->Childs[xSibling->n];
                        x->n++;

                        xSibling->Keys.erase(xSibling->Keys.begin());
                        xSibling->Childs.erase(xSibling->Childs.begin());

                        parent->Keys[xIndex] = xSibling->Keys[0];
                    }
                }
                _write(Tree,parent,parent->selfPlace);
                _write(Tree, xSibling, xSibling->selfPlace);
                _write(Tree,x,x->selfPlace);
            }
        } else {
            if (!st.empty()) {
                B_Node * parent = st.top().first;
                if (xIndex != 0 && parent->Keys[xIndex - 1] == key) {
                    parent->Keys[xIndex - 1] = x->Keys[0];
                    _write(Tree,parent,parent->selfPlace);
                }
            }
        }
        //_megre(st);
    } else {
        cout << "key not found\n";
    }
}


template<size_t t, typename D>
void B_Tree<t, D>::_merge(stack<pair<B_Node *, int>> &st) {

    while (!st.empty() && st.top().first->n < t / 2) {

    }
}

template <size_t t, typename D>
int B_Tree<t, D>::Search(int key) {
    int count = 0;
    B_Node * nod = new B_Node(false, 0);

    _read(Tree, nod, 0);//читає з позиції 0

    int i;
    while (!nod->isLeaf) {
        i = 0;
        while (i < nod->n && key >= nod->Keys[i]) {
            i++;
        }

        auto * temp = new B_Node(false, 0);
        _read(Tree, temp, nod->Childs[i]);

        nod = temp;
        count++;
    }
    i = 0;
    while (i < nod->n && nod->Keys[i] != key) {
        i++;
    }
    cout << "height: " << count << "\t";
    if (nod->Keys[i] == key) {
        return nod->Childs[i];
    } else {
        return -1;
    }
}



//template<size_t t, typename D>
//void B_Tree<t, D>::_merge(stack<pair<B_Node *, int>> &st) {
//    B_Node * curr = st.top().first;
//
//    if (!st.empty()) {
//        int centr;
//        B_Node * p = st.top().first;
//        B_Node * r = new B_Node(false,0);
//        if (xIndex == t) {
//            centr = t - 2;
//            _read(Tree,r,p->Childs[xIndex - 1]);
//            swap(x,r);
//        } else {
//            centr = xIndex;
//            _read(Tree,r,p->Childs[xIndex + 1]);
//        }
//
//        if (x->n + r->n < t) {
//            for (int j = 0; j < r->n; j++) {
//                x->Keys[j + x->n] = r->Keys[j];
//                x->Childs[j + x->n] = r->Childs[j];
//            }
//            x->n += r->n;
//            r->isExist = 0;
//            p->Keys.erase(p->Keys.begin() + centr);
//            p->Childs.erase(p->Childs.begin() + centr + 1);
//        }
//        _write(Tree,r,r->selfPlace);
//    }
//}


#endif //B__TREE_B_TREE_H
