#include <stdint.h> // to use uint32_t
// represent integer values ranging from 0 to 2^32 - 1
#include <stdio.h>
#include <string.h>
// uint32_t plaintext[2];
uint32_t key[4] = {0xA56BABCD, 0x56781234, 0xDEF91234, 0x12345678}; // 128-bit key
uint32_t v[2];

void hexToString(char* hex, char* str, int hex_len) {
    int i;
    for (i = 0; i < hex_len; i += 2) {
        unsigned int hex_val;
        sscanf(&hex[i], "%2x", &hex_val);
        sprintf(&str[i/2], "%c", hex_val);
    }
}
/* take 64 bits of data in v[0] and v[1] and 128 bits of key[0] - key[3] */
void encipher(unsigned int pengulangan, char buffer[17], uint32_t const key[4], char encrypted_value[17], char password[8]){
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
    //printf("Hasil Encrypt: %08X%08X\n", v[0], v[1]);
    encrypted_value = ((uint64_t)v[0] << 32) | v[1];
    return encrypted_value;
}

void decipher(unsigned int pengulangan, char encrypted_value[17], uint32_t const key[4]) {
    unsigned int i;
    char buffer[17];
    char password[8];
    char output[9]; // hex to string
    uint32_t v[2];
    char decrypted_str[17];
    encipher(pengulangan, buffer, key, encrypted_value, password);
    sscanf(encrypted_value, "%8X%8X", &v[0], &v[1]);
    uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*pengulangan;
    for (i=0; i < pengulangan; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
    uint64_t decrypted_value = ((uint64_t)v[0] << 32) | v[1];
    sprintf(decrypted_str, "%08X%08X", v[0], v[1]);
    /*for (i = 0; i < sizeof(decrypted_value)/sizeof(char)/2; i++) {
        sscanf(&decrypted_str[2*i], "%2X", (unsigned int*)&output[i]);  // Read two hex characters at a time
    }*/
    hexToString(decrypted_str, output, 16);
    
    printf("Hasil decrypt : %08X%08X\n", v[0], v[1]);
    printf("Password anda: %s\n", output);
}

void cekPass(char* ID){
    char line[25];
    char password[17];
    char listpassword[100];
    int print;
    FILE* stream = fopen("akun.txt", "r");
    int pengulangan = 32;
    while (fgets(line, sizeof(line), stream)) {
        sscanf(line, "%s %*s", listpassword);
        if (strncmp(listpassword, ID, 4) == 0) {
            sscanf("%s",line);
            printf("%s", line);
            printf("%s\n", listpassword);
            print = 1;
            char *ptr = strchr(line, ' '); // find the space character
            if (ptr != NULL) {
                char *password = ptr + 1; // extract the substring after the space character
                decipher(pengulangan, password, key);
            }
        }
    }
    if (print == 0){
        printf("ID yang anda masukkan salah");
    }
    
}

void login(char* str, char* ID){
    char line[200];
    char listpassword[100];
    char password[17];
    int print;
    FILE* stream = fopen("akun.txt", "r");
    int pengulangan = 32;
    char encrypted_value[17];
    int len = strlen(str);
        char buffer[17]; // allocate a buffer to hold the hex string, which has twice the length of input string
        buffer[16] = '\0'; // set the last character of the buffer to null terminator

        for (int i = 0; i < len; i++) {
            sprintf(&buffer[2*i], "%02X", str[i]); // convert each character in the input string to its hex representation
        }
        //printf("\nPlaintext: %s\n", buffer); // dari plaintext diatas
//printf("Key       :%08X%08X%08X%08X\n", key[0], key[1], key[2], key[3]); // key sudah dideklarasi di atas

        while (fgets(line, sizeof(line), stream)) {
            sscanf(line, "%s %*s", listpassword);
            if (strncmp(listpassword, ID, 4) == 0) {
            sscanf("%s",line);
            printf("%s", line);
            printf("%s\n", listpassword);
            char *ptr = strchr(line, ' '); // find the space character
            if (ptr != NULL) {
                char *password = ptr + 1;
            encipher(pengulangan, buffer, key, encrypted_value,password);
            }
        }
        printf("%s", encrypted_value);
        if (print == 0){
        printf("Maaf anda belum terdaftar atau password anda salah.\n");
        }
    
        }
}


int main(){
    // Rounds from XTEA is a fixed 32 rounds
    int pilihan;
    char str[9];
    char ID[5];
    char hehe[10];
    
    printf("      ==== E-VOTING ====       \n");
    printf(" ==== LUBERJUDIL MANTAP ====  \n");
    printf("==== Masuk ke dalam Akun Voting ====\n\n");
    // FILE* stream = fopen("akun.txt", "r");
    
    printf("Halo, Selamat Datang!\n");
    printf("1. Lupa password nih\n");
    printf("2. Ingin masuk akun\n");
    printf("Masukkan pilihan : ");
    scanf("%d", &pilihan);
    getchar();
    
    if(pilihan == 1){
        printf("Masukkan ID [4 karakter]: ");
        scanf("%s", &ID);
        //printf("==CEK AKUN==\n");
        //printf("Masukkan anime kesukaan anda: ");
        //scanf("%s", &hehe);
        cekPass(ID);
    }
    else if (pilihan == 2){
        printf("Masukkan ID [4 karakter]: ");
        scanf("%s", &ID);
        getchar();
        printf("Masukkan Password [8 karakter]: ");
        fgets(str, sizeof(str), stdin); // read the input string from user
        login(str, ID);
    }
    
    else{
        printf("Anda salah memasukkan pilihan");
        return 0;
    }
    return 0;
}

/*0123456789ABCDEF*/