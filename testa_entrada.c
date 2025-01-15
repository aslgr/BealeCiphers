#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "testa_entrada.h"

void free_argumentos(argumentos *a)
{
    if (a != NULL)
    {
        if (a->ArquivoDeChaves != NULL)
            free(a->ArquivoDeChaves);

        if (a->LivroCifra != NULL)
            free(a->LivroCifra);

        if (a->MensagemCodificada != NULL)
            free(a->MensagemCodificada);
        
        if (a->MensagemOriginal !=NULL)
            free(a->MensagemOriginal);
            
        if (a->MensagemDecodificada != NULL)
            free(a->MensagemDecodificada);

        free(a);
    }

    return;
}

void reporta_erro(argumentos *a)
{
    printf("\nAlgo deu errado.\n");
    printf("Verifique a corretude dos argumentos\n");
    printf("utilizados na execucao do programa.\n\n");

    free_argumentos(a);

    return;
}

void reporta_erro_indicador(argumentos *a)
{
    printf("\nAlgo deu errado.\n");
    printf("Priorize o argumento que indica ao programa\n");
    printf("qual instrucao devera ser executada.\n\n");

    free_argumentos(a);

    return;
}

argumentos *BuscaArgumentos(int argc, char *argv[])
{
    argumentos *a;
    if ((a = (argumentos*) malloc (sizeof(argumentos))) == NULL)
        return NULL;

    int i;

    a->Indicador = 0;
    a->ArquivoDeChaves = a->LivroCifra = a->MensagemCodificada = a->MensagemOriginal = a->MensagemDecodificada = NULL;

    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-e") == 0)
        {
            if (a->Indicador != 0)
            {
                reporta_erro(a);
                return NULL;
            }

            a->Indicador = 1;
        }

        else if (strcmp(argv[i], "-d") == 0)
        {
            if (a->Indicador != 0)
            {
                reporta_erro(a);
                return NULL;
            }
            
            a->Indicador = -1;
        }

        else if (strcmp(argv[i], "-b") == 0)
        {
            if(a->LivroCifra != NULL || argv[i+1] == NULL)
            {
                reporta_erro(a);
                return NULL;
            }

            a->LivroCifra = (char*) malloc ((strlen(argv[i+1])+1) * sizeof(char));

            strcpy(a->LivroCifra, argv[i+1]);
        }

        else if (strcmp(argv[i], "-m") == 0)
        {
            if(a->MensagemOriginal != NULL || argv[i+1] == NULL)
            {
                reporta_erro(a);
                return NULL;
            }

            a->MensagemOriginal = (char*) malloc ((strlen(argv[i+1])+1) * sizeof(char));

            strcpy(a->MensagemOriginal, argv[i+1]);
        }

        else if (strcmp(argv[i], "-i") == 0)
        {
            if(a->MensagemCodificada != NULL || argv[i+1] == NULL)
            {
                reporta_erro(a);
                return NULL;
            }

            a->MensagemCodificada = (char*) malloc ((strlen(argv[i+1])+1) * sizeof(char));

            strcpy(a->MensagemCodificada, argv[i+1]);
        }

        else if (strcmp(argv[i], "-c") == 0)
        {
            if(a->ArquivoDeChaves != NULL || argv[i+1] == NULL)
            {
                reporta_erro(a);
                return NULL;
            }

            a->ArquivoDeChaves = (char*) malloc ((strlen(argv[i+1])+1) * sizeof(char));

            strcpy(a->ArquivoDeChaves, argv[i+1]);
        }

        else if (strcmp(argv[i], "-o") == 0)
        {
            if (a->Indicador == 0)
            {
                reporta_erro_indicador(a);
                return NULL;
            }
            else if (a->Indicador > 0)
            {
                if(a->MensagemCodificada != NULL || argv[i+1] == NULL)
                {
                    reporta_erro(a);
                    return NULL;
                }

                a->MensagemCodificada = (char*) malloc ((strlen(argv[i+1])+1) * sizeof(char));

                strcpy(a->MensagemCodificada, argv[i+1]);
            }
            else if (a->Indicador < 0)
            {
                if(a->MensagemDecodificada != NULL || argv[i+1] == NULL)
                {
                    reporta_erro(a);
                    return NULL;
                }

                a->MensagemDecodificada = (char*) malloc ((strlen(argv[i+1])+1) * sizeof(char));

                strcpy(a->MensagemDecodificada, argv[i+1]);
            }
        }
    }

    if ((a->Indicador > 0 && a->LivroCifra != NULL && a->MensagemOriginal != NULL && a->MensagemCodificada != NULL) || 
         (a->Indicador < 0 && a->MensagemCodificada != NULL && a->MensagemDecodificada != NULL
         && ((a->ArquivoDeChaves != NULL) ^ (a->LivroCifra != NULL))))
    {
        return a;
    }

    reporta_erro(a);
    return NULL;
}
