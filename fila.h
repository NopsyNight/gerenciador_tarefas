/*
 * fila.h — Fila encadeada (FIFO)
 *
 * Cada nó guarda uma string (descrição da tarefa).
 * A fila mantém ponteiros para a cabeça (frente) e para a cauda (fim).
 * Enqueue na cauda: O(1)
 * Dequeue na cabeça: O(1)
 * Enqueue_frente (para desfazer): O(1)
 */

#ifndef FILA_H
#define FILA_H

#include <stddef.h>

/* Nó interno da fila — não exposto ao usuário */
typedef struct NoFila {
    char           *descricao;
    struct NoFila  *prox;
} NoFila;

/* Estrutura principal da fila */
typedef struct {
    NoFila *frente;   /* próximo a sair */
    NoFila *fim;      /* onde inserções normais vão */
    int     tamanho;
} Fila;

/* Cria uma fila vazia. Retorna NULL em caso de falha. */
Fila  *fila_criar(void);

/* Retorna 1 se a fila estiver vazia, 0 caso contrário. */
int    fila_vazia(const Fila *f);

/* Retorna o número de elementos na fila. */
int    fila_tamanho(const Fila *f);

/* Insere ao final da fila. Retorna 1 em sucesso, 0 em falha. */
int    fila_enqueue(Fila *f, const char *descricao);

/* Insere na FRENTE da fila (para operação de desfazer). */
int    fila_enqueue_frente(Fila *f, const char *descricao);

/*
 * Remove da frente e copia descrição em 'buf' (máximo 'buf_len' bytes).
 * Retorna 1 em sucesso, 0 se vazia ou erro.
 */
int    fila_dequeue(Fila *f, char *buf, size_t buf_len);

/* Imprime todas as tarefas da frente ao fim, numeradas. */
void   fila_imprimir(const Fila *f);

/* Libera toda a memória associada à fila. */
void   fila_destruir(Fila *f);

#endif /* FILA_H */
