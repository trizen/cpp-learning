/*
 * Source code of the RC4 stream cipher.
 *
 * See also:
 *  https://en.wikipedia.org/wiki/RC4
 *
 * Based on code from:
 *  https://cypherpunks.venona.com/date/1994/09/msg00304.html
 *
*/

#include <stdio.h>

typedef struct rc4_key {
    unsigned char state[256];
    unsigned char x;
    unsigned char y;
} rc4_key;

static void swap_byte(unsigned char *a, unsigned char *b) {
    unsigned char swapByte;

    swapByte = *a;
    *a = *b;
    *b = swapByte;
}

void prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key) {
    unsigned char swapByte;
    unsigned char index1;
    unsigned char index2;
    unsigned char *state;
    short counter;

    state = &key->state[0];
    for (counter = 0; counter < 256; counter++)
        state[counter] = counter;

    key->x = 0;
    key->y = 0;
    index1 = 0;
    index2 = 0;

    for (counter = 0; counter < 256; counter++) {
        index2 = (key_data_ptr[index1] + state[counter] + index2) % 256;
        swap_byte(&state[counter], &state[index2]);
        index1 = (index1 + 1) % key_data_len;
    }
}

void rc4(unsigned char *buffer_ptr, int buffer_len, rc4_key *key) {
    unsigned char x;
    unsigned char y;
    unsigned char *state;
    unsigned char xorIndex;
    short counter;

    x = key->x;
    y = key->y;

    state = &key->state[0];
    for (counter = 0; counter < buffer_len; counter ++) {
        x = (x + 1) % 256;
        y = (state[x] + y) % 256;
        swap_byte(&state[x], &state[y]);
        xorIndex = state[x] + (state[y]) % 256;
        buffer_ptr[counter] ^= state[xorIndex];
    }
    key->x = x;
    key->y = y;
}

int main() {
    int i;

    unsigned char s[] = "abcdefghijklmnopqrstuvwxyz";
    printf("Encoding '%s' of size %d\n", s, sizeof(s));

    rc4_key key;
    prepare_key(key.state, sizeof(key.state), &key);

    printf("Key code points: ");
    for(i = 0; i < sizeof(key.state); ++i) {
        printf("%d, ", key.state[i]);
    }
    printf("\n");

    rc4(s, sizeof(s), &key);
    printf("RC4 encoded = %s\n", s);

    printf("Encoded string code points: ");
    for(i = 0; i < sizeof(s); ++i) {
        printf("%d, ", s[i]);
    }
    printf("\n");

    prepare_key(key.state, sizeof(key.state), &key);

    rc4(s, sizeof(s), &key);
    printf("Decoded = %s\n", s);
    return 0;
}
