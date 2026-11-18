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

// void removerItemLista(Lista *l, int id) {

//     ponteiro p, q;
//     p = l->prim;
//     if (p =) {

//     }
//     while (p->chave.id != id)
//     {
//         q = p;
//         p = p->prox;
//     }

// }

void exibe_lista(Lista l)
{
    ponteiro p;
    char prioridade[30];
    p = l.prim->prox;
    printf("\n******************************************\n");
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
    printf("2 - Ver lista\n");
    printf("3 - Ver lista ordenada por prioridade\n");
    printf("4 - Encerrar Programa\n");
    printf("Entre com a opcao desejada: \n");
}

int main()
{
    TAREFA T;
    Lista L, ListaOrdenada;
    inicializa_lista(&L);
    inicializa_lista(&ListaOrdenada);
    int op;
    do
    {
        MENU();
        scanf("%d", &op);
        getchar();
        switch (op)
        {
        case 1:
            ler_tarefa(&T);
            adiciona_item_lista(&L, T);
            adiciona_item_lista_ordenada(&ListaOrdenada, T);
            break;
        case 2:
            exibe_lista(L);
            break;
        case 3:
            exibe_lista(ListaOrdenada);
            break;
        default:
            break;
        }
    } while (op != 4);

    return 0;
}