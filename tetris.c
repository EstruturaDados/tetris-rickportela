#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definindo a capacidade máxima da fila de peças futuras.
#define TAMANHO_FILA 5 

// Variável global para rastrear o ID único das peças geradas.
int proximo_id_peca = 0; 

/**
 * @brief Struct que representa uma peça do Tetris.
 * Atributos: nome (tipo da peça) e id (identificador único).
 */
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L', etc.)
    int id;     // Identificador único da peça
} Peca;

// O vetor (array) que implementa a fila de peças.
Peca fila_pecas[TAMANHO_FILA];

// Variáveis de controle da Fila Circular.
// 'frente' aponta para o elemento a ser removido (dequeue).
// 'tras' aponta para o próximo local vago para inserção (enqueue).
int frente = -1; 
int tras = -1; 
int num_elementos = 0; // Conta quantos elementos estão atualmente na fila.


// --- Protótipos das Funções ---
Peca gerarPeca();
void exibirFila();
int filaVazia();
int filaCheia();
void enfileirar(Peca nova_peca); // Inserir
Peca desenfileirar(); // Jogar (Remover)
void exibirMenu();


// ====================================================================
// GERAÇÃO DE PEÇAS E UTILS DA FILA
// ====================================================================

/**
 * @brief Gera uma nova peça com tipo aleatório e um ID único.
 * * Atende ao requisito: Peças são geradas automaticamente.
 * @return Peca A nova peça gerada.
 */
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'}; // Os 7 tipos de peças de Tetris
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);
    
    // Escolhe um tipo de peça aleatoriamente
    int indice_aleatorio = rand() % num_tipos; 
    nova.nome = tipos[indice_aleatorio];
    
    // Atribui o ID único e incrementa o contador global
    nova.id = proximo_id_peca;
    proximo_id_peca++; 

    return nova;
}

/**
 * @brief Verifica se a fila está vazia.
 * @return int 1 se vazia, 0 caso contrário.
 */
int filaVazia() {
    // A fila está vazia se não houver elementos.
    return (num_elementos == 0); 
}

/**
 * @brief Verifica se a fila está cheia.
 * @return int 1 se cheia, 0 caso contrário.
 */
int filaCheia() {
    // A fila está cheia se o número de elementos for igual ao seu tamanho máximo.
    return (num_elementos == TAMANHO_FILA);
}

// ====================================================================
// MANIPULAÇÃO DA FILA (ENQUEUE E DEQUEUE)
// ====================================================================

/**
 * @brief Insere uma peça ao final da fila (enqueue).
 * * Atende ao requisito: Inserir nova peça ao final da fila, se houver espaço.
 * @param nova_peca A peça a ser inserida.
 */
void enfileirar(Peca nova_peca) {
    if (filaCheia()) {
        printf("\n Fila Cheia! Não é possível inserir mais peças.\n");
        return;
    }
    
    // Se a fila estava vazia, inicializa 'frente' e 'tras' para 0.
    if (filaVazia()) {
        frente = 0;
        tras = 0;
    } else {
        // Para a fila circular: 'tras' avança e volta para 0 se atingir o limite.
        tras = (tras + 1) % TAMANHO_FILA; 
    }
    
    // Insere a peça na nova posição 'tras'.
    fila_pecas[tras] = nova_peca;
    num_elementos++;
    
    printf("\n Peça [%c %d] enfileirada com sucesso (na posição %d).\n", 
           nova_peca.nome, nova_peca.id, (tras + 1));
}

/**
 * @brief Remove e retorna a peça da frente da fila (dequeue).
 * * Atende ao requisito: Jogar uma peça (remove a peça da frente da fila).
 * @return Peca A peça removida.
 */
Peca desenfileirar() {
    Peca peca_removida = {'\0', -1}; // Struct de erro padrão
    
    if (filaVazia()) {
        printf("\n Fila VAZIA! Nenhuma peça para jogar.\n");
        return peca_removida; 
    }
    
    // Armazena a peça que está na frente para retorná-la.
    peca_removida = fila_pecas[frente];
    
    // 'frente' avança, seguindo a lógica circular.
    frente = (frente + 1) % TAMANHO_FILA;
    num_elementos--;

    // Se a fila ficou vazia após a remoção, reseta os ponteiros.
    if (filaVazia()) {
        frente = -1;
        tras = -1;
    }
    
    printf("\n Peça JOGADA (removida): Tipo [%c], ID [%d].\n", peca_removida.nome, peca_removida.id);
    return peca_removida;
}

