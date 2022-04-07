#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAX 50

typedef struct _NODE {
	void* data;
	_NODE* next;
}NODE;

typedef struct _SMARTPHONE {
	char marca[STR_MAX];
	char modelo[STR_MAX];
	float preco;
	int stock;
}SMARTPHONE;

// Macros

typedef NODE* LIST;
#define NEXT(node) ((node)->next)
#define DATA(node) ((node)->data)

// Protótipos

bool addToHead(LIST* head, void* data); // Adiciona à cabeça da lista
bool addToTail(LIST* head, void* data); // Adiciona à cauda da lista
void showList(LIST head); // Mostra a lista
void showStockList(LIST head); // Mostra da lista todos os Smartphone que estão em stock
SMARTPHONE* readSmartphone(void); // Lê e retorna um nodo smartphone
void Buffer(void);
char Menu(void);
bool readFile(LIST* head, const char* name); // Carrega para uma lista, Smartphones lidos de um ficheiro
bool deleteList(LIST* head); // Apaga completamente uma lista
float calculaCusto(LIST head); // Retorna o custo de todos os Smartphones da lista
bool deleteUnstockedSmartphones(LIST* head); // Apaga todos os Smartphones que estão assinalados como fora de stock
bool saveToFile(LIST head, const char* fileName); // Guarda uma lista num ficheiro cujo nome foi passado por parâmetro

// Main Program

int main(void) {

	LIST Smartphones = NULL;
	char OP = ' ';
	char strAUX[STR_MAX];

	do {
		switch ((OP = Menu())) {
		// Sair do programa
		case '0': return 0; break;
		// Carregar Smartphones de um ficheiro
		case '1':
			deleteList(&Smartphones);
			printf("Qual o nome do ficheiro a ler? ");
			gets_s(strAUX, STR_MAX);
			Buffer();
			if (readFile(&Smartphones, strAUX))
				printf("Dados carregados com sucesso!\n");
			else
				printf("Ocorreu um erro ao carregar os dados!\n");
			break;
		// Adicionar novo Smartphone à lista
		case '2':
			if (addToTail(&Smartphones, readSmartphone()))
				printf("Smartphone adicionado com sucesso!\n");
			else
				printf("Ocorreu um erro ao tentar adicionar o Smartphone\n");
			break;
		// Mostrar todos os Smartphones
		case '3': showList(Smartphones); break;
		// Mostrar todos os Smartphones em stock
		case '4': showStockList(Smartphones); break;
		// Calcular o custo de todos os smartphones
		case '5': printf("Custo total dos Smartphones em registo: %f\n", calculaCusto(Smartphones)); break;
		// Apagar Smartphones fora de stock
		case '6': 
			if (deleteUnstockedSmartphones(&Smartphones))
				printf("Todos os Smartphones fora de Stock foram eliminados!\n");
			else
				printf("Ocorreu um erro ao tentar apagar os Smartphones!\n");
			break;
		// Guardar Smartphones num Ficheiro
		case '7' :
			printf("Qual o nome do Ficheiro? ");
			gets_s(strAUX, STR_MAX);
			Buffer();
			if (saveToFile(Smartphones, strAUX))
				printf("Dados guardados com sucesso!\n");
			else
				printf("Ocorreu um erro ao tentar guardar os dados!\n");
			break;
		default:printf("Opcao invalida, Tente Novamente!\n"); break;
		}
	system("pause");
	} while (OP != '0');
	return 0;
}

// Funções

