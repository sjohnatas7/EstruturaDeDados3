#include <stdio.h>

typedef struct tecnologia{
    char *tecnologiaOrigem;
    char *tecnologiaDestino;
}Tecnologia;

typedef struct hash{
    int qtdTECDistintas, qtdParesTEC, TABLE_SIZE;
    char **tecnologia;

} Hash;


unsigned converter(char *s);
void liberaHash(Hash *ha);
Hash *criaHash(int TABLE_SIZE);
int valorString(char *str);