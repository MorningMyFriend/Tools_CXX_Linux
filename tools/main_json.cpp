//
// Created by wurui on 18-9-12.
//

#include <iostream>
#include <map>

#include "json/json.h"

using namespace std;

int main(){
    map<string, int> map1 = {pair<string, int>("a",1),pair<string, int>("b",2),};
    Json::Value jarray;
    for (auto itr=map1.begin(); itr!=map1.end();++itr) {
        Json::Value item;
        item[itr->first] = std::to_string(itr->second);
        jarray.append(item);
    }
    string jstr = jarray.toStyledString();
    cout << jstr << endl;
}


