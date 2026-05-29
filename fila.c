

#include "fila.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- Funções auxiliares internas ---- */

static NoFila *no_criar(const char *descricao) {
    NoFila *no = (NoFila *)malloc(sizeof(NoFila));
    if (!no) return NULL;

    no->descricao = (char *)malloc(strlen(descricao) + 1);
    if (!no->descricao) {
        free(no);
        return NULL;
    }

    strcpy(no->descricao, descricao);
    no->prox = NULL;
    return no;
}

static void no_destruir(NoFila *no) {
    if (no) {
        free(no->descricao);
        free(no);
    }
}

/* ---- Interface pública ---- */

Fila *fila_criar(void) {
    Fila *f = (Fila *)malloc(sizeof(Fila));
    if (!f) return NULL;
    f->frente  = NULL;
    f->fim     = NULL;
    f->tamanho = 0;
    return f;
}

int fila_vazia(const Fila *f) {
    return f->frente == NULL;
}

int fila_tamanho(const Fila *f) {
    return f->tamanho;
}

int fila_enqueue(Fila *f, const char *descricao) {
    NoFila *no = no_criar(descricao);
    if (!no) return 0;

    if (fila_vazia(f)) {
        f->frente = no;
        f->fim    = no;
    } else {
        f->fim->prox = no;
        f->fim       = no;
    }
    f->tamanho++;
    return 1;
}

int fila_enqueue_frente(Fila *f, const char *descricao) {
    NoFila *no = no_criar(descricao);
    if (!no) return 0;

    if (fila_vazia(f)) {
        f->frente = no;
        f->fim    = no;
    } else {
        no->prox  = f->frente;
        f->frente = no;
    }
    f->tamanho++;
    return 1;
}

int fila_dequeue(Fila *f, char *buf, size_t buf_len) {
    if (fila_vazia(f)) return 0;

    NoFila *no = f->frente;
    strncpy(buf, no->descricao, buf_len - 1);
    buf[buf_len - 1] = '\0';

    f->frente = no->prox;
    if (f->frente == NULL)
        f->fim = NULL;   /* fila ficou vazia */

    no_destruir(no);
    f->tamanho--;
    return 1;
}

void fila_imprimir(const Fila *f) {
    int i = 1;
    NoFila *atual = f->frente;
    while (atual) {
        printf("  %2d. %s\n", i++, atual->descricao);
        atual = atual->prox;
    }
}

void fila_destruir(Fila *f) {
    if (!f) return;
    NoFila *atual = f->frente;
    while (atual) {
        NoFila *prox = atual->prox;
        no_destruir(atual);
        atual = prox;
    }
    free(f);
}
