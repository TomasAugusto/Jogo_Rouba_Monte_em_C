#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <locale.h> 

#define MAX_CARTAS 52 // Quantidade máxima de cartas em um baralho
#define MAX_PILHA 52 // Tamanho máximo da pilha de compra
#define MAX_JOGADORES 10 // Quantidade máxima de jogadores

typedef struct {
    int numero; // Número da carta
    char naipe; // Naipe da carta
} Carta;

void inicializar_baralho(Carta *baralho) {
    int index = 0; // Índice para percorrer o baralho
    char naipes[4] = {'C', 'O', 'P', 'E'}; // Naipes: Copas, Ouros, Paus, Espadas

    for (int numero = 1; numero <= 13; numero++) { // Loop de Ás a Rei (13 cartas)
        for (int naipe = 0; naipe < 4; naipe++) { // Loop pelos 4 naipes
            baralho[index].numero = numero; // Atribui o número da carta
            baralho[index].naipe = naipes[naipe]; // Atribui o naipe da carta
            index++; // Incrementa o índice para a próxima carta no baralho
        }
    }
}

void embaralhar_baralho(Carta *baralho, int num_cartas) {
    srand(time(NULL)); // Inicializa a semente para a função rand() com base no tempo

    for (int i = num_cartas - 1; i > 0; i--) { // Loop reverso para embaralhar o baralho
        int j = rand() % (i + 1); // Gera um índice aleatório entre 0 e i
        Carta temp = baralho[i]; // Armazena temporariamente uma carta do baralho
        baralho[i] = baralho[j]; // Troca cartas de posição no baralho
        baralho[j] = temp; // Atribui a carta temporária para completar a troca
    }
}


//------------------------------------PILHA COMPRA-----------------------------------------------

// Definição de uma estrutura Pilha que contém um array de Carta e um inteiro que representa o topo da pilha
typedef struct {
    Carta cartas[MAX_PILHA]; // Array de cartas representando a pilha
    int topo; // Índice do topo da pilha
} Pilha;

// Função para inicializar a pilha, atribuindo -1 ao topo para indicar que a pilha está vazia
void inicializar_pilha(Pilha *pilha) {
    pilha->topo = -1; // Inicializa o topo como -1 para indicar uma pilha vazia
}

// Função que verifica se a pilha está vazia, retornando 1 se estiver e 0 se não estiver vazia
int pilha_vazia(Pilha *pilha) {
    return (pilha->topo == -1); // Verifica se a pilha está vazia
}

// Função que verifica se a pilha está cheia, retornando 1 se estiver e 0 se não estiver cheia
int pilha_cheia(Pilha *pilha) {
    return (pilha->topo == MAX_PILHA - 1); // Verifica se a pilha está cheia
}

// Função para adicionar uma carta à pilha (push)
void push(Pilha *pilha, Carta carta) {
    if (!pilha_cheia(pilha)) { // Verifica se a pilha não está cheia
        pilha->topo++; // Incrementa o topo da pilha
        pilha->cartas[pilha->topo] = carta; // Adiciona a carta ao topo da pilha
    } else {
        printf("Erro: a pilha está cheia!\n"); // Mensagem de erro se a pilha estiver cheia
    }
}

// Função para retirar uma carta da pilha (pop)
Carta pop(Pilha *pilha) {
    Carta carta_vazia = {-1, 'X'}; // Carta para indicar uma pilha vazia ou erro
    if (!pilha_vazia(pilha)) { // Verifica se a pilha não está vazia
        Carta carta_retirada = pilha->cartas[pilha->topo]; // Armazena a carta do topo da pilha
        pilha->topo--; // Decrementa o topo da pilha
        return carta_retirada; // Retorna a carta retirada
    } else {
        printf("Erro: a pilha está vazia!\n"); // Mensagem de erro se a pilha estiver vazia
        return carta_vazia; // Retorna uma carta vazia para indicar o erro
    }
}

// Função para criar a pilha de compra
void criar_pilha_compra(Pilha *pilha, Carta *baralho, int num_cartas) {
    inicializar_pilha(pilha); // Inicializa a pilha de compra
    for (int i = 0; i < num_cartas; i++) { // Loop para adicionar cartas ao baralho
        push(pilha, baralho[i]); // Adiciona a carta do baralho à pilha de compra
    }
    embaralhar_baralho(pilha->cartas, num_cartas); // Embaralha as cartas na pilha de compra
}


