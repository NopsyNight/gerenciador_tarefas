/*MENU GERENCIADOR DE TAREFAS*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"
#include "pilha.h"
#include "persistencia.h"

#define ARQUIVO_DADOS "tarefas.dat"


void limpar_tela(void) {
    printf("\033[H\033[J");
}

void exibir_menu(void) {
    printf("=== Sistema de Gerenciamento de Tarefas ===\n");
    printf("1 - Adicionar tarefa \n");
    printf("2 - Listar tarefas pendentes \n");
    printf("3 - Concluir próxima tarefa \n");
    printf("4 - Desfazer última tarefa concluída \n");
    printf("5 - Listar histórico de tarefas concluídas \n");
    printf("6 - Sair \n");
    printf("Escolha: ");
}

void adicionar_tarefa(Fila *fila) {
    char descricao[256];
    printf("\nDescrição da tarefa: ");
    fflush(stdout); /* */
    if (fgets(descricao, sizeof(descricao), stdin) == NULL) return;

    /* Remove o '\n' do final */
    size_t len = strlen(descricao);
    if (len > 0 && descricao[len - 1] == '\n')
        descricao[len - 1] = '\0';

    if (strlen(descricao) == 0) {
        printf("Descrição não pode estar vazia.\n");
        return;
    }

    if (fila_enqueue(fila, descricao)) {
        printf("Tarefa adicionada com sucesso!\n");
    } else {
        printf("Erro ao adicionar tarefa.\n");
    }
}

void listar_tarefas(Fila *fila) {
    printf("\n--- Tarefas Pendentes ---\n");
    if (fila_vazia(fila)) {
        printf("Nenhuma tarefa pendente.\n");
        return;
    }
    fila_imprimir(fila);
}

void concluir_tarefa(Fila *fila, Pilha *historico) {
    if (fila_vazia(fila)) {
        printf("\nNenhuma tarefa para concluir.\n");
        return;
    }

    char descricao[256];
    if (!fila_dequeue(fila, descricao, sizeof(descricao))) {
        printf("\nErro ao concluir tarefa.\n");
        return;
    }

    /* Empilha no histórico para possiblidade de desfazer */
    if (pilha_push(historico, descricao)) {
        printf("\nTarefa concluída: \"%s\"\n", descricao);
        printf("  (Use opção 4 para desfazer)\n");
    } else {
        printf("\nTarefa concluída, mas histórico cheio (não será possível desfazer).\n");
    }
}

void desfazer_conclusao(Fila *fila, Pilha *historico) {
    if (pilha_vazia(historico)) {
        printf("\nNada para desfazer.\n");
        return;
    }

    char descricao[256];
    if (!pilha_pop(historico, descricao, sizeof(descricao))) {
        printf("\nErro ao desfazer.\n");
        return;
    }
    if (fila_enqueue_frente(fila, descricao)) {
        printf("\nDesfeito! Tarefa devolvida: \"%s\"\n", descricao);
    } else {
        printf("\nErro ao reinserir tarefa na fila.\n");
    }
}

void listar_historico(Pilha *historico) {
    printf("\n--- Tarefas Concluídas (mais recente primeiro) ---\n");
    if (pilha_vazia(historico)) {
        printf("Nenhuma tarefa concluída ainda.\n");
        return;
    }
    pilha_imprimir(historico);
}

int main(void) {
    Fila    *fila      = fila_criar();
    Pilha   *historico = pilha_criar();
 
    if (!fila || !historico) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        return EXIT_FAILURE;
    }
 
    /* Carrega estado salvo do disco */
    carregar_estado(ARQUIVO_DADOS, fila, historico);
 
    int opcao;
    char buf[16];
 
    do {
        limpar_tela();
        exibir_menu();
 
        if (fgets(buf, sizeof(buf), stdin) == NULL) break;
        opcao = atoi(buf);
 
        limpar_tela();
 
        switch (opcao) {
            case 1: adicionar_tarefa(fila);              break;
            case 2: listar_tarefas(fila);                break;
            case 3: concluir_tarefa(fila, historico);    break;
            case 4: desfazer_conclusao(fila, historico); break;
            case 5: listar_historico(historico);          break;
            case 6:
                printf("Salvando dados e saindo...\n");
                salvar_estado(ARQUIVO_DADOS, fila, historico);
                break;
            default:
                printf("Opção inválida.\n");
        }
 
        if (opcao != 6) {
            printf("\nPressione ENTER para continuar...");
            fflush(stdout);
            while (getchar() != '\n');
        }
 
    } while (opcao != 6);
 
    fila_destruir(fila);
    pilha_destruir(historico);
 
    printf("Até logo!\n");
    return EXIT_SUCCESS;
}