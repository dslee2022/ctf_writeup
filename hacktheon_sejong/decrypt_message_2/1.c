#include <stdio.h>
//#include <rand.h>
#include <stdlib.h>
#include <string.h>

#define _DWORD unsigned int

char* derive_key(int a1)
{
        int i; // [rsp+14h] [rbp-2Ch]
        char* v3; // [rsp+18h] [rbp-28h]
        int v4; // [rsp+24h] [rbp-1Ch]

        v4 = strlen("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
        v3 = (char*)malloc(a1 + 1);
        char *string = (char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        for (i = 0; i < a1; ++i)
                v3[i] = string[rand() % v4];
        v3[a1] = 0;
        return v3;
}

_DWORD* copy_and_allocate_bf(char* a1, int a2)
{
	int i; // [rsp+Ch] [rbp-24h]
	_DWORD* v4; // [rsp+10h] [rbp-20h]

	v4 = (_DWORD*)malloc(4LL * a2);
	for (i = 0; i < a2; ++i)
		v4[i] = a1[i];
	return v4;
}

void xor_(char* a1, char* a2, int a3, int a4)
{
	int i; // [rsp+Ch] [rbp-24h]

	for (i = 0; i < a3; ++i)
		*(_DWORD*)&a1[4 * i] ^= *(_DWORD*)&a2[4 * (i % a4)];
	return;
}

void sub_1310(char* plain, char* key, int plain_len, int key_len)
{
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
	for (j = 0; j < key_len; ++j)
	{
		for (k = j + 1; k < key_len; ++k)
		{
			if (*(_DWORD*)&key[4 * j] > *(_DWORD*)&key[4 * k])
			{
				v9 = *(_DWORD*)&key[4 * j];
				*(_DWORD*)&key[4 * j] = *(_DWORD*)&key[4 * k];
				*(_DWORD*)&key[4 * k] = v9;
				v10 = v14[j];
				v14[j] = v14[k];
				v14[k] = v10;
			}
		}
	}
	ptr = (_DWORD*)malloc(4LL * plain_len);
	for (m = 0; m < plain_len; m += key_len)
	{
		for (n = 0; n < key_len; ++n)
			ptr[n + m] = *(_DWORD*)&plain[4 * v14[n] + 4 * m];
	}
	for (ii = 0; ii < plain_len; ++ii)
		*(_DWORD*)&plain[4 * ii] = ptr[ii];
	free(ptr);
	free(v14);
	return;
}

char* sub_11C0(char* a1, int a2)
{
	int i; // [rsp+Ch] [rbp-24h]
	char* v4; // [rsp+10h] [rbp-20h]

	v4 = (char*)malloc(2 * a2 + 1);
	for (i = 0; i < a2; ++i)
		sprintf(&v4[2 * i], "%02x", *(unsigned int*)&a1[4 * i]);
	return v4;
}

char* encrypt(char* plain, int key_len, char* key)
{
	char* v3; // [rsp+10h] [rbp-40h]
	char* key_buf; // [rsp+18h] [rbp-38h]
	char* plain_buf; // [rsp+20h] [rbp-30h]
	int plain_len; // [rsp+30h] [rbp-20h]

	plain_len = strlen(plain);
	if (plain_len % key_len)
		return (char*)"";
	//key = derive_key(key_len);
	//printf("key @ %s\n", key);
	plain_buf = (char*)copy_and_allocate_bf(plain, plain_len);
	key_buf = (char*)copy_and_allocate_bf(key, key_len);
	xor_(plain_buf, key_buf, plain_len, key_len);
	sub_1310(plain_buf, key_buf, plain_len, key_len);
	v3 = sub_11C0(plain_buf, plain_len);
	free(plain_buf);
	free(key_buf);
	//free(key);
	return v3;
}

int main()
{
        // srand(1714186782);
        // char* key = derive_key(5);
        // printf("%s\n", key);
		// char* plain = "THIS_IS_FAKE_FLAG!!!";
		// char* enc = encrypt(plain, 5, key);
		// printf("%s\n", enc);


	for (int i = 1690000000; i < 1790000000; i++)
	{
		srand(i);
		char* key = derive_key(5);
		char* a1 = encrypt((char*)"BrU7e", 5, key);
		if (strcmp(a1, "4467092135") == 0)
		{
			
			printf("this:%s %s %d\n", key, a1, i);
			break;
		}
		else
		{
			printf("not this:%s %s %d\n", key, a1, i);
			//cout << "not this:" << key << endl;
		}
	}

        //cout << key << endl;
        return 0;
}                                                                      
