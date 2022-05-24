#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "stdlib.h"
#include "Windows.h"

#define MAX_NAME 30
#define DIRETORIAS 15
int i = 1;

typedef struct DIR {
    char nome[MAX_NAME]; // Nome Ficheiro
    int n_fich; // Nº Ficheiros
    int n_Kb; // Nº Kbytes
};
typedef struct BTREE_NODE {
    void* data;
    struct BTREE_NODE* left;
    struct BTREE_NODE* right;
};

// Macros

#define DATA(node) ((node)->data)
#define LEFT(node) ((node)->left)
#define RIGHT(node) ((node)->right)

// Protótipos

BTREE_NODE* NewBtreeNode(void* data); // Tenta alocar e retornar um novo nó com a data passada por parâmetro
void BtreeFree(BTREE_NODE* root); // Liberta todos os nós de uma árvore previamente alocados
BTREE_NODE* InitNode(void* data, BTREE_NODE* left, BTREE_NODE* right); // Inicializa um nó alocado-o e fazendo a ligação com o filho esquerdo e direito
BTREE_NODE* CreateBtree(void** data, int i, int size); // Inicializa uma árvore através da leitura de um apontador carregado com dados do tipo void**
bool ReadFile(char* fileName, void** data); // Função que lê as diretorias de um ficheiro, (no caso "Diretorias.txt"), formato dos dados: string;int;int\n
void findDirectories(BTREE_NODE* root, int value); // Percorre a árvore de forma inorder e apresenta o nome das diretorias com nº de Kbs superior a value
int CountAllDirectoryFiles(BTREE_NODE* root, char* name); // Devolve um inteiro que representa o nº total de ficheiros numa diretoria e suas subdiretorias, (nº de nós abaixo de um nó)
bool IsDirectory(BTREE_NODE* root, const char* name); // Verifica o nó passado é a diretoria passada
bool IsBtreeNode(BTREE_NODE* root, const char* name); // Verifica se um nó pertence a uma árvore
int CountDirectoryFiles(BTREE_NODE* Root); // Retorna o nº de ficheiros que uma única diretoria contém
int CountEmptyDirectories(BTREE_NODE* root); // Retorna o número total de diretorias vazias (n_fich = 0)
void Menu(char* OP);
void Buffer(void);

int main(void)
{
    BTREE_NODE* tree;
    void* Diretorias[DIRETORIAS];
    char file[MAX_NAME] = "Diretorias.txt";
    char OP = ' ';
    char dirName[MAX_NAME] = " ";
    printf("Tentando ler o ficheiro %s...\n", file);
    Sleep(1000);
    if (!ReadFile(file, Diretorias)) {
        printf("Ocorreu um erro ao tentar ler o ficheiro!\"%s\"\n", file);
        return 0;
    }
    printf("\"%s\" lido com sucesso!\n", file);
    if ((tree = CreateBtree(Diretorias, 0, DIRETORIAS)) == NULL) {
        printf("Ocorreu um erro ao tentar carregar a informacao para uma arvore!\n");
        return 0;
    }
    Sleep(1000);
    system("CLS");
    // Apresentação do Menu
    do {
        Menu(&OP);
        switch (OP) {
            // Saída do programa
        case '0':
            exit(0);
            break;
            // Apresenta diretorias com nº Kbs superior a 1000
        case '1':
            printf("\n-------------------------------\nDiretorias com mais de 1000 Kbs\n-------------------------------\n");
            findDirectories(tree, 1000);
            i = 1;
            break;
            // Apresenta no ecrã o número de ficheiros dentro de uma diretoria (Nº de nós abaixo de um nó)
        case '2':
            printf("\n-------------------------------------------\nNumero de ficheiros dentro de uma Diretoria\n-------------------------------------------\n");
            printf("Nome da Diretoria: ");
            scanf("%s", &dirName);
            if (IsBtreeNode(tree, dirName)) {
                printf("Numero de Ficheiros da Diretoria \"%s\" e suas Subdiretorias: %d Ficheiros\n", dirName, CountAllDirectoryFiles(tree, dirName));
            }
            else
                printf("A Diretoria \"%s\" nao existe!\n", dirName);
            break;
            // Apresenta no ecrã o número de diretorias vazias
        case '3':
            printf("\n---------------------------\nNumero de Diretorias vazias\n---------------------------\n");
            printf("Numero de Diretorias vazias: %d\n", CountEmptyDirectories(tree));
            break;
            // Opção inválida
        default:
            printf("Opcao invalida!\n");
            break;
        }
        strcpy(dirName, " ");
        printf("\n");
        Buffer();
        system("pause");
        system("CLS");
    } while (OP != '0');
}

// Funções

