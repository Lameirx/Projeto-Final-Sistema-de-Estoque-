#ifndef PRODUTO_H
#define PRODUTO_H

#include "categoria.h"

#define MAX_PRODUTOS 50

typedef struct {
    int codigo;
    char nome[50];
    char prateleira;
    float preco;
    int quantidade;
    int disponivel;
    int codigoCategoria; // Campo adicionado na Tarefa 4
} Produto;

int buscarPorCodigo(Produto lista[], int total, int codigo);
int buscarPorNome(Produto lista[], int total, const char nome[]);
void exibirProduto(Produto p, Categoria listaCat[], int totalCat);
int cadastrarProduto(Produto lista[], int total, Categoria listaCat[], int totalCat);
void listarProdutos(Produto lista[], int total, Categoria listaCat[], int totalCat);
void pesquisarProduto(Produto lista[], int total, Categoria listaCat[], int totalCat);

// Persistencia de dados
int carregarProdutos(Produto lista[], const char *nomeArquivo);
void salvarProdutos(Produto lista[], int total, const char *nomeArquivo);

#endif
