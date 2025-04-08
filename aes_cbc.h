#ifndef AES_CBC_H
#define AES_CBC_H

#include <stdint.h>

#define TAMANHO_BLOCO 16

void aes_cbc_cifrar(uint8_t *entrada, uint8_t *saida, int tamanho, uint8_t *chave, int chave_len, uint8_t *vetor_inicializador);
void aes_cbc_decifrar(uint8_t *entrada, uint8_t *saida, int tamanho, uint8_t *chave, int chave_len, uint8_t *vetor_inicializador);

// --- Padding PKCS#7 ---
int pkcs7_pad(uint8_t *entrada, int input_len, uint8_t *saida, int tamanho_bloco);
int pkcs7_unpad(uint8_t *entrada, int input_len);

#endif // AES_CBC_H
