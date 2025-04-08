#include <stdint.h>
#include <string.h>

#define TAMANHO_BLOCO 16


static const uint8_t s_box[256] = {
  // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, // 0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, // 1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, // 2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, // 3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, // 4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, // 5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, // 6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, // 7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, // 8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, // 9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, // A
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, // B
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, // C
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, // D
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, // E
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16  // F
};


static const uint8_t s_box_reversa[256] = {
  // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, // 0
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, // 1
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, // 2
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, // 3
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, // 4
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, // 5
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, // 6
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, // 7
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, // 8
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, // 9
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, // A
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, // B
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, // C
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, // D
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, // E
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d  // F
};


static const uint8_t constante_rodada[255] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};


// Rotaciona uma palavra de 4 bytes para a esquerda
// É utilizada durante a expansão da chave (P0, P1, P2, P3) -> (P1, P2, P3, P0)
void shift_circular(uint8_t *palavra) {
    uint8_t tmp = palavra[0];
    palavra[0] = palavra[1];
    palavra[1] = palavra[2];
    palavra[2] = palavra[3];
    palavra[3] = tmp;
}


// Aplica a substituição de bytes de uma palavras de 4 bytes usando a S-Box
// É utilizada durante a expansão da chave (P1, P2, P3, P0) -> (P1', P2', P3', P0')
void sub_bytes(uint8_t *palavra) {
    for (int i = 0; i < 4; i++)
        palavra[i] = s_box[palavra[i]];
}


// Expande a chave inicial para gerar as chaves de cada rodada no AES
// Ela identifica o número de palavras segundo o tamanho da chave, que muda com o AES (128, 192, 256) = (4, 6 ou 8 palavras de 4 bytes)
// O número de rodadas, então, será 10, 12 ou 14
// O total de palavras de 4 bytes geradas pela expansão da chave varia segundo o número de rodadas (40, 48, 56) + 4 correspondente ao estado
void expandir_chave(uint8_t *chave, uint8_t *chave_de_rodada, int chave_len) {
    int numero_palavras = chave_len / 4;
    int numero_rodadas = numero_palavras + 6;
    int total_palavras = 4 * (numero_rodadas + 1);

    for (int i = 0; i < numero_palavras; i++) {
        chave_de_rodada[4 * i + 0] = chave[4 * i + 0];
        chave_de_rodada[4 * i + 1] = chave[4 * i + 1];
        chave_de_rodada[4 * i + 2] = chave[4 * i + 2];
        chave_de_rodada[4 * i + 3] = chave[4 * i + 3];
    }

    uint8_t temp[4];
    for (int i = numero_palavras; i < total_palavras; i++) {
        memcpy(temp, &chave_de_rodada[4 * (i - 1)], 4);

        if (i % numero_palavras == 0) {
            shift_circular(temp);
            sub_bytes(temp);
            temp[0] ^= constante_rodada[i / numero_palavras];
        } else if (numero_palavras > 6 && i % numero_palavras == 4) {
            sub_bytes(temp);
        }

        for (int j = 0; j < 4; j++) {
            chave_de_rodada[4 * i + j] = chave_de_rodada[4 * (i - numero_palavras) + j] ^ temp[j];
        }
    }
}


// Realiza um XOR entre a matriz estado e a chave da rodada
// Efetivamente uma adição em GF(2)
void add_chave_rodada(uint8_t *estado, uint8_t *round_key) {
    for (int i = 0; i < TAMANHO_BLOCO; i++)
        estado[i] ^= round_key[i];
}


// Aplica a substituição de bytes na matriz estado, também utilizando a S-BOX
void sub_bloco_bytes(uint8_t *estado) {
    for (int i = 0; i < TAMANHO_BLOCO; i++)
        estado[i] = s_box[estado[i]];
}


// O inverso da função acima
void sub_bloco_bytes_reverso(uint8_t *estado) {
    for (int i = 0; i < TAMANHO_BLOCO; i++)
        estado[i] = s_box_reversa[estado[i]];
}


// Deslocamento circular nas linhas da matriz estado durante a função de transformação
void shift_linhas(uint8_t *estado) {
    uint8_t tmp;

    tmp = estado[1]; estado[1] = estado[5]; estado[5] = estado[9]; estado[9] = estado[13]; estado[13] = tmp;

    tmp = estado[2]; estado[2] = estado[10]; estado[10] = tmp;
    tmp = estado[6]; estado[6] = estado[14]; estado[14] = tmp;

    tmp = estado[3]; estado[3] = estado[15]; estado[15] = estado[11]; estado[11] = estado[7]; estado[7] = tmp;
}


