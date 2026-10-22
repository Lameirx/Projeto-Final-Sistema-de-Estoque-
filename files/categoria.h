#ifndef CATEGORIA_H
#define CATEGORIA_H

#define MAX_CATEGORIAS 20

typedef struct {
    int codigo;
    char nome[30];
} Categoria;

int buscarCategoriaPorCodigo(Categoria lista[], int total, int codigo);
int cadastrarCategoria(Categoria lista[], int total);
void listarCategorias(Categoria lista[], int total);

// Persistencia de dados
int carregarCategorias(Categoria lista[], const char *nomeArquivo);
void salvarCategorias(Categoria lista[], int total, const char *nomeArquivo);

#endif