// ====================================================================
// INTERFACE E EXIBIÇÃO
// ====================================================================

/**
 * @brief Exibe o estado atual da fila, mostrando o tipo e ID de cada peça.
 * * Atende ao requisito: Exibir o estado atual da fila após cada ação.
 */
void exibirFila() {
    printf("\n======================================================\n");
    printf("         FILA DE PEÇAS FUTURAS (Next Queue)         \n");
    printf("======================================================\n");

    if (filaVazia()) {
        printf("A fila está VAZIA. Gere uma nova peça para começar!\n");
        printf("------------------------------------------------------\n");
        return;
    }

    // Exibe o cabeçalho e os ponteiros
    printf("Capacidade: %d | Ocupação Atual: %d\n", TAMANHO_FILA, num_elementos);
    printf("Frente (Dequeue): %d | Trás (Enqueue): %d\n", frente, tras);
    
    printf("\nFila de Peças: ");
    
    // Percorrer a fila circularmente, começando pela 'frente'.
    // O loop continua enquanto 'i' for diferente de (tras + 1)
    int i = frente;
    int count = 0;
    
    // Usa 'count' para garantir que percorre apenas o número de elementos, 
    // mesmo que 'i' circule.
    while (count < num_elementos) {
        printf("[%c %d]", fila_pecas[i].nome, fila_pecas[i].id);
        
        // Se não for o último elemento exibido, adiciona uma seta.
        if (count < num_elementos - 1) {
            printf(" -> ");
        }

        i = (i + 1) % TAMANHO_FILA;
        count++;
    }
    
    printf("\n------------------------------------------------------\n");
}

/**
 * @brief Exibe o menu de opções para o jogador.
 * * Atende ao requisito não funcional de Usabilidade.
 */
void exibirMenu() {
    printf("\n--- OPÇÕES DE AÇÃO ---\n");
    printf("Código | Ação\n");
    printf("---------------------------\n");
    printf("  1    | Jogar peça (dequeue)\n");
    printf("  2    | Inserir nova peça (enqueue)\n");
    printf("  0    | Sair\n");
    printf("---------------------------\n");
    printf("Escolha uma opção: ");
}

// ====================================================================
// FUNÇÃO PRINCIPAL
// ====================================================================

/**
 * @brief Inicializa a fila e controla o fluxo de execução do programa.
 */
int main() {
    int opcao;
    
    // Inicialização do gerador de números aleatórios
    srand(time(NULL)); 

    printf("==================================================\n");
    printf("    Tetris Stack - Controle da Fila de Peças   \n");
    printf("==================================================\n");
    
    // REQUISITO: Inicializar a fila de peças com um número fixo de elementos (5).
    printf("\n Inicializando fila com %d peças iniciais...\n", TAMANHO_FILA);
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(gerarPeca());
    }
    
    // Exibe o estado inicial da fila
    exibirFila();
    
    // Loop principal do programa
    do {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            // Tratamento de entrada inválida (não numérica)
            printf("\n Entrada inválida. Por favor, digite um número.\n");
            while(getchar() != '\n'); // Limpa o buffer
            opcao = -1; 
            continue;
        }

        switch (opcao) {
            case 1: // Jogar peça (Dequeue)
                desenfileirar();
                break;
                
            case 2: { // Inserir nova peça (Enqueue)
                Peca nova = gerarPeca();
                enfileirar(nova);
                break;
            }
                
            case 0:
                printf("\n Saindo do Tetris Stack. Peças remanescentes:\n");
                break;
                
            default:
                printf("\n Opção inválida! Escolha 1, 2 ou 0.\n");
                break;
        }
        
        // REQUISITO: Exibir o estado atual da fila após CADA ação (exceto ao sair)
        if (opcao != 0) {
            exibirFila();
        }

    } while (opcao != 0);

    return 0;
}