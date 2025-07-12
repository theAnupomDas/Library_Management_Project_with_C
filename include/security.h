#define DEFAULT_SHIFT 3

void caesarEncrypt(char *text, int shift);
void caesarDecrypt(char *text, int shift);
unsigned long long fnv1_32(const char *data);