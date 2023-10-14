#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Nome do Arquivo: arquivos.c
 * Descrição: Define as structs e o cabecalho das funcoes.
 * Autor: Johnatas Luiz dos Santos - 13676388
 *        Gabriel Garcia Ferreira  - 13677160
 */

#define TAM_REGISTRO 76
#define TAM_REGISTRO_FIXO 21
#define TAM_CABECALHO 13

#define LIXO '$'
#define REMOVIDO '1'
#define NAO_REMOVIDO '0'

#define TAM_STATUS 1
#define TAM_PROX_RRN 4
#define TAM_NRO_TECNOLOGIAS 4
#define TAM_NRO_PARES_TECNOLOGIAS 4

#define TAM_REMOVIDO 1
#define TAM_GRUPO 4
#define TAM_POPULARIDADE 4
#define TAM_PESO 4
#define TAM_TEC_ORIGEM 4
#define TAM_TEC_DESTINO 4

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
    StringVariavel tecnologiaOrigem;
    StringVariavel tecnologiaDestino;
} Registro;

typedef struct {
    char nomeTecnologiaOrigem[30]; 
    char nomeTecnologiaDestino[30]; 
}ParTecnologias;

void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);
void funcionalidade1(FILE *nomeArquivo);
void funcionalidade2(FILE *arquivoBin);
void funcionalidade3(FILE *arquivoBin);
void funcionalidade4(FILE *arqBin);

FILE *abrirArquivo(char *nomeDoArquivo,char *modo);
void fecharArquivo(FILE *arq);

int lerRegistroCSV(Registro *reg,FILE *arqCSV);
void lerCampoFixoDoCSV(int *aux, FILE *arqCSV);
int lerCampoVariavelDoCSV(StringVariavel *aux, FILE *arqCSV);
void imprimirRegistro(Registro registro);
void imprimirRegistroBin(Registro *registro);
void imprimirCampoFixo(int campo);
void imprimirCampoVariavel(StringVariavel *str);

int lerCabecalho(Cabecalho *cabecalho,FILE *arqBin);

void escreveCabecalho(Cabecalho *cabecalho, FILE *arqBin);
void escreverRegistroBin(Registro *reg, FILE *arqBin);
int lerCampoVariavelBin(StringVariavel *str,FILE * arqBin);
int lerRegistroBin(Registro *reg,FILE *arqBin);

void buscarCampoFixo(FILE *arqBin, int n);
void buscarCampoVariavel(FILE *arqBin, int n);

void verificaSeTecnologiaExiste(char tecnologiasUnicas[200][50], Registro *reg, Cabecalho *cabecalho);
void verificaSeParJaExiste(ParTecnologias *paresUnicos, Registro *reg, Cabecalho *cabecalho);

int arquivoEstaInconsistente(Cabecalho *cabecalho);
void liberarRegistro(Registro *reg);