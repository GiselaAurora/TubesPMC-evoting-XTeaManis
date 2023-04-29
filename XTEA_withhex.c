#include <stdint.h> // to use uint32_t
// represent integer values ranging from 0 to 2^32 - 1
#include <stdio.h>
#include <string.h>
// uint32_t plaintext[2];
uint32_t key[4] = {0xA56BABCD, 0x56781234, 0xDEF91234, 0x12345678}; // 128-bit key

/* take 64 bits of data in v[0] and v[1] and 128 bits of key[0] - key[3] */

void encipher(unsigned int pengulangan, char buffer[17], uint32_t const key[4], char encrypted_value[17]) {
    unsigned int i;
    uint32_t v[2];
    sscanf(buffer, "%8X%8X", &v[0], &v[1]); 
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < pengulangan; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
    // decipher(pengulangan, v, key);
    printf("Hasil Encrypt: %08X%08X\n", v[0], v[1]);
    encrypted_value = ((uint64_t)v[0] << 32) | v[1];
}

void decipher(unsigned int pengulangan, char encrypted_value[17], uint32_t const key[4]) {
    unsigned int i;
    char buffer[17];
    uint32_t v[2];
    encipher(pengulangan, buffer, key, encrypted_value);
    sscanf(encrypted_value, "%8X%8X", &v[0], &v[1]);
    uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*pengulangan;
    for (i=0; i < pengulangan; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
    uint64_t decrypted_value = ((uint64_t)v[0] << 32) | v[1];
    printf("Hasil Decrypt: %08X%08X\n", v[0], v[1]);
}

int main(){
    // Rounds from XTEA is a fixed 32 rounds
    int pengulangan = 32;
    char encrypted_value[17];
    // Get plaintext from user input
    //char buffer[17];
    char str[9];
    //buffer[16] = '\0';
    printf("      ==== E-VOTING ====       \n");
    printf(" ==== LUBERJUDIL MANTAP ====  \n");
    printf("==== Masuk ke dalam Akun Voting ====\n\n");
    FILE* stream = fopen("akun.txt", "r");
    printf("Masukkan Password [8 karakter]: ");
    fgets(str, sizeof(str), stdin); // read the input string from user

    int len = strlen(str);
    char buffer[17]; // allocate a buffer to hold the hex string, which has twice the length of input string
    buffer[16] = '\0'; // set the last character of the buffer to null terminator

    for (int i = 0; i < len; i++) {
        sprintf(&buffer[2*i], "%02X", str[i]); // convert each character in the input string to its hex representation
    }

    // Print plaintext and key
    printf("\nPlaintext: %s\n", buffer); // dari plaintext diatas
    printf("Key:       %08X%08X%08X%08X\n", key[0], key[1], key[2], key[3]); // key sudah dideklarasi di atas

    // Encrypt plaintext
    // with decrypt inside enchiper function
    encipher(pengulangan, buffer, key, encrypted_value);
    decipher(pengulangan, encrypted_value, key);

    return 0;
}

/*0123456789ABCDEF*/