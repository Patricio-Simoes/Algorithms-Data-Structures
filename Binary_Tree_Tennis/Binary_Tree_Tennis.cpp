#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define MAX_NAME 20
#define STAGES 15

// Data do nó

typedef struct PLAYER {
    char name[MAX_NAME];
    int sets;
};

// Nó da árvore

typedef struct BTREE_NODE {
    void* data;
    struct BTREE_NODE* left;
    struct BTREE_NODE* right;
};

// Macros

typedef enum BTREE_LOCATION { BTREE_LEFT, BTREE_RIGHT };
#define DATA(node) ((node)->data)
#define LEFT(node) ((node)->left)
#define RIGHT(node) ((node)->right)

// Protótipos

BTREE_NODE* NewBtreeNode(void* data); // Cria um novo nó alocando-o dinâmicamente, retorna null caso falhe.
BTREE_NODE* AddBtreeNode(BTREE_NODE* upnode, BTREE_NODE* node, BTREE_LOCATION where); // Anexa um nó a uma árvore dados o nó diretamente superior e a orientação.
bool ReadPlayersFromFile(void** players, char* file_name); // Carrega uma lista de jogadores lida através de um ficheiro para um apontador tipo void**.
bool IsBtreeLeaf(BTREE_NODE* node); // Devolve true caso o nó passado seja folha, false cc.
int CountLeafs(BTREE_NODE* btree);  // Devolve um inteiro com o número de folhas atual.
void PrintLeafs(BTREE_NODE* root);  // Apresenta no ecrã os nós folha dada a raíz de uma árvore.
BTREE_NODE* CreateBtree(void** v, int i, int size); // Cria uma árvore dados o apontador void** que contém os dados (previamente lidos do ficheiro), o contador inicial e a altura máxima da árvore.
BTREE_NODE* InitNode(void* data, BTREE_NODE* node1, BTREE_NODE* node2); // Inicializa umnó atribuíndo-lhe uma data, um nó esquerdo e um nó direito.
void PrintGame(BTREE_NODE* root); // Função que mostra no ecrã os dados de um único jogo (Jogo entre filho da esquerda vs filho da direita).
void PrintAllGames(BTREE_NODE* root); // Função semelhante à função PrintGame(), mas no caso mostra todos os jogos.
int BTreeDeep(BTREE_NODE* root); // Função que calcula a altura da árvore (Distância máxima da raíz até a folha mais afastada).
int BTreeSize(BTREE_NODE* root); // Função que conta o número de nós da árvore e retorna esse valor.
int CountTotalSets(BTREE_NODE* root); // Função que conta o número de sets de cada jogador e retorna o somatório deles.
void PrintWinnerGames(BTREE_NODE* root); // Função que apresenta no ecrã os jogos dispotados pelo vencedor.
int CountWinnerSets(BTREE_NODE* root, void* winner); // Função que conta o total de sets ganhos pelo vencedor.
void Buffer(void); // Função que limpa o buffer.
void Menu(char* OP); // Menu de opções.

int main(void)
{
    BTREE_NODE* tree;
    char OP = ' ';
    void* players[STAGES];
    char file_name[MAX_NAME] = "Torneio.txt";
    printf("Lendo o ficheiro %s\n", file_name);
    Sleep(1000);
    if (ReadPlayersFromFile(players, file_name)) {
        printf("%s lido com sucesso!", file_name);
        Sleep(1000);
    }
    else {
        printf("Ocorreu um erro ao ler o ficheiro \"%s\"\n", file_name);
        return 0;
    }
    system("CLS");
    tree = CreateBtree(players, 0, STAGES);;
    do {
        Menu(&OP);
        switch (OP) {
            // Sair do Programa
        case '0': exit(0); break;
            // Apresenta a lista dos jogadores
        case '1':
            printf("\n---------------------------\nLista dos %d Participantes\n---------------------------\n", CountLeafs(tree));
            PrintLeafs(tree);
            break;
            // Apresenta a lista de jogos
        case '2':
            printf("\n--------------\nLista de Jogos\n--------------\n\n");
            PrintAllGames(tree);
            break;
            // Apresenta o número de eliminatórias
        case '3':
            printf("\n-----------------------\nNumero de Eliminatorias\n-----------------------\n");
            printf("\n%d\n", BTreeDeep(tree));
            break;
            // Apresenta o número de jogos
        case '4':
            printf("\n---------------\nNumero de Jogos\n---------------\n");
            printf("\n%d\n", BTreeSize(tree)/2);
            break;
            // Apresenta o número de sets
        case '5':
            printf("\n--------------\nNumero de Sets\n--------------\n");
            printf("\n%d\n", CountTotalSets(tree));
            break;
            // Apresenta o vencedor do torneio
        case '6':
            printf("\n-------------------\nVencedor do Torneio\n-------------------\n");
            printf("\nVencedor: %s\n", ((PLAYER*)DATA(tree))->name); // Quem está na raíz da árvore é o atual vencedor.
            break;
            // Apresenta os jogos dispotados pelo vencedor
        case '7':
            printf("\n------------------------------\nJogos dispotados pelo Vencedor\n------------------------------\n\n");
            PrintWinnerGames(tree);
            break;
            // Apresenta o número de sets ganhos pelo vencedor
        case '8':
            printf("\n-------------------------\nSets ganhos pelo Vencedor\n-------------------------\n");
            printf("\n%d\n", CountWinnerSets(tree, ((PLAYER*)DATA(tree))->name));
            break;
        default:
            printf("Opcao invalida!\n");
            break;
        }
        printf("\n");
        Buffer();
        system("pause");
        system("cls");
    } while (OP != '0');
    return 0;
}

