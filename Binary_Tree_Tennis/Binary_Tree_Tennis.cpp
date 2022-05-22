#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define MAX_NAME 20
#define STAGES 15

// Data do n�

typedef struct PLAYER {
    char name[MAX_NAME];
    int sets;
};

// N� da �rvore

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

// Prot�tipos

BTREE_NODE* NewBtreeNode(void* data); // Cria um novo n� alocando-o din�micamente, retorna null caso falhe.
BTREE_NODE* AddBtreeNode(BTREE_NODE* upnode, BTREE_NODE* node, BTREE_LOCATION where); // Anexa um n� a uma �rvore dados o n� diretamente superior e a orienta��o.
bool ReadPlayersFromFile(void** players, char* file_name); // Carrega uma lista de jogadores lida atrav�s de um ficheiro para um apontador tipo void**.
bool IsBtreeLeaf(BTREE_NODE* node); // Devolve true caso o n� passado seja folha, false cc.
int CountLeafs(BTREE_NODE* btree);  // Devolve um inteiro com o n�mero de folhas atual.
void PrintLeafs(BTREE_NODE* root);  // Apresenta no ecr� os n�s folha dada a ra�z de uma �rvore.
BTREE_NODE* CreateBtree(void** v, int i, int size); // Cria uma �rvore dados o apontador void** que cont�m os dados (previamente lidos do ficheiro), o contador inicial e a altura m�xima da �rvore.
BTREE_NODE* InitNode(void* data, BTREE_NODE* node1, BTREE_NODE* node2); // Inicializa umn� atribu�ndo-lhe uma data, um n� esquerdo e um n� direito.
void PrintGame(BTREE_NODE* root); // Fun��o que mostra no ecr� os dados de um �nico jogo (Jogo entre filho da esquerda vs filho da direita).
void PrintAllGames(BTREE_NODE* root); // Fun��o semelhante � fun��o PrintGame(), mas no caso mostra todos os jogos.
int BTreeDeep(BTREE_NODE* root); // Fun��o que calcula a altura da �rvore (Dist�ncia m�xima da ra�z at� a folha mais afastada).
int BTreeSize(BTREE_NODE* root); // Fun��o que conta o n�mero de n�s da �rvore e retorna esse valor.
int CountTotalSets(BTREE_NODE* root); // Fun��o que conta o n�mero de sets de cada jogador e retorna o somat�rio deles.
void PrintWinnerGames(BTREE_NODE* root); // Fun��o que apresenta no ecr� os jogos dispotados pelo vencedor.
int CountWinnerSets(BTREE_NODE* root, void* winner); // Fun��o que conta o total de sets ganhos pelo vencedor.
void Buffer(void); // Fun��o que limpa o buffer.
void Menu(char* OP); // Menu de op��es.

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
            // Apresenta o n�mero de eliminat�rias
        case '3':
            printf("\n-----------------------\nNumero de Eliminatorias\n-----------------------\n");
            printf("\n%d\n", BTreeDeep(tree));
            break;
            // Apresenta o n�mero de jogos
        case '4':
            printf("\n---------------\nNumero de Jogos\n---------------\n");
            printf("\n%d\n", BTreeSize(tree)/2);
            break;
            // Apresenta o n�mero de sets
        case '5':
            printf("\n--------------\nNumero de Sets\n--------------\n");
            printf("\n%d\n", CountTotalSets(tree));
            break;
            // Apresenta o vencedor do torneio
        case '6':
            printf("\n-------------------\nVencedor do Torneio\n-------------------\n");
            printf("\nVencedor: %s\n", ((PLAYER*)DATA(tree))->name); // Quem est� na ra�z da �rvore � o atual vencedor.
            break;
            // Apresenta os jogos dispotados pelo vencedor
        case '7':
            printf("\n------------------------------\nJogos dispotados pelo Vencedor\n------------------------------\n\n");
            PrintWinnerGames(tree);
            break;
            // Apresenta o n�mero de sets ganhos pelo vencedor
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
    // Se a aloca��o falhar, retorna
    if ((ptr = (BTREE_NODE*)malloc(sizeof(BTREE_NODE))) == NULL)
        return NULL;
    // Caso contr�rio, inicializa o n� por defeito
    DATA(ptr) = data;
    LEFT(ptr) = NULL;
    RIGHT(ptr) = NULL;
    return ptr;
}

