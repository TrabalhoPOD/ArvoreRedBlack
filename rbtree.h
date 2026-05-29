#ifndef RBTREE_H
#define RBTREE_H

// Definição das cores
typedef enum
{
    RED,
    BLACK
} Color;

// Estrutura do Nó
typedef struct Node
{
    int data;
    Color color;
    struct Node *left, *right, *parent;
} Node;

typedef struct RBTree
{
    Node *root;
    Node *nil;
} RBTree;

RBTree *createTree();
Node *createNode(RBTree *T, int data);
void leftRotate(RBTree *T, Node *x);
void rightRotate(RBTree *T, Node *y);
void rbTreeDelete(RBTree *T, int data);
void rbTreeDeleteNode(RBTree *T, Node *nodeToDelete);
void rbTransplant(RBTree *T, Node *oldNode, Node *newNode);
void rbDeleteFixup(RBTree *T, Node *fixNode);
Node *findMin(RBTree *T, Node *node);
Node *search(RBTree *T, Node *node, int data);

void rbInsertFixup(RBTree *T, Node *z);
void rbTreeInsert(RBTree *T, int data);

#endif