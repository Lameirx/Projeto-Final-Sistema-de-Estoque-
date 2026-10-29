# Sistema de Estoque

Sistema de controle de estoque em linguagem C, executado no terminal, com cadastro de produtos e categorias, movimentação de estoque, remoção lógica e persistência em arquivos de texto.

## Integrantes

| Nome | RGM |
|------|-----|
| Melissa Chagas Lameiras | 04896622-3 |

## Menu

```
1 - Cadastrar Produto
2 - Listar Produtos
3 - Pesquisar Produto (por código ou por nome)
4 - Alterar Estoque de um Produto
5 - Remover Produto
6 - Categorias (cadastrar, listar, voltar)
7 - Sair
```

## Funcionalidades e regras

- Produtos (até 50) e categorias (até 20), cada um com código positivo e único
- Não é possível cadastrar produto sem categoria ou com categoria inexistente
- O nome da categoria é exibido ao cadastrar, listar e pesquisar um produto
- Preço maior que zero; quantidade zero ou maior; prateleira A, B, C ou D
- **Disponibilidade sempre calculada** a partir da quantidade (nunca digitada)
- **Alterar estoque:** entrada soma e saída subtrai, sem permitir estoque negativo
- **Remover produto:** exclusão lógica (campo `ativo`). O produto removido não aparece em listagem nem em pesquisa, mas continua nos arquivos; seu código não pode ser reutilizado
- Dados salvos em `categorias.txt` e `produtos.txt` e carregados ao iniciar o programa

## Estrutura

| Arquivo | Responsabilidade |
|---------|------------------|
| `main.c` | Menus do sistema (sem regras de negócio) |
| `produto.c` / `produto.h` | Regras e operações de produto (validação específica do domínio) |
| `categoria.c` / `categoria.h` | Regras e operações de categoria |
| `util.c` / `util.h` | Leitura e validação genérica de entradas |

## Como compilar e executar

Todos os arquivos `.c` precisam ser compilados juntos:

```
gcc main.c produto.c categoria.c util.c -o estoque
./estoque        (no Windows: estoque.exe)
```

No Dev-C++, crie um projeto C (Console Application), adicione os 4 arquivos `.c` e use *Executar → Reconstruir Tudo*.
