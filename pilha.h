#ifndef PILHA_H
#define PILHA_H

#include <stddef.h>

typedef struct NoPilha {
    char           *descricao;
    struct NoPilha *abaixo;   /* aponta para o elemento anterior */
} NoPilha;

typedef struct {
    NoPilha *topo;
    int      tamanho;
} Pilha;

Pilha *pilha_criar(void);

/* Retorna 1 se a pilha estiver vazia. */
int    pilha_vazia(const Pilha *p);

int    pilha_tamanho(const Pilha *p);

/* Empilha um elemento. Retorna 1 em sucesso, 0 em falha. */
int    pilha_push(Pilha *p, const char *descricao);

int    pilha_pop(Pilha *p, char *buf, size_t buf_len);

int    pilha_peek(const Pilha *p, char *buf, size_t buf_len);

/* Imprime do topo à base (mais recente primeiro). */
void   pilha_imprimir(const Pilha *p);

/* Libera toda a memória. */
void   pilha_destruir(Pilha *p);

#endif /* PILHA_H */
