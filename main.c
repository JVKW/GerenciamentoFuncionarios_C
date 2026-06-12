#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// APP_PATH_H
// # inicio
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path) _mkdir(path)
    #define SEP "\\"
#elif __APPLE__
    #include <sys/types.h>
    #define MKDIR(path) mkdir(path, 0755)
    #define SEP "/"
#else
    #include <sys/types.h>
    #define MKDIR(path) mkdir(path, 0755)
    #define SEP "/"
#endif

// char *destino : Buffer de saída, ponteiro para string que o caminho deve ser salvo
// size_t tamanho : Tamanho do Buffer *destino para evitar overflow
    // * recomendável usar sizeoff()
// const char *nomeProjeto : Auto-Explicativo, nome do path para ser salvo
int obter_diretorio_app(char *destino, size_t tamanho, const char *nomeProjeto) {

    #ifdef _WIN32
        const char *base = getenv("APPDATA");
        if (!base) return 0;

        snprintf(destino, tamanho, "%s%s%s", base, SEP, nomeProjeto);

    #elif __APPLE__
        const char *home = getenv("HOME");
        if (!home) return 0;

        snprintf(destino, tamanho,
                "%s/Library/Application Support/%s",
                home, nomeProjeto);

    #else
        const char *home = getenv("HOME");
        if (!home) return 0;

        snprintf(destino, tamanho,
                "%s/.local/share/%s",
                home, nomeProjeto);
    #endif

        return 1;
}

// const char *caminho : verifica se *caminho existe
int diretorio_existe(const char *caminho) {
    struct stat info;
    return (stat(caminho, &info) == 0) && (info.st_mode & S_IFDIR);
}
// saídas :
// 1 : existe (diretorio)
// 0 : nao existe (ou nao e diretorio)


void inicializar_path_app(const char *path_usr) {
    if (diretorio_existe(path_usr)) {
        printf("Diretorio ja existente.\n");
    } else {
        printf("Diretorio nao existente. Criando...\n");

        if (MKDIR(path_usr) == 0) {
            printf("Diretorio criado com sucesso.\n");
        } else {
            perror("Erro ao criar pasta");
        }
    }
}

// RESUMO
// * obter_diretorio_app() : Monta o caminho do diretorio app no sistema
// * diretorio_existe() : Verifica se o caminho passado ja existe
// * inicializar_path_app() : Garante que a pasta exista (se nao existir, cria)

// # fim

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

int cadastrar(char *nome, int codigo, char *cargo, double salario){
    NO*busca = inicio;

    while(busca != NULL){
        if(busca->codigo == codigo){
            printf("Erro: ja existe um funcionario com o codigo %d", codigo);
            return 0;
        }
        busca = busca->prox;
    }
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

    return 1;
}

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
                limpar_terminal();
                printf("\nAlteracoes concluidas!\n");
                break;
        
            default:
                limpar_terminal();
                printf("Opcao invalida!\n");
                break;
            }
        } while (opcao != 0);
        
    }
}




int main() {    
    // Tenta carregar o arquivo logo ao abrir o programa
    carregar_dados();
    int opcao = -1;
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
                
                if(cadastrar(nome, cod, cargo, salario)){
                    printf("\n=> Funcionario cadastrado com sucesso!\n\n");
                }else{
                    free(nome);
                    free(cargo);
                }
                
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
                
            case 4:{
                limpar_terminal();
                printf("\n=== ALTERAR FUNCIONARIO ===\n");
                printf("Digite o codigo para alterar: ");
                scanf("%d", &cod);
                NO *alterarFuncionario = buscar(cod);
                alterar(alterarFuncionario);
                pressionar_enter();
                break;
            }
                
                
            case 5:
                limpar_terminal();
                printf("\n=== REMOVER FUNCIONARIO ===\n");
                printf("Digite o codigo para remover: ");
                scanf("%d", &cod);
                if (remover(cod) == NULL) {
                    printf("\n=> Nenhum funcionario encontrado com o codigo %d.\n", cod);
                } else {
                    printf("\n=> Usuario com codigo %d removido com sucesso!\n", cod);
                }
                pressionar_enter();
                break;
                
                
            case 0:
                limpar_terminal();
                printf("\nEncerrando o programa...\n");
                salvar_dados(inicio); //pro documento ficar salvo la na funcao
                
                
                break;
                
            default:
                limpar_terminal();
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
