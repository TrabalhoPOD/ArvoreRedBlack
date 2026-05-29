#include <stdlib.h>
#include "rbtree.h"

// Inicializa a árvore e o nó sentinela NIL
RBTree *createTree()
{
    RBTree *T = (RBTree *)malloc(sizeof(RBTree));

    // Configuração do nó sentinela (NIL)
    T->nil = (Node *)malloc(sizeof(Node));
    T->nil->color = BLACK; // Regra: Todo nó nulo é considerado preto
    T->nil->left = NULL;
    T->nil->right = NULL;
    T->nil->parent = NULL;
    T->nil->data = 0;

    // A árvore começa vazia, então a raiz aponta para o sentinela
    T->root = T->nil;

    return T;
}

// Cria um novo nó
Node *createNode(RBTree *T, int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->color = RED;   // Regra padrão: novos nós sempre nascem vermelhos
    newNode->left = T->nil; // Filhos apontam para o sentinela em vez de NULL
    newNode->right = T->nil;
    newNode->parent = T->nil;

    return newNode;
}

// Rotação à esquerda em torno do nó 'x'
void leftRotate(RBTree *T, Node *x)
{
    Node *y = x->right; // Define y
    x->right = y->left; // Transforma a subárvore esquerda de y na subárvore direita de x

    if (y->left != T->nil)
    {
        y->left->parent = x;
    }

    y->parent = x->parent; // Conecta o pai de x a y

    if (x->parent == T->nil)
    {
        T->root = y; // Se x era a raiz, y vira a nova raiz
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }

    y->left = x; // Coloca x à esquerda de y
    x->parent = y;
}

// Rotação à direita em torno do nó 'y'
void rightRotate(RBTree *T, Node *y)
{
    Node *x = y->left;  // Define x
    y->left = x->right; // Transforma a subárvore direita de x na subárvore esquerda de y

    if (x->right != T->nil)
    {
        x->right->parent = y;
    }

    x->parent = y->parent; // Conecta o pai de y a x

    if (y->parent == T->nil)
    {
        T->root = x; // Se y era a raiz, x vira a nova raiz
    }
    else if (y == y->parent->right)
    {
        y->parent->right = x;
    }
    else
    {
        y->parent->left = x;
    }

    x->right = y; // Coloca y à direita de x
    y->parent = x;
}