#include <stdio.h>
#include <stdlib.h>


#define TAM_REGISTRO 76
#define TAM_REGISTRO_FIXO 21
#define TAM_CABECALHO 13

#define LIXO '$'
#define REMOVIDO '1'
#define NAO_REMOVIDO '0'

typedef struct {
    char status;
    int proxRRN;
    int nroTecnologias;
    int nroParesTecnologias;
} Cabecalho;

typedef struct {
    int tamanho;
    char * string;
} StringVariavel;

typedef struct {
    char removido;
    int grupo;
    int popularidade;
    int peso;
    StringVariavel tecnologiaOrdem;
    StringVariavel tecnologiaDestino;
} Registro;

void binarioNaTela(char *nomeArquivoBinario);
void funcionalidade1(char *nomeArquivo);