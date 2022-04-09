#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_MAX 50

typedef struct _NODE {
	void* data;
	_NODE* next;
}NODE;

typedef struct _ESTACAO {
	char desig[STR_MAX];
	float custo;
	bool ativa, no;
}ESTACAO;

// MACROS

#define DATA(node) ((node)->data)
#define NEXT(node) ((node)->next)
typedef NODE* LIST;

// PROT�TIPOS

void Buffer(void);
bool addToHead(LIST* head, void* data); // Adiciona � cabe�a da lista
bool addToTail(LIST* head, void* data); // Adiciona � cauda da lista
bool addToMiddle(LIST* head, const char* name, void* data); // Adiciona a seguir a uma esta��o
bool isValidNode(LIST head, const char* name); // Retorna true se a Esta��o pertencer � lista
void printList(LIST head); // Mostra as informa��es da lista no ecr�
void printActiveList(LIST head); // Mostra as informa��es das Esta��es ativas da lista no ecr�
ESTACAO* readEstacao(void); // L� do teclado os dados de uma esta��o
void Menu(char* OP);
bool readFile(LIST* head, const char* fileName); // Carrega para uma lista, dados lidos atrav�s de um ficheiro cujo nome � passado por par�metro
void deleteList(LIST* head); // Apaga completamente uma lista
float calculaCusto(LIST head, const char* start, const char* end); // Retorna o custo de uma viagem, dadas as esta��es de partida e chegada
bool deleteNonActiveStations(LIST* head); // Apaga todas as esta��es que n�o est�o ativas da Linha passada por par�metro
bool changeStationStatus(LIST* head, const char* name, int OP); // Ativa/Desativa uma esta��o consoante a escolha do utilizador
bool saveToFile(LIST head, const char* name); // Guarda uma Linha num ficheiro

// MAIN PROGRAM

