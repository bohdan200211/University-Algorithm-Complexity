//
// Created by Bohdan on 09.02.2021.
//

#ifndef RB_TREE_RBTREE_H
#define RB_TREE_RBTREE_H

#include <vector>

namespace my_rbtree {
    template <typename K, typename D>
    class RBtree {
    public:
        RBtree() : NIL(new node), root(NIL), previous_roots({}), _indexRoot(-1) {}

        void insert (const K key, const D data);
        void delet (const K & key);
        const D & search (const K & z) const;

        //print
        void print_bts () {
            _printST(root);
        }

        //for persist tree
        void getBackRoot () {
            if (previous_roots.size() != 0 && _indexRoot != 0) {
                if (_indexRoot == -1) {
                    previous_roots.push_back(root);
                    _indexRoot = previous_roots.size() - 2;
                } else {
                    _indexRoot--;
                }
                root = previous_roots[_indexRoot];
                restoreParents(root, NIL);
            } else {
                std::cout << "can't get a back root" << std::endl;
            }
        }
        void getforwardRoot () {
            if (_indexRoot > -1 && _indexRoot < previous_roots.size() - 1) {
                _indexRoot++;
                root = previous_roots[_indexRoot];
                restoreParents(root, NIL);
            } else {
                std::cout << "can't get a forward root" << std::endl;
            }
        }

        //TODO прибрати
        void test_funct() {
            /*for (auto ptr : previous_roots) {
                _printST(ptr);
                std::cout << std::endl;
            }*/
            _printST(root);
        }


    private:
        int _indexRoot;
        enum _color {red, black};

        class node {
        public:
            const K key;
            const D data;
            _color color;
            node * left, *right, *p;

            node(const K & _key = K(), const D & _data = D(),_color color = black, node *left = nullptr,
                 node *right = nullptr, node *p = nullptr) :
                    key(_key), data(_data), color(color), left(left), right(right), p(p) {}
        };

        //member-variables
        node * NIL;
        node * root;
        std::vector<node*> previous_roots;

        //methods
        node * _find(const K & key) const;
        void transplant (node * u, node * v);
        void left_rotate (node * x);
        void right_rotate (node * x);
        void insert_fixup (node * z);
        void rb_delete_fixup(node * x);

        //for persist tree
        node * fullCopyNode (node * prototype) {
            if (prototype == NIL) {
                return prototype;
            }
            node * newNode = new node(prototype->key,prototype->data,prototype->color,prototype->left,
                                      prototype->right, prototype->p);

            if (prototype->right && prototype->right != NIL)
                prototype->right->p = newNode;
            if (prototype->left && prototype->left != NIL)
                prototype->left->p = newNode;

            return newNode;
        }
        node * createPersistPath (node * prevTree_temp, node * prev_temp, node * temp, const K & key) {
            temp->p = prev_temp;
            while (prevTree_temp != NIL && prevTree_temp->key != key) {
                prev_temp = temp;

                if (key < prevTree_temp->key) {
                    prevTree_temp = prevTree_temp->left;
                    temp->left = fullCopyNode(prevTree_temp);
                    temp = temp->left;
                }
                else {
                    prevTree_temp = prevTree_temp->right;
                    temp->right = fullCopyNode(prevTree_temp);
                    temp = temp->right;
                }
            }

            return prev_temp;
        }
        void restoreParents (node * currentNode, node * parentNode) {
            if (currentNode != NIL) {
                currentNode->p = parentNode;
                restoreParents(currentNode->left, currentNode);
                restoreParents(currentNode->right, currentNode);
            }
        }


        //for print
        void _printST(node * _r, const std::string& prefix = "", bool isLeft = false) {
            if(_r != nullptr && _r != NIL) {
                std::cout << prefix;

                std::cout << (isLeft ? "├──" : "└──" );

                std::cout << _r->key  << " : "
                          << _r->data << " : "
                          << (_r->color == red ? "red" : "black")
                          << std::endl; // custom data output

                _printST(_r->left,prefix + (isLeft ? "│   " : "    "), true);
                _printST(_r->right, prefix + (isLeft ? "│   " : "    "),false);
            }
        }
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
            root = y;
        }
        else if (x == x->p->left) {
            x->p->left = y;
        }
        else {
            x->p->right = y;
        }

        y->left = x;

