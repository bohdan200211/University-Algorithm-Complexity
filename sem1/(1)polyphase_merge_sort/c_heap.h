//
// Created by Bohdan on 07.09.2020.
//

#ifndef U_POLYPHASE_MERGE_SORT_C_HEAP_H
#define U_POLYPHASE_MERGE_SORT_C_HEAP_H
#include <utility>
#include <cstdio>
#include <cstdint>
const int nt_size = sizeof(int);
using namespace std;
class c_heap
{
private:
    struct abc {
        int val;
        FILE * f;
        int iter;
    };
    int capasity;
    int size;
    abc * heap;

    int parent(int i) {return (i - 1)/2;}
    int left(int i) {return i*2 + 1;}
    int right(int i) {return i*2 + 2;}

    void heapify(int index) {
        int r = right(index);
        int l = left(index);

        int smallest = index;

        if (l < size && heap[l].val < heap[smallest].val)
            smallest = l;

        if (r < size && heap[r].val < heap[smallest].val)
            smallest = r;

        if (smallest != index) {
            swap(heap[smallest], heap[index]);
            heapify(smallest);
        }
    }
    template<class type>
    void swap(type & a, type & b) {
        type t = a;
        a = b;
        b = t;
    }
public:
    c_heap(int n_size) {
        capasity = n_size;
        size = 0;
        heap = new abc[n_size];
    }
    void set_size_zero () {
        size = 0;
        return;
    }
    bool is_empty() {
        return heap[0].val == INT32_MAX;
    }
    int get_min() {
        int root = heap[0].val;
        if(!fread(&heap[0].val,nt_size,1,heap[0].f) || root > heap[0].val) {
            if(root > heap[0].val)
                fseek(heap[0].f,-nt_size,SEEK_CUR);
            heap[0].val = INT32_MAX;
        }
        heapify(0);
        return root;
    }
    void insert(int x,FILE * ptr, int y) {
        size++;
        int index = size-1;
        if (x == INT32_MAX) {
            heap[index] = {INT32_MAX,ptr,0};
            return;
        } else {
            heap[index] = {x,ptr,y};
        }
        while (index != 0)
        {
            int p = parent(index);
            if(heap[p].val <= heap[index].val) break;

            swap(heap[p],heap[index]);
            index = p;
        }
    }
};
#endif //U_POLYPHASE_MERGE_SORT_C_HEAP_H
