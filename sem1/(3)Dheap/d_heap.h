//
// Created by Bohdan on 03.10.2020.
//

#ifndef U_DHEAP_D_HEAP_H
#define U_DHEAP_D_HEAP_H
// i = (1 -> d)
template <class type>
class d_heap {
private:
    int d;
    int capacity;
    int size;
    type * heap;

    int parent(int i) {
        return (i - 1) / d;
    }
    int i_fst_son (int i) {
        return i * d + 1;
    }
    void swap(type & a, type & b);
    void heapify (int index);
public:
    d_heap(int cap, int d);
    type extract_max ();
    void insert (type x);
    void increase (int i, type n_key);
};

template<class type>
void d_heap<type>::swap(type & a, type & b) {
    type t = a;
    a = b;
    b = t;
}

template<class type>
d_heap<type>::d_heap(int cap, int _d) {
    d = _d;
    capacity = cap;
    size = 0;
    heap = new type[capacity];
}

template<class type>
void d_heap<type>::heapify(int index) {
    int temp = i_fst_son(index);

    int max = index;

    while (temp < size) {
        if (heap[max] < heap[temp])
            max = temp;
        temp++;
    }

    if (max != index) {
        swap(heap[max], heap[index]);
        heapify(max);
    }
}

template<class type>
void d_heap<type>::insert(type x) {
    int index = size;
    heap[index] = x;

    while (index != 0 && heap[parent(index)] < heap[index]) {
        swap(heap[parent(index)], heap[index]);
        index = parent(index);
    }
    size++;
}

template<class type>
void d_heap<type>::increase(int index, type n_key) {
    heap[index] = n_key;

    while (index != 0 && heap[parent(index)] < heap[index]) {
        swap(heap[parent(index)], heap[index]);
        index = parent(index);
    }
}

template<class type>
type d_heap<type>::extract_max() {
    if(size == 1)
    {
        size--;
        return heap[0];
    }
    type root = heap[0];
    heap[0] = heap[size-1];
    size--;
    heapify(0);
    return root;
}

#endif //U_DHEAP_D_HEAP_H
