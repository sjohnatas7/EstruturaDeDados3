#include "arquivos.h"
#include <string.h>

/*
 * Nome do Arquivo: arquivos.c
 * Descrição: Este arquivo contém funções relacionadas ao processamento de dados em arquivos CSV e binario.
 * Autor: Johnatas Luiz dos Santos - 13676388
 *        Gabriel Garcia Ferreira  - 13677160
 */



/*------------------------------------------Manipulação de arquivo---------------------------------------------*/

/**
 * Abre um arquivo com o nome especificado no modo especificado.
 *
 * @param nomeDoArquivo Nome do arquivo a ser aberto.
 * @param modo Modo de abertura do arquivo.
 * @return Ponteiro para o arquivo aberto, ou NULL em caso de falha.
 */
FILE *abrirArquivo(char *nomeDoArquivo, char *modo)
{
    FILE *arqBin = fopen(nomeDoArquivo, modo);

    if (arqBin == NULL) // Verifica se o arquivo é valido
    {
        printf("Falha no processamento do arquivo.\n");
    }

    return arqBin;
}

/**
 * Fecha um arquivo e atualiza o status do cabeçalho para consistente.
 *
 * @param arq Ponteiro para o arquivo a ser fechado.
 */
void fecharArquivo(FILE *arq)
{
    // Garante o status de consistente para o arquivo
    fseek(arq, 0, SEEK_SET);
    char statusConsistente = '1';
    fwrite(&statusConsistente, TAM_STATUS, 1, arq);

    // Fecha o arquivo
    fclose(arq);
}
/*-------------------------------------Manipulacao de cabecalho----------------------------------------------*/

/**
 * Função para escrever o cabeçalho no arquivo binário.
 *
 * @param cabecalho Ponteiro para a estrutura de cabeçalho a ser escrita.
 * @param arqBin Ponteiro para o arquivo binário onde o cabeçalho será escrito.
 */

void escreveCabecalho(Cabecalho *cabecalho, FILE *arqBin)
{
    fseek(arqBin, 0, SEEK_SET); // Garante que o ponteiro esta no comeco do arquivo

    // Escreve no binario os dados do cabecalho
    fwrite(&cabecalho->status, TAM_STATUS, 1, arqBin);
    fwrite(&cabecalho->proxRRN, TAM_PROX_RRN, 1, arqBin);
    fwrite(&cabecalho->nroTecnologias, TAM_NRO_TECNOLOGIAS, 1, arqBin);
    fwrite(&cabecalho->nroParesTecnologias, TAM_NRO_PARES_TECNOLOGIAS, 1, arqBin);
}
/**
 * Função para ler o cabeçalho do arquivo binário.
 *
 * @param cabecalho Ponteiro para a estrutura de cabeçalho a ser preenchida.
 * @param arqBin Ponteiro para o arquivo binário.
 * @return 1 se o arquivo estiver inconsistente, 0 caso contrário.
 */
int lerCabecalho(Cabecalho *cabecalho, FILE *arqBin)
{
    fseek(arqBin, 0, SEEK_SET);                       // Garante que o ponteiro esta no comeco do arquivo
    fread(&cabecalho->status, TAM_STATUS, 1, arqBin); // Le o status do arquivo

    // Verifica se o arquivo esta inconsistente
    if (arquivoEstaInconsistente(cabecalho))
    {
        printf("Falha no processamento do arquivo.\n");
        return 1;
    }

    // Escreve o status de inconsistente enquanto trabalhamos com o arquivo
    char statusInconsistente = '0';                      // Status para o arquivo inconsistente
    fseek(arqBin, 0, SEEK_SET);                          // Garante que o ponteiro esta no comeco do arquivo
    fwrite(&statusInconsistente, TAM_STATUS, 1, arqBin); // Escreve o status de insconsistente para o arquivo

    // Le o restante do cabecalho
    fread(&cabecalho->proxRRN, TAM_PROX_RRN, 1, arqBin);
    fread(&cabecalho->nroTecnologias, TAM_NRO_TECNOLOGIAS, 1, arqBin);
    fread(&cabecalho->nroParesTecnologias, TAM_NRO_PARES_TECNOLOGIAS, 1, arqBin);

    return 0; // retorna sucesso
}
/*--------------------------------------------Manipulacao do CSV------------------------------------------*/


