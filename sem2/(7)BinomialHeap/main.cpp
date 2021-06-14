#include "BinomialHeap.hpp"


int main() {
    /*auto lst1 = new BinomialList<int, char>(6,'b',5);
    lst1->AddToStart(6,'b',3);
    lst1->AddToStart(6,'b',2);
    lst1->AddToStart(6,'b',1);
    lst1->AddToStart(6,'b',0);


    auto lst2 = new BinomialList<int, char>(15,'b',4);
    lst2->AddToStart(15,'b',2);
    lst2->AddToStart(15,'b',1);



    BinomialList<int,char> * res = MergeList(lst1, lst2);

    lst2->AddToStart(15,'b',1);*/

    BinomialHeap<int, char> H;
    for (int i = 1; i <= 16; i++) {
        H.Insert(i, char('a' + i % 26));
    }


//    test union
//    test: delete & decreaseKey
    H.PrintHeap();
    for (int i = 1; i <= 12; i++) {
        H.Delete(i);
        H.PrintHeap();
    }


//    test: insert & minimum & extractminimum
//    int prev = 0;
//    int successor;
//    while (!H.isEmpty()) {
//        successor = H.Minimum();
//
//        cout << /*prev - */successor << endl;
//        /*prev = successor;*/
//        H.ExtractMinimum();
//    }
    H.PrintHeap();

    return 0;
}
