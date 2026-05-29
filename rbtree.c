#include <stdio.h>
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

// Esta função substitui a posição de um nó (oldNode) na árvore por outro nó (newNode)
// É usada durante a remoção para desconectar o nó que será removido
void rbTransplant(RBTree *T, Node *oldNode, Node *newNode)
{
    // Se oldNode é a raiz, newNode vira a nova raiz
    if (oldNode->parent == T->nil)
    {
        T->root = newNode;
    }
    // Senão, ajusta o ponteiro do pai de oldNode para apontar para newNode
    else if (oldNode == oldNode->parent->left)
    {
        oldNode->parent->left = newNode;
    }
    else
    {
        oldNode->parent->right = newNode;
    }

    // Atualiza o pai de newNode para ser o pai de oldNode
    newNode->parent = oldNode->parent;
}

// Busca recursiva por um valor na árvore
Node *search(RBTree *T, Node *node, int data)
{
    // Se achou o nó ou chegou no sentinela NIL, retorna
    if (node == T->nil || node->data == data)
    {
        return node;
    }

    // Busca recursivamente na subárvore apropriada
    if (data < node->data)
    {
        return search(T, node->left, data);
    }
    else
    {
        return search(T, node->right, data);
    }
}

// Encontra o nó com o menor valor a partir de um determinado nó
// Usado para encontrar o SUCESSOR (filho mínimo da subárvore direita)
Node *findMin(RBTree *T, Node *node)
{
    if (node == T->nil)
    {
        return T->nil;
    }

    // Vai sempre para a esquerda até não poder mais
    while (node->left != T->nil)
    {
        node = node->left;
    }

    return node;
}

// Corrige possíveis violações das propriedades Red-Black após uma inserção
void rbInsertFixup(RBTree *T, Node *z)
{
    while (z->parent->color == RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            Node *y = z->parent->parent->right; // Tio de z

            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    leftRotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(T, z->parent->parent);
            }
        }
        else
        {
            Node *y = z->parent->parent->left; // Tio de z

            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rightRotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(T, z->parent->parent);
            }
        }
    }
    T->root->color = BLACK;
}

// Insere um novo valor na árvore Red-Black
void rbTreeInsert(RBTree *T, int data)
{
    Node *y = T->nil;
    Node *x = T->root;

    // Procura o local de inserção e verifica se o nó já existe
    while (x != T->nil)
    {
        y = x;
        if (data < x->data)
        {
            x = x->left;
        }
        else if (data > x->data)
        {
            x = x->right;
        }
        else
        {
            // O valor já existe, ignora a operação
            printf("Valor ja esta presente na arvore\n");
            return;
        }
    }

    Node *z = createNode(T, data);
    z->parent = y;

    if (y == T->nil)
    {
        T->root = z;
    }
    else if (z->data < y->data)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }

    rbInsertFixup(T, z);
}

