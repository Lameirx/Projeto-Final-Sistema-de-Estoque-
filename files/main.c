#include <stdio.h>
#include "produto.h"
#include "categoria.h"
#include "util.h"

#define ARQ_CATEGORIAS "categorias.txt"
#define ARQ_PRODUTOS "produtos.txt"

void menuCategorias(Categoria listaCat[], int *totalCat) {
    int opcaoCat = 0; // inicializada: o 'continue' abaixo testa a condicao do while

    do {
        limparTela();
        printf("=== MENU DE CATEGORIAS ===\n");
        printf("1 - Cadastrar Categoria\n");
        printf("2 - Listar Categorias\n");
        printf("3 - Voltar\n");

        if (!lerInteiro("Escolha uma opcao: ", &opcaoCat)) {
            printf("\nOpcao invalida! Digite apenas numeros.\n\n");
            pausar();
            continue;
        }

        limparTela(); // a tela do menu some e a operacao escolhida aparece sozinha

        switch (opcaoCat) {
            case 1:
                *totalCat = cadastrarCategoria(listaCat, *totalCat);
                salvarCategorias(listaCat, *totalCat, ARQ_CATEGORIAS); // Salvamento imediato por seguranca
                pausar();
                break;
            case 2:
                listarCategorias(listaCat, *totalCat);
                pausar();
                break;
            case 3:
                break; // volta ao menu principal
            default:
                printf("\nOpcao invalida! Tente novamente.\n\n");
                pausar();
                break;
        }
    } while (opcaoCat != 3);
}

int main(void) {
    Categoria categorias[MAX_CATEGORIAS];
    int totalCategorias = 0;

    Produto estoque[MAX_PRODUTOS];
    int totalProdutos = 0;

    int opcao = 0; // inicializada: o 'continue' abaixo testa a condicao do while

    // Carrega dados persistidos ao iniciar o sistema
    totalCategorias = carregarCategorias(categorias, ARQ_CATEGORIAS);
    totalProdutos = carregarProdutos(estoque, ARQ_PRODUTOS);

    do {
        limparTela();
        printf("=== SISTEMA DE ESTOQUE ===\n");
        printf("1 - Cadastrar Produto\n");
        printf("2 - Listar Produtos\n");
        printf("3 - Pesquisar Produto\n");
        printf("4 - Alterar Estoque de um Produto\n");
        printf("5 - Remover Produto\n");
        printf("6 - Categorias\n");
        printf("7 - Sair\n");

        if (!lerInteiro("Escolha uma opcao: ", &opcao)) {
            printf("\nOpcao invalida! Digite apenas numeros.\n\n");
            pausar();
            continue;
        }

        limparTela(); // a tela do menu some e a operacao escolhida aparece sozinha

        switch (opcao) {
            case 1:
                totalProdutos = cadastrarProduto(estoque, totalProdutos, categorias, totalCategorias);
                salvarProdutos(estoque, totalProdutos, ARQ_PRODUTOS); // Salvamento imediato por seguranca
                pausar();
                break;
            case 2:
                listarProdutos(estoque, totalProdutos, categorias, totalCategorias);
                pausar();
                break;
            case 3:
                pesquisarProduto(estoque, totalProdutos, categorias, totalCategorias);
                pausar();
                break;
            case 4:
                alterarEstoque(estoque, totalProdutos, categorias, totalCategorias);
                salvarProdutos(estoque, totalProdutos, ARQ_PRODUTOS);
                pausar();
                break;
            case 5:
                removerProduto(estoque, totalProdutos);
                salvarProdutos(estoque, totalProdutos, ARQ_PRODUTOS);
                pausar();
                break;
            case 6:
                menuCategorias(categorias, &totalCategorias);
                break;
            case 7:
                // Persistencia ao sair
                salvarCategorias(categorias, totalCategorias, ARQ_CATEGORIAS);
                salvarProdutos(estoque, totalProdutos, ARQ_PRODUTOS);
                printf("\nDados salvos com sucesso! Saindo do sistema... Ate logo!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n\n");
                pausar();
                break;
        }
    } while (opcao != 7);

    return 0;
}
