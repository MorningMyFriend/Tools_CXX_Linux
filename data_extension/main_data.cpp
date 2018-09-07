//
// Created by wurui on 18-9-5.
//

#include <iostream>
#include "basic_data_type_op.h"

using namespace std;
using namespace wrz;

void test_container() {
    vector<string> v1 = {"a", "b", "c", "b"};
    int num = ContainCount(v1, "b");
    cout << num << endl;

    vector<pair<int, string>> v2;
    v2.emplace_back(1, "a");
    v2.emplace_back(2, "a");
    v2.emplace_back(1, "b");

    int num_a = ContainCount(v2, pair<int, string>(0, "a"),
                             [](const pair<int, string> &p1, const pair<int, string> &p2) {
                                 return p1.second == p2.second;
                             });
    cout<<num_a<<endl;

    num = ContainCount(v1, "c");
    cout << num << endl;
}

int main() {
    test_container();
}