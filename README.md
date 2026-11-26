# Guia do Código: Sistema de Gerenciamento de Tarefas em C

Este documento explica o funcionamento completo de um sistema de gerenciamento de tarefas escrito em linguagem C. O programa utiliza o conceito de **Listas Encadeadas Simples (Singly Linked Lists)** com alocação dinâmica de memória para armazenar e organizar os dados.

O sistema mantém três listas ativas simultaneamente:
1. Uma lista padrão (`L` - ordem de inserção).
2. Uma lista ordenada por prioridade (`ListaOrdenada`).
3. Uma lista de tarefas concluídas (`ListaConcluidas`).

---

## 1. Estruturas de Dados (`structs`)

Como o C não possui "objetos" nativos, moldamos os dados utilizando `structs` e ponteiros.

```c
typedef struct {
    int id;             // Identificador único e imutável da tarefa
    int numero;         // Posição visual na lista (1, 2, 3...)
    int prioridade;     // 1 (Baixa), 2 (Média), 3 (Alta)
    char tarefa[50];    // Descrição da tarefa
} TAREFA;

typedef struct Elem {
    TAREFA chave;         // Os dados da tarefa
    struct Elem *prox;    // Ponteiro: guarda o endereço de memória da PRÓXIMA caixa
} Caixa;

typedef Caixa *ponteiro; // Apelido: 'ponteiro' agora significa 'Caixa *'

typedef struct {
    ponteiro prim, ult;   // Ponteiros para o início (nó cabeça) e fim da lista
    int qtd;              // Contador total de tarefas na lista
} Lista;
```

* **A Mágica do Encadeamento**: O campo `struct Elem *prox;` é o que cria a corrente. As tarefas não ficam lado a lado na memória RAM (como em um vetor). Elas ficam espalhadas, mas cada "Caixa" sabe exatamente onde está a próxima.

---

## 2. Gerenciamento de Memória (Alocação Dinâmica)

Para que a lista cresça e diminua sob demanda, pedimos memória ao Sistema Operacional usando `malloc` e a devolvemos com `free`.

```c
void inicializa_lista(Lista *l) {
    l->prim = (ponteiro)malloc(sizeof(Caixa)); // Pede memória para 1 caixa
    l->ult = l->prim;                          // O último é igual ao primeiro
    l->prim->prox = NULL;                      // A caixa não aponta para ninguém
    l->qtd = 0;
}
```

* **O Nó Sentinela (Nó Cabeça)**: Note que `inicializa_lista` cria uma caixa vazia. Isso é um truque clássico em C para garantir que o início da lista nunca seja `NULL`, simplificando as inserções e remoções. A primeira tarefa real sempre será acessada via `l->prim->prox`.
* **`libera_lista(Lista *l)`**: Chamada ao encerrar o programa. Caminha pela lista usando dois ponteiros (`p` avança e `aux` fica para trás) dando `free(aux)` em cada nó. Isso evita vazamentos de memória (*memory leaks*).

---

## 3. Funções de Inserção (Costurando a Lista)

Ao adicionar uma tarefa, os ponteiros são reajustados para encaixar a nova caixa na corrente.

### Inserção no Final (`adiciona_item_lista`)
Usada para a lista cronológica e para a lista de concluídas.
```c
p = l->prim;
while ((p != l->ult)) { // Caminha caixa por caixa até o final
    p = p->prox;
}
q->prox = p->prox;  // A nova caixa 'q' aponta para o vazio (NULL)
p->prox = q;        // A antiga última ('p') agora aponta para 'q'
```

### Inserção Ordenada (`adiciona_item_lista_ordenada`)
Insere a tarefa na posição exata de acordo com sua prioridade (da maior para a menor):
```c
while (p->prox != NULL && p->prox->chave.prioridade >= q->chave.prioridade) {
    p = p->prox;
}
```
Ele avança apenas enquanto existir um próximo item **E** a prioridade desse próximo item for maior ou igual à da nova tarefa. Quando o loop para, ele encontrou a "fenda" exata onde a tarefa deve entrar. Logo após inserir, um outro loop percorre a lista renumerando o campo `chave.numero` (1, 2, 3...) para manter a exibição organizada.

---

## 4. Remoção e Conclusão (Cortando o Elo)

Para remover um item (`removerItemLista`), o código busca o nó *anterior* ao alvo, religa os ponteiros e apaga o alvo da memória:

