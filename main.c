#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NOME 100 //Constantes da estrutura base
#define MAX_CARGO 50 //buffers de leitura

typedef struct Funcionario{
    char *nome;
    int codigo;
    char *cargo;
    double salario;
    struct Funcionario *prox;
    struct Funcionario *ant;
} NO;

NO *inicio = NULL;
NO *fim = NULL;
int tam = 0;

void cadastrar(char *nome, int codigo, char *cargo, double salario){
    //cadastrar sem nenhuma condição necessariamente, a diferença é que a gente pode voltar pra um funcionario anterior se quiser.

    // TODO (Xanderson/Nicolas): implementar
    // TODO (Pedro): validar retorno de malloc/strdup, verificar codigo duplicado
}

void salvar_dados(){
    // TODO (Victor): implementar salvamento em arquivo
    //salvar os dados em arquivos
}

void carregar_dados(){
    // TODO (Victor): implementar carregamento de arquivo
    //carregar os dados em arquivos
}

void alterar(int codigo){
    // TODO (Kevin/Nicolas): implementar alteracao
    // TODO (Pedro): validar se codigo existe
}

void remover(int codigo){
    // TODO (Xanderson/Kevin): implementar remocao
    // TODO (Pedro): validar se codigo existe, dar free nas strings
}

void listar_funcionarios(){
    // TODO (Xanderson/Nicolas): implementar listagem
    // TODO (Pedro): validar se lista esta vazia
}

NO* buscar_funcionario(int codigo){
    //logica para buscar funcionarios recebendo o codigo do funcionario como parametro
    // TODO (Kevin/Nicolas): implementar busca
    return NULL;
}

int main() {
    int cod;
    char *nome = malloc(MAX_NOME);
    char *cargo = malloc(MAX_CARGO);
    double sal;
    // Tenta carregar o arquivo logo ao abrir o programa
    carregar_dados();
    int opcao;

    do {
        printf("\n============================\n");
        printf("    MENU DE FUNCIONARIOS    \n");
        printf("============================\n\n");
        printf("1 - Cadastrar funcionario\n");
        printf("2 - Listar funcionarios\n");
        printf("3 - Buscar funcionario\n");
        printf("4 - Alterar funcionario\n");
        printf("5 - Remover funcionario\n");
        printf("0 - Sair e Salvar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n--- NOVO CADASTRO ---\n");
                printf("Codigo: ");
                scanf("%d", &cod);
                printf("Nome: ");
                scanf(" %[^\n]", nome);
                printf("Cargo: ");
                scanf(" %[^\n]", cargo);
                printf("Salario: ");
                scanf("%lf", &sal);

                cadastrar(nome, cod, cargo, sal);
                break;

            case 2:
                listar_funcionarios();
                break;

            case 3: {
                printf("Digite o codigo para buscar: ");
                scanf("%d", &cod);
                NO *func = buscar_funcionario(cod);
                if (func != NULL) {
                    printf("\n--- FUNCIONARIO ENCONTRADO ---\n");
                    printf("Codigo: %d\n", func->codigo);
                    printf("Nome: %s\n", func->nome);
                    printf("Cargo: %s\n", func->cargo);
                    printf("Salario: %.2f\n", func->salario);
                } else {
                    printf("Funcionario com codigo %d nao encontrado.\n", cod);
                }
                break;
            }

            case 4:
                printf("Digite o codigo para alterar: ");
                scanf("%d", &cod);
                alterar(cod);
                break;

            case 5:
                printf("Digite o codigo para remover: ");
                scanf("%d", &cod);
                remover(cod);
                break;

            case 0:
                printf("\nEncerrando o programa...\n");
                salvar_dados();
                // TODO (Pedro): chamar liberar_lista() para evitar memory leak
                //para o documento ficar salvo lá na função
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    free(nome);
    free(cargo);
    // TODO (Pedro): liberar_lista() — percorrer a lista e dar free em todos os nos e strings
    return 0;
}
