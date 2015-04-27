/*
    LZT: A very simple file compressor
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
    Description: A new type of LZ compression, very similar with LZSS.

    Date: 12.10.2014 18:33:59 EEST
    License: GPLv3
    Blog: http://trizenx.blogspot.ro/2014/05/lz-compression.html
    Website: http://github.com/trizen

    Compilation:
        g++ -std=c++11 -march=native -Ofast -o lzt lzt-simple.cpp

    Usage:
        ./lzt [input] [output]
*/

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <array>

#define MIN 4
#define BUFFER_SIZE 256

using namespace std;

void compress(string text, ofstream &fout) {

    map <size_t, size_t[2]> dict;
    size_t length = text.size();

    for (size_t i = length / 2 ; i >= MIN; i--) {
        //for (size_t j = length - i * 2; j > 0; j--) {
        for (size_t j = 0; j <= length - i * 2; j++) {
            size_t pos = text.find(text.substr(j, i), j + i);
            if (pos != string::npos) {
                if (dict.find(pos) == dict.end() || i > dict[pos][1]) {
                    dict[pos][0] = j;
                    dict[pos][1] = i;
                }
            }
        }
    }

    size_t last_pos = 0;
    string uncompressed = "";
    vector <array<size_t, 3>> pairs;

    for (size_t i = 0 ; i < length ; i++, last_pos++) {
        if (dict.find(i) != dict.end()) {
            size_t vlen = dict[i][1];
            pairs.push_back(array<size_t, 3> {last_pos, dict[i][0], vlen});
            i += vlen - 1;
            last_pos = 0;
        }
        else {
            uncompressed += text[i];
        }
    }

    size_t ulength = uncompressed.size();
    fout << static_cast<char>(ulength - 1) << static_cast<char>(pairs.size());

    for (auto arr : pairs) {
        for (auto item : arr) {
            fout << static_cast<char>(item);
        }
    }

    fout << uncompressed;
}

int main(int argc, char **argv) {

    string input, output;
    if (argc > 1) {
        input = argv[1];
        if (argc > 2) {
            output = argv[2];
        }
        else {
            output = input + ".lzt";
        }
    }
    else {
        cerr << "usage: " << argv[0] << " input [output]" << endl;
        return 2;
    }

    cout << "Input: " << input << endl;
    cout << "Output: " << output << endl;

    // Open input file
    ifstream fin(input, ios::binary);

    if (fin.is_open()) {

        // Open output file
        ofstream fout(output, ios::binary);
        if (fout.is_open()) {
            fout << "LZT" << char {1};
        }
        else {
            cerr << "Cannot open/create file: " << output << endl;
            return 2;
        }

        char *buffer = new char[BUFFER_SIZE];
        while (fin.read(buffer, BUFFER_SIZE)) {
            string text(buffer);
            compress(text, fout);
        }

        string text(buffer, fin.gcount());
        compress(text, fout);

        delete[] buffer;
        fin.close();
    }
    else {
        cerr << "Cannot open file: " << input << endl;
        return 2;
    }

    return 0;
}