/**
 * Função para ler um registro a partir de um arquivo CSV.
 *
 * @param reg Ponteiro para a estrutura de registro.
 * @param arquivoCSV Ponteiro para o arquivo CSV.
 * @return 1 se houver um registro válido, 0 caso contrário.
 */
int lerRegistroCSV(Registro *reg, FILE *arquivoCSV)
{
    int temRegistro = 0;

    // Lê campos do arquivo CSV e preenche a estrutura de registro
    if (lerCampoVariavelDoCSV(&reg->tecnologiaOrigem, arquivoCSV))
    {
        // Usa um tipo diferente de leitura para o respectivo campo
        lerCampoFixoDoCSV(&reg->grupo, arquivoCSV);
        lerCampoFixoDoCSV(&reg->popularidade, arquivoCSV);
        lerCampoVariavelDoCSV(&reg->tecnologiaDestino, arquivoCSV);
        lerCampoFixoDoCSV(&reg->peso, arquivoCSV);

        reg->removido = '0'; // Garante que o registro incialmente nao esta removido
        temRegistro++;       // Flag para verificar se registro foi lido
    }

    return temRegistro; // retorna se registro foi lido com sucesso
}

/**
 * Lê um campo de tamanho variável do arquivo CSV.
 *
 * @param aux Ponteiro para a estrutura StringVariavel onde o campo será armazenado.
 * @param arqCSV Ponteiro para o arquivo CSV de entrada.
 * @return Retorna 1 se a leitura foi bem-sucedida e 0 em caso de erro.
 */
int lerCampoVariavelDoCSV(StringVariavel *aux, FILE *arqCSV)
{
    char palavra[TAM_REGISTRO - TAM_REGISTRO_FIXO];
    int i = 0;

    // Le os caracteres do arquivo CSV
    for (; i < TAM_REGISTRO - TAM_REGISTRO_FIXO; i++)
    {
        // Verifica se o ponteiro chegou em outro campo, ou no final da linha ou final do arquivo
        if (fread(&palavra[i], sizeof(char), 1, arqCSV) != 1 || palavra[i] == ',' || palavra[i] == '\n')
        {
            break;
        }
    }
    // Copia a string para o StringVariavel
    palavra[i] = '\0';
    aux->string = (char *)malloc((i + 1) * sizeof(char));
    strncpy(aux->string, palavra, i + 1);
    aux->tamanho = i;

    // Verifica possivel erro
    if (aux->string == NULL)
    {
        printf("ERRO COPIANDO STRNNG VARIAVEL, I:%d, palavra %s", i, palavra);
        return 0; // Retorna 0 para indicar erro
    }
    
    // Retorna se a string é valida
    return aux->tamanho != 0;
}

/**
 * Função para ler um campo fixo de um arquivo binário.
 *
 * @param aux Ponteiro para a variável onde o valor do campo fixo será armazenado.
 * @param arqBin Ponteiro para o arquivo binário.
 */
void lerCampoFixoDoCSV(int *aux, FILE *arqBin)
{
    char palavra[TAM_REGISTRO - TAM_REGISTRO_FIXO];
    int i = 0;

    // Le os caracteres do arquivo CSV
    for (; i < TAM_REGISTRO - TAM_REGISTRO_FIXO; i++)
    {
        // Verifica se o ponteiro chegou em outro campo, ou no final da linha ou final do arquivo
        if (fread(&palavra[i], sizeof(char), 1, arqBin) != 1 || palavra[i] == ',' || palavra[i] == '\n')
        {
            break;
        }
    }

    palavra[i] = '\0';    // Cria a flag que terminou o campo
    *aux = atoi(palavra); // Transformar a string em um inteiro

    // Verifica se o campo é nulo
    if (*aux == 0)
    {
        *aux = -1; // Campo é nulo, atribui -1 diretamente
    }
}
/**
 * Função para verificar se um par de tecnologias já existe com base em um Array.
 *
 * @param paresUnicos Array de pares de tecnologias únicos.
 * @param reg Ponteiro para o registro a ser verificado.
 * @param cabecalho Ponteiro para o cabeçalho.
 */