void Buffer(void) {
	while (getchar() != '\n');
}
SMARTPHONE* readSmartphone(void) {
	int stock = NULL;
	// Nodo a ser retornado
	SMARTPHONE* newSmartphone = NULL;
	// Alocação dinâmica
	if ((newSmartphone = ((SMARTPHONE*)malloc(sizeof(SMARTPHONE)))) == NULL)
		return NULL;
	// Foi possível alocar o nodo, leitura dos dados
	printf("-----------------------\nRegistar novo Smartphone\n-----------------------\n");
	printf("Marca do Smartphone: ");
	gets_s(newSmartphone->marca, STR_MAX);
	Buffer();
	printf("Modelo do Smartphone: ");
	gets_s(newSmartphone->modelo, STR_MAX);
	Buffer();
	printf("Preco do Smartphone: ");
	scanf("%f", &newSmartphone->preco);
	Buffer();
	do {
		printf("Produto em Stock?\n\n[1]Sim\n[0]Nao\n\nOpcao: ");
		scanf("%d", &stock);
	} while (stock != 1 && stock != 0);
	Buffer();
	switch (stock) {
	case 0: newSmartphone->stock = false; break;
	case 1: newSmartphone->stock = true; break;
	default:break;
	}
	return newSmartphone;
}
bool addToTail(LIST* head, void* data) {
	LIST newTail = NULL;
	LIST current = *head;
	// data nula?
	if (data == NULL)
		return false;
	// Alocação dinâmica
	if (!(newTail = (NODE*)malloc(sizeof(NODE))))
		return false;
	DATA(newTail) = data;
	NEXT(newTail) = NULL;
	// Cabeça nula?
	if (current == NULL) {
		*head = newTail;
		return true;
	}
	// Percorrer a lista até chegar ao fim
	while (NEXT(current) != NULL)
		current = NEXT(current);
	// Estamos no fim la lista
	NEXT(current) = newTail;
	return true;
}
bool addToHead(LIST* head, void* data) {
	LIST newHead = NULL;
	// data nula?
	if (data == NULL)
		return false;
	// Alocação dinâmica
	if (!(newHead = (NODE*)malloc(sizeof(LIST))))
		return false;
	NEXT(newHead) = NULL;
	DATA(newHead) = data;
	// Cabeça nula?
	if (head == NULL) {
		*head = newHead;
		return true;
	}
	// Cabeça não nula
	NEXT(newHead) = *head;
	*head = newHead;
	return true;
}
void showList(LIST head) {
	int i = 1;
	// Cabeça nula?
	if (head == NULL) {
		printf("Lista vazia!\n");
		return;
	}
	// Percorre até chegar ao fim e vai mostrando
	while (head != NULL) {
		printf("------------\nSmartphone %d\n------------\n", i);
		printf("#Marca: %s\n#Modelo: %s\n#Preco: %f\n", ((SMARTPHONE*)DATA(head))->marca, ((SMARTPHONE*)DATA(head))->modelo, ((SMARTPHONE*)DATA(head))->preco);
		switch (((SMARTPHONE*)DATA(head))->stock) {
		case 0: printf("#Stock: Nao\n"); break;
		case 1: printf("#Stock: Sim\n"); break;
		default:break;
		}
		head = NEXT(head);
		i++;
	}
	return;
}
void showStockList(LIST head) {
	int i = 1;
	// Cabeça nula?
	if (head == NULL) {
		printf("Lista vazia!\n");
		return;
	}
	// Percorre até chegar ao fim e vai mostrando se o produto estiver em Stock
	
	while (head != NULL) {
		if (((SMARTPHONE*)DATA(head))->stock) {
			printf("------------\nSmartphone %d\n------------\n", i);
			printf("#Marca: %s\n#Modelo: %s\n#Preco: %f\nStock: Sim\n", ((SMARTPHONE*)DATA(head))->marca, ((SMARTPHONE*)DATA(head))->modelo, ((SMARTPHONE*)DATA(head))->preco);
			i++;
		}
		head = NEXT(head);
		
	}
	return;
}
bool readFile(LIST* head, const char* name) {
	FILE* ptr;
	SMARTPHONE* newSmartphone = NULL;
	int stock = 0;
	// Nome nulo?
	if (name == NULL)
		return false;
	// Foi possível abrir o ficheiro específicado?
	if (ptr = fopen(name, "r")) {
		// Até chegar ao fim
		while (!(feof(ptr))) {
			// Tenta alocar memória
			if ((newSmartphone = (SMARTPHONE*)malloc(sizeof(SMARTPHONE))) != NULL) {
				fscanf(ptr, "%[^;];%[^;];%f;%d;\n", newSmartphone->marca, newSmartphone->modelo, &newSmartphone->preco, &stock);
				switch (stock) {
					case 0: newSmartphone->stock = false; break;
					case 1: newSmartphone->stock = true; break;
					default:break;
				}
				// Caso ocorra um erro a meio do processo de leitura, nesse caso, apaga-se a lista em criação
				if (!addToTail(head, newSmartphone)) {
					deleteList(head);
					fclose(ptr);
					return false;
				}
			}
		}
		fclose(ptr);
		return true;
	}
	// Não foi possível abrir o ficheiro
	return false;
}
bool deleteList(LIST* head) {
	LIST prev = *head;
	LIST current = *head;

	// Lista nula <=> Nada a apagar <=> true, why not?
	if (head == NULL)
		return true;
	// Percorre a lista até ao fim
	while (current != NULL) {
		// Vai nodo a nodo e liberta-os a da memória
		prev = current;
		current = NEXT(current);
		free(prev);
	}
	// Pointer da cabeça passa a ser nulo
	*head = NULL;
	return true;
}
float calculaCusto(LIST head) {
	float total = 0;
	// Cabeça nula <=> custo 0
	if (head == NULL)
		return 0;
	// Cabeça não nula, percorre a lista toda até ao fim
	while (head != NULL) {
		total += ((SMARTPHONE*)DATA(head))->preco;
		head = NEXT(head);
	}
	return total;
}
bool deleteUnstockedSmartphones(LIST* head) {
	// Serve para mostrar no ecra o nº de smartphones apagados
	int i = 1;
	LIST temp = *head;
	LIST prev = NULL;
	// Cabeça nula?
	if (head == NULL)
		return false;
	// Alocação dinâmica
	if(!(prev=(LIST)malloc(sizeof(SMARTPHONE))))
		return false;
	// Os nodos a apagar são cabeças seguidas?
	while (temp != NULL && ((SMARTPHONE*)DATA(temp))->stock == false) {
		// Procede para apagar as cabeças seguidas
		printf("%d Smartphone apagado com sucesso!\n", i);
		i++;
		*head = NEXT(temp);
		free(temp);
		temp = *head;
	}
	// Apagou as cabeças que estavam seguidas, encontrou uma cabeça que estava em stock e agora está a apagar do meio da lista
	while (head != NULL) {
		// Procura por nodos que contenhas Smartphones fora de stock
		while (temp != NULL && (((SMARTPHONE*)DATA(temp))->stock) != false) {
			prev = temp;
			temp = NEXT(temp);
		}
		// Se temp = NULL, então, percorreu a lista toda e não encontrou mais nenhum smartphone fora de stock
		if (temp == NULL)
			return true;
		// temp não é NULL aqui, logo, temp contém um Smartphone fora de stock
		NEXT(prev) = NEXT(temp);
		printf("%d Smartphone apagado com sucesso!\n", i);
		i++;
		free(temp);
		// O nodo antigo de temp saiu da memória, temp é igualado ao próximo node a seguir ao prev para continuar a busca
		temp = NEXT(prev);
	}
	return true;
}
bool saveToFile(LIST head, const char* fileName) {
	FILE* file_ptr = NULL;
	// Lista ou fileNames nulos?
	if (head == NULL || fileName == NULL)
		return false;
	// Foi possível abrir o Ficheiro?
	if ((file_ptr = fopen(fileName, "w")) == NULL)
		return NULL;
	// Percorre a lista toda
	while (head != NULL) {
		fprintf(file_ptr, "%s;%s;%f;", (((SMARTPHONE*)DATA(head))->marca), (((SMARTPHONE*)DATA(head))->modelo), (((SMARTPHONE*)DATA(head))->preco));
		if ((((SMARTPHONE*)DATA(head))->stock))
			fprintf(file_ptr, "1;");
		else
			fprintf(file_ptr, "0;");
		// Impede que fique uma linha vazia no fim
		if (NEXT(head) != NULL)
			fprintf(file_ptr, "\n");
		head = NEXT(head);
	}
	Buffer();
	fclose(file_ptr);
	return true;
}
char Menu(void) {
	char OP = ' ';
	system("cls");
	printf("--------------------\nSelecione uma opcao:\n--------------------\n");
	printf("[1]Carregar Lista de Smartphones\n");
	printf("[2]Adicionar novo Smartphone\n");
	printf("[3]Listar todos Smartphones\n");
	printf("[4]Listar Smartphones em Stock\n");
	printf("[5]Calcular o custo total dos Smartphones listados (Stock + Sem stock)\n");
	printf("[6]Apagar Smartphones fora de Stock\n");
	printf("[7]Guardar Smartphones num Ficheiro\n");
	printf("[0]Sair\nOpcao: ");
	scanf("%c", &OP);
	Buffer();
	return OP;
}