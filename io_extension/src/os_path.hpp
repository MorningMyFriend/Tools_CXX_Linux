//
// Created by wurui on 18-9-5.
//

#ifndef TOOLS_CXX_LINUX_OS_PATH_H
#define TOOLS_CXX_LINUX_OS_PATH_H

#include <iostream>
#include <vector>
#include <string.h>
#include <dirent.h>
#include <algorithm>

using namespace std;

namespace wrz {

    void listdir(string path, vector<string> &files) {
        if (path[path.size() - 1] != '/') {
            path += "/";
        }

        DIR *dir;
        struct dirent *ptr;
        char base[1000];

        if ((dir = opendir(path.c_str())) == NULL) {
            perror("Open dir error...");
            exit(1);
        }

        while ((ptr = readdir(dir)) != NULL) {
            if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) {   ///current dir OR parrent dir
                continue;
            } else if (ptr->d_type == 8) {   ///file
                files.push_back(string(ptr->d_name));
            } else if (ptr->d_type == 10) {  ///link file
                continue;
            } else if (ptr->d_type == 4) {   ///dir
                files.push_back(string(ptr->d_name));
            }
        }
        closedir(dir);

        sort(files.begin(), files.end());
        for (int i = 0; i < files.size(); ++i) {
            files[i] = path + files[i];
        }
    }

    bool walkdir(string path, vector<string> &subdirs, vector<string> &files) {
        // if there is no dir in path, return true
        if (path[path.size() - 1] != '/') {
            path += "/";
        }

        DIR *dir;
        struct dirent *ptr;
        char base[1000];

        if ((dir = opendir(path.c_str())) == NULL) {
            perror("Open dir error...");
            exit(1);
        }

        vector<string> this_subdir;

        while ((ptr = readdir(dir)) != NULL) {
            if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) {   ///current dir OR parrent dir
                continue;
            } else if (ptr->d_type == 8) {   ///file
                files.emplace_back(string(ptr->d_name));
            } else if (ptr->d_type == 10) {  ///link file
                continue;
            } else if (ptr->d_type == 4) {   ///dir
                this_subdir.emplace_back(string(ptr->d_name));
            }
        }
        closedir(dir);

        sort(files.begin(), files.end());
        for (int i = 0; i < files.size(); ++i) {
            files[i] = path + files[i];
        }

        for (int i = 0; i < this_subdir.size(); ++i) {
            this_subdir[i] = path + this_subdir[i];
            subdirs.emplace_back(this_subdir[i]);
        }

        for (int i = 0; i < this_subdir.size(); ++i) {
            walkdir(this_subdir[i], subdirs, files);
        }
    }
}

#endif //TOOLS_CXX_LINUX_OS_PATH_H
