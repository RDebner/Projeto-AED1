#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    int numero;
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

void adiciona_item_lista(Lista *l, TAREFA t, int id)
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

    q->chave.id = id;
    q->chave.numero = l->qtd;
    if (p == l->ult)
    {
        l->ult = q;
    }
}

void adiciona_item_lista_ordenada(Lista *l, TAREFA t, int id)
{
    ponteiro p, q;
    q = (ponteiro)malloc(sizeof(Caixa));
    if (q == NULL) return;

    q->chave = t;
    q->chave.id = id;

    p = l->prim; // Começa no nó cabeça
    while (p->prox != NULL && p->prox->chave.prioridade >= q->chave.prioridade)
    {
        p = p->prox;
    }

    q->prox = p->prox;
    p->prox = q;

    if (p == l->ult)
    {
        l->ult = q;
    }

    l->qtd++;
    
    // Atualiza a numeração sequencial da lista ordenada
    int num = 1;
    p = l->prim->prox;
    while (p != NULL) {
        p->chave.numero = num++;
        p = p->prox;
    }
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
    p = l->prim;
    while (p->prox != NULL)
    {
        if (p->prox->chave.numero > aux->chave.numero)
        {
            p->prox->chave.numero--;
        }
        p = p->prox;
    }
    free(aux);
}

void marcarTarefaConcluida(Lista *l, Lista *lo, Lista *lc, int id)
{
    ponteiro p;
    p = l->prim;

    while (p->prox != NULL && p->prox->chave.id != id)
    {
        p = p->prox;
    }

    adiciona_item_lista(lc, p->prox->chave, id);
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
    case 5:
        printf("\n************ TAREFAS PENDENTES ************\n");
        break;
    case 4:
        printf("\n*********** TAREFAS CONCLUIDAS ************\n");
        break;
    case 6:
        printf("\n************* REMOVER TAREFA **************\n");
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

            printf("\n(ID #%d) %d - %s %s", p->chave.id, p->chave.numero, p->chave.tarefa, prioridade);
            p = p->prox;
        }
        printf("\n\n******************************************\n");
    }
}