// após remover um nó preto, a árvore pode violar as propriedades Red-Black
// Esta função restaura essas propriedades através de rotações e recolorimento
// 
// Propriedade importante: Todos os caminhos devem ter o mesmo número de nós pretos
// Se removemos um preto, precisamos restaurar isso
//
void rbDeleteFixup(RBTree *T, Node *fixNode)
{
    // continua enquanto fixNode não é a raiz e é preto
    // (Se é vermelho, podemos simplesmente pintar de preto)
    while (fixNode != T->root && fixNode->color == BLACK)
    {
        if (fixNode == fixNode->parent->left)  // fixNode é filho ESQUERDO
        {
            Node *sibling = fixNode->parent->right; // sibling é o IRMÃO de fixNode

            // 1: IRMÃO É VERMELHO
            // Se o irmão é vermelho, o convertemos para preto via rotação
            // Isso nos leva para um dos casos 2, 3 ou 4
            if (sibling->color == RED)
            {
                sibling->color = BLACK;           // Pinta irmão de preto
                fixNode->parent->color = RED;     // Pinta pai de vermelho
                leftRotate(T, fixNode->parent);   // Rotaciona pai para esquerda
                sibling = fixNode->parent->right; // Atualiza o irmão (que mudou após rotação)
            }

            // 2: IRMÃO COR PRETA COM AMBOS FILHOS PRETOS
            // Se o irmão e seus filhos são pretos, simplesmente pinta o irmão de vermelho
            // Isso redistribui a "falta de preto" para o nível acima
            if (sibling->left->color == BLACK && sibling->right->color == BLACK)
            {
                sibling->color = RED;              // Pinta irmão de vermelho
                fixNode = fixNode->parent;         // Continua a correção no nível acima
            }
            else  // Irmão tem pelo menos um filho vermelho
            {
                // 3: IRMÃO COR PRETO, FILHO ESQUERDO VERMELHO
                // Se o filho direito do irmão é preto mas o esquerdo é vermelho,
                // fazemos uma rotação à direita no irmão para chegar no Caso 4
                if (sibling->right->color == BLACK)
                {
                    sibling->left->color = BLACK;  // Pinta filho esquerdo de preto
                    sibling->color = RED;          // Pinta irmão de vermelho
                    rightRotate(T, sibling);       // Rotaciona irmão para direita
                    sibling = fixNode->parent->right; // Atualiza o irmão (que mudou após rotação)
                }

                // CASO 4: IRMÃO COR PRETO COM FILHO DIREITO VERMELHO
                // Este é o caso "padrão" - executamos a rotação final
                sibling->color = fixNode->parent->color;         // Irmão assume cor do pai
                fixNode->parent->color = BLACK;                  // Pai vira preto
                sibling->right->color = BLACK;                   // Filho direito do irmão vira preto
                leftRotate(T, fixNode->parent);                  // Rotaciona pai para esquerda
                fixNode = T->root;                               // Termina o loop (fixNode virou raiz)
            }
        }
        else  // fixNode é filho DIREITO (tratamento simétrico)
        {
            Node *sibling = fixNode->parent->left;  // sibling é o IRMÃO de fixNode

            // 1: IRMÃO É VERMELHO (SIMÉTRICO)
            if (sibling->color == RED)
            {
                sibling->color = BLACK;
                fixNode->parent->color = RED;
                rightRotate(T, fixNode->parent);  // Rotaciona para DIREITA (oposto do caso acima)
                sibling = fixNode->parent->left;
            }

            // 2: IRMÃO COR PRETA COM AMBOS FILHOS PRETOS
            if (sibling->right->color == BLACK && sibling->left->color == BLACK)
            {
                sibling->color = RED;
                fixNode = fixNode->parent;
            }
            else
            {
                // CASO 3: IRMÃO COR PRETA, FILHO DIREITO VERMELHO (SIMÉTRICO)
                if (sibling->left->color == BLACK)
                {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    leftRotate(T, sibling);        // Rotaciona para ESQUERDA (oposto do caso acima)
                    sibling = fixNode->parent->left;
                }

                // CASO 4: IRMÃO COR PRETA COM FILHO ESQUERDO VERMELHO (SIMÉTRICO)
                sibling->color = fixNode->parent->color;
                fixNode->parent->color = BLACK;
                sibling->left->color = BLACK;
                rightRotate(T, fixNode->parent);   // Rotaciona para DIREITA (oposto do caso acima)
                fixNode = T->root;
            }
        }
    }

    // Garante que fixNode é preto no final
    fixNode->color = BLACK;
}

