Uma implementação em C do AES no modo CBC, permitindo a cifração e decifração de qualquer arquivo de entrada desejado, não necessarimaente se limitando a arquivos txt.

**Compilação:**
    gcc -o aes main.c aes.c

**Uso:**
    ./aes <cifrar|decifrar> <arquivo_entrada> <arquivo_saida> <chave_em_hex>

**Exemplo:**
    ./aes cifrar textoclaro.txt textocifrado.txt 00112233445566778899aabbccddeeff

**Observações:**
- A chave deve ser 128, 192 ou 256 bits (32, 48 ou 64 caracteres).
- O vetor de inicialização é fixo, porém pode ser modificado manualmente no main.c, linha 60.
- O formato de ambos arquivos de entrada e saída são definidos pelo usuário. Logo, se ele desejar decifrar e não conhecer a extensão original do arquivo, provavelmente encontrará problemas.
