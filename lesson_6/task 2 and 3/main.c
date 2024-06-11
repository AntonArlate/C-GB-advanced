#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum
{
    false = 0,
    true = 1
} bool;

typedef struct tree
{
    int key;
    struct tree *left, *right;
    struct tree *parent;
} tree;

/*
ЗАДАНИЕ 2. Сумма всех элементов
Напишите программу, которая возвращает сумму всех элементов в дереве.
*/
void preorderAndSumm(tree *root, int *summ)
{
    if (root == NULL)
        return;
    *summ += root->key;
    if (root->left)
        preorderAndSumm(root->left, summ);
    if (root->right)
        preorderAndSumm(root->right, summ);
}

/*
ЗАДАНИЕ 3. Разница между элементами
Напишите программу, которая находит разницу между максимальным и минимальным элементом в дереве.
*/

int diffElement(tree *root)
{
    int max, min;

    if (root == NULL)
        return 0;

    tree *ptr;

    for (ptr = root; ptr->right != NULL; ptr = ptr->right)
    {
    }
    max = ptr->key;

    for (ptr = root; ptr->left != NULL; ptr = ptr->left)
    {
    }
    min = ptr->key;

    return max - min;
}

// Функции далее не относятся к заданиям
bool isBST(tree *root);
void insert(tree **root, int key, tree *pt);
void printTree(tree *root, int level);

int main()
{
    tree *tr = NULL;

    // Инициализация массива int
    int test_array[] = {10, 5, 15, 3, 7, 13, 18, 1, 6};
    int size = sizeof(test_array) / sizeof(test_array[0]); // Вычисление размера массива

    for (int i = 0; i < size; i++)
    {
        insert(&tr, test_array[i], NULL);
    }

    printTree(tr, 0);

    printf("Is BST? %s\n", isBST(tr) ? "YES" : "NO");

    // test task 1
    int summ = 0;
    preorderAndSumm(tr, &summ);
    printf("Summ all keys = %d\n", summ);

    // test task 2
    int diff = 0;
    diff = diffElement(tr);
    printf("MAX - MIN = %d\n", diff);

    system("\npause");
    return 0;
}

// проверка дерева
bool isBST(tree *root)
{
    static tree *prev = NULL;
    if (root) // пока функция вызывается с ненулевым root
    {
        // проваливаемся до левого листа
        if (!isBST(root->left)) // если левое поддерево вернуло ошибку
            return false;
        if (prev != NULL && root->key <= prev->key)
            return false;
        prev = root;
        return isBST(root->right); // делаем шаг на право
    }
    return true;
}

// вставка узла
void insert(tree **root, int key, tree *pt)
{
    if (!(*root))
    { // дерево пустое или дошли до нужного места
        *root = calloc(1, sizeof(tree));
        (*root)->key = key;
        (*root)->parent = pt;
    }
    else if (key < (*root)->key)
        insert(&((*root)->left), key, *root);
    else
        insert(&((*root)->right), key, *root);
}

void printTree(tree *root, int level)
{
    if (root == NULL)
    {
        return;
    }

    // Вывод правого поддерева
    printTree(root->right, level + 1);

    // Вывод текущего узла
    for (int i = 0; i < level; i++)
    {
        printf("    ");
    }
    if (root->parent != NULL && root == root->parent->right)
    {
        printf("┌── %d\n", root->key);
    }
    else
    {
        printf("└── %d\n", root->key);
    }

    // Вывод левого поддерева
    printTree(root->left, level + 1);
}

