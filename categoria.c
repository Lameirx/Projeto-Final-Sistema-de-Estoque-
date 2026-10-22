#include <stdio.h>
#include <string.h>
#include "categoria.h"
#include "util.h"

int buscarCategoriaPorCodigo(Categoria lista[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

int cadastrarCategoria(Categoria lista[], int total) {
    Categoria c;

    if (total >= MAX_CATEGORIAS) {
        printf("\n[ERRO] O limite de categorias (%d) foi atingido!\n\n", MAX_CATEGORIAS);
        return total;
    }

    printf("\n=== CADASTRO DE CATEGORIA ===\n");

    while (1) {
        c.codigo = lerIntPositivo("Digite o codigo da categoria (positivo): ");
        if (buscarCategoriaPorCodigo(lista, total, c.codigo) != -1) {
            printf("Erro: Ja existe uma categoria cadastrada com o codigo %d!\n", c.codigo);
        } else {
            break;
        }
    }

    lerTextoNaoVazio("Digite o nome da categoria: ", c.nome, sizeof(c.nome));

    lista[total] = c;
    total++;

    printf("\n====================================\n");
    printf("   CATEGORIA CADASTRADA COM SUCESSO \n");
    printf("====================================\n");
    printf("Codigo: %d | Nome: %s\n", c.codigo, c.nome);
    printf("====================================\n\n");

    return total;
}

void listarCategorias(Categoria lista[], int total) {
    int i;

    if (total == 0) {
        printf("\nNenhuma categoria cadastrada no momento.\n\n");
        return;
    }

    printf("\n====================================\n");
    printf("        LISTA DE CATEGORIAS         \n");
    printf("====================================\n");
    for (i = 0; i < total; i++) {
        printf("Codigo: %d | Nome: %s\n", lista[i].codigo, lista[i].nome);
    }
    printf("------------------------------------\n\n");
}

int carregarCategorias(Categoria lista[], const char *nomeArquivo) {
    int total = 0;
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        return 0; // Arquivo ainda nao existe: comeca com a lista vazia
    }

    while (total < MAX_CATEGORIAS &&
           fscanf(arquivo, "%d;%29[^\n]\n", &lista[total].codigo, lista[total].nome) == 2) {
        total++;
    }

    fclose(arquivo);
    return total;
}

void salvarCategorias(Categoria lista[], int total, const char *nomeArquivo) {
    int i;
    FILE *arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        printf("[ERRO] Nao foi possivel abrir o arquivo %s para escrita.\n", nomeArquivo);
        return;
    }

    for (i = 0; i < total; i++) {
        fprintf(arquivo, "%d;%s\n", lista[i].codigo, lista[i].nome);
    }

    fclose(arquivo);
}
