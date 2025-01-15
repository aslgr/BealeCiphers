#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera_chaves.h"
#include "codifica.h"

int busca_codigo(lista *l, char chave)
{
    nodo *aux = l->head;

    if (chave >= 'A' && chave <= 'Z')
        chave += 32;

    while(aux != NULL && aux->chave != chave)
        aux = aux->prox;

    if(aux != NULL)
        return aux->codigo[rand() % aux->tamanho];

    printf("Erro. Nao foi possivel criptografar a mensagem com base no Livro Cifra dado.\n");
    return -100;
}

int codifica_mensagem(lista *l, char *linha_mensagem_original, FILE *MensagemCodificada, int *flag)
{
    int i, codigo;

    for (i = 0; i < strlen(linha_mensagem_original)-*flag; i++)
    {
        if((codigo = busca_codigo(l, linha_mensagem_original[i])) == -100)
            return 0;

        fprintf(MensagemCodificada, "%d ", codigo);
    }

    if (flag)
    {
        fprintf(MensagemCodificada, "\n");
        *flag = 0;
    }

    return 1;
}
