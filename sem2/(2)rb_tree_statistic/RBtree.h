//
// Created by Bohdan on 09.02.2021.
//

#ifndef RB_TREE_RBTREE_H
#define RB_TREE_RBTREE_H


namespace my_rbtree {
    template <typename K, typename   D>
    class RBtree {
    public:
        RBtree() : NIL(new node), root(NIL) {}

        void insert (const K & key, const D & data);
        void delet (const K & key);
        const D & search (const K & z) const;
        const D & os_select(int i) const;

    private:
        enum _color {red, black};

        class node {
        public:
            const K & key;
            const D & data;
            _color color;
            node * left, *right, *p;
            int size;

            node(const K & _key = K(), const D & _data = D(), int rank = 0) :
                    key(_key), data(_data), color(black), left(nullptr), right(nullptr), p(nullptr),size(rank) {}
        };
        node * NIL;
        node * root;

        node * _find(const K & key) const;
        node * tree_minimum(node * nd) const;
        void transplant (node * u, node * v);
        void left_rotate (node * x);
        void right_rotate (node * X);
        void insert_fixup (node * z);
        void rb_delete_fixup(node * x);
    };


    //private methods
    template<typename K, typename D>
    typename RBtree<K,D>::node * RBtree<K, D>::_find(const K & key) const {
        node * rt = this->root;
        while (rt != NIL && rt->key != key) {

            if (key < rt->key)
                rt = rt->left;
            else
                rt = rt->right;
        }

        return rt;
    }


    template <typename K, typename D>
    void RBtree<K,D>::transplant (node * u, node * v) {
        if (u->p == NIL) this->root = v;
        else if (u == u->p->left) u->p->left = v;
        else u->p->right = v;

        if (v) v->p = u->p;
    }


    template <typename K, typename D>
    void RBtree<K,D>::left_rotate (node * x) {
        node * y = x->right;
        x->right = y->left;

        if (y->left != NIL) {
            y->left->p = x;
        }
        y->p = x->p;

        if (x->p == NIL) {
            this->root = y;
        }
        else if (x == x->p->left) {
            x->p->left = y;
        }
        else {
            x->p->right = y;
        }

        y->left = x;

        x->p = y;

        y->size = x->size;
        x->size = x->left->size + x->right->size + 1;
    }


    template <typename K, typename D>
    void RBtree<K,D>::right_rotate (node * X) {
        node * Y = X->left;
        X->left = Y->right;

        if (Y->right != NIL) {
            Y->right->p = X;
        }
        Y->p = X->p;

        if (X->p == NIL) {
            this->root = Y;
        }
        else if (X == X->p->left) {
            X->p->left = Y;
        }
        else {
            X->p->right = Y;
        }
        Y->right = X;

        X->p = Y;

        Y->size = X->size;
        X->size = X->left->size + X->right->size + 1;
    }


    template <typename K, typename D>
    void RBtree<K,D>::insert_fixup (node * z) {
        node * y;
        while (z->p->color == red) {
            if (z->p == z->p->p->left) {
                y = z->p->p->right;
                if (y->color == red) {
                    z->p->color = black;
                    y->color = black;
                    z->p->p->color = red;
                    z = z->p->p;
                }
                else {
                    if (z == z->p->right) {
                        z = z->p;
                        left_rotate(z);
                    }
                    z->p->color = black;
                    z->p->p->color = red;
                    right_rotate(z->p->p);
                }
            }
            else {
                y = z->p->p->left;
                if (y->color == red) {
                    z->p->color = black;
                    y->color = black;
                    z->p->p->color = red;
                    z = z->p->p;
                }
                else {
                    if (z == z->p->left) {
                        z = z->p;
                        right_rotate(z);
                    }
                    z->p->color = black;
                    z->p->p->color = red;
                    left_rotate(z->p->p);
                }
            }
        }
        this->root->color = black;
    }


    template <typename K, typename D>
    void RBtree<K,D>::rb_delete_fixup(node * x) {
        node *w;
        while (x != root && x->color == black) {
            if (x == x->p->left) {
                w = x->p->right;
                if (w->color == red) {
                    w->color = black;
                    x->p->color = red;
                    left_rotate(x->p);
                    w = x->p->right;
                }
                if (w->left->color == black && w->right->color == black) {
                    w->color = red;
                    x = x->p;
                } else {
                    if (w->right->color == black) {
                        w->left->color = black;
                        w->color = red;
                        right_rotate(w);
                        w = x->p->right;
                    }
                    w->color = x->p->color;
                    x->p->color = black;
                    w->right->color = black;
                    left_rotate(x->p);
                    x = root;
                }
            } else {
                w = x->p->left;
                if (w->color == red) {
                    w->color = black;
                    x->p->color = red;
                    right_rotate(x->p);
                    w = x->p->left;
                }
                if (w->left->color == black && w->right->color == black) {
                    w->color = red;
                    x = x->p;
                } else {
                    if (w->left->color == black) {
                        w->right->color = black;
                        w->color = red;
                        left_rotate(w);
                        w = x->p->left;
                    }
                    w->color = x->p->color;
                    x->p->color = black;
                    w->left->color = black;
                    right_rotate(x->p);
                    x = root;
                }
            }
        }
        x->color = black;
    }


    template<typename K, typename D>
    typename RBtree<K, D>::node * RBtree<K, D>::tree_minimum(node * nd) const {
        while (nd->left != NIL)
            nd = nd->left;

        return nd;
    }

    //public methods

    template<typename K, typename D>
    void RBtree<K, D>::insert(const K & key, const D& data) {
        node * in = new node(key, data, 1);
        node * temp = root, *pre_temp = NIL;

        while (temp != NIL) {
            pre_temp = temp;

            temp->size++;
            if (in->key < temp->key) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }
        }
        in->p = pre_temp;

        if (pre_temp == NIL) {
            this->root = in;
        }
        else if (in->key < pre_temp->key) {
            pre_temp->left = in;
        }
        else {
            pre_temp->right = in;
        }
        in->left = in->right = NIL;
        in->color = red;
        insert_fixup(in);
    }


    template<typename K, typename D>
    void RBtree<K, D>::delet(const K & key) {
        node * z = _find(key);
        node *y = z,*x;
        _color color_y = y->color;


        if(z->left == NIL) {
            x = z;
            while (x != NIL) {
                x->size--;
                x = x->p;
            }
            x = z->right;
            transplant(z,z->right);
        }
        else if(z->right == NIL) {
            x = z;
            while (x != NIL) {
                x->size--;
                x = x->p;
            }
            x = z->left;
            transplant(z,z->left);
        }
        else {
            x = y = tree_minimum(z->right);
            while (x != NIL) {
                x->size--;
                x = x->p;
            }
            color_y = y->color;
            x = y->right;
            if(y->p == z) {
                x->p = y;
            }
            else {
                transplant(y,y->right);
                y->right = z->right;
                y->right->p = y;
            }
            transplant(z,y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;
            y->size += y->left->size + 1;
        }
        if(color_y == black)
        {
            rb_delete_fixup(x);
        }
    }


    template<typename K, typename D>
    const D & RBtree<K, D>::search(const K & z) const {
        return _find(z)->data;
    }


    template<typename K, typename D>
    const D & RBtree<K, D>::os_select(int i) const {
        node * temp = root;

        while (temp != NIL) {
            int r = temp->left->size + 1;
            if (i == r) {
                return temp->data;
            }
            else if (i < r) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
                i = i - r;
            }
        }
        return D();
    }
}
#endif //RB_TREE_RBTREE_H
