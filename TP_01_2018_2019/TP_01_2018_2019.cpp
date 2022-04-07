#define _CRT_SECURE_NO_WARNINGS
#define STR_MAX 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _NODE {
	void* data;
	_NODE* next;
}NODE;

typedef struct _FUNC {
	char nome[STR_MAX];
	char dpt[STR_MAX];
	float ordenado;
	bool quadro;
}FUNC;

// Macros

typedef NODE* LIST;
#define NEXT(node) ((node)->next)
#define DATA(node) ((node)->data)

// Protótipos das funções da lista

LIST newNode(void* data); // Retorna um novo pointer para um NODEionário
bool addToHead(LIST* head, void* data); // Adiciona à cabeça da lista
bool addToTail(LIST* head, void* data); // Adiciona à cauda da lista
void printList(LIST head); // Mostra as informações dos Funcionários da lista
void printBoardList(LIST head); // Mostra as informações dos Funcionários DO QUADRO da lista
FUNC* readFunc(void); // Lê um Funcionário a partir do teclado
void Buffer();
char Menu();
bool readFile(LIST* head, const char* fileName); // Carrega Funcionários para uma lista através de um ficheiro
void deleteList(LIST* head); // Apaga completamente uma lista
float calculaOrdenados(LIST head); // Retorna o total acumulado em ordenados de Funcionários de uma lista
bool isValidNode(LIST head, const char* name); // Verifica se um certo Funcionário pertence a uma certa lista
bool makeActive(LIST head, const char* name); // Torna um Funcionário ativo no quadro
bool deleteNode(LIST* head, const char* value); // Apaga um nodo da lista caso a descrição seja igual ao value passado por parâmetro
bool deleteNonBoardMembers(LIST* head); // Apaga todos os funcionários não pertencentes ao quadro
bool saveFile(LIST head, const char* fileName); // Guarda os funcionários num ficheiro

// Main program

int main(void) {
	LIST Funcionarios = NULL;
	char OP = ' ';
	char stringAUX[STR_MAX];
	do {
		OP = Menu();
		switch (OP) {
			// Sair do programa
		case '0': exit(0); break;
			// Ler dados do ficheiro
		case '1':
			deleteList(&Funcionarios);
			printf("Insira o nome do ficheiro: ");
			gets_s(stringAUX, STR_MAX);
			Buffer();
			if (readFile(&Funcionarios, stringAUX))
				printf("Funcionarios lidos com sucesso!\n");
			else
				printf("Ocorreu um erro na leitura do Ficheiro!\n");
			break;
			// Adicionar Funcionário à Lista
		case '2': 
			if (addToTail(&Funcionarios, readFunc()))
				printf("Funcionario adicionado com sucesso!\n");
			else
				printf("Ocorreu um erro ao adicionar o Funcionario!\n");
			break;
			// Listar Funcionários
		case '3' :
			printList(Funcionarios);
			break;
			// Listar Funcioários do quadro
		case '4': 
			printBoardList(Funcionarios);
			break;
			// Calcular ordenados
		case '5':
			printf("Total gasto em ordenados este mes: %f\n", calculaOrdenados(Funcionarios));
			break;
			// Passar Funcionário para o quadro
		case '6':
			printf("Insira o nome do Funcionario: ");
			gets_s(stringAUX, STR_MAX);
			Buffer();
			if (makeActive(Funcionarios, stringAUX))
				printf("Funcionario inserido no Quadro com sucesso!\n");
			else
				printf("Ocorreu um erro, operacao nao concluida!\n");
			break;
			// Eliminar Funcionários que não são do quadro
		case '7':
			if (deleteNonBoardMembers(&Funcionarios))
				printf("Funcionarios eliminados com sucesso!\n");
			else
				printf("Ocorreu um erro ao tentar eliminar os Funcionarios!\n");
			break;
			// Apagar 1 Funcionário específico
		case '8':
			printf("Insira o Nome do Funcionario a apagar: ");
			gets_s(stringAUX, STR_MAX);
			Buffer();
			if (isValidNode(Funcionarios, stringAUX)) {
				if (deleteNode(&Funcionarios, stringAUX))
					printf("Funcionario apagado com sucesso!\n");
			}
			else
				printf("Ocorreu um erro ao apagar o Funcionario!\n");
			break;
			// Guardar num ficheiro
		case '9':
			printf("Qual o nome do ficheiro? ");
			gets_s(stringAUX, STR_MAX);
			Buffer();
			if (saveFile(Funcionarios, stringAUX))
				printf("Dados guardados com sucesso!\n");
			else
				printf("Ocorreu um erro ao tentar guardar os dados dos Funcionarios!\n");
			break;
		default:
			printf("Opcao invalida!\n");
			break;
		}
		system("pause");
	} while (OP != '0');

	return 0;
}

