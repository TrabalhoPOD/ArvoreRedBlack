#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

void inserir(RBTree *T, int valor);
void remover(RBTree *T, int valor);
void buscarValor(RBTree *T, int valor);
void imprimirArvore(RBTree *T);
void imprimirEmOrdem(RBTree *T);
void imprimirPreOrdem(RBTree *T);
void validar(RBTree *T);
void corromper(RBTree *T);
void limparBuffer();

int main()
{
    // cria a arvore
    RBTree *T = createTree();

    // variaveis pra controlar a entrada do terminal
    char comando;
    int valor;

    // loop principal do programa
    while (1)
    {   
        // escaneia o comando do usuario
        if (scanf(" %c", &comando) != 1)
            break;
        // switch para lidar com todos os comandos
        switch (comando)
        {
        case 'I':
            if (scanf("%d", &valor) != 1)
            {
            printf("Valor invalido.\n");
            limparBuffer();
            break;
            }
            inserir(T, valor);
            break;

        case 'R':
            if (scanf("%d", &valor) != 1)
            {
            printf("Valor invalido.\n");
            limparBuffer();
            break;
            }
            remover(T, valor);
            break;

        case 'B':
            if (scanf("%d", &valor) != 1)
            {
            printf("Valor invalido.\n");
            limparBuffer();
            break;
            }
            buscarValor(T, valor);
            break;

        case 'P':
            imprimirArvore(T);
            break;

        case 'E':
            imprimirEmOrdem(T);
            break;

        case 'O':
            imprimirPreOrdem(T);
            break;

        case 'V':
            validar(T);
            break;

        case 'F':
            printf("Programa encerrado.\n");
            return 0;

        default:
            printf("Comando invalido.\n");
        }
    }

    return 0;
}

void inserir(RBTree *T, int valor)
{
    rbTreeInsert(T, valor);
    printf("Inserido: %d\n", valor);
}

void remover(RBTree *T, int valor)
{
    rbTreeDelete(T, valor);
    printf("Removido: %d\n", valor);
}

void buscarValor(RBTree *T, int valor)
{
    Node *resultado = search(T, T->root, valor);

    if (resultado != T->nil)
        printf("Busca %d: encontrado\n", valor);
    else
        printf("Busca %d: nao encontrado\n", valor);
}

void imprimirArvore(RBTree *T)
{
    imprimeRB(T);
}

void imprimirEmOrdem(RBTree *T)
{
    emOrdem(T);
    printf("\n");
}

void imprimirPreOrdem(RBTree *T)
{
    preOrdem(T);
    printf("\n");
}

void validar(RBTree *T)
{
    validaArvore(T);
    printf("\n");
}

void corromper(RBTree *T)
{
    corromperArvore(T);
}

// função para remover o bug de o programa devolver "comando invalido" logo apos "valor invalido"
void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


/*
// Exemplo de entrada

I 10
I 20
I 30
P
E
B 20
R 20
P
V
F

// Saida esperada do exemplo

Inserido: 10
Inserido: 20
Inserido: 30
Arvore:
20(B)
  10(R)
  30(R)
Em ordem: 10(R) 20(B) 30(R)
Busca 20: encontrado
Removido: 20
Arvore:
30(B)
  10(R)
Valida: sim


// teste mais pesado 

I 100
I 50
I 150
I 25
I 75
I 125
I 175
I 10
I 30
I 60
I 90
I 110
I 140
I 160
I 190
R 25
R 150
R 90
R 10
V
P
E
O
F


*/