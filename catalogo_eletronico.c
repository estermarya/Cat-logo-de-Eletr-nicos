#include <stdio.h>
#include <string.h>

#define MAX_PRODUTOS 100

// Estrutura para guardar os dados dos produtos
struct Produto {
    int codigo;
    char nome[50];
    char categoria[30];
    char marca[30];
    float preco;
};

struct Produto produtos[MAX_PRODUTOS];
int quantidade = 0;

// Procura um produto pelo codigo
int buscarCodigo(int codigo) {
    int i;

    for (i = 0; i < quantidade; i++) {
        if (produtos[i].codigo == codigo) {
            return i;
        }
    }

    return -1;
}

// Salva os produtos em um arquivo para nao perder os dados
void salvarDados() {
    FILE *arquivo;

    arquivo = fopen("produtos.dat", "wb");

    if (arquivo != NULL) {
        fwrite(&quantidade, sizeof(int), 1, arquivo);
        fwrite(produtos, sizeof(struct Produto), quantidade, arquivo);
        fclose(arquivo);
    }
}

// Carrega os produtos que ja foram salvos
void carregarDados() {
    FILE *arquivo;
    int qtdSalva;

    arquivo = fopen("produtos.dat", "rb");

    if (arquivo != NULL) {
        if (fread(&qtdSalva, sizeof(int), 1, arquivo) == 1 &&
            qtdSalva >= 0 && qtdSalva <= MAX_PRODUTOS) {
            if (fread(produtos, sizeof(struct Produto), qtdSalva, arquivo) == qtdSalva) {
                quantidade = qtdSalva;
            }
        }

        fclose(arquivo);
    }
}

// Mostra os dados de um produto
void mostrarProduto(struct Produto p) {
    printf("\nCodigo: %d\n", p.codigo);
    printf("Nome: %s\n", p.nome);
    printf("Categoria: %s\n", p.categoria);
    printf("Marca: %s\n", p.marca);
    printf("Preco: R$ %.2f\n", p.preco);
}

// Cadastra um produto novo
void cadastrarProduto() {
    int codigo;

    if (quantidade >= MAX_PRODUTOS) {
        printf("\nLimite de produtos atingido!\n");
        return;
    }

    printf("\n===== CADASTRAR PRODUTO =====\n");

    printf("Digite o codigo: ");
    scanf("%d", &codigo);
    getchar();

    if (codigo <= 0) {
        printf("O codigo deve ser maior que zero!\n");
        return;
    }

    if (buscarCodigo(codigo) != -1) {
        printf("Esse codigo ja existe!\n");
        return;
    }

    produtos[quantidade].codigo = codigo;

    printf("Digite o nome: ");
    fgets(produtos[quantidade].nome, 50, stdin);
    produtos[quantidade].nome[strcspn(produtos[quantidade].nome, "\n")] = '\0';

    printf("Digite a categoria (Celulares, Computadores, Acessorios, Videogames ou Outros): ");
    fgets(produtos[quantidade].categoria, 30, stdin);
    produtos[quantidade].categoria[strcspn(produtos[quantidade].categoria, "\n")] = '\0';

    printf("Digite a marca: ");
    fgets(produtos[quantidade].marca, 30, stdin);
    produtos[quantidade].marca[strcspn(produtos[quantidade].marca, "\n")] = '\0';

    printf("Digite o preco: ");
    scanf("%f", &produtos[quantidade].preco);
    getchar();

    if (produtos[quantidade].preco <= 0) {
        printf("O preco precisa ser maior que zero!\n");
        return;
    }

    quantidade++;
    salvarDados();

    printf("\nProduto cadastrado com sucesso!\n");
}

// Lista todos os produtos
void listarProdutos() {
    int i;

    printf("\n===== PRODUTOS CADASTRADOS =====\n");

    if (quantidade == 0) {
        printf("Nao ha produtos cadastrados.\n");
        return;
    }

    for (i = 0; i < quantidade; i++) {
        printf("\n--------------------------\n");
        mostrarProduto(produtos[i]);
    }
}

// Pesquisa um produto pelo codigo
void pesquisarPorCodigo() {
    int codigo;
    int posicao;

    printf("\nDigite o codigo do produto: ");
    scanf("%d", &codigo);
    getchar();

    posicao = buscarCodigo(codigo);

    if (posicao == -1) {
        printf("Produto nao encontrado.\n");
    } else {
        printf("\nProduto encontrado:\n");
        mostrarProduto(produtos[posicao]);
    }
}

// Pesquisa produtos pelo nome
void pesquisarPorNome() {
    char nome[50];
    int i;
    int encontrou = 0;

    printf("\nDigite o nome ou parte do nome: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    for (i = 0; i < quantidade; i++) {
        if (strstr(produtos[i].nome, nome) != NULL) {
            mostrarProduto(produtos[i]);
            encontrou = 1;
        }
    }

    if (encontrou == 0) {
        printf("Produto nao encontrado.\n");
    }
}

