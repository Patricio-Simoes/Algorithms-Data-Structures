#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_NAME 30
#define DEPARTAMENTOS 15

typedef struct DEPT {
	char desig[20];
	double ganhos, despesas;
	int n_pessoas;
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

// Variáveis Globais
int workers = 0;

// Protótipos

BTREE_NODE* NewTreeNode(void* data); // Tenta alocar um novo nó com a data passada, retorna false se falhar
void BtreeFree(BTREE_NODE* root); // Desaloca uma árvore inteira da memória
BTREE_NODE* InitNode(void* data, BTREE_NODE* left, BTREE_NODE* right); // Inicializa um nó com os filhos e data passados
BTREE_NODE* CreateBtree(void** data, int i, int size); // Cria uma árvore com a data carregada no apontador void**
bool ReadFile(char* fileName, void** data); // Lê um ficheiro e carrega os dados para o ponteiro void**
double GetSingleDptProfit(BTREE_NODE* root); // Retorna um float que representa os lucros de um departamento (ganhos-despesas)
double GetCompanyProfits(BTREE_NODE* root); //Retorna um float que representa os lucros de todos os departamentos da empresa (ganhos-despesas)
int GetMostPeopleDepartment(BTREE_NODE* root); // Retorna o número de Pessoas do departamento com maior número de pessoas
void ShowMostPeopleDepartment(BTREE_NODE* root, int count); // Mostra as informações do(s) Departamento(s) com maior nº de pessoas
void ShowDepartmentInfo(BTREE_NODE* root); // Mostra as informações de um departamento
bool IsDepartment(BTREE_NODE* root, const char* name); // Verifica o nó passado é a diretoria passada
bool IsBtreeNode(BTREE_NODE* root, const char* name); // Verifica se um nó pertence a uma árvore
BTREE_NODE* find(BTREE_NODE* root, char* name); // Procura um departamento através da designação e retorna o seu apontador
void ShowDepartment(BTREE_NODE* root); // Mostra no ecrã as informações do departamento passado
void ShowAllDepartments(BTREE_NODE* root); // Mostra todos os departamentos

void Menu(char* OP); // Menu de opções
void Buffer(void); // Limpa o buffer

int main(void)
{
	char fileName[MAX_NAME] = "Departamentos.txt";
	char OP = ' ';
	void* Departamentos[DEPARTAMENTOS];
	int count = 0;
	char desig[MAX_NAME] = " ";
	BTREE_NODE* tree = NULL;
	BTREE_NODE* aux = NULL;

	printf("Tentando abrir o ficheiro %s...\n", fileName);
	Sleep(1000);

	// Tenta abrir o ficheiro

	if (!(ReadFile(fileName, Departamentos))) {
		printf("Impossivel abrir o ficheiro \"%s\"...\n", fileName);
		Sleep(1000);
		return 0;
	}
	printf("\"%s\" lido com sucesso!\n", fileName);
	Sleep(1000);
	if ((tree = CreateBtree(Departamentos, 0, DEPARTAMENTOS)) == NULL) {
		printf("Ocorreu um erro ao tentar carregar a informacao para uma arvore!\n");
		Sleep(1000);
		return 0;
	}
	Sleep(1000);
	system("CLS");
	// Apresentação do Menu
	do {
		Menu(&OP);
		Buffer();
		switch (OP) {
			// Sai do programa
		case '0':
			exit(0);
			break;
			// Mostra lucros da empresa
		case '1':
			printf("\n-------------------------\nMostar Lucros da Empresa\n-------------------------\n");
			printf("Total de Lucros: Aprox. %.2lf Euros", GetCompanyProfits(tree));
			break;
			// Mostrar departamento com maior número de pessoas
		case '2':
			printf("\n---------------------------------------------------\nMostrar Departamento(s) com maior numero de Pessoas\n---------------------------------------------------\n");
			count = GetMostPeopleDepartment(tree);
			ShowMostPeopleDepartment(tree, count);
			break;
			//  Elimina todos os sub-departamentos de um departamento e transfere todos os trabalhadores para este
		case '3':
			printf("\n----------------------------------------------\nEliminar Sub-Departamentos de um Departamento:\n----------------------------------------------\n");
			printf("\nDesignacao do Departamento: ");
			scanf("%s", &desig);
			if (!IsBtreeNode(tree, desig))
				// Verifica se o departamento introduzido se encontra na árvore
				printf("O Departamento \"%s\" nao existe!\n", desig);
			else {
				// Liberta as sub-árvores da esquerda ou da direita deixando a raíz intacta de modo a que esta "acolha" os trabalhadores dos filhos apagados
				/*BtreeFree(LEFT(find(tree, desig)));
				BtreeFree(RIGHT(find(tree, desig)));
				((DEPT*)DATA(find(tree, desig)))->n_pessoas += workers;*/
				BtreeFree(find(tree, desig));
				printf("\nSub-Departamentos eliminados com sucesso!\n");
				printf("\nNovas informacoes do Departamento \"%s\":\n\n", desig);
				ShowDepartment(find(tree, desig));
			}
			break;
			// Mostra Departamento
		case '4':
			printf("\n-------------------------\nInformacoes Departamento:\n-------------------------\n");
			printf("\nDesignacao do Departamento: ");
			scanf("%s", &desig);
			if (!IsBtreeNode(tree, desig))
				// Verifica se o departamento introduzido se encontra na árvore
				printf("O Departamento \"%s\" nao existe!\n", desig);
			else {
				ShowDepartment(find(tree, desig));
				printf("\n------------------------------\nInformacoes Sub-Departamentos:\n------------------------------\n\n");
				ShowAllDepartments(LEFT(find(tree, desig)));
				ShowAllDepartments(RIGHT(find(tree, desig)));
			}
			break;
			// Mostra todos os departamentos
		case '5':
			printf("\n--------------------------\nInformacoes Departamentos:\n--------------------------\n");
			ShowAllDepartments(tree);
			break;
			// Opção inválida
		default:
			printf("Opcao invalida!\n");
			break;
		}

		workers = 0;
		count = 0;
		strcpy(fileName, " ");
		strcpy(desig, " ");
		printf("\n");
		Buffer();
		system("pause");
		system("CLS");
	} while (OP != '0');
	return 0;
}

// Funções

BTREE_NODE* NewTreeNode(void* data) {
	BTREE_NODE* newNode = NULL;
	// Verifica se a data é válida
	if (data == NULL)
		return NULL;
	// Alocação dinâmica
	if (newNode = (BTREE_NODE*)malloc(sizeof(BTREE_NODE))) {
		// Atribuição de dados
		DATA(newNode) = data;
		LEFT(newNode) = NULL;
		RIGHT(newNode) = NULL;
	}
	// Retorno
	return newNode;
}
void BtreeFree(BTREE_NODE* root) {
	// Caso a root seja nula retorna para libertar o nó pai, se a root for nula desde vo início, a árvore está vazia
	if (root == NULL)
		return;
	// Chamada recursiva para libertar primeiro todos os nós que estão abaixo do atual
	BtreeFree(LEFT(root));
	BtreeFree(RIGHT(root));
	if (LEFT(root) != NULL){
		((DEPT*)DATA(root))->n_pessoas += ((DEPT*)DATA(LEFT(root)))->n_pessoas;
		free(DATA(LEFT(root)));
		free (LEFT(root));
		LEFT(root) = NULL;
	}
	if (RIGHT(root) != NULL) {
		((DEPT*)DATA(root))->n_pessoas += ((DEPT*)DATA(RIGHT(root)))->n_pessoas;
		free(DATA(RIGHT(root)));
		free(RIGHT(root));
		RIGHT(root) = NULL;
	}

}
	
BTREE_NODE* InitNode(void* data, BTREE_NODE* left, BTREE_NODE* right) {
	BTREE_NODE* newNode = NULL;
	// Verifica se a data é válida
	if (data == NULL)
		return NULL;
	// Tenta inicializar o nó
	if (!(newNode = NewTreeNode(data)))
		return NULL;
	// Atribui os filhos
	LEFT(newNode) = left;
	RIGHT(newNode) = right;
	// Retorna
	return newNode;
}
BTREE_NODE* CreateBtree(void** data, int i, int size) {
	if (i >= size)
		return NULL;
	return(InitNode(*(data + i), CreateBtree(data, 2 * i + 1, size), CreateBtree(data, 2 * i + 2, size)));
}
bool ReadFile(char* fileName, void** data) {
	FILE* ptr = NULL;
	void* voidPTR = NULL;
	int i = 0;
	// Verifica se os dados são válidos
	if (fileName == NULL || data == NULL)
		return false;
	// Tenta abrir o ficheiro
	if (!(ptr=fopen(fileName,"r")))
		return false;
	// Carrega os dados para o ponteiro
	while (!feof(ptr)){
		// Tenta alocar dinâmicamente um ponteiro void* para carregar as informações no ponteiro void**
		if ((voidPTR = (DEPT*)malloc(sizeof(DEPT))) != NULL) {
			fscanf(ptr, "%[^;];%lf;%lf;%d\n", ((DEPT*)voidPTR)->desig, &(((DEPT*)voidPTR)->ganhos), &(((DEPT*)voidPTR)->despesas), &(((DEPT*)voidPTR)->n_pessoas));
			data[i++] = voidPTR;
		}
		// Caso esta alocação falhe, é necessário desalocar quaisquer dados que tenham sido préviamente alocados
		else {
			for (int j = i; j >= 0; j--)
				free(data[j]);
			return false;
		}
	}
	fclose(ptr);
	return true;
}
double GetSingleDptProfit(BTREE_NODE* root) {
	// Se a raíz for nula, a árvore está vazia, ou seja, não há nenhum departamento
	if (root == NULL)
		return 0;
	return ((((DEPT*)DATA(root))->ganhos) - (((DEPT*)DATA(root))->despesas));
}
double GetCompanyProfits(BTREE_NODE* root) {
	// Se a raíz for nula, a árvore está vazia, ou seja, não há nenhum departamento
	if (root == NULL)
		return 0;
	//return GetSingleDptProfit(LEFT(root)) + GetSingleDptProfit(RIGHT(root));
	return GetSingleDptProfit(root) + GetCompanyProfits(LEFT(root)) + GetCompanyProfits(RIGHT(root));

}
int GetMostPeopleDepartment(BTREE_NODE* root) {
	// Se a raíz for nula não há departamentos a procurar
	int maior = 0;
	int maiorLeft,maiorRight ;
	if (root == NULL)
		return maior;
	// Compara o maior número de pessoas atual com o número de pessoas do nó atual, se menor atualiza-o
	if ((((DEPT*)DATA(root))->n_pessoas) > maior)
		maior = (((DEPT*)DATA(root))->n_pessoas);
	// Chamada recursiva para as sub-árvores da esquerda e direita
	maiorLeft = GetMostPeopleDepartment(LEFT(root));
	maiorRight = GetMostPeopleDepartment(RIGHT(root));
	
	if (maiorLeft > maior)
		maior = maiorLeft;
	if (maiorRight > maior)
		maior = maiorRight;
	return maior;
}
void ShowMostPeopleDepartment(BTREE_NODE* root, int count) {
	// Se a raíz é nula, não há departamentos
	if (root == NULL)
		return;
	// Compara o número de pessoasa do departamento do nó atal com o maior número de pessoas, se for igual mostra as informações do departamento
	if ((((DEPT*)DATA(root))->n_pessoas) == count)
		ShowDepartmentInfo(root);
	// Chamadas recursivas (Pode haver 2 departamentos com o maior número de pessoas)
	ShowMostPeopleDepartment(LEFT(root), count);
	ShowMostPeopleDepartment(RIGHT(root), count);
	return;
}
void ShowDepartmentInfo(BTREE_NODE* root) {
	printf("\nNome: %s\nGanhos: %.2lf Euros\nDespesas: %.2lf Euros\nNumero de Pessoas: %d\n", ((DEPT*)DATA(root))->desig, ((DEPT*)DATA(root))->ganhos, ((DEPT*)DATA(root))->despesas, ((DEPT*)DATA(root))->n_pessoas);
}
bool IsDepartment(BTREE_NODE* root, const char* name) {
	// Verifica se os dados são válidos
	if (root == NULL || name == NULL)
		return false;
	// Verifica se o nó atual é o nó procurado
	if (!strcmp((((DEPT*)DATA(root))->desig), name))
		return true;
	return false;
}
bool IsBtreeNode(BTREE_NODE* root, const char* name) {
	// Verifica se os dados são válidos
	if (root == NULL || name == NULL)
		return false;
	// Verifica se o nó atual é o procurado
	if (IsDepartment(root, name))
		return true;

	// Chamada recursiva até encontrar o nó procurado, (se não encontrar retorna false)
	return IsBtreeNode(LEFT(root), name) || IsBtreeNode(RIGHT(root), name);
}
BTREE_NODE* find(BTREE_NODE* root, char* name)
{
	// Se a raíz for nula, não há departamentos
	if (root == NULL)
		return NULL;
	// Caso o departamento seja o nó atual
	if (!strcmp(((DEPT*)DATA(root))->desig, name))
		return root;
	// Chamada recursiva para a sub-árvore esquerda
	BTREE_NODE* left = find(root->left, name);
	if (left)
		return left;
	// Chamada recursiva para a sub-árvore direita
	BTREE_NODE* right = find(root->right, name);
	if (right)
		return right;
	// Não encontrou
	return NULL;
}
void ShowDepartment(BTREE_NODE* root) {
	if (root == NULL) {
		printf("Departamento nao existente!\n");
		return;
	}
	printf("Designacao: %s\nGanhos: %.2lf\nDespesas: %.2lf\nTrabalhadores: %d\n\n", ((DEPT*)DATA(root))->desig, ((DEPT*)DATA(root))->ganhos, ((DEPT*)DATA(root))->despesas, ((DEPT*)DATA(root))->n_pessoas);
}
void ShowAllDepartments(BTREE_NODE* root){
	if (root == NULL)
	return;

	ShowAllDepartments(LEFT(root));
	ShowAllDepartments(RIGHT(root));
	ShowDepartment(root);
}
void Menu(char* OP) {
	printf("--------------------\nSelecione uma opcao:\n--------------------\n");
	printf("[1] Mostrar lucros da Empresa\n");
	printf("[2] Mostrar dados do departamento com maior numero de pessoas\n");
	printf("[3] Eliminar todos os sub-departamentos de um departamento e transferir trabalhadores para este\n");
	printf("[4] Mostrar dados de um Departamento\n");
	printf("[5] Mostrar dados de todos os Departamentos\n");
	printf("Opcao: ");
	scanf("%c", OP);
}
void Buffer(void) {
	while (getchar() != '\n');
	fflush(stdin);
}