```c
aux = p->prox;       // 'aux' é a caixa alvo (a que queremos deletar)
p->prox = aux->prox; // A caixa anterior ('p') passa a apontar para a caixa DEPOIS de 'aux'
free(aux);           // A memória de 'aux' é devolvida para o sistema
```
Depois do `free`, um loop diminui em `-1` o `chave.numero` de todas as tarefas que estavam depois da removida, evitando buracos na numeração.

* **`marcarTarefaConcluida`**: Reaproveita as funções prontas. Ela busca a tarefa pelo `id`, copia seus dados para a lista de concluídas (`adiciona_item_lista`) e a deleta tanto da lista normal quanto da ordenada (`removerItemLista`).

---

## 5. Funções Auxiliares e de Validação

Para evitar que o programa trave (ex: tentar remover algo de uma lista vazia ou digitar um ID que não existe), o código conta com duas funções de segurança:

* **`verificarListaVazia(Lista *l)`**: Compara `if (l->prim == l->ult)`. Se o ponteiro do primeiro elemento (o nó cabeça) for igual ao do último, significa que nenhuma tarefa foi adicionada. Retorna `0` (vazia) ou `1` (contém itens).
* **`verificarIdValido(Lista *l, int id)`**: Percorre a lista do início ao fim comparando `p->prox->chave.id == id`. Se encontrar, retorna `1` (verdadeiro); se chegar ao `NULL` sem achar, retorna `0` (falso), fazendo o menu pedir o ID novamente.

---

## 6. Entrada e Saída de Dados (Interface)

### Lendo Textos (`ler_tarefa`)
O C exige cuidados especiais ao ler strings com espaços do teclado:
```c
fgets(t->tarefa, 50, stdin);
t->tarefa[strcspn(t->tarefa, "\n")] = '\0';
```
* **`fgets`**: Lê a linha inteira até o usuário apertar Enter, permitindo nomes compostos (ex: *"Estudar ponteiros"*), diferente do `scanf` que cortaria no primeiro espaço.
* **O truque do `strcspn`**: O `fgets` salva a tecla "Enter" (`\n`) no final da string. O `strcspn` encontra a posição exata desse `\n` e o substitui por `\0` (caractere nulo que indica o fim de uma string em C).

### Exibição Dinâmica (`exibe_lista` e `MENU`)
* **`MENU()`**: Apenas imprime as 7 opções na tela.
* **`exibe_lista(Lista l, int op)`**: Recebe uma cópia da lista e a opção escolhida no menu (`op`). Usa um `switch(op)` para decidir qual título imprimir no topo (ex: *TAREFAS PENDENTES*, *TAREFAS CONCLUIDAS* ou *REMOVER TAREFA*). Em seguida, percorre os nós traduzindo o número da prioridade (`1`, `2` ou `3`) em texto usando `strcpy` e imprime tudo formatado.

---

## 7. O Coração do Programa (`main`)

A função `main()` é o maestro que coordena todas as funções acima:

1. **Preparação**: Declara as três listas (`L`, `ListaOrdenada`, `ListaConcluidas`), chama `inicializa_lista` para cada uma e cria a variável `ID = 1` (que funciona como um gerador automático de IDs únicos, sendo incrementada a cada nova tarefa).
2. **Loop Principal (`do-while`)**: Mantém o programa rodando até que o usuário digite `7`.
3. **Limpeza de Buffer (`getchar()`)**: Logo após ler a opção do menu com `scanf("%d", &op);`, há um `getchar();`. Ele serve para "engolir" o Enter que sobrou no buffer do teclado. Sem ele, o `fgets` da opção 1 leria esse Enter fantasma e pularia a digitação do nome da tarefa.
4. **Roteamento (`switch-case`)**:
   * **Caso 1**: Lê a tarefa, insere na lista `L` e na `ListaOrdenada` com o mesmo `ID`, e soma `ID++`.
   * **Casos 2, 3 e 4**: Chamam `exibe_lista` passando a lista correspondente.
   * **Casos 5 e 6**: Verificam se a lista tem itens, exibem as tarefas, pedem o `id` (permitindo digitar `0` para cancelar/voltar), validam se o `id` existe com `verificarIdValido` e executam a conclusão ou remoção.
5. **Encerramento**: Ao sair do loop (opção `7`), chama `libera_lista` para as três listas antes do `return 0;`, garantindo um encerramento limpo da memória.
