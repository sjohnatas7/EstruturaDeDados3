#include "hash.h"

unsigned converter(char *s)
{
    unsigned h = 0;
    for (int i = 0; s[i] != '\0'; i++)
        h = h * 256 + s[i];
    return h;
}
Hash *criaHash(int TABLE_SIZE)
{
    Hash *ha = (Hash *)malloc(sizeof(Hash));
    if (ha != NULL)
    {
        int i;
        ha->TABLE_SIZE = TABLE_SIZE;
        ha->qtdParesTEC = ha->qtdTECDistintas = 0;
        ha->tecnologia = (char *)malloc(TABLE_SIZE * sizeof(char));

        for (i = 0; i < ha->TABLE_SIZE; i++)
            ha->tecnologia[i] = NULL;
    }
    return ha;
}
void liberaHash(Hash *ha)
{
    if (ha != NULL)
    {
        int i;
        for (i = 0; i < ha->TABLE_SIZE; i++)
        {
            if (ha->tecnologia[i] != NULL)
                free(ha->tecnologia[i]);
        }
        free(ha->tecnologia);
        free(ha);
    }
}
int valorString(char *str)
{
    int i, valor = 7;
    int tam = strlen(str);
    for (i = 0; i < tam; i++)
        valor = 31 * valor + (int)str[i];
    return (valor & 0x7FFFFFFF);
}