/*
 * use boost: -boost_system -boost_filesystem
 * #include <boost/filesystem>
 * using namespace boost::filesystem;
 * for(auto itr=directory_iterator(dir);itr!=directory_iterator();++itr){
 *     string path = itr->path().string();
 *     string filename = itr->path().filename().string();
 * }
 */

#define __QT__ 0


#include <iostream>
#include <vector>
#include <string.h>
#include <dirent.h>
#include <algorithm>

using namespace std;

namespace wrz {


}