#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Funcionario{

    char *nome;
    int codigo;
    char *cargo;
    double salario;

    struct Funcionario *prox;
    struct Funcionario *ant;

}NO;

NO *inicio = NULL;
NO *fim = NULL;
int tam = 0;

void cadastrar(char *nome, int codigo, char *cargo, double salario){
    NO *novo = malloc(sizeof(NO));
    novo->nome = nome;
    novo->codigo = codigo;
    novo->cargo = cargo;
    novo->salario = salario;
    novo->prox = NULL;
    novo->ant = NULL;

    if(inicio == NULL){ //lista vazia
        inicio = novo;
        fim = novo;
        tam++;
    }else{ // lista nao esta vazia
        if(novo->codigo < inicio->codigo){ // caso do inicio
            novo->prox = inicio;
            inicio->ant = novo;
            inicio = novo;
            tam++;
        }else if(novo->codigo > fim->codigo){ //caso do fim
            fim->prox = novo;
            novo->ant = fim;
            fim = novo;
            tam++;
        }else{
            //meio
            NO* aux = inicio;
            while(aux->prox->codigo < novo->codigo){
                aux = aux->prox;
            }
            novo->prox =aux->prox;
            novo->ant = aux;
            aux->prox->ant = novo;
            aux->prox = novo;
            tam++;
        }
    }

}
void salvar_dados (){
    //salvar os dados em arquivos


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
    NO *aux = inicio;
    if(aux == NULL){
        printf("=> Ainda nao ha funcionarios cadastrados.\n\n");
    }else{
        printf("=== LISTA DE FUNCIONARIOS ===\n\n");
        while (aux != NULL){
            printf("    Nome: %s\n", aux->nome);
            printf("    Codigo: %d\n", aux->codigo);
            printf("    Cargo: %s\n", aux->cargo);
            printf("    Salario: R$ %.2lf\n", aux->salario);
            printf("_______________________________\n\n");
            aux = aux->prox;
        }
    }
    
}

NO * buscar(int codigo){
//logica para buscar funcionarios recebendo o codigo do funcionario como parametro
    NO * aux = inicio;

    if(inicio == NULL){
        printf("=> A lista de funcionarios esta vazia.\n\n");
        return NULL;
    }
    printf("\n=== BUSCA DE FUNCIONARIO ===\n\n");
    while(aux != NULL && aux->codigo != codigo){
        aux = aux->prox;
    }
        
    if(aux == NULL){
        printf("=> Nenhum funcionario encontrado com o codigo: %d\n\n", codigo);
    }else{
        printf("=> Funcionario encontrado com o codigo '%d'!\n", codigo);
        printf("    Nome: %s\n", aux->nome);
        printf("    Codigo: %d\n", aux->codigo);
        printf("    Cargo: %s\n", aux->cargo);
        printf("    Salario: R$ %.2lf\n", aux->salario);
        printf("_______________________________\n\n");
    }

    return aux;

}


int main() {    
    // Tenta carregar o arquivo logo ao abrir o programa
    carregar_dados();
    int opcao;
    int cod; 
    system("cls");

    do {
        printf("\n============================\n");
        printf("    MENU DE FUNCIONARIOS    \n");
        printf("============================\n\n");
        printf("=> 1 - Cadastrar funcionario\n");
        printf("=> 2 - Listar funcionarios\n");
        printf("=> 3 - Buscar funcionario\n");
        printf("=> 4 - Alterar funcionario\n");
        printf("=> 5 - Remover funcionario\n");
        printf("=> 0 - Sair e Salvar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);


        switch (opcao) {
            case 1:
                system("cls");
                char *nome = malloc(sizeof(char)*255);
                char *cargo = malloc(sizeof(char)*100);
                double salario; 
                
                printf("\n=== NOVO CADASTRO ===\n");
                printf("Codigo: "); 
                scanf("%d", &cod);

                printf("Nome: "); 
                scanf(" %[^\n]", nome); 

                printf("Cargo: "); 
                scanf(" %[^\n]", cargo); 

                printf("Salario: "); 
                scanf("%lf", &salario);
                
                cadastrar(nome, cod, cargo, salario);
                printf("\n=> Funcionario cadastrado com sucesso!\n\n");
                
                break;
                
            case 2:
                system("cls");
                listar_funcionarios();
                break;
                
            case 3:
                system("cls");
                printf("\n=== BUSCAR FUNCIONARIO ===\n");
                printf("Digite o codigo para buscar: ");
                scanf("%d", &cod);
                buscar(cod);
                break;
                
            case 4:
                system("cls");
                printf("Digite o codigo para alterar: ");
                scanf("%d", &cod);
                alterar();
                break;
                
            case 5:
                system("cls");
                printf("Digite o codigo para remover: ");
                scanf("%d", &cod);
                remover();
                break;
                
            case 0:
                system("cls");
                printf("\nEncerrando o programa...\n");
                salvar_dados(); //pro documento ficar salvo la na funcao
                break;
                
            default:
                system("cls");
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
