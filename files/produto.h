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
    int disponivel;      // sempre calculado a partir da quantidade (1 se quantidade > 0)
    int codigoCategoria; // Campo adicionado na Tarefa 4
    int ativo;           // Campo adicionado na Tarefa 5: 1 = normal, 0 = removido (exclusao logica)
} Produto;

// Busca em TODOS os registros, inclusive removidos (usada para garantir codigo unico)
int buscarPorCodigo(Produto lista[], int total, int codigo);
// Busca apenas entre os produtos ATIVOS (usada por pesquisar, alterar estoque e remover)
int buscarAtivoPorCodigo(Produto lista[], int total, int codigo);
// Busca apenas entre os produtos ATIVOS
int buscarPorNome(Produto lista[], int total, const char nome[]);

void exibirProduto(Produto p, Categoria listaCat[], int totalCat);
int cadastrarProduto(Produto lista[], int total, Categoria listaCat[], int totalCat);
void listarProdutos(Produto lista[], int total, Categoria listaCat[], int totalCat);
void pesquisarProduto(Produto lista[], int total, Categoria listaCat[], int totalCat);

// Tarefa 5
void alterarEstoque(Produto lista[], int total, Categoria listaCat[], int totalCat);
void removerProduto(Produto lista[], int total);

// Persistencia de dados
int carregarProdutos(Produto lista[], const char *nomeArquivo);
void salvarProdutos(Produto lista[], int total, const char *nomeArquivo);

#endif