void BtreeFree(BTREE_NODE* root) {
    // Caso a root seja nula retorna para libertar o nó pai, se a root for nula desde vo início, a árvore está vazia
    if (root == NULL)
        return;
    // Chamada recursiva para libertar primeiro todos os nós que estão abaixo do atual
    BtreeFree(LEFT(root));
    BtreeFree(RIGHT(root));
    // Libertação dos dados
    free(DATA(root));
    free(root);
}
BTREE_NODE* CreateBtree(void** data, int i, int size) {
    if (i >= size)
        return NULL;
    return(InitNode(*(data + i), CreateBtree(data, 2 * i + 1, size), CreateBtree(data, 2 * i + 2, size)));
}
BTREE_NODE* InitNode(void* data, BTREE_NODE* left, BTREE_NODE* right) {
    BTREE_NODE* newNode;
    // Verifica se os dados são válidos
    if (data == NULL)
        return NULL;
    // Tenta inicializar o nó
    if ((newNode=NewBtreeNode(data)) == NULL)
        return NULL;
    // Estabelece a ligação entre os filhos
    LEFT(newNode) = left;
    RIGHT(newNode) = right;
    // Retorna o nodo inicializado
    return newNode;

}
bool ReadFile(char* fileName, void** data) {
    FILE* ptr = NULL;
    void* voidPTR = NULL;
    int i = 0;
    // Verifica se os parâmetros são válidos
    if (fileName == NULL && data == NULL)
        return false;
    // Tenta abrir o ficheiro
    if ((ptr = fopen(fileName, "r")) == NULL)
        return false;
    // Carrega os dados para o ponteiro void**
    while (!feof(ptr)) {
        // Tenta alocar dinâmicamente um ponteiro void* para carregar as informações no ponteiro void**
        if ((voidPTR = malloc(sizeof(DIR))) != NULL) {
            fscanf(ptr, "%[^;];%d;%d\n", ((DIR*)voidPTR)->nome, &((DIR*)voidPTR)->n_fich, &((DIR*)voidPTR)->n_Kb);
            data[i++] = voidPTR;
        }
        // Caso esta alocação falhe, é necessário desalocar qualquer dados que tenha sido préviamente
        else {
            for (int j = i; j >= 0; j--)
                free(data[j]);
            return false;
        }
    }
    fclose(ptr);
    return true;
}
BTREE_NODE* NewBtreeNode(void* data) {
    BTREE_NODE* newNode;
    // Verifica se a data é válida
    if (data == NULL)
        return NULL;
    // Tenta alocar dinâmicamente
    if ((newNode = (BTREE_NODE*)malloc(sizeof(BTREE_NODE))) == NULL)
        return NULL;
    // Atribuição dos dados
    DATA(newNode) = data;
    LEFT(newNode) = NULL;
    RIGHT(newNode) = NULL;
    // Retorno
    return newNode;
}

void findDirectories(BTREE_NODE* root, int value) {
    // Verifica se os dados são válidos
    if (root == NULL || value == NULL)
        return;
    // Faz a comparação, no caso do ex. 1000 Kbs
    if (((DIR*)DATA(root))->n_Kb > value)
        printf("\n#Diretoria %d:\nNome: %s\nNumero do Ficheiro: %i\nNumero de Kbs: %i\n", i++, ((DIR*)DATA(root))->nome, ((DIR*)DATA(root))->n_fich, ((DIR*)DATA(root))->n_Kb);
    // Chamada recursiva para as sub-árvores esquerda e direita
    findDirectories(LEFT(root), value);
    findDirectories(RIGHT(root), value);
}
bool IsDirectory(BTREE_NODE* root, const char* name) {
    // Verifica se os dados são válidos
    if (root == NULL || name == NULL)
        return false;
    // Verifica se o nó atual é o nó procurado
    if (!strcmp((((DIR*)DATA(root))->nome), name))
        return true;
    return false;
}
int CountDirectoryFiles(BTREE_NODE* root) {
    // Se a raíz for nula, não há nada a contar
    if (root == NULL)
        return 0;
    // Chamada recursiva que retorna o somatório dos ficheiros das sub-árvores esquerda e direita
    return ((DIR*)DATA(root))->n_fich + CountDirectoryFiles(LEFT(root)) + CountDirectoryFiles(RIGHT(root));
}
int CountAllDirectoryFiles(BTREE_NODE* root, char* name) {

    // Verifica se os dados são válidos
    if (root == NULL || name == NULL)
        return 0;
    // Verfifica se a diretoria pertence à árvore
    if (IsDirectory(root, name))
        return CountDirectoryFiles(root); //retorna o numero de files desse nó para baixo

    // Chamada recursiva às sub-árvores da esquerda e direita para fazer a contagem dos ficheiros que se encontram abaixo
    return CountAllDirectoryFiles(LEFT(root), name) + CountAllDirectoryFiles(RIGHT(root), name);
}

bool IsBtreeNode(BTREE_NODE* root, const char* name) {
    // Verifica se os dados são válidos
    if (root == NULL || name == NULL)
        return false;
    // Verifica se o nó atual é o procurado
    if (IsDirectory(root, name))
        return true;

    // Chamada recursiva até encontrar o nó procurado, (se não encontrar retorna false)
    return IsBtreeNode(LEFT(root), name) || IsBtreeNode(RIGHT(root), name);
}

int CountEmptyDirectories(BTREE_NODE* root) {
    int count = 0;
    // Se a raíz fora nula não há nada a contar
    if (root == NULL)
        return 0;
    // Se o nó atual tiver um número de ficheiros igual a 0, soma 1 ao contador
    if (((DIR*)DATA(root))->n_fich == 0)
        return 1;
    // Chama a função recursivamente de modo a contar as diretorias vazias das sub-árvores esquerda e direita
    count += CountEmptyDirectories(LEFT(root)) + CountEmptyDirectories(RIGHT(root));
}
void Menu(char* OP) {
    printf("--------------------\nSelecione uma opcao:\n--------------------\n");
    printf("[1]Apresentar diretorias com mais de 1000 Kbs armazenados\n");
    printf("[2]Contar o numero de Ficheiros de uma Diretoria\n");
    printf("[3]Contar o numero de Diretorias Vazias\n");
    printf("[0]Sair\nOpcao: ");
    scanf("%c", OP);
}
void Buffer(void) {
    while (getchar() != '\n');
    fflush(stdin);
}
