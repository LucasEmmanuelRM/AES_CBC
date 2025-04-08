#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "aes_cbc.h"

#define MAX_TAMANHO_ARQUIVO (1024 * 1024 * 64) // 64 MB
#define AES_TAMANHO_BLOCO 16

void print_usage(const char *prog) {
    printf("Uso:\n");
    printf("  %s <cifrar|decifrar> <arquivo_entrada> <arquivo_saida> <chave> <chave_bits>\n", prog);
    printf("Exemplo:\n");
    printf("  %s cifrar entrada.txt saida.aes minha_chave 128 (ou 192 ou 256)\n", prog);
}

void gerar_iv_aleatorio(uint8_t *vetor_inicializador) {
    srand((unsigned int) time(NULL));
    for (int i = 0; i < AES_TAMANHO_BLOCO; ++i) {
        vetor_inicializador[i] = rand() % 256;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        print_usage(argv[0]);
        return 1;
    }

    const char *modo = argv[1];
    const char *arquivo_entrada = argv[2];
    const char *arquivo_saida = argv[3];
    const char *chave_str = argv[4];
    int chave_bits = atoi(argv[5]);
    int chave_len = chave_bits / 8;

    if (chave_bits != 128 && chave_bits != 192 && chave_bits != 256) {
        printf("Tamanho da chave inválido. Use 128, 192 ou 256.\n");
        return 1;
    }

    if ((int)strlen(chave_str) < chave_len) {
        printf("Chave curta demais. Forneça ao menos %d caracteres.\n", chave_len);
        return 1;
    }

    uint8_t chave[32] = {0};
    memcpy(chave, chave_str, chave_len);

    // Ler arquivo
    FILE *fin = fopen(arquivo_entrada, "rb");
    if (!fin) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    uint8_t *entrada = malloc(MAX_TAMANHO_ARQUIVO);
    if (!entrada) return 1;

    int entrada_len = fread(entrada, 1, MAX_TAMANHO_ARQUIVO, fin);
    fclose(fin);

    uint8_t *saida = malloc(MAX_TAMANHO_ARQUIVO + AES_TAMANHO_BLOCO * 2); // padding + vetor inicializador
    int saida_len = 0;

    FILE *fout = fopen(arquivo_saida, "wb");
    if (!fout) {
        perror("Erro ao criar arquivo de saída");
        return 1;
    }

    if (strcmp(modo, "cifrar") == 0) {
        uint8_t vetor_inicializador[AES_TAMANHO_BLOCO];
        gerar_iv_aleatorio(vetor_inicializador);

        // Padding
        int padded_len = pkcs7_pad(entrada, entrada_len, saida, AES_TAMANHO_BLOCO);

        // CBC
        aes_cbc_cifrar(saida, saida, padded_len, chave, chave_len, vetor_inicializador);

        // Escreve o vetor inicializador primeiro, depois os dados
        fwrite(vetor_inicializador, 1, AES_TAMANHO_BLOCO, fout);
        fwrite(saida, 1, padded_len, fout);
        printf("Criptografado com sucesso. Vetor Inicializador salvo no início do arquivo.\n");

    } else if (strcmp(modo, "decifrar") == 0) {
        uint8_t vetor_inicializador[AES_TAMANHO_BLOCO];

        // Lê o vetor inicializador do início
        memcpy(vetor_inicializador, entrada, AES_TAMANHO_BLOCO);
        entrada_len -= AES_TAMANHO_BLOCO;

        // Desloca ponteiro
        uint8_t *dados_cifrados = entrada + AES_TAMANHO_BLOCO;

        aes_cbc_decifrar(dados_cifrados, saida, entrada_len, chave, chave_len, vetor_inicializador);
        saida_len = pkcs7_unpad(saida, entrada_len);

        fwrite(saida, 1, saida_len, fout);
        printf("Arquivo descriptografado com sucesso.\n");

    } else {
        printf("Modo inválido. Use cifrar ou decifrar.\n");
        return 1;
    }

    fclose(fout);
    free(entrada);
    free(saida);

    return 0;
}