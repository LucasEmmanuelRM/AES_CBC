Uma implementação em C do AES (Advanced Encryption Standard) no modo CBC (Cipher Block Chaining), permitindo a cifração e decifração de qualquer arquivo de entrada desejado, não necessarimaente se limitando a arquivos txt.

**Compilação:**
    gcc -o aes_cb main.c aes_cbc.c

**Uso:**
    ./aes_cbc <cifrar|decifrar> <arquivo_entrada> <arquivo_saida> <chave_secreta> <tamanho_chave_bits>

**Exemplo:**
    ./aes_cbc cifrar arquivoclaro.pdf arquivocifrado.pdf SbubblesSbubbles 128

**Observações:**
- A chave deve ser 128, 192 ou 256 bits (32, 48 ou 64 caracteres). Com um comprimento maior, bytes em excesso são ignorados;
- O comprimento mínimo da chave é de 16 caracteres;
- O vetor de inicialização é gerado aleatoriamente durante a cifração e é salvo no início da saída. A decifração resgata esse vetor buscando-o no início do arquivo cifrado;
- O formato de ambos arquivos de entrada e saída são definidos pelo usuário. Logo, se ele desejar decifrar e não conhecer a extensão original do arquivo, provavelmente encontrará problemas;
- Foi implementado padding **PKCS#7** para garantir alinhamento de blocos, permitindo o uso do CBC eficientemente;
- IMPORTANTE: O TAMANHO MÁXIMO SUPORTADO DO ARQUIVO DE ENTRADA É, POR PADRÃO, 64 MB. Isso pode ser modificado na main, linha 8.

**Esquema Visual de uma Cifração:**

【Entrada (ex: 52 bytes)】

          ↓   PKCS#7
 
【Pad até múltiplo de 16 bytes (agora 64 bytes)】

          ↓   AES CBC Encrypt
 
【Divide em blocos, aplicando XOR + AES neles】

          ↓
 
【Concatena blocos cifrados】

          ↓
 
【Grava em disco (com vetor inicializador no início)】
