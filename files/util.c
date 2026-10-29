#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <float.h>
#include "util.h"

#define TAM_LINHA 128

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Le UMA linha inteira do teclado (sem o '\n') para 'destino'.
// Retorna 1 se a linha coube no buffer e 0 se era maior (o excesso e descartado).
// Se a entrada terminar (EOF), encerra o programa para nao ficar em loop infinito.
// Como toda leitura consome a linha inteira, nunca sobra lixo no buffer.
static int lerLinha(char *destino, int tamanho) {
    size_t len;
    int c;

    if (fgets(destino, tamanho, stdin) == NULL) {
        printf("\n[ERRO] Entrada encerrada. Finalizando o programa.\n");
        exit(1);
    }

    len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n') {
        destino[len - 1] = '\0';
        return 1;
    }

    // Nao achou '\n': o buffer encheu (ou era a ultima linha do arquivo)
    c = getchar();
    if (c == '\n' || c == EOF) {
        return 1; // coube exatamente
    }
    limparBuffer(); // havia excesso: descarta o resto da linha
    return 0;
}

// Remove espacos do inicio e do fim do texto
static void aparar(char *texto) {
    size_t ini = 0;
    size_t fim = strlen(texto);

    while (fim > 0 && isspace((unsigned char)texto[fim - 1])) {
        fim--;
    }
    texto[fim] = '\0';

    while (isspace((unsigned char)texto[ini])) {
        ini++;
    }
    if (ini > 0) {
        memmove(texto, texto + ini, fim - ini + 1);
    }
}

// Converte texto em int. Rejeita "abc", "12abc", "1.5" e valores fora do intervalo.
static int converterInteiro(const char *texto, int *valor) {
    char *fim;
    long numero;

    errno = 0;
    numero = strtol(texto, &fim, 10);
    if (fim == texto) {
        return 0; // nenhum digito
    }
    while (isspace((unsigned char)*fim)) {
        fim++;
    }
    if (*fim != '\0') {
        return 0; // sobrou lixo depois do numero
    }
    if (errno == ERANGE || numero > INT_MAX || numero < INT_MIN) {
        return 0;
    }
    *valor = (int)numero;
    return 1;
}

// Converte texto em float. Aceita virgula ou ponto como separador decimal (12,50 ou 12.50).
static int converterDecimal(const char *texto, float *valor) {
    char copia[TAM_LINHA];
    char *fim;
    char *p;
    double numero;

    strncpy(copia, texto, sizeof(copia) - 1);
    copia[sizeof(copia) - 1] = '\0';
    for (p = copia; *p != '\0'; p++) {
        if (*p == ',') {
            *p = '.';
        }
    }

    errno = 0;
    numero = strtod(copia, &fim);
    if (fim == copia) {
        return 0;
    }
    while (isspace((unsigned char)*fim)) {
        fim++;
    }
    if (*fim != '\0') {
        return 0;
    }
    // rejeita overflow, infinito e NaN
    if (errno == ERANGE || numero != numero || numero > FLT_MAX || numero < -FLT_MAX) {
        return 0;
    }
    *valor = (float)numero;
    return 1;
}

int lerInteiro(const char *mensagem, int *valor) {
    char linha[TAM_LINHA];
    int numero;

    printf("%s", mensagem);
    if (!lerLinha(linha, sizeof(linha)) || !converterInteiro(linha, &numero)) {
        return 0;
    }
    *valor = numero;
    return 1;
}

int lerIntPositivo(const char *mensagem) {
    int valor;
    while (1) {
        if (!lerInteiro(mensagem, &valor)) {
            printf("Erro: Entrada invalida! Digite um numero inteiro.\n");
            continue;
        }
        if (valor > 0) {
            return valor;
        }
        printf("Erro: O valor deve ser um numero maior que zero!\n");
    }
}

int lerIntNaoNegativo(const char *mensagem) {
    int valor;
    while (1) {
        if (!lerInteiro(mensagem, &valor)) {
            printf("Erro: Entrada invalida! Digite um numero inteiro.\n");
            continue;
        }
        if (valor >= 0) {
            return valor;
        }
        printf("Erro: O valor nao pode ser negativo!\n");
    }
}

int lerIntFaixa(const char *mensagem, int minimo, int maximo) {
    int valor;
    while (1) {
        if (!lerInteiro(mensagem, &valor)) {
            printf("Erro: Entrada invalida! Digite um numero inteiro.\n");
            continue;
        }
        if (valor >= minimo && valor <= maximo) {
            return valor;
        }
        printf("Erro: Digite um valor entre %d e %d!\n", minimo, maximo);
    }
}

float lerFloatPositivo(const char *mensagem) {
    char linha[TAM_LINHA];
    float valor;
    while (1) {
        printf("%s", mensagem);
        if (!lerLinha(linha, sizeof(linha)) || !converterDecimal(linha, &valor)) {
            printf("Erro: Entrada invalida! Digite um numero valido.\n");
            continue;
        }
        if (valor > 0) {
            return valor;
        }
        printf("Erro: O valor deve ser um numero maior que zero!\n");
    }
}

void lerTextoNaoVazio(const char *mensagem, char *destino, int tamanho) {
    while (1) {
        printf("%s", mensagem);
        if (!lerLinha(destino, tamanho)) {
            printf("Erro: O texto deve ter no maximo %d caracteres!\n", tamanho - 1);
            continue;
        }
        aparar(destino);
        if (destino[0] == '\0') {
            printf("Erro: O texto nao pode ficar em branco!\n");
            continue;
        }
        // ';' e o separador dos campos nos arquivos .txt: se entrasse no nome,
        // o carregamento dos dados ficaria corrompido.
        if (strchr(destino, ';') != NULL) {
            printf("Erro: O texto nao pode conter o caractere ';'!\n");
            continue;
        }
        return;
    }
}

char lerCaractere(const char *mensagem) {
    char linha[TAM_LINHA];
    int i = 0;
    int j;

    printf("%s", mensagem);
    if (!lerLinha(linha, sizeof(linha))) {
        return '\0';
    }
    while (isspace((unsigned char)linha[i])) {
        i++;
    }
    if (linha[i] == '\0') {
        return '\0'; // linha em branco
    }
    j = i + 1;
    while (isspace((unsigned char)linha[j])) {
        j++;
    }
    if (linha[j] != '\0') {
        return '\0'; // mais de um caractere
    }
    return linha[i];
}
