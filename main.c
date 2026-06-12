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
void salvar_dados(){
    //salvar os dados em arquivos


}
void carregar_dados(){
    //carregar os dados em arquivos


}

void imprimir_removido(NO* Funcionario){
    printf("\n=== FUNCIONARIO REMOVIDO ===\n\n");
    printf("Nome: %s\n", Funcionario->nome);
    printf("Codigo: %d\n", Funcionario->codigo);
    printf("Cargo: %s\n", Funcionario->cargo);
}

NO* remover(int codigo){
    //remover com base no codigo desejada, se a posição nao existir imprima quais posições existem.
    if(inicio == NULL){
        return NULL;
    }
    if(inicio->codigo == codigo){ //inicio
        NO *aux = inicio;
        inicio = inicio->prox;
        inicio->ant = NULL;
        if(tam == 1){
            fim = NULL;
        }
        //free(aux);
        imprimir_removido(aux);
        tam--;
        return aux;
    }else if(fim->codigo == codigo){ // fim
        NO *aux = fim;
        fim->ant->prox = NULL;
        fim = fim->ant;
        //free(aux);
        imprimir_removido(aux);
        tam--;
        return aux;
    }else{
        //meio....
        NO *aux = inicio;
        NO *lixo = aux;
        while(aux->prox->codigo != codigo){
            aux = aux->prox;
        }
        lixo = aux->prox;
        aux->prox = lixo->prox;
        aux->prox->ant = aux; 
        //free(lixo);
        imprimir_removido(lixo);
        tam--;
        return lixo;
    }
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

void alterar(NO *funcionario){
    //evidentemente também com base na posição desejada, ai imprime a atual situação do funcionario e modifica o que for necessário
    if(funcionario == NULL){
        return;
    }else{
        int opcao;
        do{
            printf("=> 1 - Alterar Nome\n");
            printf("=> 2 - Alterar Cargo\n");
            printf("=> 3 - Alterar Salario\n");
            printf("=> 0 - Concluir\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao);

            switch (opcao){
            case 1:{
                char novo_nome[255];
                printf("Digite o novo nome para %s: ", funcionario->nome);
                scanf(" %[^\n]", novo_nome);
                strcpy(funcionario->nome, novo_nome);
                break;
            }
            case 2:{
                char novo_cargo[255];
                printf("Digite o novo cargo %s: ", funcionario->nome);
                scanf(" %[^\n]", novo_cargo);
                strcpy(funcionario->cargo, novo_cargo);
                break;
            }
            case 3:
                printf("Digite o novo salario de %s: ", funcionario->nome);
                scanf("%lf", &funcionario->salario);
                break;
            case 0:
                system("cls");
                printf("\nAlteracoes concluidas!\n");
                break;
        
            default:
                system("cls");
                printf("Opcao invalida!\n");
                break;
            }
        } while (opcao != 0);
        
    }
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
                
            case 4:{
                system("cls");
                printf("\n=== ALTERAR FUNCIONARIO ===\n");
                printf("Digite o codigo para alterar: ");
                scanf("%d", &cod);
                NO *alterarFuncionario = buscar(cod);
                alterar(alterarFuncionario);
                break;
            }
                
                
            case 5:
                system("cls");
                printf("\n=== REMOVER FUNCIONARIO ===\n");
                printf("Digite o codigo para remover: ");
                scanf("%d", &cod);
                remover(cod) == NULL ? 
                        printf("=> Nenhum funcionario cadastrado."):
                        printf("Usuario com codigo %d removido da lista de funcionarios.\n", cod);
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