//------------------------------------LISTA ÁREA DE DESCARTE-----------------------------------------------

// Definição da estrutura No que representa um nó da lista, contendo uma Carta e um ponteiro para o próximo nó
typedef struct No {
    Carta carta;
    struct No *prox;
} No;

// Definição da estrutura Lista que mantém o ponteiro para o início da lista
typedef struct {
    No *inicio; // Ponteiro para o início da lista
} Lista;

// Função para inicializar a lista, atribuindo NULL ao início para indicar uma lista vazia
void inicializar_lista(Lista *lista) {
    lista->inicio = NULL; // Inicializa o início da lista como NULL
}

// Função que verifica se a lista está vazia, retornando 1 se estiver e 0 se não estiver vazia
int lista_vazia(Lista *lista) {
    return (lista->inicio == NULL); // Verifica se o início da lista é NULL
}

// Função para inserir um novo nó no início da lista
void inserir_inicio(Lista *lista, Carta carta) {
    No *novo_no = (No *)malloc(sizeof(No)); // Aloca memória para um novo nó
    if (novo_no != NULL) {
        novo_no->carta = carta; // Atribui a carta ao novo nó
        novo_no->prox = lista->inicio; // Faz o novo nó apontar para o início atual da lista
        lista->inicio = novo_no; // Atualiza o início da lista para o novo nó
    } else {
        printf("Erro: falha ao alocar memória para o nó!\n"); // Mensagem de erro se a alocação falhar
    }
}

// Função para remover um nó de uma posição específica na lista
int remover_posicao(Lista *lista, int posicao) {
    if (lista_vazia(lista) || posicao < 0) { // Verifica se a lista está vazia ou a posição é inválida
        printf("Erro: lista vazia ou posição inválida!\n"); // Mensagem de erro se a lista estiver vazia ou a posição for inválida
        return 0;
    }

    No *anterior = NULL;
    No *atual = lista->inicio;
    int cont = 0;

    while (atual != NULL && cont < posicao) { // Percorre a lista até a posição desejada
        anterior = atual;
        atual = atual->prox;
        cont++;
    }

    if (atual == NULL) {
        printf("Erro: posição inválida!\n"); // Mensagem de erro se a posição for inválida
        return 0;
    }

    if (anterior == NULL) {
        lista->inicio = atual->prox; // Se o nó a ser removido for o primeiro
    } else {
        anterior->prox = atual->prox; // Se o nó a ser removido estiver no meio ou no final da lista
    }

    free(atual); // Libera a memória do nó removido
    return 1;
}

// Função para liberar a memória alocada para a lista
void liberar_lista(Lista *lista) {
    No *atual = lista->inicio; // Define o nó atual como o início da lista
    while (atual != NULL) { // Percorre a lista
        No *temp = atual; // Armazena o nó atual temporariamente
        atual = atual->prox; // Move para o próximo nó
        free(temp); // Libera a memória do nó armazenado temporariamente
    }
    lista->inicio = NULL; // Define o início da lista como NULL após liberar toda a memória
}


//------------------------------------PILHA JOGADOR-----------------------------------------------

// Define a quantidade máxima de cartas que um jogador pode ter como igual ao máximo de cartas em um baralho
#define MAX_CARTAS_JOGADOR MAX_CARTAS

// Define a estrutura PilhaJogador que contém um array de cartas e um índice do topo da pilha
typedef struct {
    Carta cartas[MAX_CARTAS_JOGADOR]; // Array de cartas do jogador
    int topo; // Índice do topo da pilha do jogador
} PilhaJogador;

// Função para inicializar a pilha do jogador, atribuindo -1 ao topo para indicar uma pilha vazia
void inicializar_pilha_jogador(PilhaJogador *pilha) {
    pilha->topo = -1; // Inicializa o topo como -1 para indicar uma pilha vazia
}

// Verifica se a pilha do jogador está vazia
int pilha_jogador_vazia(PilhaJogador *pilha) {
    return (pilha->topo == -1); // Verifica se a pilha do jogador está vazia
}

