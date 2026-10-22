#ifndef UTIL_H
#define UTIL_H

// Descarta o que sobrou na linha de entrada (ate o '\n').
void limparBuffer(void);

// Le um numero inteiro (qualquer valor). Retorna 1 se leu um numero valido
// e 0 se a entrada nao era um numero. Em caso de erro, *valor nao e alterado.
int lerInteiro(const char *mensagem, int *valor);

// Leituras com validacao: repetem a pergunta ate o usuario digitar algo valido.
int lerIntPositivo(const char *mensagem);
int lerIntNaoNegativo(const char *mensagem);
float lerFloatPositivo(const char *mensagem);
void lerTextoNaoVazio(const char *mensagem, char *destino, int tamanho);
char lerPrateleira(const char *mensagem);

#endif
