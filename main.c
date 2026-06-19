#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h> // Incluído para fazer o isdigit funcionar

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

int obter_diretorio_app(char *destino, size_t tamanho, const char *nomeProjeto) {
    #ifdef _WIN32
        const char *base = getenv("APPDATA");
        if (!base) return 0;
        snprintf(destino, tamanho, "%s%s%s", base, SEP, nomeProjeto);
    #elif __APPLE__
        const char *home = getenv("HOME");
        if (!home) return 0;
        snprintf(destino, tamanho, "%s/Library/Application Support/%s", home, nomeProjeto);
    #else
        const char *home = getenv("HOME");
        if (!home) return 0;
        snprintf(destino, tamanho, "%s/.local/share/%s", home, nomeProjeto);
    #endif
    return 1;
}

int diretorio_existe(const char *caminho) {
    struct stat info;
    return (stat(caminho, &info) == 0) && (info.st_mode & S_IFDIR);
}

void inicializar_path_app(const char *path_usr) {
    if (!diretorio_existe(path_usr)) {
        MKDIR(path_usr);
    }
}

void limpar_terminal() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressionar_enter() {
    printf("\nPressione ENTER para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
    limpar_terminal();
}

#define ARQUIVO_FUNCIONARIOS "funcionarios.csv"

// CORRIGIDO: Removidos os asteriscos para virarem strings fixas e seguras
typedef struct Funcionario {
    char nome[255];
    int codigo;
    char cargo[100];
    double salario;
    struct Funcionario *prox;
    struct Funcionario *ant;
} NO;

NO *inicio = NULL;
NO *fim = NULL;
int tam = 0;

int string_contem_numeros(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit((unsigned char)str[i])) {
            return 1; 
        }
    }
    return 0; 
}

// CORRIGIDO: cadastrar agora usa strcpy para mover os dados para dentro da struct estável
int cadastrar(char *nome, int codigo, char *cargo, double salario){
    NO* busca = inicio;
    while(busca != NULL){
        if(busca->codigo == codigo){
            printf("\nErro: ja existe um funcionario com o codigo %d\n", codigo);
            return 0;
        }
        busca = busca->prox;
    }
    
    NO *novo = malloc(sizeof(NO));
    if(!novo) return 0;

    novo->codigo = codigo;
    strcpy(novo->nome, nome);
    strcpy(novo->cargo, cargo);
    novo->salario = salario;
    novo->prox = NULL;
    novo->ant = NULL;

    if(inicio == NULL){ 
        inicio = novo;
        fim = novo;
    }else{ 
        if(novo->codigo < inicio->codigo){ 
            novo->prox = inicio;
            inicio->ant = novo;
            inicio = novo;
        }else if(novo->codigo > fim->codigo){ 
            fim->prox = novo;
            novo->ant = fim;
            fim = novo;
        }else{
            NO* aux = inicio;
            while(aux->prox->codigo < novo->codigo){
                aux = aux->prox;
            }
            novo->prox = aux->prox;
            novo->ant = aux;
            aux->prox->ant = novo;
            aux->prox = novo;
        }
    }
    tam++;
    return 1;
}

// CORRIGIDO: Salvando no caminho absoluto gerado pelo OS
void salvar_dados(const char *path_usr){
    char caminho_completo[1024];
    snprintf(caminho_completo, sizeof(caminho_completo), "%s%s%s", path_usr, SEP, ARQUIVO_FUNCIONARIOS);

    FILE * arq_func = fopen(caminho_completo, "w");
    if (arq_func == NULL) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return; 
    }

    NO * aux = inicio;
    while (aux != NULL) {
        fprintf(arq_func, "%s;%d;%s;%.2lf\n", aux->nome, aux->codigo, aux->cargo, aux->salario);
        aux = aux->prox;
    }
    fclose(arq_func);
    printf("Dados salvos com sucesso em: %s\n", caminho_completo);
}

