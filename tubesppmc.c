#include <stdint.h> // to use uint32_t
// represent integer values ranging from 0 to 2^32 - 1
#include <stdio.h>
#include <string.h>

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
    uint32_t plaintext[2];
    uint32_t key[4] = {0xA56BABCD, 0x56781234, 0xDEF91234, 0x12345678}; // 128-bit key
    int pengulangan;

    // Get plaintext from user input
    char buffer[17];
    printf("      ==== E-VOTING ====       \n");
    printf(" ==== LUBERJUDIL MANTAP ====  \n");
    printf("==== Masuk ke dalam Akun Voting ====\n\n");
    printf("Masukkan Password Anda: ");
    printf("Masukkan plaintext : ");
    scanf("%16s", buffer);
    sscanf(buffer, "%8X%8X", &plaintext[0], &plaintext[1]);

    // Print plaintext and key
    printf("Plaintext: %08X%08X\n", plaintext[0], plaintext[1]);
    printf("Key:       %08X%08X%08X%08X\n", key[0], key[1], key[2], key[3]);

    // Get the number of rounds from user input
    printf("Masukkan jumlah putaran: ");
    scanf("%d", &pengulangan);

    // Encrypt plaintext
    encipher(pengulangan, plaintext, key);

    // Print ciphertext
    printf("Hasil encrypt: %08X%08X\n", plaintext[0], plaintext[1]);

    // Decrypt ciphertext
    decipher(pengulangan, plaintext, key);
    printf("Hasil Decrypt: %08X%08X\n", plaintext[0], plaintext[1]);
    return 0;
}
