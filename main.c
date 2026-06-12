#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Funcionario{

    char *nome;
    int codigo;
    char *cargo;
    double salario;

    struct Funcionario * prox;
    struct Funcionario *ant;

}NO;

NO *inicio = NULL;
NO *fim = NULL;
int tam = 0;

void cadastrar(char *nome, int codigo, char *cargo, double salario){
    //cadastrar sem nenhuma condição necessariamente, a diferença é que a gente pode voltar pra um funcionario anterior se quiser.


}

// Salva o estado atual da lista encadeadada no arquivo funcionarios.dat
// Caso haja dados salvos, o conteúdo do arquivo é substituído
void salvar_dados(NO * inicio){
    FILE * arq_func = fopen("funcionarios.dat", "w");

    if (arq_func == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return; // fazer verificação depois
    }

    NO * aux = inicio;
    while (aux != NULL) {
        fprintf(
            arq_func,
            "%s,%d,%s,%.2lf\n",
            aux->nome,
            aux->codigo,
            aux->cargo,
            aux->salario
        );

        aux = aux->prox;
    }

    fclose(arq_func);
    printf("Dados salvos com sucesso.\n");
}

void carregar_dados (){
    //carregar os dados em arquivos


}

void alterar(){
    //evidentemente tambÃ©m com base na posição desejada, ai imprime a atual situação do funcionario e modifica o que for necessário

}

void remover(){
    //remover com base na posição desejada, se a posição nao existir imprima quais posições existem.

}

void listar_funcionarios(){
//logica para percorrer a lista e imprimir



}
NO * buscar_funcionario(int codigo){
//logica para buscar funcionarios recebendo o codigo do funcionario como parametro


}


int main() {
    //pra ler do teclado
    int cod;
    char *nome;
    char *cargo;
    double sal;

    return 0;

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
                
                // cadastrar(nome, cod, cargo, sal);
                break;
                
            case 2:
                listar_funcionarios();
                break;
                
            case 3:
                printf("Digite o codigo para buscar: ");
                scanf("%d", &cod);
                buscar_funcionario(cod);
                break;
                
            case 4:
                printf("Digite o codigo para alterar: ");
                scanf("%d", &cod);
                alterar();
                break;
                
            case 5:
                printf("Digite o codigo para remover: ");
                scanf("%d", &cod);
                remover();
                break;
                
            case 0:
                printf("\nEncerrando o programa...\n");
                salvar_dados(inicio); //pro documento ficar salvo la na funcao
                break;
                
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
