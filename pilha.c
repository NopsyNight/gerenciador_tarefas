/*
 * pilha.c — Implementação da pilha encadeada (LIFO)
 *
 * Visualização interna:
 *
 *  [topo] → [nó3] → [nó2] → [nó1] → NULL
 *
 *  Push: cria nó, nó->abaixo = topo, topo = nó
 *  Pop:  salva topo, topo = topo->abaixo, libera nó salvo
 */

#include "pilha.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static NoPilha *no_criar(const char *descricao) {
    NoPilha *no = (NoPilha *)malloc(sizeof(NoPilha));
    if (!no) return NULL;

    no->descricao = (char *)malloc(strlen(descricao) + 1);
    if (!no->descricao) {
        free(no);
        return NULL;
    }

    strcpy(no->descricao, descricao);
    no->abaixo = NULL;
    return no;
}

static void no_destruir(NoPilha *no) {
    if (no) {
        free(no->descricao);
        free(no);
    }
}

Pilha *pilha_criar(void) {
    Pilha *p = (Pilha *)malloc(sizeof(Pilha));
    if (!p) return NULL;
    p->topo     = NULL;
    p->tamanho  = 0;
    return p;
}

int pilha_vazia(const Pilha *p) {
    return p->topo == NULL;
}

int pilha_tamanho(const Pilha *p) {
    return p->tamanho;
}

int pilha_push(Pilha *p, const char *descricao) {
    NoPilha *no = no_criar(descricao);
    if (!no) return 0;

    no->abaixo = p->topo;
    p->topo    = no;
    p->tamanho++;
    return 1;
}

int pilha_pop(Pilha *p, char *buf, size_t buf_len) {
    if (pilha_vazia(p)) return 0;

    NoPilha *no = p->topo;
    strncpy(buf, no->descricao, buf_len - 1);
    buf[buf_len - 1] = '\0';

    p->topo = no->abaixo;
    no_destruir(no);
    p->tamanho--;
    return 1;
}

int pilha_peek(const Pilha *p, char *buf, size_t buf_len) {
    if (pilha_vazia(p)) return 0;
    strncpy(buf, p->topo->descricao, buf_len - 1);
    buf[buf_len - 1] = '\0';
    return 1;
}

void pilha_imprimir(const Pilha *p) {
    int i = 1;
    NoPilha *atual = p->topo;
    while (atual) {
        printf("  %2d. %s\n", i++, atual->descricao);
        atual = atual->abaixo;
    }
}

void pilha_destruir(Pilha *p) {
    if (!p) return;
    NoPilha *atual = p->topo;
    while (atual) {
        NoPilha *abaixo = atual->abaixo;
        no_destruir(atual);
        atual = abaixo;
    }
    free(p);
}
