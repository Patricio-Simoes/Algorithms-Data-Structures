#include <stdio.h>
#include <stdlib.h>

// Macros

#define DATA(node) ((node)->pData)
#define LEFT(node) ((node)->pLeft)
#define RIGHT(node) ((node)->pRight)
#define EMPTY NULL
#define NO_LINK NULL

typedef enum _STATUS { ERROR = 0, OK = 1 } STATUS; // Não uso no código
typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN; // Não uso no código

typedef struct _BTREE_NODE {
	void* pData;
	struct _BTREE_NODE* pLeft;
	struct _BTREE_NODE* pRight;
}BTREE_NODE;

typedef BTREE_NODE* BTREE;

// Protótipos das funções

bool initBTree(BTREE*); // Inicializa a raíz como nula
bool isEmptyBTree(BTREE); // Retorna true se a árvore estiver vazia (nula), false caso contrário
bool isLeaf(BTREE); // Retorna true se a raíz não tiver filhos
bool createNewBTreeNode(BTREE* pNew, void* pData); // Instancia um novo nó sem filhos com a data passada por parâmetro
void printBTreeInorder(BTREE); // Apresenta a árvore no ecrã (Left -> Root -> Right)
void printBTreePreorder(BTREE); // Apresenta a árvore no ecrã (Root -> Left -> Right)
void printBTreePostorder(BTREE); // Apresenta a árvore no ecrã (Left -> Right -> Root)
bool insertBST(BTREE* Root, void* pData); // Insere uma nova folha na árvore passada por parâmetro
BTREE searchBST(BTREE Root, int x); // Se um inteiro pertencer a uma árvore dada a sua raíz, devolve o nó
int countTreeNodes(BTREE Root); // Devolve o nº de nós de uma árvore
int countTreeLeaves(BTREE Root); // Devolve o nº de folhas de uma árvore
int treeHeight(BTREE Root); // Devolve a altura da árvore (Maior nº possível de arestas em um caminho de um nó folha até um nó alvo)
int findDepth(BTREE Root, int x); // Devolve a profundidade de um nó, -1 caso não exista
void Menu(char* OP); // Menu de opções
void Buffer(void); // Limpa o Buffer

// Main Program

int main(void) {
	BTREE Root;
	char OP = ' ';
	int value = 0, aux = 0;
	int* arr;
	initBTree(&Root);

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
			// Criação da árvore
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
		// Contagem de nós
		case '5':
			printf("\n---------------\nContagem de nos\n---------------\n");
			printf("\nNumero de nos da arvore atual: %d", countTreeNodes(Root));
			break;
		// Contagem de folhas
		case '6':
			printf("\n------------------\nContagem de folhas\n------------------\n");
			printf("\nNumero de folhas da arvore atual: %d", countTreeLeaves(Root));
			break;
		// Determinar o nivel de um nó
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

// Funções

bool initBTree(BTREE* pBT) {
	*pBT = NULL;
	return true;
}
bool isEmptyBTree(BTREE Root) {
	// Raíz está vazia?
	return (Root == NULL) ? true : false;
}
bool isLeaf(BTREE Tree) {
	// Raíz tem filho à esquerda/direita?
	return ((LEFT(Tree) == NULL) && RIGHT(Tree) == NULL) ? true : false;
}
bool createNewBTreeNode(BTREE* pNew, void* pData) {
	BTREE pTemp;
	// Alocação Dinâmica
	if ((pTemp = (BTREE)malloc(sizeof(BTREE_NODE))) == NULL)
		return false;
	*pNew = pTemp;
	DATA(pTemp) = pData;
	LEFT(pTemp) = NULL;
	RIGHT(pTemp) = NULL;
	return true;
}
void printBTreeInorder(BTREE Root) {
	// Árvore vazia?
	if (isEmptyBTree(Root) == true) 
		return;
	printBTreeInorder(LEFT(Root));
	printf("%d -> ", *(int*)DATA(Root));
	printBTreeInorder(RIGHT(Root));
	return;
}
void printBTreePreorder(BTREE Root) {
	// Árvore vazia?
	if (isEmptyBTree(Root) == true)
		return;
	printf("%d -> ", *(int*)DATA(Root));
	printBTreePreorder(LEFT(Root));
	printBTreePreorder(RIGHT(Root));
	return;
}
void printBTreePostorder(BTREE Root) {
	// Árvore vazia?
	if (isEmptyBTree(Root) == true)
		return;	
	printBTreePostorder(LEFT(Root));
	printBTreePostorder(RIGHT(Root));
	printf("%d -> ", *(int*)DATA(Root));
	return;
}
bool insertBST(BTREE* Root, void* pData) {
	// Árvore vazia? <=> Nova folha = Raíz
	if (isEmptyBTree(*Root)) {
		BTREE pNew;
		if (createNewBTreeNode(&pNew, pData) == NULL)
			return false;
		*Root = pNew;
		return true;
	}
	// Se não estiver vazia verifica onde deve colocar a nova folha
	// Se for menor coloca à esquerda
	// Se for maior ou igual coloca à direita
	if (*(int*)(pData) < *(int*)DATA(*Root))
		insertBST(&(LEFT(*Root)), pData);
	else
	insertBST(&(RIGHT(*Root)), pData);
	return true;
}
BTREE searchBST(BTREE Root, int x) {
	// Raíz nula ou valor na raíz?
	if (Root == NULL || *(int*)DATA(Root) == x)
		return Root;
	// Procura na sub-árvore da esquerda
	if (x < *(int*)DATA(Root))
		return searchBST(LEFT(Root), x);
	return searchBST(RIGHT(Root), x);
}
int countTreeNodes(BTREE Root) {
	return (Root == NULL) ? 0 : (1 + countTreeNodes(LEFT(Root)) + countTreeNodes(RIGHT(Root)));
}
int countTreeLeaves(BTREE Root) {
	// Árvore vazia?
	if (isEmptyBTree(Root))
		return 0;
	// Verifica se o nó atual é uma folha
	if (LEFT(Root) == NULL && RIGHT(Root) == NULL)
		return 1;
	// Retorna a soma de folhas da sub-árvore esquerda com a sub-árvore direita
	return countTreeLeaves(LEFT(Root)) + countTreeLeaves(RIGHT(Root));
}
int treeHeight(BTREE Root) {
	// Árvore vazia <=> Height = -1
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
	// Árvore vazia?
	if (isEmptyBTree(Root))
		return depth;
	// Raíz = nó?
	if (*(int*)DATA(Root) == x)
		return depth + 1;
	// Verifica se o valor está presente na sub-árvore da esquerda ou na sub-árvore da direita
	if ((depth = findDepth(LEFT(Root), x)) >= 0 || (depth = findDepth(RIGHT(Root), x)) >= 0)
		return depth + 1;
	// Não encontrou o nó
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
