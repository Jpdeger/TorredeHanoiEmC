#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_DISCOS 10
#define MAX_RECORDES 5

typedef struct {
    int topo;
    int discos[MAX_DISCOS];
} Pilha;

void inicializar_pilha(Pilha *p) {
    p->topo = -1;
}

int pilha_vazia(Pilha *p) {
    return p->topo == -1;
}

int pilha_cheia(Pilha *p) {
    return p->topo == MAX_DISCOS - 1;
}

void push(Pilha *p, int disco) {
    if (!pilha_cheia(p)) {
        p->discos[++(p->topo)] = disco;
    } else {
        printf("Movimento inválido: A pilha está cheia.\n");
    }
}

int pop(Pilha *p) {
    if (!pilha_vazia(p)) {
        return p->discos[(p->topo)--];
    }
    return -1;
}

void mover_disco(Pilha *origem, Pilha *destino, char origemT, char destinoT) {
    int disco = pop(origem);
    if (disco != -1) {
        if (!pilha_cheia(destino) && (destino->topo == -1 || destino->discos[destino->topo] > disco)) {
            push(destino, disco);
            printf("Mover disco %d de %c para %c\n", disco, origemT, destinoT);
        } else {
            printf("Movimento inválido: Não é permitido colocar um disco maior sobre um disco menor.\n");
            push(origem, disco); 
        }
    } else {
        printf("Movimento inválido!\n");
    }
}

void exibir_pilhas(Pilha *A, Pilha *B, Pilha *C) {
    printf("A: ");
    for (int i = 0; i <= A->topo; i++) {
        printf("%d ", A->discos[i]);
    }
    printf("\nB: ");
    for (int i = 0; i <= B->topo; i++) {
        printf("%d ", B->discos[i]);
    }
    printf("\nC: ");
    for (int i = 0; i <= C->topo; i++) {
        printf("%d ", C->discos[i]);
    }
    printf("\n");
}

void mover_discos_automatico(int discos, Pilha *origem, Pilha *destino, Pilha *auxiliar, char origemT, char destinoT, char auxT) {
    if (discos == 1) {
        mover_disco(origem, destino, origemT, destinoT);
        return;
    }
    mover_discos_automatico(discos - 1, origem, auxiliar, destino, origemT, auxT, destinoT);
    mover_disco(origem, destino, origemT, destinoT);
    mover_discos_automatico(discos - 1, auxiliar, destino, origem, auxT, destinoT, origemT);
}

void reiniciar_jogo(Pilha *A, Pilha *B, Pilha *C, int num_discos) {
    inicializar_pilha(A);
    inicializar_pilha(B);
    inicializar_pilha(C);
    for (int i = num_discos; i >= 1; i--) {
        push(A, i);
    }
}

void listar_melhores_tempos(double melhores_tempos[], int num_tempos) {
    printf("Melhores tempos:\n");
    for (int i = 0; i < num_tempos; i++) {
        printf("%d. %.2f segundos\n", i + 1, melhores_tempos[i]);
    }
}

void atualizar_melhores_tempos(double melhores_tempos[], int *num_tempos, double novo_tempo) {
    if (*num_tempos < MAX_RECORDES) {
        melhores_tempos[*num_tempos] = novo_tempo;
        (*num_tempos)++;
    } else {
        for (int i = 0; i < MAX_RECORDES; i++) {
            if (novo_tempo < melhores_tempos[i]) {
                for (int j = MAX_RECORDES - 1; j > i; j--) {
                    melhores_tempos[j] = melhores_tempos[j - 1];
                }
                melhores_tempos[i] = novo_tempo;
                break;
            }
        }
    }
    // Ordena os tempos
    for (int i = 0; i < *num_tempos - 1; i++) {
        for (int j = i + 1; j < *num_tempos; j++) {
            if (melhores_tempos[i] > melhores_tempos[j]) {
                double temp = melhores_tempos[i];
                melhores_tempos[i] = melhores_tempos[j];
                melhores_tempos[j] = temp;
            }
        }
    }
}

void iniciar_jogo_manual(Pilha *A, Pilha *B, Pilha *C, int num_discos, double melhores_tempos[], int *num_tempos) {
    int opcao;
    clock_t inicio = clock();
    
    while (1) {
        exibir_pilhas(A, B, C);
        printf("Digite o próximo movimento:\n");
        printf("1 - Mover disco do pino A para o pino B\n");
        printf("2 - Mover disco do pino A para o pino C\n");
        printf("3 - Mover disco do pino B para o pino A\n");
        printf("4 - Mover disco do pino B para o pino C\n");
        printf("5 - Mover disco do pino C para o pino A\n");
        printf("6 - Mover disco do pino C para o pino B\n");
        printf("7 - Reiniciar jogo\n");
        printf("8 - Sair\n");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1: mover_disco(A, B, 'A', 'B'); break;
            case 2: mover_disco(A, C, 'A', 'C'); break;
            case 3: mover_disco(B, A, 'B', 'A'); break;
            case 4: mover_disco(B, C, 'B', 'C'); break;
            case 5: mover_disco(C, A, 'C', 'A'); break;
            case 6: mover_disco(C, B, 'C', 'B'); break;
            case 7:
                reiniciar_jogo(A, B, C, num_discos);
                inicio = clock();
                break;
            case 8:
                printf("Saindo...\n");
                return;
            default: printf("Opção inválida!\n");
        }
        
        if (C->topo == num_discos - 1) {
            printf("Parabéns! Você completou o jogo!\n");
            clock_t fim = clock();
            double tempo_jogo = (double)(fim - inicio) / CLOCKS_PER_SEC;
            printf("Tempo: %.2f segundos\n", tempo_jogo);
            atualizar_melhores_tempos(melhores_tempos, num_tempos, tempo_jogo);
            return;
        }
    }
}

int main() {
    Pilha A, B, C;
    int num_discos;
    double melhores_tempos[MAX_RECORDES] = {0};
    int num_tempos = 0;
    int opcao_menu;

    printf("Digite o número de discos: ");
    scanf("%d", &num_discos);

    reiniciar_jogo(&A, &B, &C, num_discos);

    while (1) {
        printf("Menu:\n");
        printf("1 - Jogar no modo manual\n");
        printf("2 - Jogar no modo automático\n");
        printf("3 - Listar os melhores tempos (recordes)\n");
        printf("4 - Alterar número de discos\n");
        printf("5 - Sair do programa\n");
        scanf("%d", &opcao_menu);

        switch (opcao_menu) {
            case 1:
                reiniciar_jogo(&A, &B, &C, num_discos);
                iniciar_jogo_manual(&A, &B, &C, num_discos, melhores_tempos, &num_tempos);
                break;
            case 2:
                reiniciar_jogo(&A, &B, &C, num_discos);
                mover_discos_automatico(num_discos, &A, &C, &B, 'A', 'C', 'B');
                break;
            case 3:
                listar_melhores_tempos(melhores_tempos, num_tempos);
                break;
            case 4:
                printf("Digite o novo número de discos: ");
                scanf("%d", &num_discos);
                reiniciar_jogo(&A, &B, &C, num_discos);
                break;
            case 5:
                printf("Saindo...\n");
                return 0;
            default:
                printf("Opção inválida!\n");
        }
    }
}