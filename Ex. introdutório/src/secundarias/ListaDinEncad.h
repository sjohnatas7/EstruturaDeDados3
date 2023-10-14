//Arquivo ListaDinEncad.h

typedef struct tecnologia{
    char *nomeTec;
    char *nomePar
}Tecnologia;

typedef struct elemento* Lista;

Lista* cria_lista();
void libera_lista(Lista* li);
int insere_lista_final(Lista* li, Tecnologia al);
int insere_lista_inicio(Lista* li, Tecnologia al);
int insere_lista_ordenada(Lista* li, Tecnologia al);
int remove_lista(Lista* li, int mat);
int remove_lista_inicio(Lista* li);
int remove_lista_final(Lista* li);
int tamanho_lista(Lista* li);
int lista_vazia(Lista* li);
int lista_cheia(Lista* li);
void imprime_lista(Lista* li);
int consulta_lista_mat(Lista* li, int mat, Tecnologia *al);
int consulta_lista_pos(Lista* li, int pos, Tecnologia *al);
