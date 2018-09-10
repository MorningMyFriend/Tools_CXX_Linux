//
// Created by wurui on 18-9-5.
//

#include <iostream>
#include "union.hpp"
#include "lcs.h"

using namespace std;
using namespace wrz;

void union_test() {
    vector<int> tree = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    Union<int> anUnion(tree);
    anUnion.QuickUnion(0, 2);
    anUnion.QuickUnion(1, 4);
    anUnion.QuickUnion(1, 5);
    anUnion.QuickUnion(3, 7);
    anUnion.QuickUnion(3, 0);
    anUnion.QuickUnion(0, 1);
    anUnion.QuickUnion(6, 8);
    anUnion.QuickUnion(8, 10);
    anUnion.QuickUnion(8, 0);
    anUnion.QuickUnion(9, 11);
    anUnion.QuickUnion(12, 11);
    anUnion.QuickUnion(12, 0);
    cout << "union done!" << endl;
    anUnion.DisplayPath(3);

}

void test_lcs(){
    string s1 = "ABCBDAB";
    string s2 = "BDCABA";
    string result = LCS(s1, s2);
    cout << result << endl;
}

int main() {
//    union_test();
    test_lcs();
}
