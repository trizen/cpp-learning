/*
    Daniel "Trizen" Suteu
    Date: 25 September 2019
    https://github.com/trizen

    Compute the length of the period of continued fraction for square of n.

    See also:
        https://oeis.org/A003285 -- Period of continued fraction for square root of n (or 0 if n is a square).
        https://oeis.org/A064025 -- Length of period of continued fraction for square root of n!.
        https://en.wikipedia.org/wiki/Continued_fraction
        http://mathworld.wolfram.com/SimpleContinuedFraction.html

    Compilation:
        g++ -Ofast -march=native -lgmp file.cpp -o prog
*/

#include <gmp.h>
#include <iostream>

using namespace std;

unsigned int cfrac_sqrt_period_len(mpz_t n) {

    if (mpz_perfect_square_p(n)) {
        return 0;
    }

    mpz_t t;
    mpz_t x;
    mpz_t y;
    mpz_t z;

    mpz_inits(t, x, y, z, NULL);

    mpz_set_ui(z, 1);
    mpz_sqrt(x, n);
    mpz_set(y, x);

    unsigned int period = 0;

    do {
        // y = floor((x+y)/z)*z - y
        mpz_add(t, x, y);    // t = x+y
        mpz_div(t, t, z);    // t = floor(t/z)
        mpz_mul(t, t, z);    // t = t*z
        mpz_sub(y, t, y);    // y = t-y

        // z = (n - y*y)/z
        mpz_mul(t, y, y);         // t = y*y
        mpz_sub(t, n, t);         // t = n-t
        mpz_divexact(z, t, z);    // z = t/z

        ++period;
    }
    while (mpz_cmp_ui(z, 1) != 0);

    return period;
}

int main(int argc, char **argv) {

    mpz_t n;
    mpz_inits(n, NULL);

    for (int k = 2; k <= 20; ++k) {
        //mpz_set_ui(n, k);
        mpz_fac_ui(n, k);   // https://oeis.org/A064025
        auto period = cfrac_sqrt_period_len(n);
        cout << "a(" << k << ") = " << period << endl;
    }
}
