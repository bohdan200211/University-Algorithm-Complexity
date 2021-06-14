#include <iostream>
#include <vector>
#include "RBtree.h"

using namespace my_rbtree;

int main() {
    /*std::vector<std::pair<int,std::string>> input = {
            {0,""},
            {1,""},
            {2,""},
            {3,""},
            {4,""},
            {5,""},
            {6,"six"},
            {7,"seven"},
            {8,"eight"},
            {9,"nine"}
    };*/


    RBtree<int,std::string> T;

    for (int i = 1; i <= 20; i++) {
        T.insert(i, std::to_string(i));
        //T.print_bts();
        //std::cout << std::endl;
    }
    T.print_bts();

//    T.delet(2);
//    T.print_bts();
//
//    T.getBackRoot();
//    T.print_bts();
//    T.getBackRoot();
//    T.getBackRoot();
//    T.print_bts();

    //T.test_funct();
    return 0;
}
