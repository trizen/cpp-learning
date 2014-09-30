
/*
 * Author: Daniel "Trizen" Șuteu
 * License: GPLv3
 * Date: 30 September 2014
 * Website: http://github.com/trizen
 *
*/


#include <stdio.h>
#include <math.h>

int main() {
    int num = 851230412;
    int lim = log(num) / log(10);

    int i = 0;
    int j = 0;
    int k = 1;

    for (; i <= lim ; i++, k = pow(10, i)) {
        int u = (num - j) % (k * 10) / k;
        printf("%d\n", u);

        //// Uncomment the following lines to see the proof
        //// that 'v' will always store a whole number!!!
        //double v = (num - j) % (k * 10) / k;
        //printf("%f\n", v);

        j += u * k;
    }
}
