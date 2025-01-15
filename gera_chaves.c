#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera_chaves.h"
#define SIZE 10

lista *cria_lista()
{
    lista *l;

    if((l = (lista*) malloc(sizeof(lista))) == NULL)
        return NULL;

    l->head = NULL;

    return l;
}

nodo *cria_nodo(lista *l, char chave, int codigo, int flag)
{
    nodo *ptr;
    if((ptr = (nodo*) malloc(sizeof(nodo))) == NULL)
        return NULL;

    if((ptr->codigo = (int*) malloc(SIZE * sizeof(int))) == NULL)
        return NULL;

    ptr->chave = chave;
    ptr->codigo[0] = codigo;
    ptr->tamanho = 1;

    if (flag)
    {
        ptr->prox = NULL;
        l->head = ptr;
    }

    return ptr;
}

int add_lista(lista *l, char chave, int *count, int flag)
{
    int codigo = *count;

    nodo *aux = l->head;

    if (aux == NULL)
    {
        if ((aux = cria_nodo(l, chave, codigo, 1)))
            return 1;
        
        return 0;
    }
    
    while (aux->prox != NULL && aux->prox->chave <= chave)
        aux = aux->prox;
    
    if (aux->chave == chave)
    {
        if (!flag)
        {
            if (aux->tamanho >= SIZE)
            {
                if((aux->codigo = (int*) realloc(aux->codigo, (aux->tamanho + 1) * sizeof(int))) == NULL)
                {
                    printf("Erro ao realocar codigo");
                    return 0;
                }
            }

            aux->codigo[aux->tamanho] = codigo;
            aux->tamanho++;
        } 
        else
        {
            if (*count < 0)
                (*count)++;
            else
                (*count)--;
        }

        return 1;
    }

    nodo *ptr;

    if (! (ptr = cria_nodo(l, chave, codigo, 0)))
        return 0;

    if (chave < aux->chave)
    {
        ptr->prox = aux;
        l->head = ptr;
    }
    else
    {
        ptr->prox = aux->prox;
        aux->prox = ptr;
    }

    return 1;
}

int add_demais_chaves(lista *l, int cod_pos)
{
    int i = 32, cod_neg = -1;

    while (i <= 126)
    {
        if ( (i >= '0' && i <= '9') || (i >= 'a' && i <= 'z') )
        {
            if (!(add_lista(l, i, &cod_pos, 1)))
            {
                printf("Erro ao adicionar codigo na lista\n");
                return 0;
            }
            cod_pos++;
        }
        else if (!(i >= 'A' && i <= 'Z'))
        {
            if (!(add_lista(l, i, &cod_neg, 1)))
            {
                printf("Erro ao adicionar codigo na lista\n");
                return 0;
            }
            cod_neg--;
        }

        i++;  
    }

    return 1;    
}

int identifica_chaves_livro(lista *l, char *linha_livro, int *count, int *flag)
{
    int i;

    for (i = 0; i < strlen(linha_livro); i++)
    {
        if ( (i != 0 && linha_livro[i-1] == ' ') || *flag )
        {
            if (linha_livro[i] >= 'A' && linha_livro[i] <= 'Z')
                linha_livro[i] += 32;
        
            if( (linha_livro[i] >= 'a' && linha_livro[i] <= 'z') ||
                (linha_livro[i] >= '0' && linha_livro[i] <= '9') )
            {
                if (!(add_lista(l, linha_livro[i], count, 0)))
                {
                    printf("Erro ao adicionar codigo na lista\n");
                    return 0;
                }

                (*count)++;
            }

            *flag = 0;
        }
    }

    return 1;   
}

int identifica_chaves_arquivo(lista *l, char chave, int *arr_codigo, int tamanho)
{
    int i;

    for (i = tamanho-1; i >= 0; i--)
    {
        if (!(add_lista(l, chave, &arr_codigo[i], 0)))
        {
            printf("Erro ao adicionar codigo na lista\n");
            return 0;
        }
    }

    return 1;
}

void insere_chaves_arquivo(lista *l, FILE *ArquivoDeChaves)
{
    int i;
    nodo *aux = l->head;

    while (aux != NULL)
    {
        fprintf(ArquivoDeChaves, "'%c': ", aux->chave);

        for (i = aux->tamanho-1; i >= 0; i--)
            fprintf(ArquivoDeChaves, "%d ", aux->codigo[i]);

        fprintf(ArquivoDeChaves, "\n");

        aux = aux->prox;
    }

    return;
}

void destroi_lista(lista *l)
{
    nodo *aux = l->head;
    nodo *ptr;

    while (aux != NULL)
    {
        ptr = aux;
        aux = aux->prox;
        free(ptr->codigo);
        free(ptr);
    }

    free(l);

    return;
}