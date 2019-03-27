/*
    Daniel "Trizen" Suteu
    Date: 26 March 2019
    https://github.com/trizen

    Compute terms of the A307069 OEIS sequence.

    Defintion by Elliott Line, Mar 22 2019:
        Given a special version of Pascal's triangle where only Fibonacci numbers are
        permitted, a(n) is the row number in which the n-th Fibonacci number first appears.

*/

#include <set>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char **argv) {

    set <int> seen;
    set <int> is_fib = {1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946};

    vector <int> row = {1};

    auto end_of_fib = is_fib.end();
    auto end_of_seen = seen.end();

    for (int n = 1; n < 3000 ; ++n) {

        vector <int> t;
        vector <int> fibs;

        int upper = ((n - (n % 2)) >> 1) - 1;

        for (int j = 0; j <= upper; ++j) {

            int v = row[j] + row[j + 1];

            if ((v == 1) || (v == 2) || (v == 3) || (v == 5) || (is_fib.find(v) != end_of_fib)) {

                if (seen.find(v) == end_of_seen) {
                    seen.insert(v);
                    fibs.push_back(v);
                    end_of_seen = seen.end();
                }

                t.push_back(v);
            }
            else {
                t.push_back(1);
            }
        }

        vector <int> v;
        v.resize(t.size());
        reverse_copy(t.begin(), t.end(), v.begin());

        if (n % 2 == 0) {
            v.erase(v.begin());
        }

        for (int k : fibs) {
            printf("%d -> first appears in row %d\n", k, n);
        }

        row.clear();
        row.reserve(t.size() + v.size() + 2);
        row.insert(row.end(), t.begin(), t.end());
        row.insert(row.end(), v.begin(), v.end());

        row.push_back(1);
        row.insert(row.begin(), 1);
    }

    return 0;
}
