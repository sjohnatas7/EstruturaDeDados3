#include <stdio.h>
#include "arquivos.h"
/*
 * Nome do Arquivo: main.c
 * Descrição: Este arquivo contém o programa princiapal.
 * Autor: Johnatas Luiz dos Santos - 13676388
 *        Gabriel Garcia Ferreira  - 13677160
 */

/**
 * Função principal do programa.
 *
 * Esta função lê o número da funcionalidade a ser executada e o nome do arquivo de entrada.
 * Com base na funcionalidade escolhida, chama a função correspondente.
 * 
 * @return 0 se a execução foi bem-sucedida.
 */
int main()
{

    int funcionalidade;
    char arquivoEntrada[20];
    scanf("%d", &funcionalidade); // Le o numero da funcionalidade
    scanf("%s", arquivoEntrada); // Le o arquivo inicial
    FILE *arqEnt = abrirArquivo(arquivoEntrada, "rb+"); // Abre o arquivo incial

    if (arqEnt == NULL) // Verifica se o abertura do arquivo falhou
    {
        return 0; 
    }
    
    // Executa a funcionalidade com base na escolha
    switch (funcionalidade)
    {
    case 1:
        funcionalidade1(arqEnt);
        break;
    case 2:
        funcionalidade2(arqEnt);
        break;
    case 3:
        funcionalidade3(arqEnt);
        break;
    case 4:
        funcionalidade4(arqEnt);
        break;
    default:
        printf("Falha no processamento do arquivo.\n");
        break;
    }

    return 0;
}