void verificaSeParJaExiste(ParTecnologias *paresUnicos, Registro *reg, Cabecalho *cabecalho)
{
    int parJaExiste = 0; // flag para par já existe
    int numParesTecnologias = cabecalho->nroParesTecnologias;

    // Garante que um dos pares não é nulo
    if (strlen(reg->tecnologiaDestino.string) == 0)
    {
        return;
    }

    // lacos para verificar cada um dos pares
    for (int i = 0; i < numParesTecnologias; i++)
    {
        if (strcmp(paresUnicos[i].nomeTecnologiaOrigem, reg->tecnologiaOrigem.string) == 0 &&
            strcmp(paresUnicos[i].nomeTecnologiaDestino, reg->tecnologiaDestino.string) == 0) // verifica cada um dos pares existe
        {
            parJaExiste = 1; // flag para par já existe
            break;
        }
    }
    if (!parJaExiste) // Se par nao existir insere no array
    {
        strncpy(paresUnicos[numParesTecnologias].nomeTecnologiaOrigem, reg->tecnologiaOrigem.string, reg->tecnologiaOrigem.tamanho);
        strncpy(paresUnicos[numParesTecnologias].nomeTecnologiaDestino, reg->tecnologiaDestino.string, reg->tecnologiaDestino.tamanho);
        cabecalho->nroParesTecnologias++; // Aumenta o numero de pares de tecnologias no cabecalho
    }
}
/**
 * Função para verificar se uma tecnologia já existe com base em um Array.
 *
 * @param tecnologiasUnicas Array de tecnologias únicas.
 * @param reg Ponteiro para o registro a ser verificado.
 * @param cabecalho Ponteiro para o cabeçalho.
 */
void verificaSeTecnologiaExiste(char tecnologiasUnicas[200][50], Registro *reg, Cabecalho *cabecalho)
{
    int tecnologiaJaExiste = 0; // Flag para se a tecnologia já existe

    // Verificar se a tecnologia de origem já existe no array
    for (int i = 0; i < cabecalho->nroTecnologias; i++)
    {
        if (strcmp(tecnologiasUnicas[i], reg->tecnologiaOrigem.string) == 0)
        {
            tecnologiaJaExiste = 1;
            break;
        }
    }

    // Se a tecnologia de origem não existir, adicione-a ao array
    if (!tecnologiaJaExiste)
    {
        strcpy(tecnologiasUnicas[cabecalho->nroTecnologias], reg->tecnologiaOrigem.string);
        cabecalho->nroTecnologias++; // Aumenta o numero de tecnologias no cabecalho
    }
}
/*------------------------------------------Manipulacao do binario------------------------------------*/

/**
 * Função para escrever um registro no arquivo binário.
 *
 * @param reg Ponteiro para a estrutura de registro a ser escrita.
 * @param arqBin Ponteiro para o arquivo binário onde o registro será escrito.
 */