// Remove um nó específico da árvore Red-Black
// 
// A remoção em BST tem 3 casos:
// 1. Nó é folha ou tem só filho direito -> simples, apenas substitui
// 2. Nó tem só filho esquerdo -> simples, apenas substitui
// 3. Nó tem ambos os filhos -> substitui pelo SUCESSOR (mínimo da subárvore direita)
//
void rbTreeDeleteNode(RBTree *T, Node *nodeToDelete)
{
    Node *replacementNode = nodeToDelete;                        // replacementNode será o nó removido ou seu sucessor
    Color originalColorOfReplacement = replacementNode->color;    // Guardamos a cor para saber se precisa corrigir depois
    Node *nodeToFix;                                              // nodeToFix será o nó que pode violar as propriedades

    // CASO 1: Filho esquerdo é NIL
    // Se nodeToDelete não tem filho esquerdo, simplesmente substituímos por seu filho direito
    if (nodeToDelete->left == T->nil)
    {
        nodeToFix = nodeToDelete->right;
        rbTransplant(T, nodeToDelete, nodeToDelete->right);  // Coloca o filho direito no lugar de nodeToDelete
    }
    // CASO 2: Filho direito é NIL
    // Se nodeToDelete não tem filho direito, substituímos por seu filho esquerdo
    else if (nodeToDelete->right == T->nil)
    {
        nodeToFix = nodeToDelete->left;
        rbTransplant(T, nodeToDelete, nodeToDelete->left);   // Coloca o filho esquerdo no lugar de nodeToDelete
    }
    // CASO 3: nodeToDelete tem ambos os filhos
    // Não podemos simplesmente remover nodeToDelete, precisamos substituí-lo pelo SUCESSOR
    // O sucessor é o menor nó da subárvore direita
    else
    {
        replacementNode = findMin(T, nodeToDelete->right);       // Encontra o sucessor
        originalColorOfReplacement = replacementNode->color;      // Guarda a cor do sucessor
        nodeToFix = replacementNode->right;                       // nodeToFix será o que pode violar as propriedades

        if (replacementNode->parent == nodeToDelete)
        {
            // Se o sucessor é filho direto de nodeToDelete
            nodeToFix->parent = replacementNode;  // Mesmo que nodeToFix seja NIL, precisamos manter essa relação
        }
        else
        {
            // Se o sucessor não é filho direto de nodeToDelete
            rbTransplant(T, replacementNode, replacementNode->right);  // Remove o sucessor de sua posição
            replacementNode->right = nodeToDelete->right;              // Conecta o filho direito de nodeToDelete ao sucessor
            replacementNode->right->parent = replacementNode;
        }

        rbTransplant(T, nodeToDelete, replacementNode);          // Coloca o sucessor no lugar de nodeToDelete
        replacementNode->left = nodeToDelete->left;               // Conecta o filho esquerdo de nodeToDelete ao sucessor
        replacementNode->left->parent = replacementNode;
        replacementNode->color = nodeToDelete->color;             // O sucessor assume a cor original de nodeToDelete
    }

    // Se removemos um nó PRETO, as propriedades podem estar violadas
    // chamamos a função de correção
    if (originalColorOfReplacement == BLACK)
    {
        rbDeleteFixup(T, nodeToFix);
    }

    // Libera a memória do nó removido
    free(nodeToDelete);
}

// Recebe o valor inteiro que deve ser removido
// Funcionamento:
// 1. Busca o nó com o valor desejado
// 2. Se não achou, simplesmente retorna (sem fazer nada)
// 3. Se achou, chama a remoção do nó
//
void rbTreeDelete(RBTree *T, int data)
{
    // Busca o nó que contém o valor
    Node *nodeToDelete = search(T, T->root, data);

    // Se não encontrou (retornou NIL)
    if (nodeToDelete == T->nil)
    {
        // Simplesmente retorna sem fazer nada
        return;
    }

    // Se encontrou, remove o nó
    rbTreeDeleteNode(T, nodeToDelete);
}

void buscar(RBTree *T, int data){
    Node *resultado = search(T, T->root, data);
    if (resultado == T->nil){
        printf("Busca %d nao encontrado \n", data);
    } else {
        printf("Bsuca %d encontrado \n", data);
    }
}

void imprimeNo(RBTree *T, Node *x, int profundidaer){
    if (x == T->nil){
        return;
    }

    for (int i = 0; i < profundidaer * 2; i++){
        printf(" ");
    }

    printf("%d(%c) \n", x->data, x->color == RED ? 'R' : 'B');
    imprimeNo(T, x->left, profundidaer + 1);
    imprimeNo(T, x->right, profundidaer + 1);
}

void imprimeRB(RBTree *T){
    printf("Arvore: \n");
    if(T->root == T->nil){
        printf(" (vazia) \n");
    } else {
        imprimeNo(T, T->root, 0);
    }
}

void emOrdemAux(RBTree *T, Node *x, int *primeiro){
    if (x == T->nil){
        return;
    }
    emOrdemAux(T, x->left, primeiro);
    if(!*primeiro){
        printf(" ");
    }
    printf("%d(%c)", x->data, x->color == RED ? 'R' : 'B');
    *primeiro = 0;
    emOrdemAux(T, x->right, primeiro);
}

void emOrdem(RBTree *T){
    printf("Em ordem: ");
    if (T->root == T->nil){
        printf(" (vazia) \n");
        return;
    }
    int primeiro = 1;
    emOrdemAux(T, T->root, &primeiro);
    printf("\n");
}

void preOrdemAux(RBTree *T, Node *x, int *primeiro){
    if(x == T->nil){
        return;
    }
    if (!*primeiro){
        printf(" ");
    }
    printf("%d(%c)", x->data, x->color == RED ? 'R' : 'B');
    *primeiro = 0;
    preOrdemAux(T, x->left, primeiro);
    preOrdemAux(T, x->right, primeiro);
}

void preOrdem(RBTree *T){
    printf("Pre ordem: ");
    if(T->root == T->nil){
        printf(" (vazia) \n");
        return;
    }
    int primeiro = 1;
    preOrdemAux(T, T->root, &primeiro);
    printf("\n");
}