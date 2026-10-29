#ifndef UTIL_H
#define UTIL_H

// Funcoes GENERICAS de leitura e validacao de entrada (nao conhecem regras de produto
// nem de categoria). As regras especificas de cada dominio ficam em produto.c e categoria.c.

// Descarta o que sobrou na linha de entrada (ate o '\n').
void limparBuffer(void);

// Le um numero inteiro (qualquer valor). Retorna 1 se leu um numero valido
// e 0 se a entrada nao era um numero. Em caso de erro, *valor nao e alterado.
int lerInteiro(const char *mensagem, int *valor);

// Leituras com validacao: repetem a pergunta ate o usuario digitar algo valido.
int lerIntPositivo(const char *mensagem);                          // > 0
int lerIntNaoNegativo(const char *mensagem);                       // >= 0
int lerIntFaixa(const char *mensagem, int minimo, int maximo);     // minimo <= valor <= maximo
float lerFloatPositivo(const char *mensagem);                      // > 0
void lerTextoNaoVazio(const char *mensagem, char *destino, int tamanho);

// Le exatamente UM caractere (ignora espacos nas pontas).
// Retorna '\0' se a entrada estiver vazia ou tiver mais de um caractere.
char lerCaractere(const char *mensagem);

#endif
