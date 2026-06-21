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

// ==========================================
//          CORES ANSI PARA A UX 
// ==========================================
#define ANSI_RESET   "\x1b[0m"
#define ANSI_BOLD    "\x1b[1m"
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_WHITE   "\x1b[37m"

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
    printf("\n" ANSI_YELLOW "➔ Pressione ENTER para continuar..." ANSI_RESET);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
    limpar_terminal();
}

#define ARQUIVO_FUNCIONARIOS "funcionarios.dat"

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

int cadastrar(char *nome, int codigo, char *cargo, double salario){
    NO* busca = inicio;
    while(busca != NULL){
        if(busca->codigo == codigo){
            printf("\n" ANSI_RED "✖ Erro: já existe um funcionario com o codigo %d" ANSI_RESET "\n", codigo);
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

void salvar_dados(const char *path_usr){
    char caminho_completo[1024];
    snprintf(caminho_completo, sizeof(caminho_completo), "%s%s%s", path_usr, SEP, ARQUIVO_FUNCIONARIOS);

    FILE * arq_func = fopen(caminho_completo, "w");
    if (arq_func == NULL) {
        printf(ANSI_RED "✖ Erro ao abrir o arquivo para salvar." ANSI_RESET "\n");
        return; 
    }

    NO * aux = inicio;
    while (aux != NULL) {
        fprintf(arq_func, "%s;%d;%s;%.2lf\n", aux->nome, aux->codigo, aux->cargo, aux->salario);
        aux = aux->prox;
    }
    fclose(arq_func);
    printf(ANSI_GREEN "✔ Dados salvos com sucesso em: %s" ANSI_RESET "\n", caminho_completo);
}

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
    printf("\n" ANSI_YELLOW "┌────────────────────────────────────────┐\n");
    printf("│        FUNCIONÁRIO REMOVIDO            │\n");
    printf("├────────────────────────────────────────┤\n");
    printf("│ Código: %-30d │\n", Funcionario->codigo);
    printf("│ Nome:   %-30s │\n", Funcionario->nome);
    printf("│ Cargo:  %-30s │\n", Funcionario->cargo);
    printf("└────────────────────────────────────────┘" ANSI_RESET "\n");
}

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
    free(aux); 
    tam--;
    return aux;
}

void listar_funcionarios(){
    NO *aux = inicio;
    if(aux == NULL){
        printf(ANSI_YELLOW "⚠ Ainda não há funcionários cadastrados." ANSI_RESET "\n\n");
    }else{
        printf(ANSI_CYAN ANSI_BOLD "=== LISTA DE COLABORADORES (%d cadastrados) ===\n" ANSI_RESET, tam);
        printf(ANSI_WHITE "┌────────┬──────────────────────────────┬────────────────────────┬──────────────┐\n");
        printf("│ Código │ Nome                         │ Cargo                  │ Salário      │\n");
        printf("├────────┼──────────────────────────────┼────────────────────────┼──────────────┤\n");
        while (aux != NULL){
            printf("│ %-6d │ %-28s │ %-22s │ R$ %-9.2lf │\n", 
                   aux->codigo, aux->nome, aux->cargo, aux->salario);
            aux = aux->prox;
        }
        printf("└────────┴──────────────────────────────┴────────────────────────┴──────────────┘\n" ANSI_RESET);
    }
}

NO * buscar(int codigo){ 
    NO * aux = inicio;
    if(inicio == NULL){
        printf(ANSI_YELLOW "⚠ A lista de funcionários está vazia." ANSI_RESET "\n\n");
        return NULL;
    }
    while(aux != NULL && aux->codigo != codigo){
        aux = aux->prox;
    }
    if(aux == NULL){
        printf(ANSI_RED "✖ Nenhum funcionário encontrado com o código: %d" ANSI_RESET "\n\n", codigo);
    }else{
        printf("\n" ANSI_GREEN "┌────────────────────────────────────────┐\n");
        printf("│         REGISTRO ENCONTRADO            │\n");
        printf("├────────────────────────────────────────┤\n");
        printf("│ Código: %-30d │\n", aux->codigo);
        printf("│ Nome:   %-30s │\n", aux->nome);
        printf("│ Cargo:  %-30s │\n", aux->cargo);
        printf("│ Salário: R$ %-27.2lf │\n", aux->salario);
        printf("└────────────────────────────────────────┘" ANSI_RESET "\n");
    }
    return aux;
}

void alterar(NO *funcionario){
    if (funcionario == NULL) return;
    int opcao;
    char buffer[255]; 
    double novo_salario;

    do {
        printf("\n" ANSI_CYAN "⚙ Modificando: %s (Cod: %d)" ANSI_RESET "\n", funcionario->nome, funcionario->codigo);
        printf("  [" ANSI_WHITE "1" ANSI_RESET "] Alterar Nome\n");
        printf("  [" ANSI_WHITE "2" ANSI_RESET "] Alterar Cargo\n");
        printf("  [" ANSI_WHITE "3" ANSI_RESET "] Alterar Salário\n");
        printf("  [" ANSI_WHITE "0" ANSI_RESET "] Concluir Alterações\n");
        printf("Escolha uma opção: ");
        if(scanf("%d", &opcao) != 1) { 
             while(getchar()!='\n'); 
             opcao = -1;
        }

        switch (opcao) {
            case 1:
                printf("Digite o novo nome: ");
                scanf(" %[^\n]", buffer);
                if (string_contem_numeros(buffer)) {
                    printf(ANSI_RED "✖ Erro: Nome não pode conter números." ANSI_RESET "\n");
                } else {
                    strcpy(funcionario->nome, buffer); 
                    printf(ANSI_GREEN "✔ Nome alterado com sucesso." ANSI_RESET "\n");
                }
                break;
            case 2:
                printf("Digite o novo cargo: ");
                scanf(" %[^\n]", buffer);
                if (string_contem_numeros(buffer)) {
                    printf(ANSI_RED "✖ Erro: Cargo não pode conter números." ANSI_RESET "\n");
                } else {
                    strcpy(funcionario->cargo, buffer);
                    printf(ANSI_GREEN "✔ Cargo alterado com sucesso." ANSI_RESET "\n");
                }
                break;
            case 3:
                printf("Digite o novo salário: ");
                if(scanf("%lf", &novo_salario) != 1 || novo_salario <= 0) {
                     printf(ANSI_RED "✖ Erro: Salário inválido." ANSI_RESET "\n");
                     while(getchar()!='\n');
                } else {
                    funcionario->salario = novo_salario;
                    printf(ANSI_GREEN "✔ Salário alterado com sucesso." ANSI_RESET "\n");
                }
                break;
            case 0:
                printf("\n" ANSI_GREEN "✔ Alterações concluídas!" ANSI_RESET "\n");
                break;
            default:
                printf(ANSI_RED "⚠ Opção inválida!" ANSI_RESET "\n");
                break;
        }
    } while (opcao != 0);
}

int main() {  
    
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif
    
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
        // LETREIRO ASCII - Identidade Visual Forte para seu Portfólio
        printf(ANSI_CYAN ANSI_BOLD);
        printf("  ██████╗ ██████╗ ███╗   ███╗██████╗ \n");
        printf(" ██╔════╝██╔═══██╗████╗ ████║██╔══██╗\n");
        printf(" ██║     ██║   ██║██╔████╔██║██████╔╝\n");
        printf(" ██║     ██║   ██║██║╚██╔╝██║██╔═══╝ \n");
        printf(" ╚██████╗╚██████╔╝██║ ╚═╝ ██║██║     \n");
        printf("  ╚══════╝ ╚═════╝ ╚═╝     ╚═╝╚═╝     \n");
        printf("  [ RH Management System v1.0 ]      \n" ANSI_RESET);
        printf("  Armazenamento: " ANSI_WHITE "%s" ANSI_RESET "\n\n", path_dados);

        printf(ANSI_BOLD "📌 MENU PRINCIPAL" ANSI_RESET "\n");
        printf("  [" ANSI_CYAN "1" ANSI_RESET "] Cadastrar funcionário\n");
        printf("  [" ANSI_CYAN "2" ANSI_RESET "] Listar funcionários\n");
        printf("  [" ANSI_CYAN "3" ANSI_RESET "] Buscar funcionário\n");
        printf("  [" ANSI_CYAN "4" ANSI_RESET "] Alterar funcionário\n");
        printf("  [" ANSI_CYAN "5" ANSI_RESET "] Remover funcionário\n");
        printf("  [" ANSI_RED "0" ANSI_RESET "] Sair e Salvar\n\n");
        printf("Escolha uma opção: ");
        
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
                
                printf(ANSI_CYAN ANSI_BOLD "\n=== NOVO CADASTRO ===" ANSI_RESET "\n");

                do {
                    printf("\n" ANSI_WHITE "➔" ANSI_RESET " Código: "); 
                    if (scanf("%d", &cod) != 1) {
                        while(getchar() != '\n'); 
                        cod = -1;
                    }
                    if(cod < 0) {
                        printf(ANSI_RED "✖ Erro: Código não pode ser negativo." ANSI_RESET "\n");
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
                            printf(ANSI_RED "✖ Erro: já existe um funcionário com o código %d" ANSI_RESET "\n", cod);
                        } else {
                            codigo_valido = 1; 
                        }
                    }
                } while (!codigo_valido);

                do {
                    printf("\n" ANSI_WHITE "➔" ANSI_RESET " Nome: "); 
                    scanf(" %[^\n]", nome);
                    if(string_contem_numeros(nome)) {
                        printf(ANSI_RED "✖ Erro: O nome não deve conter números." ANSI_RESET "\n");
                    }
                } while(string_contem_numeros(nome));

                do {
                    printf("\n" ANSI_WHITE "➔" ANSI_RESET " Cargo: "); 
                    scanf(" %[^\n]", cargo);
                    if(string_contem_numeros(cargo)) {
                        printf(ANSI_RED "✖ Erro: O cargo não deve conter números." ANSI_RESET "\n");
                    }
                } while(string_contem_numeros(cargo));

                printf("\n" ANSI_WHITE "➔" ANSI_RESET " Salário: ");
                do {
                    if(scanf("%lf", &salario) != 1) {
                        while(getchar() != '\n');
                        salario = -1;
                    }
                    if(salario <= 0) printf(ANSI_RED "✖ Erro: O salário deve ser maior que zero. Digite novamente: " ANSI_RESET);
                } while (salario <= 0);

                if(cadastrar(nome, cod, cargo, salario)){
                    printf("\n" ANSI_GREEN "✔ Funcionário cadastrado com sucesso!" ANSI_RESET "\n\n");
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
                printf(ANSI_CYAN ANSI_BOLD "\n=== BUSCAR FUNCIONÁRIO ===" ANSI_RESET "\n");
                printf("Digite o código para buscar: ");
                scanf("%d", &cod);
                buscar(cod);
                pressionar_enter();
                break;
            case 4: {
                limpar_terminal();
                printf(ANSI_CYAN ANSI_BOLD "\n=== ALTERAR FUNCIONÁRIO ===" ANSI_RESET "\n");
                printf("Digite o código para alterar: ");
                scanf("%d", &cod);
                NO *alterarFuncionario = buscar(cod);
                alterar(alterarFuncionario);
                pressionar_enter();
                break;
            }
            case 5:
                limpar_terminal();
                printf(ANSI_CYAN ANSI_BOLD "\n=== REMOVER FUNCIONÁRIO ===" ANSI_RESET "\n");
                printf("Digite o código para remover: ");
                scanf("%d", &cod);
                if (remover(cod) == NULL) {
                    printf("\n" ANSI_RED "✖ Nenhum funcionário encontrado com o código %d." ANSI_RESET "\n", cod);
                } else {
                    printf("\n" ANSI_GREEN "✔ Usuário removido com sucesso!" ANSI_RESET "\n");
                }
                pressionar_enter();
                break;
            case 0:
                limpar_terminal();
                printf(ANSI_YELLOW "Sincronizando dados no disco..." ANSI_RESET "\n");
                salvar_dados(path_dados); 
                printf(ANSI_GREEN "\n[Programa encerrado com segurança]" ANSI_RESET "\n");
                break;
            default:
                limpar_terminal();
                printf(ANSI_RED "⚠ Opção inválida! Tente novamente." ANSI_RESET "\n");
        }
    } while (opcao != 0);

    return 0;
}