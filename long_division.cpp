/*

    Copyright 2014 Daniel "Trizen" Șuteu <trizenx@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301, USA.

*/

/*
    Very fast arbitrary long division of two integers.

    Date: 27 March 2014
    http://trizenx.blogspot.com

    Compilation:
     g++ -std=c++11 -march=native -Ofast long_division.cpp -o long_division

    Usage:
     ./long_division 429429 4512512 2344
*/

#include <iostream>
#include <string>

using namespace std;

void _divide(int &x, int &y, int &f, int &c, string &z) {

    int i = x / y;
    z += to_string(i);
    x -= y * i;

    signed int s = -1;

    while (y > x) {
        x *= 10;
        s++;
        if (x == 0) break;
    }

    if (c == 0) z += ".";
    if (s > 0)  z += string(s, '0');
    if ((c += s + 1) <= f)
        _divide(x, y, f, c, z);
}

string divide(int x, int y, int f) {

    signed int c = 0;
    string z = "";

    z.reserve(f + 2);
    _divide(x, y, f, c, z);

    return z;
}

int main(int argc, char **argv) {
    int x, y, f;

    if (argc < 3) {
        cerr << "usage: " << argv[0] << " A B [precision]" << endl;
        return 1;
    }

    if (argc < 4) f = 64;
    else f = atoi(argv[3]);

    x = atoi(argv[1]);
    y = atoi(argv[2]);

    cout << divide(x, y, f) << endl;
    return 0;
}
