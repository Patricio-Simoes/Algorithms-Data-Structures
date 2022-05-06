#include <stdio.h>
#include <stdlib.h>

// Macros

#define DATA(node) ((node)->pData)
#define LEFT(node) ((node)->pLeft)
#define RIGHT(node) ((node)->pRight)
#define EMPTY NULL
#define NO_LINK NULL

typedef enum _STATUS { ERROR = 0, OK = 1 } STATUS; // N�o uso no c�digo
typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN; // N�o uso no c�digo

typedef struct _BTREE_NODE {
	void* pData;
	struct _BTREE_NODE* pLeft;
	struct _BTREE_NODE* pRight;
}BTREE_NODE;

typedef BTREE_NODE* BTREE;

// Prot�tipos das fun��es

bool initBTree(BTREE*); // Inicializa a ra�z como nula
bool isEmptyBTree(BTREE); // Retorna true se a �rvore estiver vazia (nula), false caso contr�rio
bool isLeaf(BTREE); // Retorna true se a ra�z n�o tiver filhos
bool createNewBTreeNode(BTREE* pNew, void* pData); // Instancia um novo n� sem filhos com a data passada por par�metro
void printBTreeInorder(BTREE); // Apresenta a �rvore no ecr� (Left -> Root -> Right)
void printBTreePreorder(BTREE); // Apresenta a �rvore no ecr� (Root -> Left -> Right)
void printBTreePostorder(BTREE); // Apresenta a �rvore no ecr� (Left -> Right -> Root)
bool insertBST(BTREE* Root, void* pData); // Insere uma nova folha na �rvore passada por par�metro
BTREE searchBST(BTREE Root, int x); // Se um inteiro pertencer a uma �rvore dada a sua ra�z, devolve o n�
int countTreeNodes(BTREE Root); // Devolve o n� de n�s de uma �rvore
int countTreeLeaves(BTREE Root); // Devolve o n� de folhas de uma �rvore
int treeHeight(BTREE Root); // Devolve a altura da �rvore (Maior n� poss�vel de arestas em um caminho de um n� folha at� um n� alvo)
int findDepth(BTREE Root, int x); // Devolve a profundidade de um n�, -1 caso n�o exista
void Menu(char* OP); // Menu de op��es
void Buffer(void); // Limpa o Buffer

// Main Program

int main(void) {
	BTREE Root;
	char OP = ' ';
	int value = 0, aux = 0;
	int* pA;
	int* arr;
	initBTree(&Root);
	/*pA = (int*)malloc(sizeof(int));
	*pA = 0;
	initBTree(&Root);
	int x = 5;
	insertBST(&Root, &x);
	int y = 1;
	insertBST(&Root, &y);
	int z = 1;
	insertBST(&Root, &z);
	insertBST(&Root, pA);
	int t = 4;
	insertBST(&Root, &t);*/

	do {
		Menu(&OP);
		switch (OP) {
		// Sair do Programa
		case '0': exit(0); break;
		// Criar arvore atraves da leitura de um array
		case '1': 
			printf("\n-----------------\nCriacao da Arvore\n-----------------\n\n");
			printf("Numero de elementos a inserir: ");
			scanf_s("%d", &aux);
			if ((arr = (int*)calloc(aux, sizeof(int))) == NULL)
				printf("\nOcorreu um erro na alocacao dinamica!");
			// Leitura
			for (int i = 0; i < aux; i++) {
				printf("Elemento %d: ", i);
				scanf_s("%d", &arr[i]);
			}
			// Cria��o da �rvore
			for (int i = 0; i < aux; i++)
				insertBST(&Root, &arr[i]);
			break;
		// Travessia Inorder
		case '2':
			printf("\n----------------\nTravessia Inorder\n----------------\n\n");
			printBTreeInorder(Root);
			break;
		// Travessia Preorder
		case '3':
			printf("\n-----------------\nTravessia Preorder\n-----------------\n\n");
			printBTreePreorder(Root);
			break;
		// Travessia Postorder
		case '4':
			printf("\n------------------\nTravessia Postorder\n------------------\n\n");
			printBTreePostorder(Root);
			break;
		// Contagem de n�s
		case '5':
			printf("\n---------------\nContagem de nos\n---------------\n");
			printf("\nNumero de nos da arvore atual: %d", countTreeNodes(Root));
			break;
		// Contagem de folhas
		case '6':
			printf("\n------------------\nContagem de folhas\n------------------\n");
			printf("\nNumero de folhas da arvore atual: %d", countTreeLeaves(Root));
			break;
		// Determinar o nivel de um n�
		case '7':
			printf("\n--------------\nNivel de um no\n--------------\n");
			printf("Valor do no a procurar: ");
			scanf_s("%d", &value);
			Buffer();
			switch (aux = findDepth(Root, value)) {
			case -1: printf("\nValor nao encontrado!"); break;
			default: printf("Valor encontrado. Profundidade: %d", aux); break;
			}
			break;
		// Determinar a profundidade da arvore
		case '8' :
			printf("\n----------------------\nProfundidade da Arvore\n----------------------\n");
			printf("Profundidade da arvore atual: %d", treeHeight(Root));
			break;
		// Opcao fora do esperado
		default:
			printf("\nOpcao invalida! Tente novamente!");
			break;
		}
		printf("\n\n");
		system("pause");
		Buffer();
		system("cls");
	} while (OP != '0');
	return 0;
}

