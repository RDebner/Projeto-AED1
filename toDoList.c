#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    int prioridade;
    char tarefa[50];
} TAREFA;

typedef struct Elem
{
    TAREFA chave;
    struct Elem *prox;
} Caixa;

typedef Caixa *ponteiro;

typedef struct
{
    ponteiro prim, ult;
    int qtd;
} Lista;

void inicializa_lista(Lista *l)
{
    l->prim = (ponteiro)malloc(sizeof(Caixa));
    l->ult = l->prim;
    l->prim->prox = NULL;
    l->qtd = 0;
}

void adiciona_item_lista(Lista *l, TAREFA t)
{
    ponteiro p, q;

    q = (ponteiro)malloc(sizeof(Caixa));
    q->chave = t;

    p = l->prim;
    while ((p != l->ult))
    {
        p = p->prox;
    }
    q->prox = p->prox;
    p->prox = q;
    l->qtd++;
    q->chave.id = l->qtd;
    if (p == l->ult)
    {
        l->ult = q;
    }
}

void adiciona_item_lista_ordenada(Lista *l, TAREFA t)
{
    ponteiro p, q;

    q = (ponteiro)malloc(sizeof(Caixa));
    if (q == NULL)
        return; // Boa prática: verificar se a memória foi alocada

    q->chave = t;
    q->prox = NULL;

    // CASO 1: Lista vazia
    if (l->prim == NULL)
    {
        l->prim = q;
        l->ult = q;
    }
    // CASO 2: Inserir no início (nova tarefa tem prioridade maior que a primeira)
    else if (q->chave.prioridade > l->prim->chave.prioridade)
    {
        q->prox = l->prim;
        l->prim = q;
    }
    // CASO 3: Inserir no meio ou no final
    else
    {
        p = l->prim;
        // Olha sempre para o PRÓXIMO nó (p->prox) antes de avançar
        while (p->prox != NULL && p->prox->chave.prioridade >= q->chave.prioridade)
        {
            p = p->prox;
        }

        q->prox = p->prox;
        p->prox = q;

        // Se inseriu depois do último, atualiza o ponteiro l->ult
        if (p == l->ult)
        {
            l->ult = q;
        }
    }

    l->qtd++;
    q->chave.id = l->qtd;
}

void removerItemLista(Lista *l, int id)
{
    ponteiro p, aux;
    p = l->prim;

    while (p->prox != NULL && p->prox->chave.id != id)
    {
        p = p->prox;
    }

    aux = p->prox;
    p->prox = aux->prox;
    if (aux == l->ult)
    {
        l->ult = p;
    }
    l->qtd--;
    free(aux);
    while (p->prox != NULL)
    {
        p->prox->chave.id = id;
        id++;
    }
}

void marcarTarefaConcluida(Lista *l, Lista *lo, Lista *lc, int id)
{
    ponteiro p;
    p = l->prim;

    while (p->prox != NULL && p->prox->chave.id != id)
    {
        p = p->prox;
    }

    adiciona_item_lista(lc, p->prox->chave);
    removerItemLista(l, id);
    removerItemLista(lo, id);
}

void exibe_lista(Lista l, int op)
{
    ponteiro p;
    char prioridade[30];
    switch (op)
    {
    case 2:
        printf("\n************ TAREFAS PENDENTES ************\n");
        break;
    case 3:
        printf("\n************ TAREFAS PENDENTES ************\n");
        break;
    case 4:
        printf("\n*********** TAREFAS CONCLUIDAS ************\n");
        break;
    default:
        break;
    }
    if (l.prim == l.ult)
    {
        printf("\n(lista vazia)");
        printf("\n\n******************************************\n");
    }
    else
    {
        p = l.prim->prox;
        while (p != NULL)
        {

            switch (p->chave.prioridade)
            {
            case 1:
                strcpy(prioridade, "(Prioridade Baixa)");
                break;
            case 2:
                strcpy(prioridade, "(Prioridade Media)");
                break;
            case 3:
                strcpy(prioridade, "(Prioridade Alta)");
                break;
            default:
                break;
            }

            printf("\n%d - %s %s", p->chave.id, p->chave.tarefa, prioridade);
            p = p->prox;
        }
        printf("\n\n******************************************\n");
    }
}

void ler_tarefa(TAREFA *t)
{
    printf("Escreva a tarefa a ser adicionada na lista: \n");
    fgets(t->tarefa, 50, stdin);
    printf("Nivel de Prioridade (1 - Baixa, 2 - Media, 3 - Alta): \n");
    scanf("%d", &t->prioridade);
}

void MENU()
{
    printf("****************  M E N U ****************\n");
    printf("1 - Inserir tarefa na lista\n");
    printf("2 - Ver lista de tarefas pendentes\n");
    printf("3 - Ver lista ordenada por prioridade\n");
    printf("4 - Ver lista de tarefas concluidas\n");
    printf("5 - Marcar tarefa como concluida\n");
    printf("6 - Remover tarefa da lista\n");
    printf("7 - Encerrar Programa\n");
    printf("Entre com a opcao desejada: \n");
}

int verificarListaVazia(Lista *l) {
    if (l->prim == l->ult) {
        return 0;
    } else {
        return 1;
    }
}

int main()
{
    TAREFA T;
    Lista L, ListaOrdenada, ListaConcluidas;
    inicializa_lista(&L);
    inicializa_lista(&ListaOrdenada);
    inicializa_lista(&ListaConcluidas);
    int op, id;
    do
    {
        MENU();
        scanf("%d", &op);
        while (op < 1 || op > 7)
        {
            printf("Opcao invalida, digite uma opcao valida: \n");
            scanf("%d", &op);
        }
        getchar();
        switch (op)
        {
        case 1:
            ler_tarefa(&T);
            adiciona_item_lista(&L, T);
            adiciona_item_lista_ordenada(&ListaOrdenada, T);
            break;
        case 2:
            exibe_lista(L, op);
            break;
        case 3:
            exibe_lista(ListaOrdenada, op);
            break;
        case 4:
            exibe_lista(ListaConcluidas, op);
            break;
        case 5:
            printf("Digite o numero da tarefa concluida: \n");
            scanf("%d", &id);
            marcarTarefaConcluida(&L, &ListaOrdenada, &ListaConcluidas, id);
            break;
        case 6:
            if(verificarListaVazia(&L) == 0) {
                printf("\n******************************************\n");
                printf("\n(Nao ha tarefas na lista para remover)\n");
                printf("\n\n******************************************\n");
            } else {
                printf("Digite o numero da tarefa que deseja remover: \n");
                scanf("%d", &id);
                removerItemLista(&L, id);
                removerItemLista(&ListaOrdenada, id);
            }
            break;
        default:
            break;
        }
    } while (op != 7);

    return 0;
}