// Verifica se a pilha do jogador está cheia
int pilha_jogador_cheia(PilhaJogador *pilha) {
    return (pilha->topo == MAX_CARTAS_JOGADOR - 1); // Verifica se a pilha do jogador está cheia
}

// Insere uma carta na pilha do jogador
void push_jogador(PilhaJogador *pilha, Carta carta) {
    if (!pilha_jogador_cheia(pilha)) {
        pilha->topo++;
        pilha->cartas[pilha->topo] = carta;
    } else {
        printf("Erro: a pilha do jogador está cheia!\n");
    }
}

// Remove e retorna uma carta da pilha do jogador
Carta pop_jogador(PilhaJogador *pilha) {
    Carta carta_vazia = {-1, 'X'}; // Carta para indicar uma pilha vazia ou erro
    if (!pilha_jogador_vazia(pilha)) {
        Carta carta_retirada = pilha->cartas[pilha->topo];
        pilha->topo--;
        return carta_retirada;
    } else {
        printf("Erro: a pilha do jogador está vazia!\n");
        return carta_vazia;
    }
}

// Retorna a quantidade de cartas no monte do jogador
int quantidade_cartas_jogador(PilhaJogador *jogador) {
    return jogador->topo + 1; // Retorna a quantidade de cartas no monte do jogador
}

//------------------------------------JOGADAS-----------------------------------------------

// Função para comprar uma carta da pilha de compra
Carta comprar_carta(Pilha *pilha_compra) {
    if (!pilha_vazia(pilha_compra)) {
        return pop(pilha_compra); // Retorna a carta retirada do topo da pilha de compra
    } else {
        Carta carta_vazia = {-1, 'X'}; // Carta para indicar uma pilha vazia ou erro
        printf("Erro: a pilha de compra está vazia!\n");
        return carta_vazia;
    }
}

// Verifica se algum outro jogador possui a carta do topo do monte atual
int verificar_carta_topo_jogadores(PilhaJogador *jogadores, int num_jogadores, Carta carta_verificar, int jogador_atual) {
    // Percorre os jogadores para verificar se têm a carta do topo do monte do jogador atual
    for (int i = 0; i < num_jogadores; i++) {
        if (i != jogador_atual && !pilha_jogador_vazia(&jogadores[i]) &&
            jogadores[i].cartas[jogadores[i].topo].numero == carta_verificar.numero) {
            printf("O Jogador %d tem a carta %d%c no topo do monte!\n", i + 1, jogadores[i].cartas[jogadores[i].topo].numero, jogadores[i].cartas[jogadores[i].topo].naipe);
            return i; // Retorna o índice do jogador que possui a carta
        }
    }
    printf("Nenhum outro jogador tem a carta com o número %d no topo do monte.\n", carta_verificar.numero);
    return -1; // Retorna -1 se nenhum outro jogador tiver a carta
}

// Verifica se a carta comprada está na área de descarte
int verificar_carta_descarte(Lista *area_descarte, Carta carta_comprada, PilhaJogador *jogadores, int jogador_atual) {
    No *anterior = NULL; // Ponteiro para o nó anterior na lista
    No *atual = area_descarte->inicio; // Ponteiro para o nó atual na lista de descarte
    int encontrada = 0; // Flag indicando se a carta foi encontrada na área de descarte

    // Iteração na lista de descarte
    while (atual != NULL) {
        // Verifica se a carta na área de descarte tem o mesmo número que a carta comprada
        if (atual->carta.numero == carta_comprada.numero) {
            printf("Existe uma carta com o número %d na área de descarte.\n", atual->carta.numero);
            printf("As cartas foram adicionadas ao monte do jogador.\n");
            encontrada = 1; // Indica que a carta foi encontrada na área de descarte

            // Movendo a carta para o monte do jogador atual
            push_jogador(&jogadores[jogador_atual], atual->carta);

            // Removendo a carta encontrada da área de descarte
            if (anterior == NULL) {
                area_descarte->inicio = atual->prox; // Ajusta o início da lista
                free(atual); // Libera a memória do nó atual
                atual = area_descarte->inicio; // Atualiza o nó atual para o próximo da lista
            } else {
                anterior->prox = atual->prox; // Atualiza o ponteiro do nó anterior
                free(atual); // Libera a memória do nó atual
                atual = anterior->prox; // Atualiza o nó atual para o próximo da lista
            }

            // Adicionando a carta comprada ao monte do jogador atual
            push_jogador(&jogadores[jogador_atual], carta_comprada);
            break; // Encerra o loop após a operação
        } else {
            anterior = atual; // Atualiza o ponteiro do nó anterior
            atual = atual->prox; // Avança para o próximo nó na lista de descarte
        }
    }

    // Se a carta não foi encontrada na área de descarte, exibe uma mensagem
    if (!encontrada) {
        printf("Não existe carta com o número %d na área de descarte.\n", carta_comprada.numero);
    }
    return encontrada; // Retorna se a carta foi encontrada na área de descarte
}


