#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "testa_entrada.h"
#include "gera_chaves.h"
#include "codifica.h"
#include "decodifica.h"
#define SIZE_LINE 100
#define SIZE 10

int Analisa_Livro (lista *l, FILE *LivroCifra, int *count, int *flag)
{
    char linha_livro[SIZE_LINE];

    fgets (linha_livro, SIZE_LINE, LivroCifra);
    while (!feof (LivroCifra))
    {
        if (!(identifica_chaves_livro(l, linha_livro, count, flag)))
            return 0;

        if (strchr(linha_livro, '\n') != NULL)
            *flag = 1;

        fgets (linha_livro, SIZE_LINE, LivroCifra);
    }

    if((!add_demais_chaves(l, *count)))
        return 0;

    return 1;
}

int Analisa_Mensagem_Codificada(FILE *MensagemCodificada, int **codigo)
{
    int i = 0;
    char c;

    while (fscanf(MensagemCodificada, "%d%*c", &(*codigo)[i]) != EOF)
    {
        i++;

        if ((c = fgetc(MensagemCodificada)) == '\n')
        {
            (*codigo)[i] = -100;
            i++;
        }
        else
        {
            ungetc(c, MensagemCodificada);
        }

        if (i >= SIZE-1)
        {
            if((*codigo = (int*) realloc(*codigo, (i + 2) * sizeof(int))) == NULL)
            {   
                printf("Erro ao realocar codigo\n");
                return -1;
            }
        }
    }

    return i;
}

int main (int argc, char **argv)
{
    int i, count = 0, flag = 1, *codigo;

    FILE *LivroCifra, *ArquivoDeChaves, *MensagemOriginal, *MensagemCodificada, *MensagemDecodificada;

    argumentos *a;

    a = BuscaArgumentos(argc, argv);

    if (a == NULL)
        return 1;

    lista *l;

    if (! (l = cria_lista()))
    {
        printf("Erro ao criar lista.");
        return 1;
    }

    if (a->Indicador > 0)
    {
        LivroCifra = fopen(a->LivroCifra, "r+");

        if(!LivroCifra)
        {
            printf("Erro ao abrir o Livro Cifra: %s\n", a->LivroCifra);
            return 1;
        }

        MensagemOriginal = fopen(a->MensagemOriginal, "r+");

        if(!MensagemOriginal)
        {
            printf("Erro ao abrir a Mensagem Original: %s\n", a->MensagemOriginal);
            return 1;
        }

        MensagemCodificada = fopen(a->MensagemCodificada, "w+");

        if(!MensagemCodificada)
        {
            printf("Erro ao abrir a Mensagem Codificada: %s\n", a->MensagemCodificada);
            return 1;
        }

        if (!Analisa_Livro (l, LivroCifra, &count, &flag))
            return 1;

        if (a->ArquivoDeChaves != NULL)
        {
            ArquivoDeChaves = fopen (a->ArquivoDeChaves, "w+");

            if(!ArquivoDeChaves)
            {
                printf("Erro ao abrir o Arquivo de Chaves: %s\n", a->ArquivoDeChaves);
                return 1;
            }

            insere_chaves_arquivo(l, ArquivoDeChaves);

            fclose(ArquivoDeChaves);
        }

        srand(time(NULL));
        flag = 0;

        char linha_mensagem_original[SIZE_LINE];

        fgets (linha_mensagem_original, SIZE_LINE, MensagemOriginal);
        while (!feof (MensagemOriginal))
        {
            if (strchr(linha_mensagem_original, '\n') != NULL)
                flag = 1;
            
            if (!codifica_mensagem(l, linha_mensagem_original, MensagemCodificada, &flag))
                return 1;
            
            fgets (linha_mensagem_original, SIZE_LINE, MensagemOriginal);
        }

        fclose(LivroCifra);
        fclose(MensagemOriginal);
        fclose(MensagemCodificada);
    }

    else if (a->ArquivoDeChaves != NULL)
    {
        char chave;

        ArquivoDeChaves = fopen (a->ArquivoDeChaves, "r+");

        if(!ArquivoDeChaves)
        {
            printf("Erro ao abrir o Arquivo de Chaves: %s\n", a->ArquivoDeChaves);
            return 1;
        }

        MensagemCodificada = fopen(a->MensagemCodificada, "r+");

        if(!MensagemCodificada)
        {
            printf("Erro ao abrir a Mensagem Codificada: %s\n", a->MensagemCodificada);
            return 1;
        }

        MensagemDecodificada = fopen(a->MensagemDecodificada, "w+");

        if(!MensagemDecodificada)
        {
            printf("Erro ao abrir a Mensagem Decodificada: %s\n", a->MensagemDecodificada);
            return 1;
        }

        if ((codigo = (int*) malloc (SIZE * sizeof(int))) == NULL)
        {
            printf("Erro ao alocar vetor para os codigos.\n");
            return 1;
        }

        fgetc(ArquivoDeChaves);
        while (fscanf(ArquivoDeChaves, "%c", &chave) != EOF)
        {
            fgetc(ArquivoDeChaves);
            fgetc(ArquivoDeChaves);

            i = 0;
            while (fscanf(ArquivoDeChaves, "%d", &codigo[i]) == 1)
            {
                i++;

                if (i >= SIZE)
                {
                    if((codigo = (int*) realloc(codigo, (i + 1) * sizeof(int))) == NULL)
                    {   
                        printf("Erro ao realocar codigo\n");
                        return 1;
                    }
                }
            }
            
            if (!identifica_chaves_arquivo(l, chave, codigo, i))
                return 1;

            fgetc(ArquivoDeChaves);
        }

        if ((i = Analisa_Mensagem_Codificada(MensagemCodificada, &codigo)) == -1)
            return 1;

        if (!decodifica_mensagem(l, MensagemDecodificada, codigo, i))
            return 1;

        free(codigo);
        fclose(ArquivoDeChaves);
        fclose(MensagemCodificada);
        fclose(MensagemDecodificada);
    }

    else
    {
        LivroCifra = fopen (a->LivroCifra, "r+");

        if(!LivroCifra)
        {
            printf("Erro ao abrir o Livro Cifra: %s\n", a->LivroCifra);
            return 1;
        }

        MensagemCodificada = fopen(a->MensagemCodificada, "r+");

        if(!MensagemCodificada)
        {
            printf("Erro ao abrir a Mensagem Codificada: %s\n", a->MensagemCodificada);
            return 1;
        }

        MensagemDecodificada = fopen(a->MensagemDecodificada, "w+");

        if(!MensagemDecodificada)
        {
            printf("Erro ao abrir a Mensagem Decodificada: %s\n", a->MensagemDecodificada);
            return 1;
        }

        if (!Analisa_Livro (l, LivroCifra, &count, &flag))
            return 1;

        if ((codigo = (int*) malloc (SIZE * sizeof(int))) == NULL)
        {
            printf("Erro ao alocar vetor para os codigos.\n");
            return 1;
        }

        if ((i = Analisa_Mensagem_Codificada(MensagemCodificada, &codigo)) == -1)
            return 1;

        if (!decodifica_mensagem(l, MensagemDecodificada, codigo, i))
            return 1;

        free(codigo);
        fclose(LivroCifra);
        fclose(MensagemCodificada);
        fclose(MensagemDecodificada);

    }

    destroi_lista(l);
    free_argumentos(a);

    return 0;
}