// Consulta produtos de uma categoria
void consultarCategoria() {
    char categoria[30];
    int i;
    int encontrou = 0;

    printf("\nDigite a categoria: ");
    fgets(categoria, 30, stdin);
    categoria[strcspn(categoria, "\n")] = '\0';

    for (i = 0; i < quantidade; i++) {
        if (strcmp(produtos[i].categoria, categoria) == 0) {
            mostrarProduto(produtos[i]);
            encontrou = 1;
        }
    }

    if (encontrou == 0) {
        printf("Nenhum produto encontrado nessa categoria.\n");
    }
}

// Altera os dados de um produto
void alterarProduto() {
    int codigo;
    int posicao;

    printf("\nDigite o codigo do produto que deseja alterar: ");
    scanf("%d", &codigo);
    getchar();

    posicao = buscarCodigo(codigo);

    if (posicao == -1) {
        printf("Produto nao encontrado.\n");
        return;
    }

    printf("\nProduto atual:\n");
    mostrarProduto(produtos[posicao]);

    printf("\nDigite o novo nome: ");
    fgets(produtos[posicao].nome, 50, stdin);
    produtos[posicao].nome[strcspn(produtos[posicao].nome, "\n")] = '\0';

    printf("Digite a nova categoria: ");
    fgets(produtos[posicao].categoria, 30, stdin);
    produtos[posicao].categoria[strcspn(produtos[posicao].categoria, "\n")] = '\0';

    printf("Digite a nova marca: ");
    fgets(produtos[posicao].marca, 30, stdin);
    produtos[posicao].marca[strcspn(produtos[posicao].marca, "\n")] = '\0';

    printf("Digite o novo preco: ");
    scanf("%f", &produtos[posicao].preco);
    getchar();

    if (produtos[posicao].preco <= 0) {
        printf("Preco invalido. O produto nao foi alterado.\n");
        return;
    }

    salvarDados();
    printf("\nProduto alterado com sucesso!\n");
}

// Exclui um produto
void excluirProduto() {
    int codigo;
    int posicao;
    int i;

    printf("\nDigite o codigo do produto que deseja excluir: ");
    scanf("%d", &codigo);
    getchar();

    posicao = buscarCodigo(codigo);

    if (posicao == -1) {
        printf("Produto nao encontrado.\n");
        return;
    }

    for (i = posicao; i < quantidade - 1; i++) {
        produtos[i] = produtos[i + 1];
    }

    quantidade--;
    salvarDados();

    printf("\nProduto excluido com sucesso!\n");
}

// Cria uma pagina HTML simples com os produtos cadastrados
void gerarCatalogoHTML() {
    FILE *arquivo;
    int i;

    arquivo = fopen("catalogo.html", "w");

    if (arquivo == NULL) {
        printf("Nao foi possivel criar o catalogo HTML.\n");
        return;
    }

    fprintf(arquivo, "<!DOCTYPE html>\n<html lang='pt-br'>\n<head>\n");
    fprintf(arquivo, "<meta charset='UTF-8'>\n<title>Catalogo de Eletronicos</title>\n");
    fprintf(arquivo, "<style>body{font-family:Arial;background:#f2f2f2;padding:20px;}");
    fprintf(arquivo, "h1{color:#333;}table{border-collapse:collapse;width:100%%;background:white;}");
    fprintf(arquivo, "th,td{border:1px solid #ccc;padding:10px;text-align:left;}");
    fprintf(arquivo, "th{background:#ddd;}</style>\n</head>\n<body>\n");
    fprintf(arquivo, "<h1>Catalogo de Eletronicos</h1>\n");
    fprintf(arquivo, "<table><tr><th>Codigo</th><th>Nome</th><th>Categoria</th><th>Marca</th><th>Preco</th></tr>\n");

    for (i = 0; i < quantidade; i++) {
        fprintf(arquivo, "<tr><td>%d</td><td>%s</td><td>%s</td><td>%s</td><td>R$ %.2f</td></tr>\n",
                produtos[i].codigo, produtos[i].nome, produtos[i].categoria,
                produtos[i].marca, produtos[i].preco);
    }

    fprintf(arquivo, "</table>\n</body>\n</html>");
    fclose(arquivo);

    printf("\nCatalogo HTML criado! Abra o arquivo catalogo.html no navegador.\n");
}

int main() {
    int opcao;

    carregarDados();

    do {
        printf("\n==================================\n");
        printf("   CATALOGO DE ELETRONICOS\n");
        printf("==================================\n");
        printf("1 - Cadastrar produto\n");
        printf("2 - Listar produtos\n");
        printf("3 - Pesquisar por codigo\n");
        printf("4 - Pesquisar por nome\n");
        printf("5 - Consultar por categoria\n");
        printf("6 - Alterar produto\n");
        printf("7 - Excluir produto\n");
        printf("8 - Gerar catalogo HTML\n");
        printf("0 - Sair\n");

        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                listarProdutos();
                break;
            case 3:
                pesquisarPorCodigo();
                break;
            case 4:
                pesquisarPorNome();
                break;
            case 5:
                consultarCategoria();
                break;
            case 6:
                alterarProduto();
                break;
            case 7:
                excluirProduto();
                break;
            case 8:
                gerarCatalogoHTML();
                break;
            case 0:
                printf("\nPrograma encerrado!\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
