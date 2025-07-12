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
