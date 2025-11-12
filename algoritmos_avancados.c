#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
// --------------------------------------------------
// Estrutura básica de árvore binária para representar o mapa da mansão.
// O jogador pode explorar as salas escolhendo ir à esquerda (e) ou à direita (d).

// Estrutura da sala (nó da árvore binária)
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// --------------------------------------------------
// Função: criarSala()
// Cria dinamicamente uma nova sala com o nome informado.
// --------------------------------------------------
Sala* criarSala(char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// --------------------------------------------------
// Função: explorarSalas()
// Permite que o jogador navegue pela mansão interativamente.
// --------------------------------------------------
void explorarSalas(Sala* atual) {
    char opcao;

    printf("=== Detective Quest: A Mansão Misteriosa ===\n\n");
    printf("Você está no(a): %s\n", atual->nome);

    while (1) {
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nVocê chegou ao fim da exploração. Nenhum caminho restante!\n");
            break;
        }

        printf("\nCaminhos disponíveis:\n");
        if (atual->esquerda != NULL)
            printf("  (e) Ir para a esquerda → %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf("  (d) Ir para a direita → %s\n", atual->direita->nome);
        printf("  (s) Sair do jogo\n");

        printf("\nEscolha uma direção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
            printf("\nVocê entrou no(a): %s\n", atual->nome);
        } else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
            printf("\nVocê entrou no(a): %s\n", atual->nome);
        } else if (opcao == 's') {
            printf("\nVocê decidiu encerrar a exploração.\n");
            break;
        } else {
            printf("\nOpção inválida! Tente novamente.\n");
        }
    }
}

// --------------------------------------------------
// Função: liberar()
// Libera toda a memória alocada para a árvore binária.
// --------------------------------------------------
void liberar(Sala* raiz) {
    if (raiz != NULL) {
        liberar(raiz->esquerda);
        liberar(raiz->direita);
        free(raiz);
    }
}

// --------------------------------------------------
// Função principal: main()
// Monta o mapa fixo da mansão e inicia a exploração.
// --------------------------------------------------
int main() {

    // Criação da árvore binária (mapa fixo)
    Sala* hall = criarSala("Hall de Entrada");
    hall->esquerda = criarSala("Sala de Estar");
    hall->direita = criarSala("Biblioteca");

    hall->esquerda->esquerda = criarSala("Cozinha");
    hall->esquerda->direita = criarSala("Jardim de Inverno");
    hall->direita->esquerda = criarSala("Escritório");
    hall->direita->direita = criarSala("Porão Misterioso");

    // Início da exploração interativa
    explorarSalas(hall);

    // Liberação da memória
    liberar(hall);

    printf("\nMemória liberada. Fim do programa.\n");
    return 0;
}