// Insere uma carta na área de descarte
void inserir_descarte(Lista *area_descarte, Carta carta) {
    No *novo_no = (No *)malloc(sizeof(No)); // Aloca memória para um novo nó na área de descarte
    if (novo_no != NULL) { // Verifica se a alocação de memória foi bem-sucedida
        novo_no->carta = carta; // Atribui a carta ao novo nó
        novo_no->prox = NULL; // Define o próximo nó como nulo

        if (area_descarte->inicio == NULL) { // Verifica se a área de descarte está vazia
            area_descarte->inicio = novo_no; // Define o novo nó como o início da área de descarte
        } else {
            No *atual = area_descarte->inicio; // Ponteiro para o início da lista de descarte
            while (atual->prox != NULL) { // Encontra o último nó na lista de descarte
                atual = atual->prox;
            }
            atual->prox = novo_no; // Adiciona o novo nó ao final da lista de descarte
        }
        printf("\nNenhuma carta com número igual...\n"); // Mensagem indicando a inserção bem-sucedida
        printf("Carta %d%c foi inserida na área de descarte.\n", carta.numero, carta.naipe); // Exibe a carta inserida
    } else {
        printf("Erro: falha ao alocar memória para o nó na área de descarte!\n"); // Exibe erro se a alocação falhar
    }
}



void realizar_jogada(Pilha *pilha_compra, Lista *area_descarte, PilhaJogador *jogadores, int num_jogadores, int jogador_atual, int *quantidade_anterior) {
    // Armazena a quantidade atual de cartas do jogador
    int quantidade_atual = quantidade_cartas_jogador(&jogadores[jogador_atual]);

    Carta carta_comprada = comprar_carta(pilha_compra);
    printf("\nJogador %d comprou a carta %d%c.\n\n", jogador_atual + 1, carta_comprada.numero, carta_comprada.naipe);

    // Verifica se o número da carta comprada é igual ao número no topo do monte de outro jogador
    int jogador_carta_topo = verificar_carta_topo_jogadores(jogadores, num_jogadores, carta_comprada, jogador_atual);

    int igual_carta_topo = 0;
    if (!pilha_jogador_vazia(&jogadores[jogador_atual]) && jogadores[jogador_atual].cartas[jogadores[jogador_atual].topo].numero == carta_comprada.numero) {
        igual_carta_topo = 1;
    }

    if (igual_carta_topo && !verificar_carta_descarte(area_descarte, carta_comprada, jogadores, jogador_atual)) {
        // Se a carta comprada é igual ao topo do monte do jogador atual e não está na área de descarte
        push_jogador(&jogadores[jogador_atual], carta_comprada);
    } else if (jogador_carta_topo != -1 && !verificar_carta_descarte(area_descarte, carta_comprada, jogadores, jogador_atual)) {
        // Se o número da carta comprada é igual ao número no topo do monte de outro jogador e não está na área de descarte
        // Transferir o monte do jogador correspondente para o monte do jogador atual
        while (!pilha_jogador_vazia(&jogadores[jogador_carta_topo])) {
            Carta carta_transferida = pop_jogador(&jogadores[jogador_carta_topo]);
            push_jogador(&jogadores[jogador_atual], carta_transferida);
        }
        printf("Jogador %d 'roubou' o monte do Jogador %d!\n", jogador_atual + 1, jogador_carta_topo + 1);
        
        // Adicionar a carta comprada ao monte do jogador atual após o 'roubo'
        push_jogador(&jogadores[jogador_atual], carta_comprada);
    } else if (!igual_carta_topo && !verificar_carta_descarte(area_descarte, carta_comprada, jogadores, jogador_atual)) {
        // Se a carta comprada é diferente do topo do monte do jogador atual e não está na área de descarte
        inserir_descarte(area_descarte, carta_comprada);
    }

    // Se a quantidade de cartas do jogador atual aumentar, atualiza a variável quantidade_anterior
    if (quantidade_cartas_jogador(&jogadores[jogador_atual]) > quantidade_atual) {
        *quantidade_anterior = quantidade_atual;
    }
}

