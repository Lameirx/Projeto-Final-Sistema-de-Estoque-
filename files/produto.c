#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "produto.h"
#include "categoria.h"
#include "util.h"
 
#define TAM_LINHA_ARQ 256
 
// ---------------------------------------------------------------------------
// Funcoes internas (regras do dominio Produto)
// ---------------------------------------------------------------------------
 
// Regra unica da disponibilidade: SEMPRE derivada da quantidade.
// Cadastro, alteracao de estoque e carregamento do arquivo usam esta mesma funcao.
static void atualizarDisponibilidade(Produto *p) {
    p->disponivel = (p->quantidade > 0) ? 1 : 0;
}
 
// Prateleira so aceita A, B, C ou D (validacao especifica de produto)
static char lerPrateleira(const char *mensagem) {
    char p;
    while (1) {
        p = (char)toupper((unsigned char)lerCaractere(mensagem));
        if (p == 'A' || p == 'B' || p == 'C' || p == 'D') {
            return p;
        }
        printf("Erro: Prateleira invalida! Escolha apenas entre A, B, C ou D.\n");
    }
}
 
// Quantidade de produtos ativos (os removidos nao contam)
static int contarAtivos(Produto lista[], int total) {
    int i;
    int n = 0;
    for (i = 0; i < total; i++) {
        if (lista[i].ativo == 1) {
            n++;
        }
    }
    return n;
}
 
// ---------------------------------------------------------------------------
// Buscas
// ---------------------------------------------------------------------------
 
