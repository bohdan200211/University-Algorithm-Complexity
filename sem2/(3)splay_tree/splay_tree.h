//
// Created by Bohdan on 18.02.2021.
//

#ifndef TEST_SPLAY_TREE_H
#define TEST_SPLAY_TREE_H
#include <iostream>

namespace my_splay_tree {
    template<typename K, typename D>
    class splay_tree {
    private:
        struct node {
            node *l, *r, *p;
            const D & data;
            const K & key;

            explicit node (const K & _key = K(), const D & _data = D())
                    : l(nullptr), r(nullptr), p(nullptr), data(_data), key(_key) {}

            ~node () {
                std::cout << "del:l,r"<< data << std::endl;
                delete l;
                delete r;
            }
        };

        node * root;

    private:
        void _left_rotate(node * x);

        void _right_rotate(node * x);

        void _transplant(node * u, node * v);

        void _splay(node * x);

        node * _minimum(node * x) const;

        node * _search(const K & _key);

        void _printST(node * _r, const std::string& prefix = "", bool isLeft = false) {
            if(_r != nullptr ) {
                std::cout << prefix;

                std::cout << (isLeft ? "├──" : "└──" );

                std::cout << _r->data << std::endl;

                _printST(_r->l,prefix + (isLeft ? "│   " : "    "), true);
                _printST(_r->r, prefix + (isLeft ? "│   " : "    "),false);
            }
        }

    public:
        splay_tree() : root(nullptr) {}

        virtual ~splay_tree() {
            delete root;
        }

        void insert(const K & Key, const D & Data);

        void del(const K & key);

        D search(const K & key) {
            node * temp = _search(key);
            return temp == nullptr ? D() : temp->data;
        }

        bool isEmpty() const {
            return root  == nullptr;
        }

        friend void printST(splay_tree<K,D> & t) {
            t._printST(t.root);
        }
    };

    template<typename K, typename D>
    void splay_tree<K,D>::_left_rotate(node * x) {
        node * y = x->r;

        x->r = y->l;
        if (y->l != nullptr)
            y->l->p = x;

        _transplant(x, y);

        y->l = x;
        y->l->p = y;
    }

    template<typename K, typename D>
    void splay_tree<K,D>::_right_rotate(node * x) {
        node * y = x->l;

        x->l = y->r;
        if (y->r != nullptr)
            y->r->p = x;

        _transplant(x, y);

        y->r = x;
        y->r->p = y;
    }

    template<typename K, typename D>
    void splay_tree<K,D>::_transplant(node * u, node * v) {
        if (u->p == nullptr)
            root = v;
        else if (u == u->p->l)
            u->p->l = v;
        else if (u == u->p->r)
            u->p->r = v;

        if (v != nullptr)
            v->p = u->p;
    }

    template<typename K, typename D>
    void splay_tree<K,D>::_splay(node * x) {
        while (x != root) {
            if (x->p == root) {
                if (x == x->p->l) {
                    _right_rotate(x->p);
                }
                else if (x == x->p->r) {
                    _left_rotate(x->p);
                }
            } else {
                // Zig-Zig step.
                if (x == x->p->l && x->p == x->p->p->l) {
                    _right_rotate(x->p->p);
                    _right_rotate(x->p);

                } else if (x == x->p->r && x->p == x->p->p->r) {
                    _left_rotate(x->p->p);
                    _left_rotate(x->p);
                }
                    // Zig-Zag step.
                else if (x == x->p->r && x->p == x->p->p->l) {
                    _left_rotate(x->p);
                    _right_rotate(x->p);

                } else if (x == x->p->l && x->p == x->p->p->r) {
                    _right_rotate(x->p);
                    _left_rotate(x->p);
                }
            }
        }
    }

    template<typename K, typename D>
    typename splay_tree<K,D>::node * splay_tree<K,D>::_minimum(node * x) const {
        node * temp = x;

        while (temp->l != nullptr)
            temp = temp->l;

        return temp;
    }

    template<typename K, typename D>
    typename splay_tree<K,D>::node * splay_tree<K,D>::_search(const K & Key) {
        node * temp = root;

        while (temp != nullptr) {
            if (temp->key < Key)
                temp = temp->r;
            else if (Key < temp->key)
                temp = temp->l;
            else if (temp->key == Key) {
                _splay(temp);
                std::cout << "_search :\n";
                _printST(this->root);
                return temp;
            }
        }

        return nullptr;
    }

    template<typename K, typename D>
    void splay_tree<K,D>::insert(const K & Key, const D & Data) {
        node * pre_temp = nullptr;
        node * temp = root;

        while (temp != nullptr) {
            pre_temp = temp;

            if (temp->key < Key)
                temp = temp->r;
            else if (Key <= temp->key)
                temp = temp->l;
        }

        node * insrt_elm = new node(Key, Data);
        insrt_elm->p = pre_temp;

        if (pre_temp == nullptr)
            root = insrt_elm;
        else if (pre_temp->key < insrt_elm->key)
            pre_temp->r = insrt_elm;
        else if (insrt_elm->key < pre_temp->key)
            pre_temp->l = insrt_elm;

        _splay(insrt_elm);

        std::cout << "insert :\n";
        _printST(this->root);
    }

    template<typename K, typename D>
    void splay_tree<K,D>::del(const K & Key) {
        node * temp = _search(Key);

        if (temp != nullptr) {
            if (temp->r == nullptr)
                _transplant(temp, temp->l);
            else if (temp->l == nullptr)
                _transplant(temp, temp->r);
            else {
                node * nxt_temp = _minimum(temp->r);

                if (nxt_temp->p != temp) {
                    _transplant(nxt_temp, nxt_temp->r);

                    nxt_temp->r = temp->r;
                    nxt_temp->r->p = nxt_temp;
                }

                _transplant(temp, nxt_temp);

                nxt_temp->l = temp->l;
                nxt_temp->l->p = nxt_temp;


                _splay(nxt_temp);
                std::cout << "del :\n";
                _printST(this->root);
            }

            temp->l = temp->r = temp->p = nullptr;
            delete temp;
        }
    }
}


#endif //TEST_SPLAY_TREE_H
