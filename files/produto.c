#include <stdio.h>
#include <string.h>
#include "produto.h"
#include "categoria.h"
#include "util.h"

// 1. Buscar produto por Codigo
int buscarPorCodigo(Produto lista[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

// 2. Buscar produto por Nome
int buscarPorNome(Produto lista[], int total, const char nome[]) {
    int i;
    for (i = 0; i < total; i++) {
        if (strcmp(lista[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

// 3. Exibir os dados de um unico produto
void exibirProduto(Produto p, Categoria listaCat[], int totalCat) {
    const char *nomeCategoria = "Desconhecida";
    int posCat = buscarCategoriaPorCodigo(listaCat, totalCat, p.codigoCategoria);
    if (posCat != -1) {
        nomeCategoria = listaCat[posCat].nome;
    }

    printf("------------------------------------\n");
    printf("Codigo:               %d\n", p.codigo);
    printf("Nome:                 %s\n", p.nome);
    printf("Categoria:            %s\n", nomeCategoria);
    printf("Prateleira:           %c\n", p.prateleira);
    printf("Preco unitario:       R$ %.2f\n", p.preco);
    printf("Quantidade em estoque: %d\n", p.quantidade);
    printf("Disponivel para venda: %s (%d)\n", p.disponivel == 1 ? "Sim" : "Nao", p.disponivel);
    printf("------------------------------------\n");
}

// 4. Cadastrar produto na lista
int cadastrarProduto(Produto lista[], int total, Categoria listaCat[], int totalCat) {
    Produto p;

    if (totalCat == 0) {
        printf("\n[ERRO] Nenhuma categoria cadastrada. Cadastre ao menos uma categoria antes de cadastrar produtos!\n\n");
        return total;
    }

    if (total >= MAX_PRODUTOS) {
        printf("\n[ERRO] A lista de produtos esta cheia (limite de %d produtos atingido)!\n\n", MAX_PRODUTOS);
        return total;
    }

    printf("\n=== CADASTRO DE PRODUTO ===\n");

    // Validacao do Codigo (unico e positivo)
    while (1) {
        p.codigo = lerIntPositivo("Digite o codigo do produto (positivo): ");
        if (buscarPorCodigo(lista, total, p.codigo) != -1) {
            printf("Erro: Ja existe um produto cadastrado com o codigo %d!\n", p.codigo);
        } else {
            break;
        }
    }

    // Leitura do Nome
    lerTextoNaoVazio("Digite o nome do produto: ", p.nome, sizeof(p.nome));

    // Exibicao e Validacao da Categoria
    printf("\n--- Categorias Disponiveis ---\n");
    listarCategorias(listaCat, totalCat);
    while (1) {
        p.codigoCategoria = lerIntPositivo("Digite o codigo da categoria do produto: ");
        if (buscarCategoriaPorCodigo(listaCat, totalCat, p.codigoCategoria) == -1) {
            printf("Erro: Categoria nao encontrada! Escolha um codigo valido da lista acima.\n");
        } else {
            break;
        }
    }

    // Leitura das demais informacoes
    p.prateleira = lerPrateleira("Digite a prateleira (A, B, C ou D): ");
    p.preco = lerFloatPositivo("Digite o preco unitario (R$): ");
    p.quantidade = lerIntNaoNegativo("Digite a quantidade em estoque: ");

    // Regra da disponibilidade (calculada automaticamente)
    p.disponivel = (p.quantidade > 0) ? 1 : 0;

    lista[total] = p;
    total++;

    printf("\n====================================\n");
    printf("     PRODUTO CADASTRADO COM SUCESSO \n");
    printf("====================================\n");
    exibirProduto(p, listaCat, totalCat);
    printf("\n");

    return total;
}

// 5. Listar todos os produtos
void listarProdutos(Produto lista[], int total, Categoria listaCat[], int totalCat) {
    int i;

    if (total == 0) {
        printf("\nNenhum produto cadastrado no momento.\n\n");
        return;
    }

    printf("\n====================================\n");
    printf("        LISTA DE PRODUTOS           \n");
    printf("====================================\n");
    for (i = 0; i < total; i++) {
        printf("Produto #%d:\n", i + 1);
        exibirProduto(lista[i], listaCat, totalCat);
    }
    printf("\n");
}

// 6. Pesquisar produto por Codigo ou Nome
void pesquisarProduto(Produto lista[], int total, Categoria listaCat[], int totalCat) {
    int opcaoBusca = 0;

    if (total == 0) {
        printf("\nNenhum produto cadastrado para pesquisar.\n\n");
        return;
    }

    printf("\n=== PESQUISAR PRODUTO ===\n");
    printf("1 - Por codigo\n");
    printf("2 - Por nome\n");
    if (!lerInteiro("Escolha uma opcao de pesquisa: ", &opcaoBusca)) {
        printf("\nOpcao de pesquisa invalida!\n\n");
        return;
    }

    if (opcaoBusca == 1) {
        int codigo = lerIntPositivo("Digite o codigo do produto: ");
        int pos = buscarPorCodigo(lista, total, codigo);
        if (pos != -1) {
            printf("\nProduto encontrado:\n");
            exibirProduto(lista[pos], listaCat, totalCat);
            printf("\n");
        } else {
            printf("\nNenhum produto encontrado com o codigo %d.\n\n", codigo);
        }
    } else if (opcaoBusca == 2) {
        char nome[50];
        int pos;
        lerTextoNaoVazio("Digite o nome do produto: ", nome, sizeof(nome));
        pos = buscarPorNome(lista, total, nome);
        if (pos != -1) {
            printf("\nProduto encontrado:\n");
            exibirProduto(lista[pos], listaCat, totalCat);
            printf("\n");
        } else {
            printf("\nNenhum produto encontrado com o nome \"%s\".\n\n", nome);
        }
    } else {
        printf("\nOpcao de pesquisa invalida!\n\n");
    }
}

// 7. Carregar dados do arquivo produtos.txt
int carregarProdutos(Produto lista[], const char *nomeArquivo) {
    int total = 0;
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        return 0; // Arquivo ainda nao existe: comeca com a lista vazia
    }

    while (total < MAX_PRODUTOS &&
           fscanf(arquivo, "%d;%49[^;];%c;%f;%d;%d;%d\n",
                  &lista[total].codigo,
                  lista[total].nome,
                  &lista[total].prateleira,
                  &lista[total].preco,
                  &lista[total].quantidade,
                  &lista[total].disponivel,
                  &lista[total].codigoCategoria) == 7) {
        total++;
    }

    fclose(arquivo);
    return total;
}

// 8. Salvar dados no arquivo produtos.txt
void salvarProdutos(Produto lista[], int total, const char *nomeArquivo) {
    int i;
    FILE *arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        printf("[ERRO] Nao foi possivel abrir o arquivo %s para escrita.\n", nomeArquivo);
        return;
    }

    for (i = 0; i < total; i++) {
        fprintf(arquivo, "%d;%s;%c;%.2f;%d;%d;%d\n",
                lista[i].codigo,
                lista[i].nome,
                lista[i].prateleira,
                lista[i].preco,
                lista[i].quantidade,
                lista[i].disponivel,
                lista[i].codigoCategoria);
    }

    fclose(arquivo);
}