int main(void) {
	int nLinhas = 5;
	// Para usar mais do que 5 Linhas, basta alterar o nLinhas e inicializar mais Linhas na l.48, o programa faz o resto
	LIST Linhas[5] = {NULL,NULL,NULL,NULL,NULL};
	char OP = ' ';
	char strAUX[STR_MAX], start[STR_MAX], end[STR_MAX];
	int aux = 0, intOP = 0;
	do {
		Menu(&OP);
		Buffer();
		switch (OP) {
		// Sair do Programa
		case '0': exit(0); break;
		// Carregar Estacoes de um ficheiro
		case '1':
			printf("\n--------------------------------\nCarregar Estacoes de um Ficheiro\n--------------------------------\n\n");
			printf("Insira o nome do ficheiro a ler: ");
			gets_s(strAUX, STR_MAX);
			Buffer();
			do {
				printf("Carregar dados na Linha: ");
				scanf("%d", &aux);
				Buffer();
				if (!(aux > 0 && aux <= nLinhas))
					printf("\nLinha nao existente, tente novamente!\n\n");
			} while (!(aux >= 0 && aux <= nLinhas));
			aux--;
			deleteList(&Linhas[aux]);
			if (readFile(&Linhas[aux], strAUX))
				printf("\nDados carregados com sucesso!\n\n");
			else
				printf("\nOcorreu um erro na leitura do ficheiro!\n\n");
			break;
		// Mostrar Estac�es no ecr�
		case '2': 
			printf("\n----------------\nMostrar Estacoes\n----------------\n\n");
			do {
				printf("Mostrar informacoes da Linha: ");
				scanf("%d", &aux);
				Buffer();
				if (!(aux > 0 && aux <= nLinhas))
					printf("\nLinha nao existente, tente novamente!\n\n");
			} while (!(aux >= 0 && aux <= nLinhas));
			printList(Linhas[--aux]);
			break;
		// Mostrar apenas Esta��es ativas no ecr�
		case '3':
			printf("\n----------------\nMostrar Estacoes\n----------------\n\n");
			do {
				printf("Mostrar informacoes da Linha: ");
				scanf("%d", &aux);
				Buffer();
				if (!(aux > 0 && aux <= nLinhas))
					printf("\nLinha nao existente, tente novamente!\n\n");
			} while (!(aux >= 0 && aux <= nLinhas));
			printActiveList(Linhas[--aux]);
			break;
		// Adicionar nova Esta��o
		case '4':
			printf("\n-----------------------------\nAdicionar Estacao Manualmente\n-----------------------------\n\n");
			do {
				printf("[1]Inserir no inicio da Linha\n[2]Inserir no meio da Linha\n[3]Inserir no fim da Linha\n\nOpcao: ");
				scanf("%d", &intOP);
			} while (intOP != 1 && intOP != 2 && intOP != 3);
			switch (intOP) {
				// Inser��o no in�cio da lista
				case 1:
					do {
						printf("\nAdicionar Estacao na Linha: ");
						scanf("%d", &aux);
						if (!(aux > 0 && aux <= nLinhas))
							printf("\nLinha nao existente, tente novamente!\n\n");
					} while (!(aux > 0 && aux <= nLinhas));
					Buffer();
					aux--;
					if (addToHead(&Linhas[aux], readEstacao()))
						printf("\nEstacao adicionada com sucesso na Linha %d\n\n", ++aux);
					else
						printf("Ocorreu um erro ao efetuar o pedido!\n\n");
					break;
				// Inser��o no meio da lista
				case 2:
					do {
						printf("\nAdicionar Estacao na Linha: ");
						scanf("%d", &aux);
						Buffer();
						if (!(aux > 0 && aux <= nLinhas))
							printf("\nLinha nao existente, tente novamente!\n\n");
					} while (!(aux > 0 && aux <= nLinhas));
					printf("\nInserir apos Estacao: ");
					gets_s(strAUX, STR_MAX);
					Buffer();
			
				aux--;
				if (addToMiddle(&Linhas[aux], strAUX, readEstacao()))
					printf("\nEstacao adicionada com sucesso na Linha %d\n\n",++aux);
				else
					printf("\nOcorreu um erro ao efetuar o pedido!\n\n");
				break;
				// Inser��o no final da lista
				case 3:
					do {
						printf("\nAdicionar Estacao na Linha: ");
						scanf("%d", &aux);
						if (!(aux > 0 && aux <= nLinhas))
							printf("\nLinha nao existente, tente novamente!\n\n");
					} while (!(aux > 0 && aux <= nLinhas));
					Buffer();
					aux--;
					if (addToTail(&Linhas[aux], readEstacao()))
						printf("\nEstacao adicionada com sucesso na Linha %d\n\n", ++aux);
					else
						printf("Ocorreu um erro ao efetuar o pedido!\n\n");
					break;
				default:break;
			}
			break;
		// Apagar esta��es n�o ativas
		case '5':
			printf("\n--------------------------\nApagar Estacoes nao Ativas\n--------------------------\n\n");
			do {
				printf("Apagar da Linha: ");
				scanf("%d", &aux);
				Buffer();
				if (!(aux > 0 && aux <= nLinhas))
					printf("\nLinha nao existente, tente novamente!\n\n");
			} while (!(aux > 0 && aux <= nLinhas));
			if (deleteNonActiveStations(&Linhas[--aux]))
				printf("\nEstacoes apagadas com sucesso!\n\n");
			else
				printf("Ocorreu um erro ao efetuar o pedido!\n\n");
			break;
		// Ativar/Desativar Esta��o
		case '6':
			printf("\n-------------------------\nAtivar/Desativar Estacao\n-------------------------\n\n");
			do {
				printf("Alterar Linha: ");
				scanf("%d", &aux);
				Buffer();
				if (!(aux > 0 && aux <= nLinhas))
					printf("\nLinha nao existente, tente novamente!\n\n");
			} while (!(aux > 0 && aux <= nLinhas));
			printf("\nAlterar Estacao: ");
			gets_s(strAUX, STR_MAX);
			Buffer();
			do {
				printf("[0]Desativar\n[1]Ativar\n\nOpcao: ");
				scanf("%d", &intOP);
				Buffer();
			} while (intOP != 0 && intOP != 1);
			if (changeStationStatus(&Linhas[--aux], strAUX, intOP))
				printf("\nEstacao alterada com sucesso!\n");
			else
				printf("\nOcorreu um erro no processo!\n");
			break;
		// Calcular o custo de uma viagem
		case '7':
			printf("\n------------------------------\nCalcular o Custo de uma Viagem\n------------------------------\n\n");
			do {
				printf("Viajar na Linha: ");
				scanf("%d", &aux);
				Buffer();
				if (!(aux > 0 && aux <= nLinhas))
					printf("\nLinha nao existente, tente novamente!\n\n");
			} while (!(aux > 0 && aux <= nLinhas));
			printf("Estacao de partida: ");
			gets_s(start, STR_MAX);
			Buffer();
			printf("Estacao de chegada:  ");
			gets_s(end, STR_MAX);
			Buffer();
			printf("O custo da viagem entre %s e %s e de: %.2f\n\n", start, end, calculaCusto(Linhas[--aux],start,end));
			break;
		// Verifica se uma dada Esta��o pertence a uma dada Lista
		case '8':
			printf("\n---------------------------------------------\nVerificar se uma Estacao pertence a uma Linha\n---------------------------------------------\n\n");
			do {
				printf("Procurar na Linha: ");
				scanf("%d", &aux);
				Buffer();
				if (!(aux > 0 && aux <= nLinhas))
					printf("\nLinha nao existente, tente novamente!\n\n");
			} while (!(aux > 0 && aux <= nLinhas));
			printf("\nQue Estacao procura? ");
			gets_s(strAUX, STR_MAX);
			Buffer();
			if (isValidNode(Linhas[--aux], strAUX))
				printf("A Estacao: \"%s\" pertence a Linha %d\n\n", strAUX, ++aux);
			else
				printf("Nao foi possivel encontrar a Estacao: \"%s\" na Linha %d!\n\n", strAUX, ++aux);
			break;
		// Guardar informacoes num Ficheiro
		case '9':
			printf("\n--------------------------\nGuardar Linha num Ficheiro\n--------------------------\n\n");
			do {
				printf("Guardar Linha: ");
				scanf("%d", &aux);
				Buffer();
				if (!(aux > 0 && aux <= nLinhas))
					printf("\nLinha nao existente, tente novamente!\n\n");
			} while (!(aux > 0 && aux <= nLinhas));
			printf("\nGuardar no Ficheiro: ");
			gets_s(strAUX, STR_MAX);
			Buffer();
			if (saveToFile(Linhas[--aux], strAUX))
				printf("Estacoes guardadas com sucesso!\n\n");
			else
				printf("Ocorreu um erro ao efetuar o pedido!\n\n");
			break;
		default: printf("Opcao invalida, tente novamente!\n"); break;
		}
		system("pause");
		system("cls");
	} while (OP != '0');
	return 0;
}