// 1. Buscar produto por Codigo (inclui removidos: o codigo continua "reservado")
int buscarPorCodigo(Produto lista[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}
 
// 1b. Buscar produto ATIVO por Codigo
int buscarAtivoPorCodigo(Produto lista[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].ativo == 1 && lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}
 
// 2. Buscar produto ATIVO por Nome
int buscarPorNome(Produto lista[], int total, const char nome[]) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].ativo == 1 && strcmp(lista[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}
 
// ---------------------------------------------------------------------------
// Operacoes
// ---------------------------------------------------------------------------
 
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
    int pos;
 
    if (totalCat == 0) {
        printf("\n[ERRO] Nenhuma categoria cadastrada. Cadastre ao menos uma categoria antes de cadastrar produtos!\n\n");
        return total;
    }
 
    if (total >= MAX_PRODUTOS) {
        printf("\n[ERRO] A lista de produtos esta cheia (limite de %d produtos atingido)!\n\n", MAX_PRODUTOS);
        return total;
    }
 
    printf("\n=== CADASTRO DE PRODUTO ===\n");
 
    // Validacao do Codigo (unico e positivo). Codigos de produtos removidos tambem contam,
    // pois o registro continua existindo na lista e no arquivo.
    while (1) {
        p.codigo = lerIntPositivo("Digite o codigo do produto (positivo): ");
        pos = buscarPorCodigo(lista, total, p.codigo);
        if (pos == -1) {
            break;
        }
        if (lista[pos].ativo == 1) {
            printf("Erro: Ja existe um produto cadastrado com o codigo %d!\n", p.codigo);
        } else {
            printf("Erro: O codigo %d pertence a um produto removido e nao pode ser reutilizado!\n", p.codigo);
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
 
    // Disponibilidade calculada automaticamente; produto novo comeca ativo
    atualizarDisponibilidade(&p);
    p.ativo = 1;
 
    lista[total] = p;
    total++;
 
    printf("\n====================================\n");
    printf("     PRODUTO CADASTRADO COM SUCESSO \n");
    printf("====================================\n");
    exibirProduto(p, listaCat, totalCat);
    printf("\n");
 
    return total;
}
 
// 5. Listar todos os produtos ATIVOS
void listarProdutos(Produto lista[], int total, Categoria listaCat[], int totalCat) {
    int i;
    int n = 0;
 
    if (contarAtivos(lista, total) == 0) {
        printf("\nNenhum produto cadastrado no momento.\n\n");
        return;
    }
 
    printf("\n====================================\n");
    printf("        LISTA DE PRODUTOS           \n");
    printf("====================================\n");
    for (i = 0; i < total; i++) {
        if (lista[i].ativo != 1) {
            continue; // produto removido: nao aparece
        }
        n++;
        printf("Produto #%d:\n", n);
        exibirProduto(lista[i], listaCat, totalCat);
    }
    printf("\n");
}
 
// 6. Pesquisar produto ATIVO por Codigo ou Nome
void pesquisarProduto(Produto lista[], int total, Categoria listaCat[], int totalCat) {
    int opcaoBusca = 0;
 
    if (contarAtivos(lista, total) == 0) {
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
        int pos = buscarAtivoPorCodigo(lista, total, codigo);
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
 
// 7. Alterar o estoque de um produto (entrada ou saida)
void alterarEstoque(Produto lista[], int total, Categoria listaCat[], int totalCat) {
    int codigo;
    int pos;
    int tipo;
    int quantidade;
 
    if (contarAtivos(lista, total) == 0) {
        printf("\nNenhum produto cadastrado para alterar o estoque.\n\n");
        return;
    }
 
    printf("\n=== ALTERAR ESTOQUE ===\n");
    codigo = lerIntPositivo("Digite o codigo do produto: ");
    pos = buscarAtivoPorCodigo(lista, total, codigo);
    if (pos == -1) {
        printf("\nNenhum produto encontrado com o codigo %d.\n\n", codigo);
        return;
    }
 
    printf("\nProduto: %s | Estoque atual: %d\n", lista[pos].nome, lista[pos].quantidade);
    printf("1 - Entrada (chegada de mercadoria)\n");
    printf("2 - Saida (venda ou retirada)\n");
    tipo = lerIntFaixa("Escolha o tipo de movimentacao: ", 1, 2);
    quantidade = lerIntPositivo("Digite a quantidade: ");
 
    if (tipo == 1) {
        // Entrada: soma (protege contra estouro do tipo int)
        if (quantidade > INT_MAX - lista[pos].quantidade) {
            printf("\n[ERRO] A quantidade informada ultrapassa o limite do estoque. Operacao nao realizada.\n\n");
            return;
        }
        lista[pos].quantidade += quantidade;
    } else {
        // Saida: subtrai, mas o estoque nunca pode ficar negativo
        if (quantidade > lista[pos].quantidade) {
            printf("\n[ERRO] Saida de %d maior que o estoque atual (%d). Operacao nao realizada.\n\n",
                   quantidade, lista[pos].quantidade);
            return;
        }
        lista[pos].quantidade -= quantidade;
    }
 
    // A disponibilidade e recalculada sempre que a quantidade muda
    atualizarDisponibilidade(&lista[pos]);
 
    printf("\n====================================\n");
    printf("      ESTOQUE ATUALIZADO COM SUCESSO\n");
    printf("====================================\n");
    exibirProduto(lista[pos], listaCat, totalCat);
    printf("\n");
}
 
// 8. Remover produto (exclusao logica: apenas marca como inativo)
void removerProduto(Produto lista[], int total) {
    int codigo;
    int pos;
 
    if (contarAtivos(lista, total) == 0) {
        printf("\nNenhum produto cadastrado para remover.\n\n");
        return;
    }
 
    printf("\n=== REMOVER PRODUTO ===\n");
    codigo = lerIntPositivo("Digite o codigo do produto a remover: ");
    pos = buscarAtivoPorCodigo(lista, total, codigo);
    if (pos == -1) {
        printf("\nNenhum produto encontrado com o codigo %d.\n\n", codigo);
        return;
    }
 
    lista[pos].ativo = 0;
    printf("\nProduto \"%s\" (codigo %d) removido com sucesso.\n\n", lista[pos].nome, lista[pos].codigo);
}
 
// ---------------------------------------------------------------------------
// Persistencia
// Formato de cada linha:
//   codigo;nome;prateleira;preco;quantidade;disponivel;codigoCategoria;ativo
// ---------------------------------------------------------------------------
 
// 9. Carregar dados do arquivo produtos.txt
int carregarProdutos(Produto lista[], const char *nomeArquivo) {
    int total = 0;
    int lidos;
    char linha[TAM_LINHA_ARQ];
    Produto p;
    FILE *arquivo = fopen(nomeArquivo, "r");
 
    if (arquivo == NULL) {
        return 0; // Arquivo ainda nao existe: comeca com a lista vazia
    }
 
    while (total < MAX_PRODUTOS && fgets(linha, sizeof(linha), arquivo) != NULL) {
        p.ativo = 1; // arquivos da Tarefa 4 (7 campos) nao tem 'ativo': tratados como ativos
        lidos = sscanf(linha, "%d;%49[^;];%c;%f;%d;%d;%d;%d",
                       &p.codigo,
                       p.nome,
                       &p.prateleira,
                       &p.preco,
                       &p.quantidade,
                       &p.disponivel,
                       &p.codigoCategoria,
                       &p.ativo);
        if (lidos == 7 || lidos == 8) {
            p.ativo = (p.ativo != 0) ? 1 : 0;
            atualizarDisponibilidade(&p); // disponibilidade nunca vem "pronta" do arquivo
            lista[total] = p;
            total++;
        }
        // linhas em branco ou malformadas sao ignoradas
    }
 
    fclose(arquivo);
    return total;
}
 
// 10. Salvar dados no arquivo produtos.txt (inclui os removidos, com ativo = 0)
void salvarProdutos(Produto lista[], int total, const char *nomeArquivo) {
    int i;
    FILE *arquivo = fopen(nomeArquivo, "w");
 
    if (arquivo == NULL) {
        printf("[ERRO] Nao foi possivel abrir o arquivo %s para escrita.\n", nomeArquivo);
        return;
    }
 
    for (i = 0; i < total; i++) {
        fprintf(arquivo, "%d;%s;%c;%.2f;%d;%d;%d;%d\n",
                lista[i].codigo,
                lista[i].nome,
                lista[i].prateleira,
                lista[i].preco,
                lista[i].quantidade,
                lista[i].disponivel,
                lista[i].codigoCategoria,
                lista[i].ativo);
    }
 
    fclose(arquivo);
}
 
