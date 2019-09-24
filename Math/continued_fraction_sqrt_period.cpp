/*
    Daniel "Trizen" Suteu
    Date: 24 September 2019
    https://github.com/trizen

    Compute the simple continued fraction expansion of sqrt(n).

    See also:
        https://oeis.org/A240071
        https://en.wikipedia.org/wiki/Continued_fraction
        http://mathworld.wolfram.com/SimpleContinuedFraction.html
*/

#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int isqrt(int n) {
    return static_cast<int>(std::sqrt((double) n));
}

vector<int> cfrac_sqrt(int n) {

    int x = isqrt(n);
    int y = x;
    int z = 1;
    int r = 2 * x;

    vector<int> cfrac = {x};

    if (x * x == n) {
        return cfrac;
    }

    do {
        y = r * z - y;
        z = (n - y * y) / z;
        r = (x + y) / z;
        cfrac.push_back(r);
    }
    while (z != 1);

    return cfrac;
}

string join_vector(vector<int> v) {
    stringstream ss;
    for (size_t i = 0; i < v.size(); ++i) {
        if (i != 0) {
            ss << " ";
        }
        ss << v[i];
    }
    string s = ss.str();
    return s;
}

int main(int argc, char **argv) {
    for (int n = 1; n <= 30; ++n) {
        vector<int> cfrac = cfrac_sqrt(n);
        cout << "sqrt(" << n << ") = [" << join_vector(cfrac) << "]" << endl;
    }
}
