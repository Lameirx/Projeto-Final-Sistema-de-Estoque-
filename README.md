# Sistema de Estoque (Tarefa 4)

Sistema de controle de estoque em linguagem C, executado no terminal, com cadastro de produtos e categorias e persistência em arquivos de texto.

## Integrantes

| Nome | RGM |
|------|-----|
| [Nome do integrante 1] | [RGM] |
| [Nome do integrante 2] | [RGM] |

## Funcionalidades

- Cadastrar, listar e pesquisar produtos (por código ou por nome)
- Cadastrar e listar categorias (até 20) e produtos (até 50)
- O nome da categoria é exibido ao cadastrar, listar e pesquisar um produto
- Código de produto e de categoria positivos e únicos
- Não é possível cadastrar produto sem categoria ou com categoria inexistente
- Preço maior que zero; quantidade zero ou maior (disponibilidade calculada automaticamente)
- Dados salvos em `categorias.txt` e `produtos.txt` e carregados ao iniciar o programa

## Estrutura

| Arquivo | Responsabilidade |
|---------|------------------|
| `main.c` | Menus do sistema |
| `produto.c` / `produto.h` | Gestão de produtos |
| `categoria.c` / `categoria.h` | Gestão de categorias |
| `util.c` / `util.h` | Leitura e validação de entradas |

## Como compilar e executar

Todos os arquivos `.c` precisam ser compilados juntos:

```
gcc main.c produto.c categoria.c util.c -o estoque
./estoque        (no Windows: estoque.exe)
```

No Dev-C++, crie um projeto C (Console Application), adicione os 4 arquivos `.c` e use *Executar → Reconstruir Tudo*.