BTREE_NODE* NewBtreeNode(void* data)
{
    BTREE_NODE* ptr;
    // Se a alocação falhar, retorna
    if ((ptr = (BTREE_NODE*)malloc(sizeof(BTREE_NODE))) == NULL)
        return NULL;
    // Caso contrário, inicializa o nó por defeito
    DATA(ptr) = data;
    LEFT(ptr) = NULL;
    RIGHT(ptr) = NULL;
    return ptr;
}

BTREE_NODE* AddBtreeNode(BTREE_NODE* upnode, BTREE_NODE* node, BTREE_LOCATION where)
{
    BTREE_NODE* tmp = upnode;

    // Nó superior ou novo nó nulos?

    if (upnode == NULL || node == NULL)
        return NULL;

    // No caso de ser para adicionar à esquerda

    if (where == BTREE_LEFT) {
        if (LEFT(upnode) == NULL)
            LEFT(upnode) = node;
        else
            tmp = NULL;
    }

    // No caso de ser para adicionar à direita

    else if (where == BTREE_RIGHT) {
        if (RIGHT(upnode) == NULL)
            RIGHT(upnode) = node;
        else
            tmp = NULL;
    }
    return tmp;
}
bool ReadPlayersFromFile(void** players, char* file) {
    FILE* fp;
    int i = 0, j = 0;
    void* ptr;

    // Dados nulos?

    if (players == NULL || file == NULL)
        return NULL;

    // Tenta abrir o ficheiro

    if (!(fp = fopen(file, "r")))
        return NULL;

    // Ficheiro abriu, procede a ler linha a linha

    while (!feof(fp)) {
        if ((ptr = malloc(sizeof(PLAYER))) != NULL) {
            fscanf(fp, "%[^;];", ((PLAYER*)ptr)->name);
            fscanf(fp, "%d\n", &((PLAYER*)ptr)->sets);
            players[i] = ptr;
            i++;
        }

        // Se entrar aqui é porque a alocação falhou, neste caso é necessário libertar o que seja que foi criado previamente

        else {
            for (j = i; j >= 0; j--)
                free(players[j]);
            return false;
        }
    }
    fclose(fp);
    return true;
}
bool IsBtreeLeaf(BTREE_NODE* node) {

    // Um nó é folha se não tiver filhos. (Esquerda & Direita nulas)

    if ((LEFT(node)) == NULL && (RIGHT(node)) == NULL)
        return true;
    return false;
}
BTREE_NODE* InitNode(void* data, BTREE_NODE* node1, BTREE_NODE* node2)
{
    BTREE_NODE* tmp = NULL;
    if (tmp = NewBtreeNode(data)) {
        LEFT(tmp) = node1; // Este nó pode ser nulo
        RIGHT(tmp) = node2; // Este nó pode ser nulo
        return(tmp);
    }
    return NULL;
}
BTREE_NODE* CreateBtree(void** v, int i, int size)
{
    if (i >= size)
        return(NULL);
    else
        return(InitNode(*(v + i), CreateBtree(v, 2 * i + 1, size), CreateBtree(v, 2 * i + 2, size)));
}
int CountLeafs(BTREE_NODE* btree) {
    int total = 0;
    if (IsBtreeLeaf(btree) && btree != NULL)
        return 1;
    total += CountLeafs(LEFT(btree)) + CountLeafs(RIGHT(btree));
    return total;
}
void PrintLeafs(BTREE_NODE* root) {

    // Se a raíz for nula return.

    if (root == NULL)
        return;

    // No caso de ser folha, mostra a informação do jogador

    if (IsBtreeLeaf(root))
        printf("\nNome -> %s\nSets -> %i\n", ((PLAYER*)DATA(root))->name, ((PLAYER*)DATA(root))->sets);
    
    if (LEFT(root) != NULL)
        PrintLeafs(LEFT(root));
    if (RIGHT(root) != NULL)
        PrintLeafs(RIGHT(root));
}
void PrintGame(BTREE_NODE* root) {
    // Apenas temos um jogo caso haja 2 jogadores, ou seja, um filho à esquerda e outro à direita.
    // Como uma folha não tem filhos, se tentarmos chamar a função numa folha vai dar erro.

    if (root != NULL && !IsBtreeLeaf(root))
        printf("%s (%d) vs %s (%d)\n", ((PLAYER*)DATA(LEFT(root)))->name, ((PLAYER*)DATA(LEFT(root)))->sets, ((PLAYER*)DATA(RIGHT(root)))->name, ((PLAYER*)DATA(RIGHT(root)))->sets);
}
void PrintAllGames(BTREE_NODE* root) {
    // Para mostrar todos os jogos vamos usar a função PrintGame(),
    // Se não estivermos numa folha e o nó não for nulo podemos chamar a função PrintGame(),
    // Para mostrar todos os jogos basta fazer chamadas recursivas dentro desta condição.

    if (root != NULL && !IsBtreeLeaf(root)) {
        PrintAllGames(LEFT(root));
        PrintAllGames(RIGHT(root));
        PrintGame(root);
    }
}
int BTreeDeep(BTREE_NODE* root) {
    // Se a raíz for nula a árvore não existe e como tal a altura é 0.

    if (root == NULL)
        return 0;

    // Chamada recursiva para obter as alturas da sub-árvore esquerda e da sub-árvore direita.

    int leftDeep = BTreeDeep(LEFT(root));
    int rightDeep = BTreeDeep(RIGHT(root));

    if (leftDeep > rightDeep)
        return leftDeep + 1;
    else
        return rightDeep + 1;
}
int BTreeSize(BTREE_NODE* root) {
    int count = 0;
    if (root != NULL)
        count = 1 + BTreeSize(LEFT(root)) + BTreeSize(RIGHT(root));
    return count;
}
int CountTotalSets(BTREE_NODE* root) {
    // Se a raíz é nula a árvore não existe logo retorna 0.

    if (root == NULL)
        return 0;

    // Se não é nula, somamos o nº de sets do jogador no nó atual e chamamos recursivamente a função para as sub-árvores da esquerda e direita

    return ((PLAYER*)DATA(root))->sets + CountTotalSets(LEFT(root)) + CountTotalSets(RIGHT(root));
}
void PrintWinnerGames(BTREE_NODE* root) {
    // Apenas imprime caso a raíz não seja nula e não seja folha pois a função PrintGame já procura pelo filho esquerdo e direito.

    if (root != NULL && !IsBtreeLeaf(root)) {
        PrintGame(root);
        
        // Procura pelos jogos do vencedor, compara o nome, se ele veio da sub-árvore esquerda mostra os jogos da esquerda e vice-versa.

        if (!strcmp(((PLAYER*)DATA(root))->name, ((PLAYER*)DATA(LEFT(root)))->name))
            PrintWinnerGames(LEFT(root));
        else
            PrintWinnerGames(RIGHT(root));
    }
}
int CountWinnerSets(BTREE_NODE* root, void* winner) {
    int count = 0;
    if (root != NULL) {
        if (!strcmp((((PLAYER*)DATA(root))->name), ((PLAYER*)winner)->name))
            count += ((PLAYER*)DATA(root))->sets;
        count += CountWinnerSets(LEFT(root), winner) + CountWinnerSets(RIGHT(root), winner);
    }
    return count;
}
void Menu(char* OP) {
    printf("#############################################\n#                                           #\n# Bem vindo ao programa Binary Tree Tennis! #\n#                                           #\n#############################################\n\n");
    printf("[1]Lista de participantes\n");
    printf("[2]Lista de jogos\n");
    printf("[3]Numero de Eliminatorias\n");
    printf("[4]Numero de Jogos\n");
    printf("[5]Numero de Sets\n");
    printf("[6]Vencedor do Torneio\n");
    printf("[7]Jogos dispotados pelo Vencedor\n");
    printf("[8]Sets ganhos pelo Vencedor\n");
    printf("[0]Sair do programa\nOpcao: ");
    scanf("%c", OP);
}
void Buffer(void) {
    while (getchar() != '\n');
    fflush(stdin);
}