void escreverRegistroBin(Registro *reg, FILE *arqBin)
{
    // Insere cada um dos campos
    fwrite(&reg->removido, TAM_REMOVIDO, 1, arqBin);
    fwrite(&reg->grupo, TAM_GRUPO, 1, arqBin);
    fwrite(&reg->popularidade, TAM_POPULARIDADE, 1, arqBin);
    fwrite(&reg->peso, TAM_PESO, 1, arqBin);
    fwrite(&reg->tecnologiaOrigem.tamanho, TAM_TEC_ORIGEM, 1, arqBin);
    fwrite(reg->tecnologiaOrigem.string, sizeof(char), reg->tecnologiaOrigem.tamanho, arqBin);
    fwrite(&reg->tecnologiaDestino.tamanho, TAM_TEC_DESTINO, 1, arqBin);
    fwrite(reg->tecnologiaDestino.string, sizeof(char), reg->tecnologiaDestino.tamanho, arqBin);

    // Preenche o restante do registro com lixo
    int tamAtual = TAM_REGISTRO_FIXO + reg->tecnologiaOrigem.tamanho + reg->tecnologiaDestino.tamanho; // Calcula quanto de lixo precisa ser inserido
    char lixo = LIXO;                                                                                  // Caracter do lixo

    // Laco para inserir o lixo restante
    for (int i = 0; i < TAM_REGISTRO - tamAtual; i++)
    {
        fwrite(&lixo, sizeof(char), 1, arqBin);
    }
}

/**
 * Função para ler um registro a partir de um arquivo binário.
 *
 * @param reg Ponteiro para a estrutura de registro.
 * @param arqBin Ponteiro para o arquivo binário.
 * @return 1 se houver um registro válido, 0 caso contrário.
 */

int lerRegistroBin(Registro *reg, FILE *arqBin)
{
    int temRegistro = 0;
    // Lê campos do arquivo binário e preenche a estrutura de registro
    if (fread(&reg->removido, TAM_REMOVIDO, 1, arqBin))
    {
        // Le os arquivos de tamanho fixo
        fread(&reg->grupo, TAM_GRUPO, 1, arqBin);
        fread(&reg->popularidade, TAM_POPULARIDADE, 1, arqBin);
        fread(&reg->peso, TAM_PESO, 1, arqBin);

        // Le os arquivos de tamanho variavel
        lerCampoVariavelBin(&reg->tecnologiaOrigem, arqBin);
        lerCampoVariavelBin(&reg->tecnologiaDestino, arqBin);
        temRegistro++; // Flag para verificar se registro foi lido
    }
    // Calcula o tamanho do lixo e avança o cursor para o próximo registro
    int tamLixo = TAM_REGISTRO - TAM_REGISTRO_FIXO - reg->tecnologiaDestino.tamanho - reg->tecnologiaOrigem.tamanho;
    fseek(arqBin, tamLixo, SEEK_CUR);

    return temRegistro; // retorna se registro foi lido com sucesso
}


/**
 * Função para ler um campo de tamanho variável de um arquivo binário.
 *
 * @param aux Ponteiro para a estrutura StringVariavel onde o campo de tamanho variável será armazenado.
 * @param arqBin Ponteiro para o arquivo binário.
 * @return 1 se a leitura foi bem-sucedida, 0 caso contrário.
 */

int lerCampoVariavelBin(StringVariavel *aux, FILE *arqBin)
{
    if (fread(&aux->tamanho, sizeof(int), 1, arqBin) != 1) // Garante que nao chegou no final do arquivo
    {
        return 0; // retorna falha na leitura
    }

    // Verifica se o campo nao é nulo
    if (aux->tamanho > 0)
    {
        aux->string = (char *)malloc((aux->tamanho + 1) * sizeof(char)); // Aloca memoria para a string
        fread(aux->string, sizeof(char), aux->tamanho, arqBin);          // Le a string e copia para o registro
        aux->string[aux->tamanho] = '\0';
    }
    else
    {
        aux->string[0] = '\0'; // Se o campo for nulo, garante que a flag que a string é nula
    }

    return 1; // retorna sucesso na leitura
}

/**
 * Busca registros no arquivo binário com base em um valor em um campo de tamanho fixo.
 *
 * @param arqBin Ponteiro para o arquivo binário.
 * @param n Índice do campo a ser comparado (0 para grupo, 1 para popularidade, 2 para peso).
 */
