/*
 * persistencia.c — Serialização/deserialização do estado
 *
 * Ao SALVAR:
 *   1. Percorre a fila da frente ao fim e escreve linha a linha.
 *   2. Percorre a pilha do topo à base (salva na ordem mais-recente-primeiro).
 *
 * Ao CARREGAR a fila:
 *   Faz enqueue de cada linha lida (preserva ordem FIFO).
 *
 * Ao CARREGAR o histórico:
 *   A pilha foi salva mais-recente-primeiro.
 *   Para recriar a mesma ordem, precisamos empilhar em ordem INVERSA
 *   (do mais antigo ao mais recente), portanto usamos um array temporário.
 */

#include "persistencia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA   512
#define MAX_ITENS   1024

void carregar_estado(const char *caminho, Fila *fila, Pilha *historico) {
    FILE *arq = fopen(caminho, "r");
    if (!arq) return;   /* primeira execução: arquivo inexistente */

    char linha[MAX_LINHA];
    int  n = 0;

    /* --- Lê seção FILA --- */
    if (fgets(linha, sizeof(linha), arq) == NULL) { fclose(arq); return; }
    if (sscanf(linha, "FILA %d", &n) != 1)        { fclose(arq); return; }

    for (int i = 0; i < n; i++) {
        if (fgets(linha, sizeof(linha), arq) == NULL) break;
        /* Remove '\n' */
        size_t len = strlen(linha);
        if (len > 0 && linha[len - 1] == '\n') linha[len - 1] = '\0';
        fila_enqueue(fila, linha);
    }

    /* --- Lê seção HISTORICO --- */
    if (fgets(linha, sizeof(linha), arq) == NULL) { fclose(arq); return; }
    if (sscanf(linha, "HISTORICO %d", &n) != 1)   { fclose(arq); return; }

    /* Array temporário para inverter a ordem antes de empilhar */
    char **temp = (char **)malloc(n * sizeof(char *));
    if (!temp) { fclose(arq); return; }

    int lidos = 0;
    for (int i = 0; i < n; i++) {
        if (fgets(linha, sizeof(linha), arq) == NULL) break;
        size_t len = strlen(linha);
        if (len > 0 && linha[len - 1] == '\n') linha[len - 1] = '\0';

        temp[lidos] = (char *)malloc(strlen(linha) + 1);
        if (!temp[lidos]) break;
        strcpy(temp[lidos], linha);
        lidos++;
    }

    /* Empilha do mais antigo ao mais recente para reconstruir a pilha correta */
    for (int i = lidos - 1; i >= 0; i--) {
        pilha_push(historico, temp[i]);
        free(temp[i]);
    }
    free(temp);

    fclose(arq);
}

void salvar_estado(const char *caminho, const Fila *fila, const Pilha *historico) {
    FILE *arq = fopen(caminho, "w");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir arquivo para escrita: %s\n", caminho);
        return;
    }

    /* --- Serializa fila --- */
    fprintf(arq, "FILA %d\n", fila_tamanho(fila));
    NoFila *nf = fila->frente;
    while (nf) {
        fprintf(arq, "%s\n", nf->descricao);
        nf = nf->prox;
    }

    /* --- Serializa pilha (topo ao fundo = mais recente primeiro) --- */
    fprintf(arq, "HISTORICO %d\n", pilha_tamanho(historico));
    NoPilha *np = historico->topo;
    while (np) {
        fprintf(arq, "%s\n", np->descricao);
        np = np->abaixo;
    }

    fclose(arq);
    printf("Dados salvos em '%s'.\n", caminho);
}
