#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>

#define MAX_NAME 50
#define PESSOAS 15

typedef struct PERSON {
	char name[50];
	int age;
	bool alive;
};
typedef struct BTREE_NODE {
	void* data;
	struct BTREE_NODE* left;
	struct BTREE_NODE* right;
}
;

// Macros

#define DATA(node) ((node)->data)
#define LEFT(node) ((node)->left)
#define RIGHT(node) ((node)->right)

// Protótipos

BTREE_NODE* NewBTreeNode(void* data); // Tenta alocar dinamicamente um nó com a data passada e retorna-o
BTREE_NODE* InitBTreeNode(void* data, BTREE_NODE* left, BTREE_NODE* right); // Inicializa um nó com a data e filhos
bool ReadFile(void** data, char* name); // Lê um ficheiro com pessoas e carrega-as para o apontador void**
void BTreeFree(BTREE_NODE* root); // Desaloca uma árvore na sua totalidade
BTREE_NODE* CreateBTree(void** data, int i, int size); // Cria uma árvore com a data carregada no apontador void**
void CountPeople(BTREE_NODE* root, int* alive, int* dead); // Conta o número de pessoas vivas e mortas de uma árvore
void ShowPersonInfo(BTREE_NODE* root); // Mostra no ecrã as informações de uma Pessoa
void ShowAllPersonInfo(BTREE_NODE* root); // Mostra todas as Pessoas
BTREE_NODE* find(BTREE_NODE* root, char* fullName); // Procura uma Pessoa através do nome completo e retorna o seu apontador
bool IsLeaf(BTREE_NODE* root); // Retorna true se for uma folha, false caso contrário
BTREE_NODE* ReadPessoa(); // Lê os dados de uma Pessoa através do teclado
bool addPessoas(BTREE_NODE* root, BTREE_NODE* left, BTREE_NODE* right); // Adiciona os filhos a um pai
void Menu(char* OP); // Menu de opções
void Buffer(void); // Limpa o buffer

int main(void)
{
	BTREE_NODE* Tree = NULL;
	char fileName[MAX_NAME] = "Pessoas.txt", OP = ' ';
	void* Pessoas[PESSOAS];
	int alive = 0, dead = 0;
	char firstName[MAX_NAME], lastName[MAX_NAME], fullName[MAX_NAME];

	printf("Tentando abrir o ficheiro %s...\n", fileName);
	Sleep(1000);

	// Tenta abrir o ficheiro

	if ((ReadFile(Pessoas, fileName)) == NULL) {
		printf("Impossivel abrir o ficheiro \"%s\"...\n", fileName);
		Sleep(1000);
		return 0;
	}
	printf("\"%s\" lido com sucesso!\n", fileName);
	Sleep(1000);
	if ((Tree = CreateBTree(Pessoas, 0, PESSOAS)) == NULL) {
		printf("Ocorreu um erro ao tentar carregar a informacao para uma arvore!\n");
		Sleep(1000);
		return 0;
	}
	BTREE_NODE* aux = Tree;
	Sleep(1000);
	system("CLS");
	// Apresentação do Menu
	do {
		Menu(&OP);
		Buffer();
		switch (OP) {
			// Sair
		case '0': exit(0); break;
			// Contagem de pessoas
		case '1':
			printf("\n-------------------\nContagem de Pessoas\n-------------------\n");
			CountPeople(Tree, &alive, &dead);
			printf("Numero de Pessoas vivas: %d\nNumero de Pessoas mortas: %d\n", alive, dead);
			break;
			// Informações de todas as Pessoas
		case '2':
			printf("\n---------------\nMostrar Pessoas\n---------------\n");
			ShowAllPersonInfo(Tree);
			break;
			// Alterar o nome de uma Pessoa
		case '3':
			printf("\n------------\nAlterar nome\n------------\n");
			printf("Primeiro Nome: ");
			gets_s(firstName, MAX_NAME);
			Buffer();
			printf("Ultimo Nome: ");
			gets_s(lastName, MAX_NAME);
			// Junta as strings
			strcat(strcat(firstName, { " " }), lastName);
			Buffer();
			// Caso a Pessoa não exista
			if (!find(Tree, firstName))
				printf("A Pessoa inserida nao esta nos registos!\n");
			else {
				printf("Novo nome: ");
				gets_s(fullName, MAX_NAME);
				// Altera o nome
				strcpy((((PERSON*)DATA(find(Tree, firstName)))->name), fullName);
				printf("Nome alterado com sucessso!\n");
			}
			break;
			// Adiciona à folha mais à direita
		case '4':
			printf("\n-----------------\nAdicionar Pessoas\n-----------------\n");
			// Vai até a folha mais à direita
			while (RIGHT(aux) != NULL)
				aux = RIGHT(aux);
			addPessoas(aux, ReadPessoa(), ReadPessoa());
			break;
			// Opção inválida
		default:
			printf("Opcao invalida! Tente novamente");
			break;
		}
	alive = 0;
	dead = 0;
	printf("\n");
	Buffer();
	system("pause");
	system("CLS");
	} while (OP != '0');

	return 0;
}