// CORRIGIDO: Carregando usando o separador ';' para evitar conflitos
void carregar_dados(const char *path_usr) {
    char caminho_completo[1024];
    snprintf(caminho_completo, sizeof(caminho_completo), "%s%s%s", path_usr, SEP, ARQUIVO_FUNCIONARIOS);

    FILE *arq_func = fopen(caminho_completo, "r");
    if (arq_func == NULL) return;

    char linha[4096]; 
    while (fgets(linha, sizeof(linha), arq_func) != NULL) {
        linha[strcspn(linha, "\n\r")] = 0;

        char *_nome = strtok(linha, ";");
        char *_codigo = strtok(NULL, ";");
        char *_cargo = strtok(NULL, ";");
        char *_salario = strtok(NULL, ";");

        if (_nome && _codigo && _cargo && _salario) {
            cadastrar(_nome, atoi(_codigo), _cargo, atof(_salario));
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

// CORRIGIDO: Remoção segura contra travamentos e com liberação de memória efetiva
NO* remover(int codigo){
    if(inicio == NULL) return NULL;
    
    NO *aux = inicio;
    while (aux != NULL && aux->codigo != codigo) {
        aux = aux->prox;
    }
    
    if (aux == NULL) return NULL;

    if(aux == inicio){ 
        inicio = inicio->prox;
        if(inicio != NULL) {
            inicio->ant = NULL;
        } else {
            fim = NULL;
        }
    }else if(aux == fim){ 
        fim = fim->ant;
        fim->prox = NULL;
    }else{
        aux->ant->prox = aux->prox;
        aux->prox->ant = aux->ant;
    }
    
    imprimir_removido(aux);
    free(aux); // Memória limpa com sucesso
    tam--;
    return aux;
}

void listar_funcionarios(){
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
    if (funcionario == NULL) return;
    int opcao;
    char buffer[255]; 
    double novo_salario;

    do {
        printf("\nAlterando dados de: %s (Cod: %d)\n", funcionario->nome, funcionario->codigo);
        printf("=> 1 - Alterar Nome\n");
        printf("=> 2 - Alterar Cargo\n");
        printf("=> 3 - Alterar Salario\n");
        printf("=> 0 - Concluir\n");
        printf("Escolha uma opcao: ");
        if(scanf("%d", &opcao) != 1) { 
             while(getchar()!='\n'); 
             opcao = -1;
        }

        switch (opcao) {
            case 1:
                printf("Digite o novo nome: ");
                scanf(" %[^\n]", buffer);
                if (string_contem_numeros(buffer)) {
                    printf("\nErro: Nome nao pode conter numeros.\n");
                } else {
                    strcpy(funcionario->nome, buffer); 
                    printf("Nome alterado.\n");
                }
                break;
            case 2:
                printf("Digite o novo cargo: ");
                scanf(" %[^\n]", buffer);
                if (string_contem_numeros(buffer)) {
                    printf("\nErro: Cargo nao pode conter numeros.\n");
                } else {
                    strcpy(funcionario->cargo, buffer);
                    printf("Cargo alterado.\n");
                }
                break;
            case 3:
                printf("Digite o novo salario: ");
                if(scanf("%lf", &novo_salario) != 1 || novo_salario <= 0) {
                     printf("\nErro: Salario invalido.\n");
                     while(getchar()!='\n');
                } else {
                    funcionario->salario = novo_salario;
                    printf("Salario alterado.\n");
                }
                break;
            case 0:
                printf("\nAlteracoes concluidas!\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);
}

int main() {    
    // CORRIGIDO: Ativando a pasta multiplataforma do sistema para salvar dados
    char path_dados[1024];
    if (!obter_diretorio_app(path_dados, sizeof(path_dados), "GerenciadorFuncionarios")) {
        strcpy(path_dados, ".");
    }
    inicializar_path_app(path_dados);
    carregar_dados(path_dados);

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
        if (scanf("%d", &opcao) != 1) {
            while(getchar() != '\n');
            opcao = -1;
        }

        switch (opcao) {
            case 1: {
                limpar_terminal();
                char nome[255];   
                char cargo[100];  
                double salario;
                int codigo_valido = 0;
                
                printf("\n=== NOVO CADASTRO ===\n");

                do {
                    printf("\nCodigo: "); 
                    if (scanf("%d", &cod) != 1) {
                        while(getchar() != '\n'); 
                        cod = -1;
                    }
                    if(cod < 0) {
                        printf("Erro: Codigo nao pode ser negativo.\n");
                    }else {
                        NO* busca = inicio;
                        int duplicado = 0;
                        while(busca != NULL) {
                            if(busca->codigo == cod) {
                                duplicado = 1;
                                break;
                            }
                            busca = busca->prox;
                        }

                        if (duplicado) {
                            printf("Erro: ja existe um funcionario com o codigo %d\n", cod);
                        } else {
                            codigo_valido = 1; // Código é positivo e não é duplicado!
                        }
                    }
                } while (!codigo_valido);

                do {
                    printf("\nNome: "); 
                    scanf(" %[^\n]", nome);
                    if(string_contem_numeros(nome)) {
                        printf("Erro: O nome nao deve conter numeros.\n");
                    }
                } while(string_contem_numeros(nome));

                do {
                    printf("\nCargo: "); 
                    scanf(" %[^\n]", cargo);
                    if(string_contem_numeros(cargo)) {
                        printf("Erro: O cargo nao deve conter numeros.\n");
                    }
                } while(string_contem_numeros(cargo));

                printf("\nSalario: ");
                do {
                    if(scanf("%lf", &salario) != 1) {
                        while(getchar() != '\n');
                        salario = -1;
                    }
                    if(salario <= 0) printf("Erro: O salario deve ser maior que zero. Digite novamente: ");
                } while (salario <= 0);

                if(cadastrar(nome, cod, cargo, salario)){
                    printf("\n=> Funcionario cadastrado com sucesso!\n\n");
                }
                
                pressionar_enter();
                break;
            } 
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
            case 4: {
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
                    printf("\n=> Usuario removido com sucesso!\n");
                }
                pressionar_enter();
                break;
            case 0:
                limpar_terminal();
                printf("\nEncerrando o programa...\n");
                salvar_dados(path_dados); 
                break;
            default:
                limpar_terminal();
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}