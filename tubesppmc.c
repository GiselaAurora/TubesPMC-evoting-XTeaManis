#include <stdint.h> // to use uint32_t
// represent integer values ranging from 0 to 2^32 - 1
#include <stdio.h>

/* take 64 bits of data in v[0] and v[1] and 128 bits of key[0] - key[3] */

void encipher(unsigned int pengulangan, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < pengulangan; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}

void decipher(unsigned int pengulangan, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*pengulangan;
    for (i=0; i < pengulangan; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}

int main(){
    // Exaple plaintext and key
    uint32_t plaintext[2] = {0x01234567, 0x89ABCDEF}; // 64-bit plaintext
    uint32_t key[4] = {0xA56BABCD, 0x56781234, 0xDEF91234, 0x12345678}; // 128-bit key

    // Print plaintext and key
    printf("Plaintext: %08X%08X\n", plaintext[0], plaintext[1]);
    printf("Key:       %08X%08X%08X%08X\n", key[0], key[1], key[2], key[3]);

    // Encrypt plaintext
    encipher(32, plaintext, key);

    // Print ciphertext
    printf("Hasil encrypt: %08X%08X\n", plaintext[0], plaintext[1]);

    decipher(32, plaintext, key);
    printf("Hasil Decrypt: %08X%08X\n", plaintext[0], plaintext[1]);
    return 0;
}