BTREE_NODE* NewBTreeNode(void* data) {
	BTREE_NODE* newNode = NULL;
	// Verifica se a data é válida
	if (data == NULL)
		return NULL;
	// Tenta alocar dinâmicamente
	if (!(newNode = (BTREE_NODE*)malloc(sizeof(BTREE_NODE))))
		return NULL;
	// Atribui os dados
	DATA(newNode) = data;
	LEFT(newNode) = NULL;
	RIGHT(newNode) = NULL;
	// Retorna o nó criado
	return newNode;
}
BTREE_NODE* InitBTreeNode(void* data, BTREE_NODE* left, BTREE_NODE* right) {
	BTREE_NODE* newNode = NULL;
	// Tenta criar o nó
	if ((newNode = NewBTreeNode(data)) == NULL)
		return NULL;
	// Atribui os filhos
	LEFT(newNode) = left;
	RIGHT(newNode) = right;
	// Retorna o nó criado
	return newNode;
}
bool ReadFile(void** data, char* name) {
	FILE* ptr = NULL;
	void* aux = NULL;
	int alive = 0, j = 0, i = 0;
	// Verifica se oS dados são válidos
	if (data == NULL || name == NULL)
		return false;
	// Tenta abrir o ficheiro
	if ((ptr = fopen(name, "r")) == NULL)
		return false;
	// Percorre linha a linha
	while (!feof(ptr)) {
		// Com cada iteração tenta alocar dinâmicamente o apontador void* para guardar uma pessoa e adicioná-la ao apontador void**
		if ((aux = (PERSON*)malloc(sizeof(PERSON))) != NULL) {
			fscanf(ptr, "%[^;];%d;%d\n", (((PERSON*)aux)->name), &(((PERSON*)aux)->age), &alive);
			if (alive == 1)
				((PERSON*)aux)->alive = true;
			else
				((PERSON*)aux)->alive = false;
			data[i++] = aux;
		}
		// Se a alocação falhar é necessário desalocar quaisquer dados que tenham sido previamente alocados
		else {
			for (j = i; j >= 0; j--)
				free(data[j]);
		}
	}
	fclose(ptr);
	return true;
}
void BTreeFree(BTREE_NODE* root) {
	// Se a raíz é nula, a árvore já está vazia
	if (root == NULL)
		return;
	// Chamadas recursivas para começar o processo de desalocação no nível mais baixo da árvore
	BTreeFree(LEFT(root));
	BTreeFree(RIGHT(root));
	// Desaloca a data e o nó atual da árvore
	free(DATA(root));
	free(root);
}
BTREE_NODE* CreateBTree(void** data, int i, int size) {
	if (i >= size)
		return NULL;
	return(InitBTreeNode(*(data + i), CreateBTree(data, 2 * i + 1, size), CreateBTree(data, 2 * i + 2, size)));
}
void CountPeople(BTREE_NODE* root, int* alive, int* dead) {
	// Raíz nula, não há dados
	if (root == NULL)
		return;
	// Chamada recursiva para percorrer a árvore inteira
	CountPeople(LEFT(root), alive, dead);
	CountPeople(RIGHT(root), alive, dead);
	// Verifica o estado da pessoa no nó atual
	if ((((PERSON*)DATA(root))->alive) == true)
		(*alive)++;
	else
		(*dead)++;
}
void ShowPersonInfo(BTREE_NODE* root) {
	printf("\nNome: %s\nIdade: %d\nEstado: ", ((PERSON*)DATA(root))->name, ((PERSON*)DATA(root))->age);
	if (((PERSON*)DATA(root))->alive)
		printf("Viva\n");
	else
		printf("Morta\n");
}
void ShowAllPersonInfo(BTREE_NODE* root) {
	if (root == NULL)
		return;
	ShowAllPersonInfo(LEFT(root));
	ShowPersonInfo(root);
	ShowAllPersonInfo(RIGHT(root));
}
BTREE_NODE* find(BTREE_NODE* root, char* fullName)
{
	// Se a raíz for nula, não há departamentos
	if (root == NULL)
		return NULL;
	// Caso o departamento seja o nó atual
	if (!strcmp(((PERSON*)DATA(root))->name, fullName))
		return root;
	// Chamada recursiva para a sub-árvore esquerda
	BTREE_NODE* left = find(root->left, fullName);
	if (left)
		return left;
	// Chamada recursiva para a sub-árvore direita
	BTREE_NODE* right = find(root->right, fullName);
	if (right)
		return right;
	// Não encontrou
	return NULL;
}
bool IsLeaf(BTREE_NODE* root) {
	if (root == NULL)
		return false;
	if (LEFT(root) == NULL && RIGHT(root) == NULL)
		return true;
}
BTREE_NODE* ReadPessoa() {
	char nome[MAX_NAME], OP = ' ';
	int idade = 0, vivo = 0;
	void* aux;
	// Lê os dados
	printf("\n#Nova Pessoa\n\n");
	printf("Nome: ");
	gets_s(nome, MAX_NAME);
	Buffer();
	printf("Idade: ");
	scanf("%d", &idade);
	Buffer();
	printf("\n");
	do {
		printf("Vivo?\n[S][N]: ");
		scanf("%c", &OP);
		Buffer();
	} while (OP != 'S' && OP != 'N' && OP != 's' & OP != 'n');
	// Tenta alocar o apontador para a data
	if ((aux = (PERSON*)malloc(sizeof(PERSON))) != NULL) {
		// Atribui os dados
		strcpy((((PERSON*)aux)->name), nome);
		(((PERSON*)aux)->age) = idade;
		if (OP == 'S' || OP == 's')
			(((PERSON*)aux)->alive) = true;
		else
			(((PERSON*)aux)->alive) = false;
	}
	// Retorna um nó criado com a data lida
	return NewBTreeNode(aux);
}
bool addPessoas(BTREE_NODE* root, BTREE_NODE* left, BTREE_NODE* right) {
	// Se a raíz for nula, a árvore está vazia e é impossível adicionar nós filhos
	if (root == NULL)
		return false;
	// Adicionamos caso o nó à direita do atual seja nulo, ou seja, se estivermos na folha mais à direita
	if (IsLeaf(root)) {
		RIGHT(root) = right;
		LEFT(root) = left;
		return true;
	}
	else 
		// Chamada recursiva até chegar à folha mais à direita
		addPessoas(RIGHT(root), left, right);
}
void Menu(char* OP) {
	printf("--------------------\nSelecione uma opcao:\n--------------------\n");
	printf("[1] Contar o numero de pessoas\n");
	printf("[2] Mostrar Pessoas\n");
	printf("[3] Alterar nome de uma Pessoa\n");
	printf("[4] Adicionar Pessoa a folha mais a direita da arvore\n");
	printf("Opcao: ");
	scanf("%c", OP);
}
void Buffer(void) {
	while (getchar() != '\n');
	fflush(stdin);
}