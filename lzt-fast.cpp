/*
    Author: Daniel "Trizen" Șuteu
    License: GPLv3
    Date: 26 April 2015
    Website: http://github.com/trizen
*/

// A very good and very fast compression algorithm. (concept only)

// Compilation:
//  g++ -march=native -Ofast -std=c++11 lzt-fast.cpp -o lzt-fast

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>

using namespace std;

string slurp(string filename) {

    std::ifstream t(filename);
    std::string str;

    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)),
               std::istreambuf_iterator<char>());

    return str;
}

void lzt_compress(string str) {

    int total = 0;          // the total number of compressed bytes

    const int min = 4;     // the minimum length of a substring
    const int max = 30;    // the maximum length of a substring

    int k = 0;       // minimum pointer (must be zero)
    int i = 0;       // iterator (0 to length(str)-1)

    int remember = false;   // remember mode
    string memo  = "";      // short-term memory

    vector <string> dups;             // array of duplicated substrings with positions
    vector <string> cache;            // cache of substrings
    cache.reserve(str.size());        // reserve some memory

    map <string, int> dict;           // dictionary of substrings

    for (auto c : str) {

        // Start remembering if the current characters exists in the dictionary
        if (!remember && dict.find(string{c}) != dict.end()) {
            remember = true;    // activate the remember mode
        }

        // Create and cache substrings (needs to be optimized or replaced)
        for (int p = k; p <= i; ++p) {
            if (cache.size() > p) {
                cache[p] += string{c};
            }
            else {
                cache.push_back(string{c});
            }
        }

        // If remember mode is one, do some checks.
        if (remember) {

            // If it doesn't exists, then the $memo is the largest
            // duplicated substring in the dictionary at this point.
            if (dict.find(memo + string{c}) == dict.end()) {
                remember = false;              // turn-off remember mode
                if (memo.size() >= min) {      // check for the minimum length of the word
                    cout << "[" << dict[memo] << ", " << memo.size() << ", " << memo << ", " << (i - memo.size()) << "]" << endl;
                    total += memo.size();
                }

                // `memo` has been stored. Now, clear the memory.
                memo = "";
            }

            // Remember one more character
            memo += string{c};
        }

        if ((i - k) >= max) {
            ++k;        // update the minimum length
        }

        // Increment the iterator
        ++i;

        // Create the dictionary from the cache of substrings
        for (auto item : cache) {
            if (dict.find(item) == dict.end()) {
                dict[item] = i - k - item.size();
            }
        }
    }

    cout << "** Saved " << total << " bytes from a total of " << str.size() << " (" << ((float)(total) / (float)(str.size()) * 100.0) << "%)." << endl;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        string content = slurp(string{argv[1]});
        lzt_compress(content);
    }
    else {
        lzt_compress("TOBEORNOTTOBEORTOBEORNOT#");
    }
    return 0;
}