        x->p = y;
    }


    template <typename K, typename D>
    void RBtree<K,D>::right_rotate (node * x) {
        node * y = x->left;
        x->left = y->right;

        if (y->right != NIL) {
            y->right->p = x;
        }
        y->p = x->p;

        if (x->p == NIL) {
            root = y;
        }
        else if (x == x->p->left) {
            x->p->left = y;
        }
        else {
            x->p->right = y;
        }
        y->right = x;

        x->p = y;
    }


    template <typename K, typename D>
    void RBtree<K,D>::insert_fixup (node * z) {
        node * y;
        while (z->p->color == red) {
            if (z->p == z->p->p->left) {
                y = z->p->p->right;
                if (y->color == red) {
                    //y завжди не копійована нода//ліва...
                    node * newY = fullCopyNode(y);
                    z->p->p->right = newY;
                    newY->color = z->p->color = black;
                    z->p->p->color = red;
                    z = z->p->p;

                    /*z->p->color = black;
                    y->color = black;
                    z->p->p->color = red;
                    z = z->p->p;*/
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
                    //y завжди не копійована нода//права
                    node * newY = fullCopyNode(y);
                    z->p->p->left = newY;
                    newY->color = z->p->color = black;
                    z->p->p->color = red;
                    z = z->p->p;


                    /*z->p->color = black;
                    y->color = black;
                    z->p->p->color = red;
                    z = z->p->p;*/
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
        root->color = black;
    }


    template <typename K, typename D>
    void RBtree<K,D>::rb_delete_fixup(node * x) {
        node *w;
        while (x != root && x->color == black) {
            if (x == x->p->left) {
                //w = x->p->right;
                x->p->right = w = fullCopyNode(x->p->right);

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
                        w->left = fullCopyNode(w->left);

                        w->left->color = black;
                        w->color = red;
                        right_rotate(w);
                        w = x->p->right;
                    }
                    w->right = fullCopyNode(w->right);

                    w->color = x->p->color;
                    x->p->color = black;
                    w->right->color = black;
                    left_rotate(x->p);
                    x = root;
                }
            } else {
                //w = x->p->left;
                x->p->left = w = fullCopyNode(x->p->left);

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
                        w->right = fullCopyNode(w->right);

                        w->right->color = black;
                        w->color = red;
                        left_rotate(w);
                        w = x->p->left;
                    }
                    w->left = fullCopyNode(w->left);

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


    //public methods
    template<typename K, typename D>
    void RBtree<K, D>::insert(const K key, const D data) {
        if (_indexRoot != -1) {
            //memory leak!!!
            previous_roots.resize(_indexRoot+1);
        }
        _indexRoot = -1;
        previous_roots.push_back(root);
        root = fullCopyNode(root);

        node *in        = new node(key, data),
             *prev_temp = createPersistPath(previous_roots.back(), NIL, root, in->key);

        in->p = prev_temp;

        if (prev_temp == NIL) {
            root = in;
        }
        else if (in->key < prev_temp->key) {
            prev_temp->left = in;
        }
        else {
            prev_temp->right = in;
        }

        in->left  = in->right = NIL;
        in->color = red;

        insert_fixup(in);
    }


    template<typename K, typename D>
    void RBtree<K, D>::delet(const K & key) {
        if (_indexRoot != -1) {
            //memory leak!!!
            previous_roots.resize(_indexRoot+1);
        }
        _indexRoot = -1;
        previous_roots.push_back(root);
        root = fullCopyNode(root);


        node *prevTree_temp, *temp,
             *prev_temp = createPersistPath(previous_roots.back(), NIL, root,key);

        prev_temp = (prev_temp == NIL ? root : (key < prev_temp->key ? prev_temp->left : prev_temp->right));


        node * z = prev_temp;
        node *y = z,*x;
        _color color_y = y->color;


        if(z->left == NIL) {
            z->right = x = fullCopyNode(z->right);
            transplant(z,z->right);
        }
        else if(z->right == NIL) {
            z->left = x = fullCopyNode(z->left);
            transplant(z,z->left);
        }
        else {
            prevTree_temp = z->right;
            prev_temp     = z;
            temp          = fullCopyNode(z->right);

            z->right = temp;

            temp->p = prev_temp;
            while (prevTree_temp != NIL) {
                prev_temp = temp;

                prevTree_temp = prevTree_temp->left;
                temp->left = fullCopyNode(prevTree_temp);
                temp = temp->left;
            }


            //y = tree_minimum(z->right);
            y = prev_temp;
            x = y->right;

            color_y = y->color;
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
}
#endif //RB_TREE_RBTREE_H
