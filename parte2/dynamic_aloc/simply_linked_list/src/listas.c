#include <stdio.h>
#include <stdlib.h>

typedef struct TNode
{
    int data;
    struct TNode *next;
} Node;

typedef struct
{
    Node *head;
    Node *tail;
    int size;
} List;

void init(List *lista);
void add(List *lista, int valor);
void show(List *lista);
void clear(List *lista);
int removeFirst(List *lista);
int removeLast(List *lista);
int removePosition(List *lista, int pos);
void addFirst(List *lista, int valor);
void insertPosition(List *lista, int pos, int valor);

void init(List *lista)
{
    lista->head = lista->tail = NULL;
    lista->size = 0;
}

void add(List *lista, int valor)
{
    Node *novo = malloc(sizeof(Node));
    novo->data = valor;
    novo->next = NULL;
    if (lista->size == 0)
    {
        lista->head = novo;
    }
    else
    {
        lista->tail->next = novo;
    }
    lista->tail = novo;
    lista->size++;
}

void show(List *lista)
{
    Node *aux = lista->head;
    while (aux != NULL)
    {
        printf("%d ", aux->data);
        aux = aux->next;
    }
    printf("\n");
}

void clear(List *lista)
{
    // while(lista->size > 0)
    // removeFirst(lista);

    Node *aux = lista->head;
    while (aux != NULL)
    {
        aux = aux->next;
        free(lista->head);
        lista->head = aux;
    }
    lista->tail = NULL;
    lista->size = 0;
}

int removeFirst(List *lista)
{
    Node *aux = lista->head;
    lista->head = aux->next;
    int valor = aux->data;
    lista->size--;
    if (lista->size == 0)
    { // Lista vazia?
        lista->tail = NULL;
    }
    free(aux);
    return valor;
}

void addFirst(List *lista, int valor)
{
    if (lista->size == 0)
    {
        add(lista, valor);
        return;
    }
    Node *novo = malloc(sizeof(Node));
    novo->data = valor;
    novo->next = lista->head;
    lista->head = novo;
    lista->size++;
}

int removeLast(List *lista)
{
    if (lista->size == 1)
        return removeFirst(lista);
    Node *aux = lista->head;
    // Avança aux até UM antes do último
    // for (int pos = 0; pos < lista->size - 2; pos++)
    while (aux->next->next != NULL)
        aux = aux->next;
    // Target é o ÚLTIMO
    Node *target = aux->next;
    // aux agora é o final da lista!
    aux->next = NULL;
    lista->tail = aux;
    lista->size--;
    // Salva o valor a ser retornado
    int valor = target->data;
    // Libera a mem. ocupada pelo nodo target
    free(target);
    return valor;
}

int removePosition(List *lista, int pos)
{
    if (pos == 0)
        return removeFirst(lista);
    if (pos == lista->size - 1)
        return removeLast(lista);
    Node *aux = lista->head;
    // Avança aux até UM antes da posição a ser removida
    for (int i = 0; i < pos - 1; i++)
        aux = aux->next;
    Node *target = aux->next;
    aux->next = target->next;
    int valor = target->data;
    free(target);
    lista->size--;
    return valor;
}

void insertPosition(List *lista, int pos, int valor)
{
    // Terminar!
}

int main()
{
    List minhaLista;
    init(&minhaLista);

    for (int i = 0; i < 100; i++)
        addFirst(&minhaLista, i);

    show(&minhaLista);

    while (minhaLista.size > 10)
    {
        int v = removeLast(&minhaLista);
        printf("Último elemento: %d\n", v);
    }
    printf("\n");
    show(&minhaLista);

    printf("Removendo o primeiro (por posição): ");
    int valor = removePosition(&minhaLista, 0);
    printf("%d\n", valor);

    printf("\n");
    show(&minhaLista);

    printf("Removendo o último (por posição): ");
    valor = removePosition(&minhaLista, minhaLista.size - 1);
    printf("%d\n", valor);

    printf("\n");
    show(&minhaLista);

    printf("Removendo o elemento na pos. 4: ");
    valor = removePosition(&minhaLista, 4);
    printf("%d\n", valor);

    printf("\n");
    show(&minhaLista);

    clear(&minhaLista);
}