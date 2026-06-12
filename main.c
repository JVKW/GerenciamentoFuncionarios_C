#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void limpar_terminal() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressionar_enter() {
    printf("\nPressione ENTER para continuar...");
    while (getchar() != '\n');
    getchar();
    limpar_terminal();
}

#define ARQUIVO_FUNCIONARIOS "funcionarios.dat"

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

// Salva o estado atual da lista encadeadada no arquivo funcionarios.dat
// Caso haja dados salvos, o conteúdo do arquivo é substituído
void salvar_dados(NO * inicio){
    FILE * arq_func = fopen(ARQUIVO_FUNCIONARIOS, "w");

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

void carregar_dados() {
    FILE *arq_func = fopen(ARQUIVO_FUNCIONARIOS, "r");
    
    if (arq_func == NULL) {
        printf("Arquivo de dados nao encontrado ou erro ao abrir.\n");
        return;
    }

    char linha[4096]; // Variável armazenar uma linha completa do arquivo

    while (fgets(linha, sizeof(linha), arq_func) != NULL) {
        
        // Remove o caractere de nova linha (\n) do final, se existir
        linha[strcspn(linha, "\n")] = 0;

        // Separa os dados usando a vírgula
        char *_nome = strtok(linha, ",");
        char *_codigo = strtok(NULL, ",");
        char *_cargo = strtok(NULL, ",");
        char *_salario = strtok(NULL, ",");

        // Garante que todos os campos foram extraídos com sucesso
        if (_nome && _codigo && _cargo && _salario) {
            int codigo = atoi(_codigo);
            double salario = atof(_salario);

            // Alocação de memória individual para as strings.
            char *nome_copia = malloc(strlen(_nome) + 1);
            char *cargo_copia = malloc(strlen(_cargo) + 1);

            if (nome_copia != NULL && cargo_copia != NULL) {
                strcpy(nome_copia, _nome);
                strcpy(cargo_copia, _cargo);

                cadastrar(nome_copia, codigo, cargo_copia, salario);
            } else {
                printf("Erro de alocacao de memoria ao ler registro.\n");
                free(nome_copia);
                free(cargo_copia);
            }
        }
    }

    fclose(arq_func);
    printf("Dados carregados com sucesso. Total de funcionarios: %d\n", tam);
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
    limpar_terminal();

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
                limpar_terminal();
                char *nome = malloc(sizeof(char)*255);
                char *cargo = malloc(sizeof(char)*100);
                double salario; 
                
                printf("\n=== NOVO CADASTRO ===\n");
                printf("\nCodigo: "); 
                scanf("%d", &cod);

                printf("\nNome: "); 
                scanf(" %[^\n]", nome); 

                printf("\nCargo: "); 
                scanf(" %[^\n]", cargo); 

                printf("\nSalario: "); 
                scanf("%lf", &salario);
                
                cadastrar(nome, cod, cargo, salario);
                printf("\n=> Funcionario cadastrado com sucesso!\n\n");
                
                
                pressionar_enter();
                break;
                
            case 2:
                limpar_terminal();
                listar_funcionarios();
                
                pressionar_enter();
                break;
                
            case 3:
                limpar_terminal();
                printf("\n=== BUSCAR FUNCIONARIO ===\n");
                printf("Digite o codigo para buscar: ");
                scanf("%d", &cod);
                buscar(cod);
                
                pressionar_enter();
                break;
                
            case 4:
                limpar_terminal();
                printf("Digite o codigo para alterar: ");
                scanf("%d", &cod);
                alterar();
                
                pressionar_enter();
                break;
                
            case 5:
                limpar_terminal();
                printf("Digite o codigo para remover: ");
                scanf("%d", &cod);
                remover();
                
                pressionar_enter();
                break;
                
            case 0:
                limpar_terminal();
                printf("\nEncerrando o programa...\n");
                salvar_dados(inicio); //pro documento ficar salvo la na funcao
                
                pressionar_enter();
                break;
                
            default:
                limpar_terminal();
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
