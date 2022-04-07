#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA(node) ((node)->data)
#define NEXT(node) ((node)->next)

#define STR_MAX 20

typedef struct LIST_NODE {
	void* data;
	LIST_NODE* next;
};
typedef struct _PLAYER {
	char nome[STR_MAX];
	char clube[STR_MAX];
	int posicao;
	bool titular;
}PLAYER;

typedef LIST_NODE* LIST;

LIST newNode(void* value); // Cria um novo nodo
LIST addToHead(LIST* head, void* value); // Insere um nodo na cabeça da lista
LIST addToTail(LIST* head, void* value); // Insere um nodo na cauda da lista
LIST addToMiddle(LIST* head, const char* name, void* value); // Insere um nodo do meio de uma lista
void showEquipaTitular(LIST head); // Mostra os dados de todos os jogadores TITULARES de uma lista
void showEquipaCompleta(LIST head); // Mostra os dados de todos os jogadores de uma lista
bool readFile(LIST* head, const char* fileName); // Lê os dados de jogadores de um ficheiro para uma lista
PLAYER* readPlayer(void); // Lê um jogador do teclado, por defeito, um player não começará titular
void Buffer(void);
bool isValidNode(LIST head, const char* name); // Verifica se uma jogador pertence a uma lista
void deleteList(LIST* head); // Apagada completamente uma lista
int countTitulares(LIST head); // Retorna o número de titulares de uma equipa
bool makeTitular(LIST head, const char* name, int op); // Se possível, (Titulares<11), torna um jogador titular
bool saveFile(LIST head, const char* fileName); // Guarda uma lista num ficheiro
char Menu();