// Fun��es

bool initBTree(BTREE* pBT) {
	*pBT = NULL;
	return true;
}
bool isEmptyBTree(BTREE Root) {
	// Ra�z est� vazia?
	return (Root == NULL) ? true : false;
}
bool isLeaf(BTREE Tree) {
	// Ra�z tem filho � esquerda/direita?
	return ((LEFT(Tree) == NULL) && RIGHT(Tree) == NULL) ? true : false;
}
bool createNewBTreeNode(BTREE* pNew, void* pData) {
	BTREE pTemp;
	// Aloca��o Din�mica
	if ((pTemp = (BTREE)malloc(sizeof(BTREE_NODE))) == NULL)
		return false;
	*pNew = pTemp;
	DATA(pTemp) = pData;
	LEFT(pTemp) = NULL;
	RIGHT(pTemp) = NULL;
	return true;
}
void printBTreeInorder(BTREE Root) {
	// �rvore vazia?
	if (isEmptyBTree(Root) == true) 
		return;
	printBTreeInorder(LEFT(Root));
	printf("%d -> ", *(int*)DATA(Root));
	printBTreeInorder(RIGHT(Root));
	return;
}
void printBTreePreorder(BTREE Root) {
	// �rvore vazia?
	if (isEmptyBTree(Root) == true)
		return;
	printf("%d -> ", *(int*)DATA(Root));
	printBTreePreorder(LEFT(Root));
	printBTreePreorder(RIGHT(Root));
	return;
}
void printBTreePostorder(BTREE Root) {
	// �rvore vazia?
	if (isEmptyBTree(Root) == true)
		return;	
	printBTreePostorder(LEFT(Root));
	printBTreePostorder(RIGHT(Root));
	printf("%d -> ", *(int*)DATA(Root));
	return;
}
bool insertBST(BTREE* Root, void* pData) {
	// �rvore vazia? <=> Nova folha = Ra�z
	if (isEmptyBTree(*Root)) {
		BTREE pNew;
		if (createNewBTreeNode(&pNew, pData) == NULL)
			return false;
		*Root = pNew;
		return true;
	}
	// Se n�o estiver vazia verifica onde deve colocar a nova folha
	// Se for menor coloca � esquerda
	// Se for maior ou igual coloca � direita
	if (*(int*)(pData) < *(int*)DATA(*Root))
		insertBST(&(LEFT(*Root)), pData);
	else
	insertBST(&(RIGHT(*Root)), pData);
	return true;
}
BTREE searchBST(BTREE Root, int x) {
	// Ra�z nula ou valor na ra�z?
	if (Root == NULL || *(int*)DATA(Root) == x)
		return Root;
	// Procura na sub-�rvore da esquerda
	if (x < *(int*)DATA(Root))
		return searchBST(LEFT(Root), x);
	return searchBST(LEFT(Root), x);
}
int countTreeNodes(BTREE Root) {
	return (Root == NULL) ? 0 : (1 + countTreeNodes(LEFT(Root)) + countTreeNodes(RIGHT(Root)));
}
int countTreeLeaves(BTREE Root) {
	// �rvore vazia?
	if (isEmptyBTree(Root))
		return 0;
	// Verifica se o n� atual � uma folha
	if (LEFT(Root) == NULL && RIGHT(Root) == NULL)
		return 1;
	// Retorna a soma de folhas da sub-�rvore esquerda com a sub-�rvore direita
	return countTreeLeaves(LEFT(Root)) + countTreeLeaves(RIGHT(Root));
}
int treeHeight(BTREE Root) {
	// �rvore vazia <=> Height = -1
	if (isEmptyBTree(Root))
		return -1;
	int altEsquerda = treeHeight(LEFT(Root));
	int altDireita = treeHeight(RIGHT(Root));
	if (altEsquerda > altDireita)
		return altEsquerda + 1;
	return altDireita + 1;
}
int findDepth(BTREE Root, int x) {
	int depth = -1;
	// �rvore vazia?
	if (isEmptyBTree(Root))
		return depth;
	// Ra�z = n�?
	if (*(int*)DATA(Root) == x)
		return depth + 1;
	// Verifica se o valor est� presente na sub-�rvore da esquerda ou na sub-�rvore da direita
	if ((depth = findDepth(LEFT(Root), x)) >= 0 || (depth = findDepth(RIGHT(Root), x)) >= 0)
		return depth + 1;
	// N�o encontrou o n�
	return depth;
}

void Buffer(void) {
	fflush(stdin);
	while (getchar() != '\n');
}
void Menu(char* OP) {
	printf("#############################################\n#                                           #\n# Bem vindo ao programa Binary Tree Basics! #\n#                                           #\n#############################################\n\n");
	printf("[1]Criar arvore atraves da leitura de um array\n");
	printf("[2]Apresentar arvore com travessia Inorder\n");
	printf("[3]Apresentar arvore com travessia Preorder\n");
	printf("[4]Apresentar arvore com travessia Postorder\n");
	printf("[5]Contar o numero de nos\n");
	printf("[6]Contar o numero de folhas\n");
	printf("[7]Determinar o nivel de um no\n");
	printf("[8]Determinar a profundidade da arvore\n");
	printf("[0]Sair\n\nOpcao: ");
	scanf_s("%c", OP);
}