// FUN��ES

void Buffer(void) {
	while (getchar() != '\n');
}
ESTACAO* readEstacao(void) {
	ESTACAO* newEstacao = NULL;
	char OP;
	// Aloca��o din�mica
	if ((newEstacao = (ESTACAO*)malloc(sizeof(ESTACAO))) == NULL)
		return NULL;
	// Leitura dos dados
	printf("\n-------------------------\nInserir uma nova Estacao:\n-------------------------\n\n");
	printf("#Nome da Estacao : ");
	gets_s(newEstacao->desig,STR_MAX);
	Buffer();
	printf("#Custo da Estacao : ");
	scanf("%f", &(newEstacao->custo));
	Buffer();
	do {
		printf("\n#Estacao Ativa ? [y/n]\nOpcao : ");
		scanf("%c", &OP);
		Buffer();
		if (OP == 'y' || OP == 'Y')
			newEstacao->ativa = true;
		else if (OP == 'n' || OP == 'N')
			newEstacao->ativa = false;
		else
			printf("Insira uma opcao valida!\n");
	} while (OP != 'y' && OP != 'Y' && OP != 'n' && OP != 'N');
	do {
		printf("\n#E um no ? [y/n]\nOpcao : ");
		scanf("%c", &OP);
		Buffer();
		if (OP == 'y' || OP == 'Y')
			newEstacao->no = true;
		else if (OP == 'n' || OP == 'N')
			newEstacao->no = false;
		else
			printf("Insira uma opcao valida!\n");
	} while (OP != 'y' && OP != 'Y' && OP != 'n' && OP != 'N');
	return newEstacao;
}
bool addToHead(LIST* head, void* data) {
	LIST newHead = NULL;
	// Aloca��o din�mica
	if ((newHead = (NODE*)malloc(sizeof(NODE))) == NULL)
		return false;
	DATA(newHead) = data;
	NEXT(newHead) = NULL;
	// Cabe�a nula?
	if (head == NULL)
		*head = newHead;
	// Cabe�a n�o nula
	NEXT(newHead) = *head;
	*head = newHead;
	return true;
}
bool addToTail(LIST* head, void* data) {
	LIST newTail = NULL;
	LIST temp = *head;
	// Aloca��o din�mica
	if ((newTail = (NODE*)malloc(sizeof(NODE))) == NULL)
		return false;
	DATA(newTail) = data;
	NEXT(newTail) = NULL;
	// Cabe�a nula?
	if (temp == NULL) {
		*head = newTail;
		return true;
	}
	// Cabe�a n�o nula => Ir at� ao fim da Lista
	while (NEXT(temp) != NULL)
		temp = NEXT(temp);
	// Estamos no fim da Lista
	NEXT(temp) = newTail;
	return true;
}
void printList(LIST head) {
	int i = 1;
	// Cabe�a nula?
	if (head == NULL) {
		printf("\nA Linha esta vazia!\n\n");
		return;
	}
	// Cabe�a n�o nula => Ir at� ao fim da lista
	while (head != NULL) {
		printf("\n---------\nEstacao %d\n---------\n", i);
		printf("#Nome: %s\n#Custo: %.2f\n", ((ESTACAO*)DATA(head))->desig, ((ESTACAO*)DATA(head))->custo);
		switch (((ESTACAO*)DATA(head))->ativa) {
		case true: printf("#Ativa: Sim\n"); break;
		case false: printf("#Ativa: Nao\n"); break;
		default:break;
		}
		switch (((ESTACAO*)DATA(head))->no) {
		case true: printf("#No: Sim\n"); break;
		case false: printf("#No: Nao\n"); break;
		default:break;
		}
		i++;
		head = NEXT(head);
	}
	printf("\n");
}
void printActiveList(LIST head) {
	int i = 1;
	// Cabe�a nula?
	if (head == NULL) {
		printf("\nA Linha esta vazia!\n\n");
		return;
	}
	// Cabe�a n�o nula => Ir at� ao fim da lista
	while (head != NULL) {
		// Verifica se a Esta��o est� ativa
		if (((ESTACAO*)DATA(head))->ativa) {
			printf("\n---------\nEstacao %d\n---------\n", i);
			printf("#Nome: %s\n#Custo: %.2f\nAtiva: Sim\n", ((ESTACAO*)DATA(head))->desig, ((ESTACAO*)DATA(head))->custo);
			switch (((ESTACAO*)DATA(head))->no) {
			case true: printf("#No: Sim\n"); break;
			case false: printf("#No: Nao\n"); break;
			default:break;
			}
			i++;
		}
		head = NEXT(head);
	}
}
bool isValidNode(LIST head, const char* name) {
	// Cabe�a nula?
	if (head == NULL)
		return false;
	// Percorre at� ao fim ou at� encontrar a Estacao pretendida
	while (head != NULL && (strcmp(((ESTACAO*)DATA(head))->desig, name)) != 0)
		head = NEXT(head);
	// Cabe�a nula <=> N�o encontrou a esta��o
	if (head == NULL)
		return false;
	// Cabe�a n�o nula <=> Encontrou a Esta��o
	return true;
}
bool addToMiddle(LIST* head, const char* name, void* data) {
	LIST temp = *head;
	LIST newEstacao = NULL;
	// Nome ou data nulos?
	if (head == NULL || name == NULL || data == NULL)
		return false;
	// Verificar se a Esta��o pretendida n�o pertence � Linha
	if (!isValidNode(*head, name))
		return false;
	// Aloca��o din�mica
	if ((newEstacao = (NODE*)malloc(sizeof(NODE))) == NULL)
		return false;
	// Se a cabe�a for nula, addToHead()
	if (head == NULL) {
		if (addToHead(head, data))
			return true;
		return false;
	}
	// Avan�a na lista at� chegar ao nodo pretendido
	while (NEXT(temp) != NULL && (((ESTACAO*)DATA(temp))->desig == name) != 0)
		temp = NEXT(temp);
	// Se estivermos no fim da lista, addToTail()
	if (NEXT(temp) == NULL) {
		if (addToTail(head, data))
			return true;
	}
	// Se n�o estivermos no fim da lista
	DATA(newEstacao) = data;
	NEXT(newEstacao) = NEXT(temp);
	NEXT(temp) = newEstacao;
	return true;
}
bool readFile(LIST* head, const char* fileName) {
	FILE* file_ptr = NULL;
	ESTACAO* newEstacao = NULL;
	int ativa = NULL, no = NULL;
	// Nome nulo?
	if (fileName == NULL)
		return false;
	// Tenta abrir o Ficheiro
	if ((file_ptr = fopen(fileName, "r")) == NULL)
		return false;
	
	// Ficheiro aberto => Ler at� ao fim
	while (!feof(file_ptr)) {
		// Aloca��o din�mica
		if ((newEstacao = (ESTACAO*)malloc(sizeof(ESTACAO))) == NULL)
			return false;
		fscanf(file_ptr,"%[^;];%f;%d;%d;\n", newEstacao->desig, &(newEstacao->custo), &ativa, &no);
		if (ativa)
			newEstacao->ativa = true;
		else
			newEstacao->ativa = false;
		if (no)
			newEstacao->no = true;
		else
			newEstacao->no = false;
		// Pode ocorrer um erro a meio do processo
		if (!addToTail(head, newEstacao)) {
			deleteList(head);
			fclose(file_ptr);
			return false;
		}
	}
	fclose(file_ptr);
	return true;
}
void deleteList(LIST* head) {
	LIST current = *head;
	LIST prev = *head;
	// Cabe�a nula <=> Nada a apagar
	if (head == NULL)
		return;
	// Cabe�a n�o nula => Percorrer a lista completa
	while (current != NULL) {
		prev = current;
		current = NEXT(current);
		free(prev);
	}
	*head = NULL;
}
float calculaCusto(LIST head, const char* start, const char* end) {
	float custo = 0;
	// Cabe�a nula?
	if (head == NULL)
		return 0;
	// Esta��es pertencem � Linha?
	if (!(isValidNode(head, start) && isValidNode(head, end)))
		return 0;
	// Percorre at� chegar � partida
	while (head != NULL && strcmp(((ESTACAO*)DATA(head))->desig, start))
		head = NEXT(head);
	// Custo da esta��o de partida
	custo += ((ESTACAO*)DATA(head))->custo;
	// Estamos na Esta��o de partida => Come�a a contar at� chegar � Esta��o de chegada
	while (head != NULL && strcmp(((ESTACAO*)DATA(head))->desig, end)) {
		custo += ((ESTACAO*)DATA(head))->custo;
		head = NEXT(head);
	}
	return custo;
}
bool deleteNonActiveStations(LIST* head) {
	LIST prev = NULL;
	LIST current = *head;
	int i = 1;
	// Cabe�a nula <=> Nada a pagar <=> Retorno true
	if (current == NULL)
		return true;
	// Aloca��o din�mica
	if (!(prev = (LIST)malloc(sizeof(ESTACAO))))
		return false;
	printf("\n");
	// Estamos a apagar cabe�as seguidas?
	while (head != NULL && (((ESTACAO*)DATA(*head))->ativa) == false) {
		printf("%d Estacao apagada com sucesso!\n", i++);
		*head = NEXT(current);
		free(current);
		current = *head;
	}
	// Apagou as cabe�as que estavam seguidas, encontrou uma Esta��o que n�o era para apagar e agora est� a apagar do meio da Linha
	while (head != NULL) {
		// Avan�a se n�o for para apagar
		while (current != NULL && (((ESTACAO*)DATA(current))->ativa) != false) {
			prev = current;
			current = NEXT(current);
		}
		// Se current == NULL <=> Percorreu o resto na Linha e n�o encontrou mais nada a apagar
		if (current == NULL)
			return true;
		// current n�o � NULL => Apagar nodo em quest�o
		NEXT(prev) = NEXT(current);
		free(current);
		current = NEXT(prev);
		printf("%d Estacao apagada com sucesso!\n", i++);
	}
	return true;
}
bool changeStationStatus(LIST* head, const char* name, int OP) {
	LIST current = *head;
	// Cabe�a nula?
	if (current == NULL)
		return false;
	// O main switch j� faz esta verifica��o, mas, mesmo n�o sendo necess�rio, porque n�o?
	if (OP != 0 && OP != 1)
		return false;
	// Estacao pertence � Linha?
	if (!isValidNode(*head, name))
		return false;
	// Percorre at� encontrar a Esta��o pretendida
	while (current != NULL && strcmp((((ESTACAO*)DATA(current))->desig), name))
		current = NEXT(current);
	// Estamos na Esta��o a alterar, ativar ou desativar?
	if (OP)
		((ESTACAO*)DATA(current))->ativa = true;
	else
		((ESTACAO*)DATA(current))->ativa = false;
	return true;
}
bool saveToFile(LIST head, const char* name) {
	FILE* ptr = NULL;
	// Cabe�a nula <=> Nada a guardar
	if (head == NULL)
		return false;
	// Nome v�lido?
	if (name == NULL)
		return false;
	// Tenta abrir o ficheiro
	if ((ptr = fopen(name, "w")) == NULL)
		return false;
	// Percorre a lista at� ao fim e vai guardando
	while (head != NULL) {
		fprintf(ptr, "%s;%f;", ((ESTACAO*)DATA(head))->desig, ((ESTACAO*)DATA(head))->custo);
		if (((ESTACAO*)DATA(head))->ativa)
			fprintf(ptr, "1;");
		else
			fprintf(ptr, "0;");
		if (((ESTACAO*)DATA(head))->no)
			fprintf(ptr, "1;");
		else
			fprintf(ptr, "0;");
		// Impede que fique uma linha vazia no fim
		if (NEXT(head) != NULL)
			fprintf(ptr, "\n");
		head = NEXT(head);
	}
	fclose(ptr);
	return true;
}
void Menu(char* OP) {
	printf("#########################################\n#                                       #\n# Bem vindo ao programa Subway Network! #\n#                                       #\n#########################################\n\n");
	printf("[1]Carregar Estacoes de um Ficheiro\n");
	printf("[2]Mostrar Estacoes\n");
	printf("[3]Mostrar Estacoes Ativas\n");
	printf("[4]Adicionar Estacao Manualmente\n");
	printf("[5]Apagar Estacoes nao Ativas\n");
	printf("[6]Ativar/Desativar Estacao\n");
	printf("[7]Calcular o Custo de uma Viagem\n");
	printf("[8]Verificar se uma Estacao pertence a uma Linha\n");
	printf("[9]Guardar Linha num Ficheiro\n");
	printf("[0]Sair\n\nOpcao: ");
	scanf("%c", OP);
}