void buscarCampoFixo(FILE *arqBin, int n)
{
    int valorRequerido;
    scanf(" %d", &valorRequerido); // Verifica o valor desejado
    char caracterRemovido = REMOVIDO;
    Registro reg;
    int registrosEncontrados = 0;
    // Le cada um dos campos
    while (lerRegistroBin(&reg, arqBin))
    {
        if (reg.removido == caracterRemovido) // Verifica se registro nao foi removido
        {
            continue;
        }
        int campos[] = {reg.grupo, reg.popularidade, reg.peso}; // Insere os campos de tamanho fixo em um array
        if (campos[n] == valorRequerido)                        // Verifica se campo do registro é igual ao desejado
        {
            imprimirRegistroBin(&reg); // Imprime se encontrado
            registrosEncontrados++; // Flag para saber o numero de campos encontrados
        }
    }

    // Verifica se nenhum campo foi encontrado
    if (registrosEncontrados == 0)
    {
        printf("Registro inexistente.\n");
    }
}
/**
 * Função para buscar registros com base em um campo de tamanho variável.
 *
 * @param arqBin Ponteiro para o arquivo binário.
 * @param n Índice do campo a ser buscado (0 para tecnologiaOrigem, 1 para tecnologiaDestino).
 */
void buscarCampoVariavel(FILE *arqBin, int n)
{
    char valorRequerido[50];
    char caracterRemovido = REMOVIDO;
    Registro reg;
    int registrosEncontrados = 0;
    scan_quote_string(valorRequerido); // Funcao para retirar as aspas do campo
    // Le cada registro do arquivo binario
    while (lerRegistroBin(&reg, arqBin))
    {
        // Verifca se arquivo esta removido, e se sim pula para o proximo registro
        if (reg.removido == caracterRemovido)
        {
            continue;
        }
        // Guarda cada campo em um array
        char *campos[] = {reg.tecnologiaOrigem.string, reg.tecnologiaDestino.string};

        // Compara o valorRequerido com o campo do registro selecionado (n)
        if (strcmp(valorRequerido, campos[n]) == 0)
        {
            imprimirRegistroBin(&reg); // Exibe o registro correspondente
            registrosEncontrados++; // Flag para saber o numero de campos encontrados
        }
    }
    // Verifica se nenhum campo foi encontrado
    if (registrosEncontrados == 0)
    {
        printf("Registro inexistente.\n");
    }
}


/*--------------------------------Funcoes de impressao------------------------------------------------*/


/**
 *  Função auxiliar para imprimir cada registro
 *
 * @param registro Ponteiro para a estrutura de registro
 */
void imprimirRegistro(Registro registro)
{
    printf("Removido: %c\n", registro.removido);
    printf("Grupo: %d\n", registro.grupo);
    printf("Popularidade: %d\n", registro.popularidade);
    printf("Peso: %d\n", registro.peso);
    printf("Tecnologia Origem: %s\n", registro.tecnologiaOrigem.string);
    printf("Tecnologia Destino: %s\n", registro.tecnologiaDestino.string);
    printf("-------------------------------------");
}
/**
 *  Função para imprimir cada registro na forma padrão
 *
 * @param registro Ponteiro para a estrutura de registro
 */
void imprimirRegistroBin(Registro *registro)
{
    // Imprime cada campo usando a respectiva funcao para seu tamanho, fixo ou variavel
    imprimirCampoVariavel(&registro->tecnologiaOrigem);
    printf(", ");
    imprimirCampoFixo(registro->grupo);
    printf(", ");
    imprimirCampoFixo(registro->popularidade);
    printf(", ");
    imprimirCampoVariavel(&registro->tecnologiaDestino);
    printf(", ");
    imprimirCampoFixo(registro->peso);
    printf("\n");
}

/**
 * Função para imprimir um campo fixo.
 *
 * @param campo O valor do campo fixo a ser impresso.
 */
