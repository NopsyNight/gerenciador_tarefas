/*
 * persistencia.h — Leitura e escrita do estado em disco
 *
 * Formato do arquivo .dat (texto simples):
 *
 *   FILA <N>
 *   tarefa um
 *   tarefa dois
 *   ...
 *   HISTORICO <M>
 *   tarefa mais recente (topo da pilha)
 *   tarefa mais antiga
 *   ...
 *
 * Escolha do formato: texto puro foi preferido a CSV ou binário
 * porque é legível por humanos (facilita debug), portável entre
 * arquiteturas e simples de parsear linha a linha com fgets.
 */

#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "fila.h"
#include "pilha.h"

/*
 * Lê o arquivo 'caminho' e reconstrói fila e historico.
 * Se o arquivo não existir ou estiver vazio, nada é feito
 * (comportamento correto para primeira execução).
 */
void carregar_estado(const char *caminho, Fila *fila, Pilha *historico);

/*
 * Serializa fila e historico para o arquivo 'caminho'.
 * Sobrescreve o conteúdo existente.
 */
void salvar_estado(const char *caminho, const Fila *fila, const Pilha *historico);

#endif /* PERSISTENCIA_H */
