/* Header da biblioteca "testa_entrada", cujo
proposito e verificar a corretude dos argumentos
utilizados na execucao do programa "beale". */

typedef struct argumentos
{
    int Indicador;

    char *LivroCifra, *MensagemOriginal,
         *MensagemCodificada, *ArquivoDeChaves,
         *MensagemDecodificada;

} argumentos;

argumentos *BuscaArgumentos(int argc, char *argv[]);

void free_argumentos(argumentos *a);