//------------------------------------INFORMAÇÕES-----------------------------------------------


void mostrar_informacoes(Pilha *pilha_compra, Lista *area_descarte, PilhaJogador *jogadores, int num_jogadores) {
    // Mostra informações sobre a pilha de compra e o monte de cada jogador
    printf("\nQuantidade de cartas na pilha de compra: %d\n", pilha_compra->topo + 1);
    printf("Topo do monte de cada jogador:\n");
    for (int i = 0; i < num_jogadores; i++) {
        printf("Jogador %d: ", i + 1);
        if (!pilha_jogador_vazia(&jogadores[i])) {
            printf("%d%c | Total de cartas:  %d\n", jogadores[i].cartas[jogadores[i].topo].numero, jogadores[i].cartas[jogadores[i].topo].naipe, quantidade_cartas_jogador(&jogadores[i]));
        } else {
            printf("Pilha vazia\n");
        }
    }

    // Mostra as cartas na área de descarte
    printf("\nÁrea de descarte:\n");
    No *atual = area_descarte->inicio;
    while (atual != NULL) {
        printf("%d%c ", atual->carta.numero, atual->carta.naipe);
        atual = atual->prox;
    }
    printf("\n");
}


void exibir_ranking(PilhaJogador *jogadores, int num_jogadores) {
    // Cria uma matriz para armazenar o número de cartas e o índice do jogador
    int ranking[MAX_JOGADORES][2];

    // Preenche a matriz com o número de cartas e os índices dos jogadores
    for (int i = 0; i < num_jogadores; i++) {
        ranking[i][0] = quantidade_cartas_jogador(&jogadores[i]);
        ranking[i][1] = i;
    }

    // Ordena o ranking com base no número de cartas (Bubble Sort)
    for (int i = 0; i < num_jogadores - 1; i++) {
        for (int j = 0; j < num_jogadores - i - 1; j++) {
            if (ranking[j][0] < ranking[j + 1][0]) {
                // Troca de posição se o número de cartas for maior
                int temp_cartas = ranking[j][0];
                int temp_indice = ranking[j][1];
                ranking[j][0] = ranking[j + 1][0];
                ranking[j][1] = ranking[j + 1][1];
                ranking[j + 1][0] = temp_cartas;
                ranking[j + 1][1] = temp_indice;
            }
        }
    }

    // Exibe o ranking ordenado
    printf("\nRanking de Pontuações:\n");
    for (int i = 0; i < num_jogadores; i++) {
        printf("%dº lugar: Jogador %d com %d cartas\n", i + 1, ranking[i][1] + 1, ranking[i][0]);
    }
}


void mostrar_cartas_jogadores(PilhaJogador *jogadores, int num_jogadores) {
    // Mostra as cartas dos jogadores ordenadas por jogador
    printf("Cartas dos jogadores ordenadas:\n");
    for (int i = 0; i < num_jogadores; i++) {
        printf("Jogador %d: ", i + 1);
        if (!pilha_jogador_vazia(&jogadores[i])) {
            // Ordena as cartas do jogador antes de exibir
            int quantidade_cartas = quantidade_cartas_jogador(&jogadores[i]);
            Carta cartas_ordenadas[MAX_CARTAS_JOGADOR];
            for (int j = 0; j < quantidade_cartas; j++) {
                cartas_ordenadas[j] = jogadores[i].cartas[j];
            }
            // Bubble sort para ordenar as cartas
            for (int j = 0; j < quantidade_cartas - 1; j++) {
                for (int k = 0; k < quantidade_cartas - j - 1; k++) {
                    if (cartas_ordenadas[k].numero > cartas_ordenadas[k + 1].numero) {
                        Carta temp = cartas_ordenadas[k];
                        cartas_ordenadas[k] = cartas_ordenadas[k + 1];
                        cartas_ordenadas[k + 1] = temp;
                    }
                }
            }
            // Exibe as cartas ordenadas
            for (int j = 0; j < quantidade_cartas; j++) {
                printf("%d%c ", cartas_ordenadas[j].numero, cartas_ordenadas[j].naipe);
            }
            printf("\n");
        } else {
            printf("Pilha vazia\n");
        }
    }
}

