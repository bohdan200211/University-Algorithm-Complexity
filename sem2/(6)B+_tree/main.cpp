#include "B_Tree.h"
#include <cmath>

int main () {
    B_Tree<5, int> BTree("test", true);

    for (int i = 1; i <= 10; i++) {
        BTree.Insert(i, new int(pow(i,2)));
    }
    cout << BTree.Search(7) << endl;
    BTree.Delete(5);
    cout << BTree.Search(5) << endl;
//    for (int i = 4; i <= 12; i++) {
//        cout << BTree.Search(i) << endl;
//    }
//    BTree.Insert(1,new int(3));
//    BTree.Insert(2,new int(4));
//    BTree.Insert(3,new int(14));
//    BTree.Insert(4,new int(16));
//    BTree.Insert(5,new int(32));


//    fstream binFile;
//    binFile.open(".dat", ios::binary | ios::in | ios::out);
//
//    if(!binFile.is_open()) {
//        cerr << "file not open";
//        return 0;
//    }
//
//    binFile.seekg(0,ios::beg);
//
//    int * con = new int;
//
//    while (binFile.read((char *)con, 1)) {
//        cout << " " << *con;
//    }
//
//
//
//    binFile.close();


//    for (int i = 1; i <= 5; i++) {
//        A.push_back(i);
//    }
//
//    binFile.write((char *)&A[0], A.size() * sizeof(int));

//    for (int i = 1; i < 6; i++) {
//        binFile.write((char*)&i, 1);
//    }

    return 0;
}
