//
// Created by wurui on 18-9-5.
//

#include <iostream>
#include "utf8_string.hpp"

using namespace std;
using namespace wrz;

void test_utf8(){
    string s1 = "天a8@A$B\t你\n?？.。d";
    cout << "string: " << s1 << endl;
    cout << "len: " << utf8_len(s1) << endl;   // 15
    cout << "at(8): " << utf8_at(s1, 8) << endl; // 你
    cout << "at(8): " << utf8_find_first_of(s1, "。") << endl; // 13
    cout << "substr(6, 5): " << utf8_substr(s1, 6, 5) << endl;
}

int main(){
    test_utf8();
}