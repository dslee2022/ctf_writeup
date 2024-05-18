#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _DWORD unsigned int

void hex2bin(const char* src, char* target, int len) {
    for (int i = 0; i < len; i += 2) {
        sscanf(src + i, "%02x", (unsigned int*)&target[i / 2]);
    }
}

void xor_(char* a1, char* a2, int a3, int a4) {
    for (int i = 0; i < a3 / 4; ++i) // assume each _DWORD is 4 bytes
        *(_DWORD*)&a1[4 * i] ^= *(_DWORD*)&a2[4 * (i % a4)];
}

void sub_1310(char* plain, char* key, int plain_len, int key_len, _DWORD* sorted_indices) {
    _DWORD* indices = (_DWORD*)malloc(4LL * key_len);
    for (int i = 0; i < key_len; ++i) {
        indices[i] = i;
    }

    for (int j = 0; j < key_len; ++j) {
        for (int k = j + 1; k < key_len; ++k) {
            if (*(_DWORD*)&key[4 * j] > *(_DWORD*)&key[4 * k]) {
                _DWORD temp = *(_DWORD*)&key[4 * j];
                *(_DWORD*)&key[4 * j] = *(_DWORD*)&key[4 * k];
                *(_DWORD*)&key[4 * k] = temp;

                temp = indices[j];
                indices[j] = indices[k];
                indices[k] = temp;
            }
        }
    }

    for (int i = 0; i < key_len; ++i) {
        sorted_indices[i] = indices[i];
    }

    free(indices);
}

void rev_sub_1310(char* plain, char* key, int plain_len, int key_len) {
    int ii; // [rsp+4h] [rbp-4Ch]
    int n; // [rsp+8h] [rbp-48h]
    int m; // [rsp+Ch] [rbp-44h]
    _DWORD* ptr; // [rsp+10h] [rbp-40h]
    int v9; // [rsp+18h] [rbp-38h]
    int v10; // [rsp+18h] [rbp-38h]
    int k; // [rsp+1Ch] [rbp-34h]
    int j; // [rsp+20h] [rbp-30h]
    int i; // [rsp+24h] [rbp-2Ch]
    _DWORD* v14; // [rsp+28h] [rbp-28h]

    v14 = (_DWORD*)malloc(4LL * key_len);
    for (i = 0; i < key_len; ++i)
        v14[i] = i;

    for (j = 0; j < key_len; ++j) {
        for (k = j + 1; k < key_len; ++k) {
            if (*((_DWORD*)&key[4 * j]) < *((_DWORD*)&key[4 * k])) {
                v9 = *((_DWORD*)&key[4 * j]);
                *((_DWORD*)&key[4 * j]) = *((_DWORD*)&key[4 * k]);
                *((_DWORD*)&key[4 * k]) = v9;
                v10 = v14[j];
                v14[j] = v14[k];
                v14[k] = v10;
            }
        }
    }

    ptr = (_DWORD*)malloc(4LL * plain_len);
    for (m = 0; m < plain_len; ++m)
        *((_DWORD*)&plain[4 * m]) = *((_DWORD*)&plain[4 * v14[m % key_len] + 4 * (m / key_len)]);

    free(ptr);
    free(v14);
    return;
}

char* decrypt(char* ciphered_hex, int key_len, char* key) {
    int cipher_len = strlen(ciphered_hex) / 2;
    char* cipher = (char*)malloc(cipher_len + 1);
    hex2bin(ciphered_hex, cipher, strlen(ciphered_hex));
    
    char* key_buf = (char*)malloc(key_len);
    memcpy(key_buf, key, key_len);
    
    _DWORD* sorted_indices = (_DWORD*)malloc(4LL * key_len);
    sub_1310(cipher, key_buf, cipher_len, key_len, sorted_indices); // Setup the same index mapping
    rev_sub_1310(cipher, key_buf, cipher_len, key_len, sorted_indices);
    xor_(cipher, key_buf, cipher_len, key_len);
    
    cipher[cipher_len] = 0; // Null-terminate the decrypted string
    
    free(key_buf);
    free(sorted_indices);
    return cipher;
}

int main() {
    char* key = "w6tPl";
    char* ciphered = "446709213550020f3b28696533183206631e030743394d4531";
    
    char* decrypted = decrypt(ciphered, 5, key);
    printf("Decrypted text: %s\n", decrypted);
    free(decrypted);

    return 0;
}
