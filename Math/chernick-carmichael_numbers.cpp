
// Daniel "Trizen" Șuteu
// Date: 05 June 2019
// https://github.com/trizen

// Generate the smallest extended Chernick-Carmichael number with n prime factors.

// OEIS sequence:
//   https://oeis.org/A318646 -- The least Chernick's "universal form" Carmichael number with n prime factors.

// See also:
//   https://oeis.org/wiki/Carmichael_numbers
//   http://www.ams.org/journals/bull/1939-45-04/S0002-9904-1939-06953-X/home.html

// Compilation:
//  g++ -march=native -Ofast -lgmp chernick-carmichael_numbers.cpp -o x

#include <gmp.h>
#include <iostream>

using namespace std;

bool probprime(unsigned long long int k, mpz_t n) {
    mpz_set_ui(n, k);
    return mpz_probab_prime_p(n, 0);
}

bool is_chernick(int n,  unsigned long long int m, mpz_t z) {

    if (!probprime(6 * m + 1, z)) {
        return false;
    }

    if (!probprime(12 * m + 1, z)) {
        return false;
    }

    for (unsigned long long int i = n - 2; i > 0; i--) {
        if (!probprime((1 << i) * 9 * m + 1, z)) {
            return false;
        }
    }

    return true;
}

int main() {

    mpz_t z;
    mpz_inits(z, NULL);

    for (int n = 3; n <= 9; n++) {

        unsigned long long int multiplier = (n <= 4) ? 1 : (1 << (n - 4));

        if (n > 5) {
            multiplier *= 5;
        }

        for (unsigned long long int k = 1; ; k++) {

            if (is_chernick(n, k * multiplier, z)) {
                cout << "a(" << n << ") has m = " << (k * multiplier) << endl;
                break;
            }
        }
    }

    return 0;
}
