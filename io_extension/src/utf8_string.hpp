//
// Created by wurui on 18-9-5.
//

#ifndef TOOLS_CXX_LINUX_UTF8_STRING_H
#define TOOLS_CXX_LINUX_UTF8_STRING_H


#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

namespace wrz {

    int utf8_len(const string &str) {
        if (str.size() < 1) return 0;
        int count = 0;
        for (int i = 0; i < str.size(); ++i) {
            if ((str[i] & 0x80) == 0) {
                char english[2] = {0};
                count++;
            } else {
                char chinese[4] = {0};
                chinese[0] = str[i];
                chinese[1] = str[i + 1];
                chinese[2] = str[i + 2];
                count++;
                i++;
                i++;
            }
        }
        return count;
    }

    string utf8_at(const string &str, int id) {
        string output = "";
        if (str.size() < 1) {
            return output;
        } else if (id < 0) {
            return output;
        }

        int count = -1;
        for (int i = 0; i < str.size(); ++i) {
            string result;
            if ((str[i] & 0x80) == 0) {
                char english[2] = {0};
                english[0] = str[i];
                result = english;
                count++;
            } else {
                char chinese[4] = {0};
                chinese[0] = str[i];
                chinese[1] = str[i + 1];
                chinese[2] = str[i + 2];
                result = chinese;
                count++;
                i++;
                i++;
            }
            if (count == id) {
                return result;
            }
        }
    }

    string utf8_substr(const string &str, int start, int len) {
        int maxlen = utf8_len(str);
        if (start + len > maxlen || len < 0) {
            return "";
        }

        string result = "";
        for (int i = start; i < len + start; ++i) {
            result += utf8_at(str, i);
        }
        return result;
    }

    int utf8_find_first_of(const string &str, const string &target) {
        int index = -1;
        if (str.size() < 1) return -1;

        for (int i = 0; i < utf8_len(str); ++i) {
            if (utf8_at(str, i) == target) {
                index++;
                return index;
            }
            index++;
        }
        return -1;
    }

    void splitString(const string &str, const vector<string> &targets, vector<string> &result) {
        // support chinese and english chars mixed
        if (!result.empty())
            result.empty();

        if (str.size() < 1)
            return;

        if (targets.size() < 1) {
            result.emplace_back(str);
            return;
        }

        string temp = "";
        for (int i = 0; i < utf8_len(str); ++i) {
            string str_i = utf8_at(str, i);
            auto itr = std::find(targets.begin(), targets.end(), str_i);
            if (itr == targets.end()) {
                temp += str_i;
            } else {
                if (temp.size() > 0) {
                    result.emplace_back(temp);
                }
                temp = "";
            }
        }

        if (temp.size() > 0) {
            result.emplace_back(temp);
        }
    }

    void splitString(const string &str, const string &target, vector<string> &result) {
        // support chinese and english chars mixed
        if (!result.empty())
            result.empty();

        if (str.size() < 1)
            return;

        if (utf8_len(target) < 1) {
            result.emplace_back(str);
            return;
        }

        vector<string> target_vec;
        for (int i = 0; i < utf8_len(target); ++i) {
            target_vec.emplace_back(utf8_at(target, i));
        }
        splitString(str, target_vec, result);
    }

    bool isSpacialChar(char c) {
        switch (c) {
            case ' ':
            case '#':
            case '\t':
            case '\n':
                return true;
            default:
                return false;
        }
    }

    void eliminateCharInString(string &str) {
        str.erase(std::remove_if(str.begin(), str.end(), &isSpacialChar), str.end());
    }
}

#endif //TOOLS_CXX_LINUX_UTF8_STRING_H