int main(void) {
	LIST Equipa = NULL;
	PLAYER* player = NULL;
	char menuOPT = ' ';
	char stringAUX[STR_MAX];
	int intAUX = 0;

	do {
		switch (menuOPT = Menu()) {
			// Sair do program
		case '0': exit(0); break;
			// Ler dados de um ficheiro
		case '1':
			printf("Nome do ficheiro a ler: ");
			gets_s(stringAUX, STR_MAX);
			if (readFile(&Equipa, stringAUX))
				printf("Dados lidos com sucesso!\n");
			else
				printf("Impossivel abrir o ficheiro!\n");
			break;
		// Mostrar jogadores titulares
		case '2':
			showEquipaTitular(Equipa);
			break;
		// Mostrar equipa completa
		case '3':
			showEquipaCompleta(Equipa);
			break;
		case '4':
			if (addToTail(&Equipa, readPlayer()))
				printf("Jogador adicionado com sucesso!\n");
			else
				printf("Ocorreu um erro ao tentar adicionar o Jogador a Equipa!\n");
			break;
		// Tornar jogador Titular
		case '5':
			printf("Nome do Jogador: ");
			gets_s(stringAUX, STR_MAX);
			Buffer();
			printf("Tornar Titular?\n\n[0]Nao\n[1]Sim\n\nOpcao: ");
			scanf("%d", &intAUX);
			Buffer();
			if (makeTitular(Equipa, stringAUX, intAUX))
				printf("Troca realizada com sucesso!\n");
			else
				printf("Ocorreu um erro!\n\nNumero atual de Titulares: %d\n", countTitulares(Equipa));
			break;
			// Guardar num ficheiro
		case '6':
			printf("Nome do Ficheiro: ");
			gets_s(stringAUX, STR_MAX);
			if (saveFile(Equipa, stringAUX))
				printf("Dados guardados com sucesso!\n");
			else
				printf("Nao foi possivel guardar os dados!\n");
		break;
		default:
			printf("Opcao invalida!\n");
			break;
		}
		system("pause");
	}while (menuOPT != '0');

	//if (!readFile(&lista, "selecao.txt"));
	
	return 0;
}
LIST_NODE* newNode(void* value) {
	LIST node = NULL;
	// Alocação dinâmica
	if ((node = ((LIST)malloc(sizeof(LIST_NODE)))) == NULL)
		return NULL;
	// Foi possível alocar
	else {
		DATA(node) = value;
		NEXT(node) = NULL;
		return node;
	}
}
LIST addToHead(LIST* head, void* value) {
	LIST newHead = NULL;
	// Se não foi possível criar um novo nodo
	if (!(newHead = newNode(value)))
		return NULL;
	// Lista vazia?
	if (head == NULL) {
		*head = newHead;
		return newHead;
	}
	// Lista não está vazia
	else {
		NEXT(newHead) = *head;
		*head = newHead;
		return newHead;
	}
}
LIST addToTail(LIST* head, void* value) {
	LIST newTail = NULL;
	LIST temp = *head;
	// Se não foi possível criar um novo nodo
	if (!(newTail = newNode(value)))
		return NULL;
	else {
		DATA(newTail) = value;
		NEXT(newTail) = NULL;
		// Lista vazia?
		if (temp == NULL) {
			*head = newTail;
			return newTail;
		}
		// Lista não está vazia
		else {
			// Percorre até chegar ao último nodo
			while (NEXT(temp) != NULL)
				temp = NEXT(temp);
			// Estamos no último nodo
			NEXT(temp) = newTail;
			return newTail;
		}
		return NULL;
	}
}
void showEquipaTitular(LIST head) {
	int i = 1;
	// Lista vazia?
	if (head == NULL) {
		printf("Lista vazia!\n");
		return;
	}
	// Percorre a lista até chegar ao fim
	while (head != NULL) {
		//Filtra por titulares
		if ((((PLAYER*)DATA(head))->titular)) {
			printf("-----------\nJogador #%d\n---------\n#Nome: %s\n#Equipa: %s\n#Posicao: %d\n#Titular: Sim\n", i, (((PLAYER*)DATA(head))->nome), (((PLAYER*)DATA(head))->clube), (((PLAYER*)DATA(head))->posicao));
			i++;
		}
		head = NEXT(head);
	}
}
void showEquipaCompleta(LIST head) {
	int i = 1;
	// Lista vazia?
	if (head == NULL) {
		printf("Lista vazia!\n");
		return;
	}
	// Percorre a lista até chegar ao fim
	while (head != NULL) {
		printf("-----------\nJogador #%d\n---------\n#Nome: %s\n#Equipa: %s\n#Posicao: %d\n", i, (((PLAYER*)DATA(head))->nome), (((PLAYER*)DATA(head))->clube), (((PLAYER*)DATA(head))->posicao));
		if ((((PLAYER*)DATA(head))->titular))
			printf("Titular: Sim\n");
		else
			printf("Titular: Nao\n");
		i++;
		head = NEXT(head);
	}
}
PLAYER* readPlayer(void) {
	printf("--------------------\nInserir novo jogador\n--------------------\n\n");
	int aux = NULL;
	PLAYER* newPlayer;
	// Alocação dinâmica
	if ((newPlayer = (PLAYER*)malloc(sizeof(PLAYER))) == NULL)
		return NULL;
	// Leitura de dados
	printf("Insira o nome do Jogador: ");
	gets_s(newPlayer->nome, STR_MAX);
	Buffer();
	printf("Insira o clube do Jogador: ");
	gets_s(newPlayer->clube, STR_MAX);
	Buffer();
	printf("Insira a posicao do Jogador: ");
	scanf("%d", &newPlayer->posicao);
	Buffer();
	printf("\nTitular ?\n\n[0]Nao\n[1]Sim\n\nOpcao: ");
	scanf("%d", &aux);
	Buffer();
	newPlayer->titular = false;
	printf("\n");
	return newPlayer;
}
void Buffer(void) {
	while ((getchar() != '\n'));
	fflush(stdin);
}
LIST addToMiddle(LIST* head, const char* name, void* value) {
	LIST newPlayer = NULL;
	LIST temp = *head;
	// Tenta criar um novo nodo
	if (!(newPlayer = newNode(value)))
		return NULL;
	// Lista nula?
	if (head == NULL)
		if (addToHead(head, newPlayer))
			return newPlayer;
	// Nodo pertence à lista?
	if (!(isValidNode(temp, name))) {
		printf("O Jogador inserido nao existe na lista\n");
		return NULL;
	}
	// Lista não nula e nodo pertencente
	else {
		while (temp != NULL) {
			// 0 se forem iguais
			if (strcmp(((PLAYER*)DATA(temp))->nome, name) == 0) {
				NEXT(newPlayer) = NEXT(temp);
				NEXT(temp) = newPlayer;
				return newPlayer;
			}
			temp = NEXT(temp);
		}
		// Se não encontrou até aqui, o nodo está no fim, logo, addToTail()
			if (addToTail(head, newPlayer))
				return newPlayer;
		// No caso do nodo não se encontrar na lista
			else
				return NULL;
	}
}
bool isValidNode(LIST head, const char* name) {
	// Lista nula?
	if (head == NULL)
		return false;
	else {
		// Percorre a lista até encontrar o nodo
		while (head != NULL) {
			// 0 se forem iguais
			if (strcmp(((PLAYER*)DATA(head))->nome, name) == 0)
				return true;
			head = NEXT(head);
		}
		return false;
	}
}
bool readFile(LIST* head, const char* fileName) {
	// No caso de abrir um 2º ficheiro
	deleteList(head);
	FILE* ptr = NULL;
	PLAYER* newPlayer = NULL;
	int aux = 0;
	
	// Tenta abrir o ficheiro em modo leitura
	if ((ptr = fopen(fileName, "r")) != NULL) {
		// Percorre até chegar ao fim da lista
		while (!feof(ptr)) {
			// Alocação dinâmica
			if ((newPlayer = (PLAYER*)malloc(sizeof(PLAYER))) != NULL) {
				fscanf(ptr, "%[^;];%[^;];%d;%d;\n", newPlayer->nome, newPlayer->clube, &(newPlayer->posicao), &aux);
				if (aux)
					newPlayer->titular = true;
				else
					newPlayer->titular = false;

				// Pode ocorrer um erro a meio do processo
				if (!(addToTail(head, newPlayer))) {
					deleteList(head);
					fclose(ptr);
					return false;
				}
			}
		}
		fclose(ptr);
		return true;
	}
		return false;
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
int countTitulares(LIST head) {
	int i = 0;
	// Lista nula?
	if (head == NULL)
		return 0;
	while (head != NULL) {
		//Filtra por titulares
		if ((((PLAYER*)DATA(head))->titular))
			i++;
		head = NEXT(head);
	}
		return i;
}
bool makeTitular(LIST head, const char* name, int op) {
	// Lista nula?
	if (head == NULL)
		return false;
	if (countTitulares(head) >= 11 && op == 1)
		return false;
	if (!isValidNode(head,name))
		return false;
	// Procura na lista pelo jogador indicado
	while (head != NULL) {
		if (strcmp(((PLAYER*)DATA(head))->nome, name) == 0) {
			// Tornar Titular
			if (op)
				((PLAYER*)DATA(head))->titular = true;
			// Remover Titular
			else if (op == 0)
				((PLAYER*)DATA(head))->titular = false;
			// Opção inválida
			else
				return false;
		}
		head = NEXT(head);
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
		fprintf(fileptr, "%s;%s;%d;", (((PLAYER*)DATA(head))->nome), (((PLAYER*)DATA(head))->clube), (((PLAYER*)DATA(head))->posicao));
		if ((((PLAYER*)DATA(head))->titular))
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
	system("cls");
	char OP = ' ';
	printf("--------------------\nSelecione uma opcao:\n--------------------\n");
	printf("[1]Carregar Equipa de Jogador\n");
	printf("[2]Mostrar Equipa Titular\n");
	printf("[3]Mostrar Equipa Completa\n");
	printf("[4]Adicionar um novo Jogador\n");
	printf("[5]Tornar Jogador Titular\n");
	printf("[6]Guardar Dados num Ficheiro\n");
	printf("[0]Sair\nOpcao: ");
	scanf("%c", &OP);
	Buffer();
	return OP;
}