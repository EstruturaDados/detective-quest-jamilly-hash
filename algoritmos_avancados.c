#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_HASH 7   // tamanho da tabela hash (pequeno para forçar colisões)

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

// Estrutura de um suspeito
typedef struct Suspeito {
    char nome[40];
    int idade;
    int nivel;  // nível de suspeição (1–5)
    int ocorrencias; // quantas pistas o associam
    struct Suspeito* prox; // para tratamento de colisão
} Suspeito;


//        Funções para criar nós

Sala* criarSala(char* nome, char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista ? pista : "");
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

NoPista* criarNoPista(char* pista) {
    NoPista* novo = (NoPista*) malloc(sizeof(NoPista));
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

Suspeito* criarSuspeito(char* nome, int idade, int nivel) {
    Suspeito* s = (Suspeito*) malloc(sizeof(Suspeito));
    strcpy(s->nome, nome);
    s->idade = idade;
    s->nivel = nivel;
    s->ocorrencias = 1;
    s->prox = NULL;
    return s;
}

//        Inserção na árvore BST de pistas
NoPista* inserirPista(NoPista* raiz, char* pista) {
    if (!raiz) return criarNoPista(pista);

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


//                TABELA HASH DE SUSPEITOS
// hash baseado na soma ASCII do nome
int hash(char* nome) {
    int soma = 0;
    for (int i = 0; nome[i]; i++)
        soma += nome[i];
    return soma % TAM_HASH;
}

// insere ou incrementa suspeito
void inserirSuspeito(Suspeito* tabela[], char* nome, int idade, int nivel) {
    int h = hash(nome);
    Suspeito* atual = tabela[h];

    // já existe → só incrementa ocorrência
    while (atual) {
        if (strcmp(atual->nome, nome) == 0) {
            atual->ocorrencias++;
            return;
        }
        atual = atual->prox;
    }

    // novo suspeito
    Suspeito* novo = criarSuspeito(nome, idade, nivel);
    novo->prox = tabela[h];
    tabela[h] = novo;
}

// mostra tabela hash completa
void listarSuspeitos(Suspeito* tabela[]) {
    printf("\n===== SUSPEITOS IDENTIFICADOS =====\n");

    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito* s = tabela[i];
        if (!s) continue;

        printf("Bucket %d:\n", i);
        while (s) {
            printf("  - %s (Idade: %d, Nível: %d, Evidências: %d)\n",
                   s->nome, s->idade, s->nivel, s->ocorrencias);
            s = s->prox;
        }
    }
}

// encontra suspeito por nome
Suspeito* buscarSuspeito(Suspeito* tabela[], char* nome) {
    int h = hash(nome);
    Suspeito* s = tabela[h];

    while (s) {
        if (strcmp(s->nome, nome) == 0)
            return s;
        s = s->prox;
    }
    return NULL;
}

//    ASSOCIAÇÃO DE PISTAS → SUSPEITOS (Regras)
void analisarPistaEAssociar(char* pista, Suspeito* tabela[]) {

    char p[200];
    strcpy(p, pista);

    // Convertemos tudo para minúsculo
    for (int i = 0; p[i]; i++)
        p[i] = tolower(p[i]);

    // regras de associação
    if (strstr(p, "pegada") || strstr(p, "chão") || strstr(p, "passos"))
        inserirSuspeito(tabela, "João", 34, 3);

    else if (strstr(p, "copo") || strstr(p, "faca") || strstr(p, "cozinha"))
        inserirSuspeito(tabela, "Maria", 29, 4);

    else if (strstr(p, "papel") || strstr(p, "escritório") || strstr(p, "documento"))
        inserirSuspeito(tabela, "Carlos", 41, 2);

    else if (strstr(p, "metal") || strstr(p, "porão") || strstr(p, "som"))
        inserirSuspeito(tabela, "Ana", 25, 5);
}



//        Exploração da mansão
void explorar(Sala* atual, NoPista** arvorePistas, Suspeito* tabela[]) {
    char opcao;

    while (1) {
        printf("\nVocê está no(a): %s\n", atual->nome);

        // coleta da pista da sala
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);

            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
            analisarPistaEAssociar(atual->pista, tabela);
        }

        printf("\nCaminhos disponíveis:\n");
        if (atual->esquerda) printf(" (e) → %s\n", atual->esquerda->nome);
        if (atual->direita)  printf(" (d) → %s\n", atual->direita->nome);
        printf(" (s) Sair da exploração\n");

        printf("Escolha: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda)
            atual = atual->esquerda;

        else if (opcao == 'd' && atual->direita)
            atual = atual->direita;

        else if (opcao == 's')
            return;

        else
            printf("Opção inválida.\n");
    }
}


//        Função principal
int main() {
  // tabela hash inicial
    Suspeito* tabela[TAM_HASH] = {0};

    // árvore de pistas
    NoPista* arvorePistas = NULL;

    // mapa da mansão
    Sala* hall = criarSala("Hall de Entrada", "Pegada suspeita no tapete");
    hall->esquerda = criarSala("Sala de Estar", "Copo quebrado próximo ao sofá");
    hall->direita  = criarSala("Biblioteca", "Livro fora do lugar");
    hall->esquerda->esquerda = criarSala("Cozinha", "Faca fora do suporte");
    hall->esquerda->direita  = criarSala("Jardim de Inverno", "");
    hall->direita->esquerda  = criarSala("Escritório", "Papel rasgado na mesa");
    hall->direita->direita   = criarSala("Porão Misterioso", "Ruído metálico ecoando");

    printf("\n=== DETECTIVE QUEST – NÍVEL MESTRE ===\n");
    explorar(hall, &arvorePistas, tabela);

    // resultados
    printf("\n===== PISTAS COLETADAS (ordem alfabética) =====\n");
    if (!arvorePistas) printf("Nenhuma pista coletada.\n");
    else exibirPistas(arvorePistas);

    listarSuspeitos(tabela);

    // jogador acusa
    char acusado[40];
    printf("\nQuem você acha que é o culpado? ");
    scanf(" %[^\n]s", acusado);

    Suspeito* s = buscarSuspeito(tabela, acusado);

    printf("\n===== RESULTADO DA ACUSAÇÃO =====\n");

    if (!s) {
        printf("Nenhuma pista aponta para %s. Acusação rejeitada.\n", acusado);
    }
    else if (s->ocorrencias >= 2 && s->nivel >= 4) {
        printf("ACUSAÇÃO ACEITA! Todas as evidências apontam para %s.\n", acusado);
    } else {
        printf("%s possui poucas evidências. Acusação não é convincente.\n", acusado);
    }

    printf("\nFim da investigação.\n");
    return 0;
}