void imprimirCampoFixo(int campo)
{
    // Verifica se o campo não é nulo
    if (campo != -1)
    {
        printf("%d", campo);
    }
    else
    {
        printf("NULO");
    }
}
/**
 * Função para imprimir um campo de tamanho variável.
 *
 * @param str Estrutura que contém a string de tamanho variável a ser impressa.
 */
void imprimirCampoVariavel(StringVariavel *str)
{
    // Verifica se o campo não é nulo
    if (str->tamanho > 0)
    {
        printf("%s", str->string);
    }
    else
    {
        printf("NULO");
    }
}
/*--------------------------------------------Funcionalidades----------------------------------------------------------*/
/**
 * Funcionalidade 1: Leitura de registros de um arquivo CSV, geração de um arquivo binário e exibição do arquivo binário.
 *
 * @param arqCSV Ponteiro para o arquivo CSV de entrada.
 */
void funcionalidade1(FILE *arqCSV)
{
    // Cria o arquivo binario para transpor os dados
    char arquivoBinario[20];
    scanf("%s", arquivoBinario);
    FILE *arqBin = abrirArquivo(arquivoBinario, "wb+");
    if (arqBin == NULL)
        return;
    if (arqCSV == NULL)
        return;

    // Le a linha inicial do CSV
    char linha[500];
    fgets(linha, sizeof(linha), arqCSV);
    // ... Declaração de variáveis e leitura do cabeçalho ...
    ParTecnologias paresUnicos[500];
    char tecnologiasUnicas[200][50];
    Cabecalho cabecalho;
    cabecalho.status = '0';
    cabecalho.proxRRN = 0;
    cabecalho.nroTecnologias = 0;
    cabecalho.nroParesTecnologias = 0;
    Registro reg;
    // Escreve o cabecalho inicial
    escreveCabecalho(&cabecalho, arqBin);

    // Le cada registro dentro do CSV
    while (lerRegistroCSV(&reg, arqCSV))
    {
        verificaSeTecnologiaExiste(tecnologiasUnicas, &reg, &cabecalho); // Verifica numero de tecnologias unicas
        verificaSeParJaExiste(paresUnicos, &reg, &cabecalho);            // Verifica numero de pares de tecnologias unicas
        escreverRegistroBin(&reg, arqBin);                               // Escreve o registro no arquivo binario
        cabecalho.proxRRN++;                                             // Aumenta o RRN do cabecalho
    }

    escreveCabecalho(&cabecalho, arqBin); // Escreve o cabecalho final
    fclose(arqCSV);                       // Fecha arquivo CSV
    fecharArquivo(arqBin);                // Fecha arquivo binario
    binarioNaTela(arquivoBinario);        // Usa funcao binario Na Tela
}

/**
 * Funcionalidade 2: Recuperação de registros de um arquivo binário e exibição na saída padrão.
 *
 * @param arqBin Ponteiro para o arquivo binário.
 */

void funcionalidade2(FILE *arqBin)
{
    Cabecalho cabecalho;
    Registro reg;
    // Le cabecalho do arquivo binario e verifica se o arquivo esta consistente
    if (lerCabecalho(&cabecalho, arqBin))
    {
        fclose(arqBin);
        return;
    }
    // Le cada registro do arquivo binario
    while (lerRegistroBin(&reg, arqBin))
    {
        imprimirRegistroBin(&reg); // imprime o registro na tela respectivamente
    }
    // Fecha o arquivo binario
    fecharArquivo(arqBin);
}
/**
 * Funcionalidade 3: Recuperação de registros de um arquivo binário com base em critérios de busca.
 *
 * @param arqBin Ponteiro para o arquivo binário.
 */

