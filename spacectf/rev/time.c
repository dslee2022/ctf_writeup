#include <stdio.h>
#include <time.h>
#include <stdlib.h>

float *calculate()
{
    float *key = malloc(12 * sizeof(float)); // Allocate memory for 12 floats

    long v1; // [rsp+10h] [rbp-20h] BYREF
    int v2; // [rsp+1Ch] [rbp-14h]
    long v3; // [rsp+20h] [rbp-10h]
    int i; // [rsp+2Ch] [rbp-4h]

    key[0] = 1.2;
    key[1] = 0.80000001;
    key[2] = 2.5;
    key[3] = 3.0;
    key[4] = 1.5;
    key[5] = 2.0;
    key[6] = 0.69999999;
    key[7] = 1.8;
    key[8] = 2.2;
    key[9] = 1.0;
    key[10] = 0.5;
    key[11] = 2.3;


    v1 = time(0LL);
    v3 = localtime(&v1);
    v2 = 60 * *(int*)(v3 + 8) + *(int*)(v3 + 4);
    for ( i = 0; i <= 11; ++i )
        key[i] = (float)(v2 % 12) + (float)(key[i] + key[i]);

    return key;
}