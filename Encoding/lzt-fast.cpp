/*
    Author: Daniel "Trizen" Șuteu
    License: GPLv3
    Date: 27 April 2015
    Website: http://github.com/trizen
*/

// A very good and very fast compression algorithm. (concept only)

// Compilation:
//  g++ -march=native -Ofast -std=c++11 lzt-fast.cpp -o lzt-fast

#include <string>
#include <iostream>
#include <fstream>

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

void lzt_dict(string str) {

    int total = 0;          // the total number of compressed bytes

    const size_t limit = str.size();
    const size_t min = 4;
    const size_t max = limit / 2;

    size_t cur_pos = min;
    size_t old_pos = 0;
    size_t old_n   = 0;

    while (cur_pos < limit) {

        size_t n   = 2;
        size_t pos = 0;

        bool matched = false;
        for (; pos != old_pos + 1 && cur_pos + n <= limit && n <= max; n++) {
            size_t p = str.substr(0, cur_pos).find(str.substr(cur_pos, n), pos);

            if (p == string::npos) {
                break;
            }

            if (!matched && n > min) {
                matched = true;
            }
            pos = p;
        }

        if (pos == old_pos + 1) {
            cur_pos += old_n;
        }
        else {
            if (matched) {
                cout << "["
                     << cur_pos << ", "
                     << pos << ", "
                     << n - 1 << ", "
                     << str.substr(cur_pos, n - 1)
                     << "]" << endl;
                total += n - 1;
            }
            cur_pos += 1;
        }

        old_pos = pos;
        old_n   = n - 2;
    }

    cout << "** Saved " << total << " bytes from a total of " << limit << " (" << ((float)(total) / (float)(limit) * 100.0) << "%)." << endl;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        string content = slurp(string{argv[1]});
        lzt_dict(content);
    }
    else {
        lzt_dict("TOBEORNOTTOBEORTOBEORNOT#");
    }
    return 0;
}