void ler_tarefa(TAREFA *t)
{
    printf("Escreva a tarefa a ser adicionada na lista: \n");
    fgets(t->tarefa, 50, stdin);
    t->tarefa[strcspn(t->tarefa, "\n")] = '\0';
    printf("Nivel de Prioridade (1 - Baixa, 2 - Media, 3 - Alta): \n");
    scanf("%d", &t->prioridade);
    while (t->prioridade < 1 || t->prioridade > 3)
    {
        printf("Nivel de prioridade invalido, digite um numero valido (1 - Baixa, 2 - Media, 3 - Alta): \n");
        scanf("%d", &t->prioridade);
    }
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

int verificarListaVazia(Lista *l)
{
    if (l->prim == l->ult)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int verificarIdValido(Lista *l, int id)
{
    ponteiro p;
    int idValido = 0;
    p = l->prim;
    while (p->prox != NULL && p->prox->chave.id != id)
    {
        p = p->prox;
    }

    if (p->prox != NULL && p->prox->chave.id == id)
    {
        idValido = 1;
    }
    return idValido;
}

void libera_lista(Lista *l)
{
    ponteiro p = l->prim;
    while (p != NULL)
    {
        ponteiro aux = p;
        p = p->prox;
        free(aux);
    }
    l->prim = NULL;
    l->ult = NULL;
    l->qtd = 0;
}

void criarArq(Lista l) {   // sem a struct...
   FILE *fp;  
   ponteiro p;

    fp = fopen ("Lista.dat", "wb"); //arquivo para escrita binária
   if (fp == NULL) {  
      printf ("Erro ao abrir o arquivo.\n");  
      return;  
   }  
   else {  p = l.prim->prox;
	       while(p!=NULL) {
             fwrite(&(p->chave), sizeof(TAREFA), 1, fp); 
             p = p->prox;
	      }
          }         
     fclose (fp);  
       
}/*Função reponsavel por exibir o conteudo do arquivo Binario.dat*/   

void criarArqOrdenado(Lista l) {   // sem a struct...
   FILE *fp;  
   ponteiro p;

    fp = fopen ("ListaOrdenada.dat", "wb"); //arquivo para escrita binária
   if (fp == NULL) {  
      printf ("Erro ao abrir o arquivo.\n");  
      return;  
   }  
   else {  p = l.prim->prox;
	       while(p!=NULL) {
             fwrite(&(p->chave), sizeof(TAREFA), 1, fp); 
             p = p->prox;
	      }
          }         
     fclose (fp);  
       
}/*Função reponsavel por exibir o conteudo do arquivo Binario.dat*/    

void criarArqConcluido(Lista l) {   // sem a struct...
   FILE *fp;  
   ponteiro p;

    fp = fopen ("ListaConcluidas.dat", "wb"); //arquivo para escrita binária
   if (fp == NULL) {  
      printf ("Erro ao abrir o arquivo.\n");  
      return;  
   }  
   else {  p = l.prim->prox;
	       while(p!=NULL) {
             fwrite(&(p->chave), sizeof(TAREFA), 1, fp); 
             p = p->prox;
	      }
          }         
     fclose (fp);  
       
}/*Função reponsavel por exibir o conteudo do arquivo Binario.dat*/     

void carregarLista(Lista *l) {
    
    FILE *fp;
    TAREFA t;

    fp = fopen("Lista.dat", "rb");
    if (fp == NULL) {
        inicializa_lista(&(*l));
        return;
    } else {
        inicializa_lista(&(*l));
        while (fread(&t, sizeof(TAREFA), 1, fp))
        {
            adiciona_item_lista(&(*l), t, t.id);
        }
        fclose(fp);
    }
}

void carregarListaOrdenada(Lista *l) {
    
    FILE *fp;
    TAREFA t;

    fp = fopen("Lista.dat", "rb");
    if (fp == NULL) {
        inicializa_lista(&(*l));
        return;
    } else {
        inicializa_lista(&(*l));
        while (fread(&t, sizeof(TAREFA), 1, fp))
        {
            adiciona_item_lista(&(*l), t, t.id);
        }
        fclose(fp);
    }
}

void carregarListaConcluidas(Lista *l) {
    
    FILE *fp;
    TAREFA t;

    fp = fopen("Lista.dat", "rb");
    if (fp == NULL) {
        inicializa_lista(&(*l));
        return;
    } else {
        inicializa_lista(&(*l));
        while (fread(&t, sizeof(TAREFA), 1, fp))
        {
            adiciona_item_lista(&(*l), t, t.id);
        }
        fclose(fp);
    }
}

int main()
{
    TAREFA T;
    Lista L, ListaOrdenada, ListaConcluidas;
    carregarLista(&L);
    carregarListaOrdenada(&ListaOrdenada);
    carregarListaConcluidas(&ListaConcluidas);

    int op, id, ID = 1;
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
            adiciona_item_lista(&L, T, ID);
            adiciona_item_lista_ordenada(&ListaOrdenada, T, ID);
            ID++;
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
            if (L.qtd == 0)
            {
                printf("\n************ TAREFAS PENDENTES ************\n");
                printf("\n(Nao ha tarefas pendentes)");
                printf("\n\n******************************************\n");
            }
            else
            {
                exibe_lista(L, op);
                printf("Digite o ID da tarefa concluida:\n(Digite 0 para voltar)\n");
                scanf("%d", &id);
                if (id == 0) {
                    break;
                } else {
                    while (verificarIdValido(&L, id) != 1)
                    {
                        printf("ID invalido, digite o ID de uma tarefa na lista: \n");
                        scanf("%d", &id);
                    }
                }
                marcarTarefaConcluida(&L, &ListaOrdenada, &ListaConcluidas, id);
            }
            break;
        case 6:
            if (verificarListaVazia(&L) == 0)
            {
                printf("\n************* REMOVER TAREFA **************\n");
                printf("\n(Nao ha tarefas na lista para remover)");
                printf("\n\n******************************************\n");
            }
            else
            {
                exibe_lista(L, op);
                printf("Digite o ID da tarefa que deseja remover:\n(Digite 0 para voltar)\n");
                scanf("%d", &id);
                if (id == 0) {
                    break;
                } else {
                    while (verificarIdValido(&L, id) != 1)
                    {
                        printf("ID invalido, digite o ID de uma tarefa na lista: \n");
                        scanf("%d", &id);
                    }
                }
                removerItemLista(&L, id);
                removerItemLista(&ListaOrdenada, id);
            }
            break;
        default:
            break;
        }
    } while (op != 7);

    criarArq(L);
    criarArqOrdenado(ListaOrdenada);
    criarArqConcluido(ListaConcluidas);

    libera_lista(&L);
    libera_lista(&ListaOrdenada);
    libera_lista(&ListaConcluidas);

    return 0;
}