// Funções da lista

bool addToHead(LIST* head,void* data) {
	LIST newHead = NULL;
	// Alocação dinâmica
	if (!(newHead = (NODE*)malloc(sizeof(NODE))))
		return false;
	// Não foi possível criar um novo nodo com a data passada
	if (!(newHead=newNode(data)))
		return false;
	// Lista vazia?
	if (head == NULL)
		*head = newHead;
	// Lista não está vazia
	else {
		NEXT(newHead) = *head;
		*head = newHead;
	}
	return true;
}
bool addToTail(LIST* head, void* data) {
	LIST newTail = NULL;
	LIST temp = *head;
	// Alocação dinâmica
	if (!(newTail = (NODE*)malloc(sizeof(NODE))))
		return false;
	//Criação do nodo
	if (!(newTail = newNode(data)))
		return false;
	// Lista vazia?
	if (temp == NULL) {
		*head = newTail;
		return true;
	}
	// Lista não está vazia
	while (NEXT(temp) != NULL)
		temp = NEXT(temp);
	// Na cauda atual
	NEXT(temp) = newTail;
	return true;
}
LIST newNode(void* data) {
	LIST newNode = NULL;
	// data nula?
	if (data == NULL)
		return NULL;
	// Caso contrário, alocação dinâmica
	if ((newNode = (NODE*)malloc(sizeof(NODE))) == NULL)
		return NULL;
	// Passou nos failsafes
	DATA(newNode) = data;
	NEXT(newNode) = NULL;
	return newNode;
}
void printList(LIST head) {
	int i = 1;
	// Lista vazia?
	if (head == NULL) {
		printf("Lista vazia!\n");
		return;
	}
	// Percorre até chegar ao fim
	while (head != NULL) {
		printf("-----------\nFuncionario #%d\n-----------\n\n", i);
		printf("#Nome: %s\n#Departamento: %s\nSalario: %f\n", ((FUNC*)DATA(head))->nome, ((FUNC*)DATA(head))->dpt, ((FUNC*)DATA(head))->ordenado);
		// Pertence ao quadro?
		if ((((FUNC*)DATA(head))->quadro))
			printf("Quadro: Sim\n");
		else
			printf("Quadro: Nao\n");
		i++;
		head = NEXT(head);
	}
}
FUNC* readFunc(void) {
	printf("--------------------\nInserir novo Funcionario\n--------------------\n\n");
	int aux = NULL;
	FUNC* newFunc;
	// Alocação dinâmica
	if ((newFunc = (FUNC*)malloc(sizeof(FUNC))) == NULL)
		return NULL;
	// Leitura de dados
	printf("Insira o nome do Funcionario: ");
	gets_s(newFunc->nome, STR_MAX);
	Buffer();
	printf("Insira o Departamento do Funcionario: ");
	gets_s(newFunc->dpt, STR_MAX);
	Buffer();
	printf("Insira o salario do Funcionario: ");
	scanf("%f", &newFunc->ordenado);
	Buffer();
	printf("\nDo Quadro ?\n\n[0]Nao\n[1]Sim\n\nOpcao: ");
	scanf("%d", &aux);
	Buffer();
	if(aux)
		newFunc->quadro = true;
	else
		newFunc->quadro = false;
	printf("\n");
	return newFunc;
}
void Buffer(void) {
	while ((getchar() != '\n'));
	fflush(stdin);
}
bool readFile(LIST* head, const char* fileName) {
	FILE* ptr;
	FUNC* newNode = NULL;
	int aux = 0;
	// Tenta abrir o ficheiro
	if ((ptr = fopen(fileName, "r")) == NULL)
		return false;
	// Percorre o ficheiro
	while (!feof(ptr)) {
		// Tenta alocar dinamicamente
		if ((newNode = (FUNC*)malloc(sizeof(FUNC))) != NULL) {
			fscanf(ptr, "%[^;];%[^;];%f;%d;\n", newNode->nome, newNode->dpt, &(newNode->ordenado), &aux);
			if (aux)
				newNode->quadro = true;
			else
				newNode->quadro = false;
		// Failsafe contra erros a meio do processo
		if (!addToTail(head, newNode)) {
			deleteList(head);
			fclose(ptr);
			return false;
			}
		}
	}
	fclose(ptr);
	return true;
}
void deleteList(LIST* head) {
	LIST temp = *head;
	LIST prev = *head;
	// Verifica se a cabeça da lista é nula <=> Lista está vazia
	if (head == NULL) {
		return;
	}
	// Caso não esteja vazia, temos de libertar todos os nodos previamente alocados da memória.
	while (temp != NULL) {
		prev = temp;
		temp = NEXT(temp);
		free(prev);
	}
	*head = NULL;
	return;
}
void printBoardList(LIST head) {
	int i = 1;
	// Lista vazia?
	if (head == NULL) {
		printf("Lista vazia!\n");
		return;
	}
	// Percorre até chegar ao fim
	while (head != NULL) {
		// Pertence ao quadro?
		if ((((FUNC*)DATA(head))->quadro)) {
			printf("-----------\nFuncionario #%d\n-----------\n\n", i);
			printf("#Nome: %s\n#Departamento: %s\nSalario: %f\nQuadro: Sim\n", ((FUNC*)DATA(head))->nome, ((FUNC*)DATA(head))->dpt, ((FUNC*)DATA(head))->ordenado);
			i++;
		}
		head = NEXT(head);
	}
}
float calculaOrdenados(LIST head) {
	float total = 0;
	// Lista nula?
	if (head == NULL)
		return 0;
	// Percorre a lista do início ao fim
	while (head != NULL) {
		total += ((FUNC*)DATA(head))->ordenado;
		head = NEXT(head);
	}
	return total;
}
bool isValidNode(LIST head, const char* name) {
	// Lista nula?
	if (head == NULL)
		return false;
	else {
		// Percorre a lista até encontrar o nodo
		while (head != NULL) {
			// 0 se forem iguais
			if (strcmp(((FUNC*)DATA(head))->nome, name) == 0)
				return true;
			head = NEXT(head);
		}
		return false;
	}
}
bool makeActive(LIST head, const char* name) {
	// Lista nula?
	if (head == NULL)
		return false;
	// Funcionário pertence à lista?
	if (!isValidNode(head, name))
		return false;
	// Percorre a lista até encontrar o Funcionario
	while (head != NULL) {
		if (strcmp(((FUNC*)DATA(head))->nome, name) == 0) {
			((FUNC*)DATA(head))->quadro = true;
			return true;
		}
		head = NEXT(head);
	}
}
bool deleteNode(LIST* head, const char* value) {
	// Nodo que irá guardar o nodo anterior ao que foi apagado para não perder a ligação
	NODE* prev = *head;
	NODE* temp = *head;
	// A lista está vazia?
	if (head == NULL) {
		return true;
	}
	// Verificar se o nodo a ser apagado é a cabeça atual, (strcmp retorna 0 caso as strings forem iguais)
	if (!strcmp(((FUNC*)DATA(temp))->nome, value)) {
		// Neste caso, basta colocar a cabeça atual a pontar para o nodo que vem a seguir, perdendo assim o nodo para o qual a cabeça apontava
		*head = NEXT(temp);
		free(temp);
		return true;
	}
	// No caso do nodo a apagar não ser a cabeça, percorrer a lista e verificar se o nodo se encontra nela
	else {
		while (temp != NULL && strcmp(((FUNC*)DATA(temp))->nome, value)) {
			// Assim que se encontrar o nodo com a designação certa, paramos o ciclo pois esse nodo será apagado
			prev = temp;
			temp = NEXT(temp);
		}
		// Verifica se encontrou o valor procurado
		if (temp != NULL)
		{
			NEXT(prev) = NEXT(temp);
			free(temp);
			return true;
		}
		else {
			return false;
		}
	}
}
bool deleteNonBoardMembers(LIST* head) {
	int i = 1;

	LIST temp = *head, prev = NULL;
	// Cabeça nula?
	if (head == NULL)
		return false;
	// Alocação dinâmica
	if (!(prev = (NODE*)malloc(sizeof(NODE))))
		return false;

	// Apagar a cabeça ou múltiplas cabeças seguidas?
	while (temp != NULL && (((FUNC*)DATA(temp))->quadro) == false)
	{
		printf("%d Funcionario(s) apagado(s) com sucesso!\n",i);
		i++;
		*head = NEXT(temp);
		free(temp);
		temp = *head;
	}

	// Apagou a(s) cabeça(s), encontrou um Funcionário do quadro e agora está a apagar no meio da lista
	while (temp != NULL)
	{
		while (temp != NULL && (((FUNC*)DATA(temp))->quadro) != false)
		{
			prev = temp;
			temp = temp->next;
		}

		// Acabou a procura e não encontrou mais nada a apagar
		if (temp == NULL)
			return true;
		// Encontrou um Funcionário e está pronto a apagá-lo
		NEXT(prev) = NEXT(temp);
		printf("%d Funcionario(s) apagado(s) com sucesso!\n",i);
		i++;

		free(temp);

		// Após libertar o temp node, atribui-lhe um novo valor
		temp = NEXT(prev);
	}
	return true;
}
bool saveFile(LIST head, const char* fileName) {
	FILE* fileptr;
	// Tentativa de abrir o ficheiro
	if (!(fileptr = fopen(fileName, "w")))
		return false;
	// Percorre a lista até chegar ao fim
	while (head != NULL) {
		fprintf(fileptr, "%s;%s;%f;", (((FUNC*)DATA(head))->nome), (((FUNC*)DATA(head))->dpt), (((FUNC*)DATA(head))->ordenado));
		if ((((FUNC*)DATA(head))->quadro))
			fprintf(fileptr, "1;");
		else
			fprintf(fileptr, "0;");
		// Auxilia a escrita do ficheiro para não deixar uma linha vazia no fim
		if (NEXT(head) != NULL)
			fprintf(fileptr, "\n");
		head = NEXT(head);
	}
	Buffer();
	fclose(fileptr);
	return true;
}
char Menu() {
	char OP = ' ';
	system("cls");
	printf("--------------------\nSelecione uma opcao:\n--------------------\n");
	printf("[1]Carregar Lista de Funcionarios\n");
	printf("[2]Adicionar Funcionario\n");
	printf("[3]Listar Funcionarios\n");
	printf("[4]Listar Funcionarios do Quadro\n");
	printf("[5]Calcular o Gasto Total em Ordenados\n");
	printf("[6]Passar Funcionario para o Quadro\n");
	printf("[7]Eliminar todos os Funcionarios nao pertencentes ao Quadro\n");
	printf("[8]Apagar um Funcionario da Lista\n");
	printf("[9]Guardar dados num ficheiro\n");
	printf("[0]Sair\nOpcao: ");
	scanf("%c", &OP);
	Buffer();
	return OP;
}