//------------------------------------MAIN-----------------------------------------------

int main() {
	    // Configura a localização para Português
	    setlocale(LC_ALL, "Portuguese");
	
	    // Inicializa os componentes do jogo
	    Carta baralho[MAX_CARTAS];
	    inicializar_baralho(baralho);
	    Pilha pilha_compra;
	    Lista area_descarte;
	    PilhaJogador jogadores[MAX_JOGADORES];
	    int num_jogadores;
	
	    // Solicita o número de jogadores e verifica se está dentro dos limites
	    printf("Quantos jogadores irão jogar? (Entre 2 e %d): ", MAX_JOGADORES);
	    scanf("%d", &num_jogadores);
	
	    if (num_jogadores < 2 || num_jogadores > MAX_JOGADORES) {
	        printf("Número inválido de jogadores. Saindo do jogo.\n");
	        return 1;
	    }
	
	    // Inicializa os componentes do jogo
	    inicializar_baralho(baralho);
	    inicializar_pilha(&pilha_compra);
	    inicializar_lista(&area_descarte);
	
	   // Inicializa as pilhas dos jogadores
		for (int i = 0; i < num_jogadores; i++) {
		    inicializar_pilha_jogador(&jogadores[i]);
		}
		
		// Pede e valida a quantidade de cartas a ser usada no jogo
		int num_cartas;
		printf("Quantidade de cartas a ser usada no jogo (máximo %d): ", MAX_CARTAS);
		scanf("%d", &num_cartas);
		
		if (num_cartas < 1 || num_cartas > MAX_CARTAS) {
		    printf("Quantidade inválida de cartas. Saindo do jogo.\n");
		    return 1;
		}
		
		// Cria a pilha de compra com as cartas e inicia variáveis do jogo
		criar_pilha_compra(&pilha_compra, baralho, num_cartas);
		
		int jogador_atual = 0;
		int quantidade_anterior = 0;
		
		// Loop principal do jogo
		while (1) {
		    printf("-------------------------------------\n");
		    printf("\nJogador %d, é a sua vez. \nPressione '1' para iniciar a jogada ou qualquer outra tecla para sair: ", jogador_atual + 1);
		
		    char opcao;
		    scanf(" %c", &opcao);
		
		    if (opcao == '1') {
		        // Armazena a quantidade atual de cartas do jogador
		        int quantidade_atual = quantidade_cartas_jogador(&jogadores[jogador_atual]);
		
		        // Realiza a jogada do jogador atual
		        realizar_jogada(&pilha_compra, &area_descarte, jogadores, num_jogadores, jogador_atual, &quantidade_anterior);
		        mostrar_informacoes(&pilha_compra, &area_descarte, jogadores, num_jogadores);
		
		        // Verifica se a quantidade de cartas do jogador mudou
		        if (quantidade_cartas_jogador(&jogadores[jogador_atual]) > quantidade_atual) {
		            quantidade_anterior = quantidade_atual;
		        } else {
		            jogador_atual = (jogador_atual + 1) % num_jogadores;
		        }
		
		        int vitoria = 0;
		        // Verifica se algum jogador venceu ou se a pilha de compra está vazia
		        for (int i = 0; i < num_jogadores; i++) {
		            if (jogadores[i].topo == MAX_CARTAS - 1) {
		                vitoria = 1;
		                printf("\nJogador %d venceu!\n", i + 1);
		                break;
		            }
		        }
		
		        if (vitoria || pilha_vazia(&pilha_compra)) {
		            printf("\nFim de jogo!\n");
		            exibir_ranking(jogadores, num_jogadores);
		            mostrar_cartas_jogadores(jogadores, num_jogadores);
		            break;
		        }
		    } else {
		        printf("Jogo encerrado.\n");
		        break;
		    }
		}
	return 0;
}

