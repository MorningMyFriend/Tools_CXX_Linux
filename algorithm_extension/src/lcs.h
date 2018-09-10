//
// Created by wurui on 18-9-10.
//

#ifndef TOOLS_CXX_LINUX_LCS_H
#define TOOLS_CXX_LINUX_LCS_H

#include <iostream>
#include <vector>

namespace wrz {
    string LCS(const std::string &s1, const std::string &s2) {
        string::size_type m = s1.size();
        string::size_type n = s2.size();

        // map
        vector<vector<int>> L;
        for (string::size_type i = 0; i != m + 1; ++i) {
            L.emplace_back(vector<int>(n + 1, 0));
        }

        for (string::size_type i = 0; i != m+1; ++i) {
            for (string::size_type j = 0; j != n+1; ++j) {
                if (i == 0 || j == 0) {
                    L[i][j] = 0;
                } else if (s1[i - 1] == s2[j - 1]) {
                    L[i][j] = L[i - 1][j - 1] + 1;
                } else {
                    L[i][j] = std::max(L[i - 1][j], L[i][j - 1]);
                }
            }
        }

        int length = L[m][n];
        int i = m;
        int j = n;

        string result = "";

        while (length > 0) {
            if (s1[i - 1] == s2[j - 1]) {
                result = s1[i - 1] + result;
                length--;
                --i;
                --j;
            } else if (L[i - 1][j] >= L[i][j - 1]) { --i; } // up
            else { --j; } // left
        }

        return result;
    }

}

#endif //TOOLS_CXX_LINUX_LCS_H