// O inverso da função acima
void shift_reverso_linhas(uint8_t *estado) {
    uint8_t tmp;

    tmp = estado[13]; estado[13] = estado[9]; estado[9] = estado[5]; estado[5] = estado[1]; estado[1] = tmp;

    tmp = estado[2]; estado[2] = estado[10]; estado[10] = tmp;
    tmp = estado[6]; estado[6] = estado[14]; estado[14] = tmp;

    tmp = estado[3]; estado[3] = estado[7]; estado[7] = estado[11]; estado[11] = estado[15]; estado[15] = tmp;
}


// Multiplicação em GF(2^8)
// Operação realizada durante o processo de misturar as colunas
// Se o bit mais significativo é 1 -> (b6 b5 b4 b3 b2 b1 b0 0) XOR (00011011)
// Se o bit mais significativo é 1, não faço nada com os bits da matriz estado
uint8_t mult_campo_galois(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int counter = 0; counter < 8; counter++) {
        if (b & 1)
            p ^= a;
        uint8_t hi_bit_set = a & 0x80;
        a <<= 1;
        if (hi_bit_set)
            a ^= 0x1b;
        b >>= 1;
    }
    return p;
}


// [02] [03] [01] [01]
// [01] [02] [03] [01]
// [01] [01] [02] [03]
// [03] [01] [01] [02]

// A matriz acima é utilizada para misturar as colunas da matriz estado utilizando multiplicação em GF(2^8)
void misturar_colunas(uint8_t *estado) {
    uint8_t tmp[16];
    memcpy(tmp, estado, 16);
    for (int i = 0; i < 4; i++) {
        int idx = i * 4;
        estado[idx + 0] = mult_campo_galois(0x02, tmp[idx + 0]) ^ mult_campo_galois(0x03, tmp[idx + 1]) ^ tmp[idx + 2] ^ tmp[idx + 3];
        estado[idx + 1] = tmp[idx + 0] ^ mult_campo_galois(0x02, tmp[idx + 1]) ^ mult_campo_galois(0x03, tmp[idx + 2]) ^ tmp[idx + 3];
        estado[idx + 2] = tmp[idx + 0] ^ tmp[idx + 1] ^ mult_campo_galois(0x02, tmp[idx + 2]) ^ mult_campo_galois(0x03, tmp[idx + 3]);
        estado[idx + 3] = mult_campo_galois(0x03, tmp[idx + 0]) ^ tmp[idx + 1] ^ tmp[idx + 2] ^ mult_campo_galois(0x02, tmp[idx + 3]);
    }
}


// O inverso da função acima
void misturar_colunas_reverso(uint8_t *estado) {
    uint8_t tmp[16];
    memcpy(tmp, estado, 16);
    for (int i = 0; i < 4; i++) {
        int idx = i * 4;
        estado[idx + 0] = mult_campo_galois(0x0e, tmp[idx + 0]) ^ mult_campo_galois(0x0b, tmp[idx + 1]) ^ mult_campo_galois(0x0d, tmp[idx + 2]) ^ mult_campo_galois(0x09, tmp[idx + 3]);
        estado[idx + 1] = mult_campo_galois(0x09, tmp[idx + 0]) ^ mult_campo_galois(0x0e, tmp[idx + 1]) ^ mult_campo_galois(0x0b, tmp[idx + 2]) ^ mult_campo_galois(0x0d, tmp[idx + 3]);
        estado[idx + 2] = mult_campo_galois(0x0d, tmp[idx + 0]) ^ mult_campo_galois(0x09, tmp[idx + 1]) ^ mult_campo_galois(0x0e, tmp[idx + 2]) ^ mult_campo_galois(0x0b, tmp[idx + 3]);
        estado[idx + 3] = mult_campo_galois(0x0b, tmp[idx + 0]) ^ mult_campo_galois(0x0d, tmp[idx + 1]) ^ mult_campo_galois(0x09, tmp[idx + 2]) ^ mult_campo_galois(0x0e, tmp[idx + 3]);
    }
}


