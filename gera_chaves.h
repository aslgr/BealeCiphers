/* Header da biblioteca "gera_chaves", cujo proposito
e gerar uma lista com as chaves a partir de um Livro Cifra,
podendo salva-las em um Arquivo de Chaves. */

typedef struct nodo
{
    char chave;
    int *codigo;
    int tamanho;
    struct nodo *prox;

} nodo;

typedef struct lista
{
    nodo *head;

} lista;

lista *cria_lista();

int identifica_chaves_livro(lista *l, char *linha_livro, int *count, int *flag);

int identifica_chaves_arquivo(lista *l, char chave, int *arr_codigo, int tamanho);

int add_demais_chaves(lista *l, int cod_pos);

void insere_chaves_arquivo(lista *l, FILE *ArquivoDeChaves);

void destroi_lista(lista *l);