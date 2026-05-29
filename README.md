# Gerenciador de Tarefas com Desfazer

Sistema CLI em C puro (C11) que gerencia tarefas pendentes com suporte à operação de desfazer (undo).

## Problema que o sistema resolve

Gerenciar uma lista de afazeres onde o usuário pode:
- Adicionar tarefas à fila de pendências
- Concluir a próxima tarefa (remove da fila e arquiva no histórico)
- **Desfazer** a última conclusão (restaura a tarefa para o início da fila)
- Consultar o histórico de tarefas concluídas

## Estruturas de dados utilizadas

### Fila encadeada (FIFO) — `fila.c / fila.h`

Usada para a lista de tarefas pendentes.

**Por quê fila?** O fluxo de trabalho é naturalmente FIFO: a tarefa adicionada primeiro deve ser a primeira a ser concluída. Uma pilha inverteria a prioridade; uma lista duplamente encadeada seria superdimensionamento.

Operações:
- `enqueue` na cauda: O(1)
- `dequeue` da frente: O(1)
- `enqueue_frente` (para desfazer): O(1)

### Pilha encadeada (LIFO) — `pilha.c / pilha.h`

Usada para o histórico de operações concluídas (stack de undo).

**Por quê pilha?** "Desfazer" é inerentemente LIFO: a operação mais recente é a primeira a ser desfeita. É exatamente o padrão Ctrl+Z de qualquer editor de texto.

Operações:
- `push`: O(1)
- `pop`: O(1)

## Formato do arquivo de persistência (`tarefas.dat`)

Arquivo texto simples com seções separadas:

```
FILA 2
Estudar para prova de AED
Enviar relatório para o professor
HISTORICO 1
Fazer exercício da aula 5
```

**Por que texto puro?**
- Legível por humanos (facilita depuração)
- Portável entre sistemas operacionais e arquiteturas
- Sem dependências externas para parsear
- Simples de reconstruir linha a linha com `fgets`

## Compilação e execução

```bash
# Com Makefile
make
./gerenciador

# Manualmente
gcc -std=c11 -Wall -o gerenciador main.c fila.c pilha.c persistencia.c
./gerenciador
```

## Limitações conhecidas

- Tamanho máximo de descrição: 255 caracteres
- O histórico não tem limite de tamanho (cresce conforme uso)
- Persistência não é atômica: se o programa for encerrado abruptamente (Ctrl+C), as alterações da sessão são perdidas
- Não há suporte a múltiplos usuários simultâneos
- Caracteres especiais (ex: `\n` dentro da descrição) podem corromper a serialização

## Estrutura de arquivos

```
gerenciador_tarefas/
├── main.c          # Menu interativo e lógica principal
├── fila.c          # Implementação da fila encadeada
├── fila.h          # Interface da fila
├── pilha.c         # Implementação da pilha encadeada
├── pilha.h         # Interface da pilha
├── persistencia.c  # Serialização/deserialização em disco
├── persistencia.h  # Interface da persistência
├── Makefile        # Build
└── README.md       # Este arquivo
```