// Aplicação do AES de forma pura para realizar a cifração
void aes_cifra(uint8_t *entrada, uint8_t *saida, uint8_t *chave, int chave_len) {
    uint8_t estado[TAMANHO_BLOCO];
    uint8_t chave_de_rodada[240];

    int numero_palavras = chave_len / 4;
    int numero_rodadas = numero_palavras + 6;

    expandir_chave(chave, chave_de_rodada, chave_len);
    memcpy(estado, entrada, TAMANHO_BLOCO);

    add_chave_rodada(estado, chave_de_rodada);
    for (int i = 1; i < numero_rodadas; i++) {
        sub_bloco_bytes(estado);
        shift_linhas(estado);
        misturar_colunas(estado);
        add_chave_rodada(estado, chave_de_rodada + (i * TAMANHO_BLOCO));
    }
    sub_bloco_bytes(estado);
    shift_linhas(estado);
    add_chave_rodada(estado, chave_de_rodada + (numero_rodadas * TAMANHO_BLOCO));

    memcpy(saida, estado, TAMANHO_BLOCO);
}


// Aplicação do AES de forma pura para realizar a decifração
void aes_decifra(uint8_t *entrada, uint8_t *saida, uint8_t *chave, int chave_len) {
    uint8_t estado[TAMANHO_BLOCO];
    uint8_t chave_de_rodada[240];

    int numero_palavras = chave_len / 4;
    int numero_rodadas = numero_palavras + 6;

    expandir_chave(chave, chave_de_rodada, chave_len);
    memcpy(estado, entrada, TAMANHO_BLOCO);

    add_chave_rodada(estado, chave_de_rodada + (numero_rodadas * TAMANHO_BLOCO));
    for (int i = numero_rodadas - 1; i > 0; i--) {
        shift_reverso_linhas(estado);
        sub_bloco_bytes_reverso(estado);
        add_chave_rodada(estado, chave_de_rodada + (i * TAMANHO_BLOCO));
        misturar_colunas_reverso(estado);
    }
    shift_reverso_linhas(estado);
    sub_bloco_bytes_reverso(estado);
    add_chave_rodada(estado, chave_de_rodada);

    memcpy(saida, estado, TAMANHO_BLOCO);
}


// Aplicação do AES no modo CBC para realizar a cifração
// A função de cifrar com o AES puro é utilizada
void aes_cbc_cifrar(uint8_t *entrada, uint8_t *saida, int tamanho, uint8_t *chave, int chave_len, uint8_t *vetor_inicializador) {
    uint8_t bloco[TAMANHO_BLOCO];
    uint8_t xor_bloco[TAMANHO_BLOCO];
    memcpy(xor_bloco, vetor_inicializador, TAMANHO_BLOCO);

    for (int i = 0; i < tamanho; i += TAMANHO_BLOCO) {
        for (int j = 0; j < TAMANHO_BLOCO; j++)
            bloco[j] = entrada[i + j] ^ xor_bloco[j];

        aes_cifra(bloco, saida + i, chave, chave_len);
        memcpy(xor_bloco, saida + i, TAMANHO_BLOCO);
    }
}


// Aplicação do AES no modo CBC para realizar a decifração
// A função de decifrar com o AES puro é utilizada
void aes_cbc_decifrar(uint8_t *entrada, uint8_t *saida, int tamanho, uint8_t *chave, int chave_len, uint8_t *vetor_inicializador) {
    uint8_t bloco[TAMANHO_BLOCO];
    uint8_t xor_bloco[TAMANHO_BLOCO];
    memcpy(xor_bloco, vetor_inicializador, TAMANHO_BLOCO);

    for (int i = 0; i < tamanho; i += TAMANHO_BLOCO) {
        aes_decifra(entrada + i, bloco, chave, chave_len);
        for (int j = 0; j < TAMANHO_BLOCO; j++)
            saida[i + j] = bloco[j] ^ xor_bloco[j];

        memcpy(xor_bloco, entrada + i, TAMANHO_BLOCO);
    }
}


// PKCS#7 Padding para permitir o uso de qualquer entrada, não importando o tamanho do arquivo
int pkcs7_pad(uint8_t *entrada, int input_len, uint8_t *saida, int tamanho_bloco) {
    int pad_len = tamanho_bloco - (input_len % tamanho_bloco);
    memcpy(saida, entrada, input_len);
    for (int i = 0; i < pad_len; i++)
        saida[input_len + i] = (uint8_t)pad_len;
    return input_len + pad_len;
}

int pkcs7_unpad(uint8_t *entrada, int input_len) {
    if (input_len == 0) return 0;
    uint8_t pad_len = entrada[input_len - 1];
    if (pad_len > TAMANHO_BLOCO || pad_len == 0) return input_len;
    for (int i = 0; i < pad_len; i++) {
        if (entrada[input_len - 1 - i] != pad_len)
            return input_len;
    }
    return input_len - pad_len;
}
