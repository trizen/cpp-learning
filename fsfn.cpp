/*
    fsfn - Find Similar File Names
    Copyright (C) 2014 Daniel "Trizen" Șuteu

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
    Find similar file names in a given directory tree

    Date: 1st April 2014
    http://trizenx.blogspot.com

    Compilation:
        g++ -std=c++11 -march=native -Ofast fsfn.cpp -o fsfn

    Usage:
        ./fsfn [dir]
*/

#define _OPEN_SYS
#define _FILE_OFFSET_BITS 64
#include <dirent.h>
#include <sys/stat.h>
#undef _FILE_OFFSET_BITS
#undef _OPEN_SYS

#include <algorithm>
#include <string.h>
#include <iostream>
#include <vector>
#include <array>
#include <map>

using namespace std;

signed int compare_strings(string str1, string str2) {

    // Return 0 if the file names are equal
    if (str1 == str2) return 0;

    size_t len1 = str1.size();
    size_t len2 = str2.size();

    // Swap the strings if len(str1) > len(str2)
    if (len1 > len2) {
        swap(len1, len2);
        swap(str1, str2);
    }

    size_t min = (len2 % 2) ? (len2 / 2 + 1) : (len2 / 2);

    if (min > len1)
        return -1;

    // Check if str1 is a substring of str2
    size_t diff = len1 - min;

    for (size_t i = 0; i <= diff; i++) {
        for (size_t j = i; j <= diff; j++) {
            if (str2.find(str1.substr(i, min + j - i)) != string::npos)
                return 0;
        }
    }

    return 1;
}

string transform_fname(string fname) {

    transform(fname.begin(), fname.end(), fname.begin(), ::tolower);

    size_t lastdot;

    if ((lastdot = fname.find_last_of('.')) != string::npos) {
        if (fname.size() - lastdot <= 5)    // '.c' == 2  // '.avi' == 4 // '.flac' == 5
            return fname.substr(0, lastdot);
    }

    return fname;
}

void traverse(const char *fn, vector <array <string, 2>> *collection, string(*callback)(string)) {

    DIR *dir;
    struct dirent *entry;
    char path[4096];
    struct stat info;

    string path_str(fn);

    if (path_str[-1] != '/')
        path_str += "/";

    if ((dir = opendir(fn)) == NULL)
        fprintf(stderr, "opendir() error on %s: %s\n", fn, strerror(errno));
    else {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.') {

                strcpy(path, fn);
                strcat(path, "/");
                strcat(path, entry->d_name);

                if (stat(path, &info) != 0)
                    fprintf(stderr, "stat() error on %s: %s\n", path, strerror(errno));
                else if (S_ISDIR(info.st_mode))
                    traverse(path, collection, callback);
                else {
                    string fname(entry->d_name);
                    collection->push_back({path_str + fname, callback(fname)});
                }
            }
        }

        closedir(dir);
    }
}

int main(int argc, char **argv) {

    if (argc != 2) {
        cerr << "usage: " << argv[0] << " [dir]" << endl;
        return 1;
    }

    vector <array<string, 2>> collection;
    traverse(argv[1], &collection, &transform_fname);

    // Exit if the collection is empty
    if (collection.empty()) {
        if (errno != 0)
            return errno;

        return 0;
    }

    // Compare the file names and decide whether they look similar
    map<string, vector<string>> dups;

    for (size_t i = 0; i < collection.size() - 1; i++) {
        for (size_t j = i + 1; j < collection.size(); j++) {
            if (compare_strings(collection[i][1], collection[j][1]) == 0) {
                dups[collection[i][0]].push_back(collection[j][0]);
                collection.erase(collection.begin() + j--);
            }
        }
    }

    // Sort and print the groups of similar files
    for (pair <string, vector<string>> p : dups) {

        vector <string> group = p.second;
        group.push_back(p.first);
        sort(group.begin(), group.end());

        for (string &path : group)
            cout << path << endl;

        cout << std::string(80, '-') << endl;
    }

    return 0;
}
