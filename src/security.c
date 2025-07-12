#include <stdio.h>
void caesarEncrypt(char *text, int shift)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        char c = text[i];
        if (c >= 'a' && c <= 'z')
        {
            text[i] = ((c - 'a' + shift) % 26) + 'a';
        }
        else if (c >= 'A' && c <= 'Z')
        {
            text[i] = ((c - 'A' + shift) % 26) + 'A';
        }
    }
}

void caesarDecrypt(char *text, int shift)
{

    caesarEncrypt(text, 26 - (shift % 26));
}

#include <stdint.h>
#include <stddef.h>

const uint32_t FNV_prime_32 = 16777619U;
const uint32_t offset_basis_32 = 2166136261U;

uint32_t fnv1_32(const char *data) {
    uint32_t hash_value = offset_basis_32;

    for (size_t i = 0; data[i] != '\0'; ++i) {
        hash_value = (hash_value * FNV_prime_32) ^ (uint8_t)data[i];
    }

    return hash_value;
}