BTREE_NODE* AddBtreeNode(BTREE_NODE* upnode, BTREE_NODE* node, BTREE_LOCATION where)
{
    BTREE_NODE* tmp = upnode;

    // N� superior ou novo n� nulos?

    if (upnode == NULL || node == NULL)
        return NULL;

    // No caso de ser para adicionar � esquerda

    if (where == BTREE_LEFT) {
        if (LEFT(upnode) == NULL)
            LEFT(upnode) = node;
        else
            tmp = NULL;
    }

    // No caso de ser para adicionar � direita

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

        // Se entrar aqui � porque a aloca��o falhou, neste caso � necess�rio libertar o que seja que foi criado previamente

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

    // Um n� � folha se n�o tiver filhos. (Esquerda & Direita nulas)

    if ((LEFT(node)) == NULL && (RIGHT(node)) == NULL)
        return true;
    return false;
}
BTREE_NODE* InitNode(void* data, BTREE_NODE* node1, BTREE_NODE* node2)
{
    BTREE_NODE* tmp = NULL;
    if (tmp = NewBtreeNode(data)) {
        LEFT(tmp) = node1; // Este n� pode ser nulo
        RIGHT(tmp) = node2; // Este n� pode ser nulo
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

    // Se a ra�z for nula return.

    if (root == NULL)
        return;

    // No caso de ser folha, mostra a informa��o do jogador

    if (IsBtreeLeaf(root))
        printf("\nNome -> %s\nSets -> %i\n", ((PLAYER*)DATA(root))->name, ((PLAYER*)DATA(root))->sets);
    
    if (LEFT(root) != NULL)
        PrintLeafs(LEFT(root));
    if (RIGHT(root) != NULL)
        PrintLeafs(RIGHT(root));
}
void PrintGame(BTREE_NODE* root) {
    // Apenas temos um jogo caso haja 2 jogadores, ou seja, um filho � esquerda e outro � direita.
    // Como uma folha n�o tem filhos, se tentarmos chamar a fun��o numa folha vai dar erro.

    if (root != NULL && !IsBtreeLeaf(root))
        printf("%s (%d) vs %s (%d)\n", ((PLAYER*)DATA(LEFT(root)))->name, ((PLAYER*)DATA(LEFT(root)))->sets, ((PLAYER*)DATA(RIGHT(root)))->name, ((PLAYER*)DATA(RIGHT(root)))->sets);
}
void PrintAllGames(BTREE_NODE* root) {
    // Para mostrar todos os jogos vamos usar a fun��o PrintGame(),
    // Se n�o estivermos numa folha e o n� n�o for nulo podemos chamar a fun��o PrintGame(),
    // Para mostrar todos os jogos basta fazer chamadas recursivas dentro desta condi��o.

    if (root != NULL && !IsBtreeLeaf(root)) {
        PrintAllGames(LEFT(root));
        PrintAllGames(RIGHT(root));
        PrintGame(root);
    }
}
int BTreeDeep(BTREE_NODE* root) {
    // Se a ra�z for nula a �rvore n�o existe e como tal a altura � 0.

    if (root == NULL)
        return 0;

    // Chamada recursiva para obter as alturas da sub-�rvore esquerda e da sub-�rvore direita.

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
    // Se a ra�z � nula a �rvore n�o existe logo retorna 0.

    if (root == NULL)
        return 0;

    // Se n�o � nula, somamos o n� de sets do jogador no n� atual e chamamos recursivamente a fun��o para as sub-�rvores da esquerda e direita

    return ((PLAYER*)DATA(root))->sets + CountTotalSets(LEFT(root)) + CountTotalSets(RIGHT(root));
}
void PrintWinnerGames(BTREE_NODE* root) {
    // Apenas imprime caso a ra�z n�o seja nula e n�o seja folha pois a fun��o PrintGame j� procura pelo filho esquerdo e direito.

    if (root != NULL && !IsBtreeLeaf(root)) {
        PrintGame(root);
        
        // Procura pelos jogos do vencedor, compara o nome, se ele veio da sub-�rvore esquerda mostra os jogos da esquerda e vice-versa.

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