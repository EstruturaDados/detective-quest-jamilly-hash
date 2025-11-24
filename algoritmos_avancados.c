#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//   --Estruturas principais--

// Estrutura de árvore binária para o mapa da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];  // pista opcional
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Nó da árvore BST de pistas
typedef struct NoPista {
    char pista[100];
    struct NoPista* esquerda;
    struct NoPista* direita;
} NoPista;


//        Funções para criar nós

// Cria dinamicamente um cômodo com nome e pista
Sala* criarSala(char* nome, char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);

    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, "");  // sem pista

    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Cria um novo nó na BST de pistas
NoPista* criarNoPista(char* pista) {
    NoPista* novo = (NoPista*) malloc(sizeof(NoPista));
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

//        Inserção na árvore BST de pistas
NoPista* inserirPista(NoPista* raiz, char* pista) {
    if (raiz == NULL)
        return criarNoPista(pista);

    // ordem alfabética
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

//        Exibir pistas em ordem alfabética
void exibirPistas(NoPista* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf(" - %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

//        Exploração da mansão
void explorarSalasComPistas(Sala* atual, NoPista** arvorePistas) {
    char opcao;

    printf("\n=== Detective Quest: Coleta de Pistas ===\n");

    while (1) {

        printf("\nVocê está no(a): %s\n", atual->nome);

        // Coleta automática da pista
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        printf("\nCaminhos disponíveis:\n");
        if (atual->esquerda != NULL)
            printf("  (e) Seguir para a esquerda → %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf("  (d) Seguir para a direita → %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");

        printf("\nEscolha uma opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda != NULL)
            atual = atual->esquerda;
        else if (opcao == 'd' && atual->direita != NULL)
            atual = atual->direita;
        else if (opcao == 's') {
            printf("\nEncerrando exploração...\n");
            break;
        } else {
            printf("\nOpção inválida!\n");
        }
    }
}

//        Liberação de memória
void liberarSalas(Sala* raiz) {
    if (raiz != NULL) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}

void liberarPistas(NoPista* raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

//        Função principal
int main() {
 //   Construção fixa do mapa da mansão
   Sala* hall = criarSala("Hall de Entrada", "Pegada suspeita no tapete");

    hall->esquerda = criarSala("Sala de Estar", "Copo quebrado próximo ao sofá");
    hall->direita  = criarSala("Biblioteca", "Livro fora do lugar");

    hall->esquerda->esquerda = criarSala("Cozinha", "Faca fora do suporte");
    hall->esquerda->direita  = criarSala("Jardim de Inverno", "");
    hall->direita->esquerda  = criarSala("Escritório", "Papel rasgado na mesa");
    hall->direita->direita   = criarSala("Porão Misterioso", "Ruído metálico ecoando");

    // árvore de pistas inicialmente vazia
    NoPista* arvorePistas = NULL;

    // Iniciar exploração
    explorarSalasComPistas(hall, &arvorePistas);

    // Exibir pistas coletadas
    printf("\n=== Pistas coletadas (ordem alfabética) ===\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista coletada.\n");
    else
        exibirPistas(arvorePistas);

    // Liberar memória
    liberarSalas(hall);
    liberarPistas(arvorePistas);

    printf("\nMemória liberada. Fim do programa.\n");
    return 0;
}