void funcionalidade3(FILE *arqBin)
{
    int n;
    char valorCAMPO1[24];
    Cabecalho cabecalho;

    // Le cabecalho do arquivo binario e verifica se o arquivo esta consistente
    int STATUS = lerCabecalho(&cabecalho, arqBin);
    if (STATUS)
    {
        fclose(arqBin);
        return;
    }

    scanf("%d", &n); // Verifica o numero n de vezes para realizar a busca
    for (int i = 0; i < n; i++)
    {
        scanf("%s ", valorCAMPO1);
        fseek(arqBin, TAM_CABECALHO, SEEK_SET); // Garante que a busca vai comecar do comeco do arquivo

        // Com base no tipo do campo, tem a sua respectiva busca
        if (strcmp(valorCAMPO1, "grupo") == 0)
        {
            buscarCampoFixo(arqBin, 0);
        }
        else if (strcmp(valorCAMPO1, "popularidade") == 0)
        {
            buscarCampoFixo(arqBin, 1);
        }
        else if (strcmp(valorCAMPO1, "peso") == 0)
        {
            buscarCampoFixo(arqBin, 2);
        }
        else if (strcmp(valorCAMPO1, "nomeTecnologiaOrigem") == 0)
        {
            buscarCampoVariavel(arqBin, 0);
        }
        else if (strcmp(valorCAMPO1, "nomeTecnologiaDestino") == 0)
        {
            buscarCampoVariavel(arqBin, 1);
        }
    }
    // Fecha o arquivo do binario
    fecharArquivo(arqBin);
}
/**
 * Funcionalidade 4: Recuperação de um registro específico com base no RRN (Número Relativo do Registro).
 *
 * @param arqBin Ponteiro para o arquivo binário.
 */
void funcionalidade4(FILE *arqBin)
{
    int proxRRN;
    Cabecalho cabecalho;
    Registro reg;

    scanf("%d", &proxRRN); // Verifca o numero RRN para a busca

    // Le cabecalho do arquivo binario e verifica se o arquivo esta consistente
    int STATUS = lerCabecalho(&cabecalho, arqBin);
    if (STATUS)
    {
        fclose(arqBin);
        return;
    }

    // Garante que o RRN eh menor que o total no arquivo
    if (proxRRN > cabecalho.proxRRN){
        printf("Registro inexistente.\n");
        fecharArquivo(arqBin);
        return;
    }
    int byteOffset = proxRRN * TAM_REGISTRO; // Calcula o Offset com base no RRN e o tamanho do registro
    fseek(arqBin, byteOffset, SEEK_CUR);     // Coloca o ponteiro do arquivo no lugar correto
    lerRegistroBin(&reg, arqBin);            // Le o registro correto

    // Verifica se o arquivo nao esta removido, imprimindo apenas se ele estiver existindo
    char estaRemovido = REMOVIDO;
    if (reg.removido == estaRemovido)
    {
        printf("Registro inexistente.\n");
    }
    else
    {
        imprimirRegistroBin(&reg);
    }

    // Fecha o arquivo do binario
    fecharArquivo(arqBin);
}

/*-------------------------------------------
Funcoes auxiliares--------------------------------------*/

/**
 * Libera a memória alocada para os campos de tamanho variável do registro.
 *
 * @param reg Ponteiro para o registro cujos campos de tamanho variável serão liberados.
 */
void liberarRegistro(Registro *reg)
{
    // Desaloca dados da estrutura Registro
    if (reg->tecnologiaOrigem.string != NULL)
    {
        free(reg->tecnologiaOrigem.string);
        reg->tecnologiaOrigem.string = NULL;
    }

    if (reg->tecnologiaDestino.string != NULL)
    {
        free(reg->tecnologiaDestino.string);
        reg->tecnologiaDestino.string = NULL;
    }

    reg->tecnologiaOrigem.tamanho = 0;
    reg->tecnologiaDestino.tamanho = 0;
}


/**
 * Verifica se o arquivo está inconsistente com base no status do cabeçalho.
 *
 * @param cabecalho Ponteiro para o cabeçalho do arquivo.
 * @return 1 se o arquivo estiver inconsistente (status '0'), 0 caso contrário.
 */
int arquivoEstaInconsistente(Cabecalho *cabecalho)
{
    if (cabecalho->status == '0')
    {
        return 1;
    }

    return 0;
}
