#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera_chaves.h"
#include "decodifica.h"

int busca_arr(nodo *aux, int inicio, int fim, int codigo)
{
    if (inicio > fim)
        return 0;
    
    int meio = (inicio + fim)/2;

    if (aux->codigo[meio] == codigo)
        return 1;

    if (aux->codigo[meio] < codigo)
        return busca_arr(aux, meio + 1, fim, codigo);

    return busca_arr(aux, inicio, meio - 1, codigo);
}

char busca_chave(lista *l, int codigo)
{
    int found = 0;
    nodo *aux = l->head;

    while (!found && aux != NULL)
    {
        found = busca_arr(aux, 0, aux->tamanho-1, codigo);
        
        if (!found)
            aux = aux->prox;
    }

    if (aux != NULL)
        return aux->chave;

    return '\n';
}

int decodifica_mensagem(lista *l, FILE *MensagemDecodificada, int *arr_codigo, int tamanho)
{
    int i;
    char c;

    for (i = 0; i < tamanho; i++)
    {
        if (arr_codigo[i] == -100)
            fprintf(MensagemDecodificada, "\n");

        else
        {
            c = busca_chave(l, arr_codigo[i]);

            if (c == '\n')
            {
                printf("Erro ao decodificar mensagem.\n");
                printf("Lista de Chaves nao contem valor codificado.\n");
                return 0;
            }

            fprintf(MensagemDecodificada, "%c", c);
        }   
    }

    return 1;
}
