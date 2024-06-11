/*
Задание 1. Поиск первого и последнего слова

Используя код задачи 1, написать программу поиска первого и последнего слова в строке.

Пример №1
Данные на входе: efg abcd zzz.
Данные на выходе: efg zzz.

Пример №2
Данные на входе: fffff kkkkkk a.
Данные на выходе: fffff a.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define STR_SIZE 2000
typedef struct list
{
    char *word;
    struct list *next;
} list;

// (решение задачи) Функция поиска первого и последнего слова
void findFirstAndLastWords(list *head, char **firstWord, char **lastWord)
{
    if (head == NULL)
    {
        *firstWord = NULL;
        *lastWord = NULL;
        return;
    }

    *lastWord = head->word; // так как список слов организуется по принципу стека

    list *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }

    *firstWord = current->word;
}

/* Без заглавного элемента и без двойного
указателя*/
list *insert2(char *value, list *head);
void print_list(list *head);
void choose_sort_array_list(list *head);
void swap_elements(list *e1, list *e2);

int main(int argc, char **argv)
{
    list *w_list = NULL;
    char word[STR_SIZE] = "";
    int ch = ' ';
    int i = 0;
    while (EOF != (ch = getchar()))
    {
        switch (ch)
        {
        case '.':
        case ' ':
            word[i] = '\0';
            w_list = insert2(word, w_list);
            i = 0;
            break;
        default:
            word[i++] = ch;
            if (i >= STR_SIZE)
                i = 0; // Защита от переполнения строки
        }
        if ('.' == ch)
            break;
    }

    char *firstWord, *lastWord;
    findFirstAndLastWords(w_list, &firstWord, &lastWord);

    printf("First word: %s\n", firstWord);
    printf("Last word: %s\n", lastWord);
    return 0;
}

void choose_sort_array_list(list *head)
{
    for (list *i = head; i->next != NULL; i = i->next) // начинаем со слова которое будет отмечено как i
    {
        list *min = i;
        for (list *j = i->next; j != NULL; j = j->next) // ищем минимум в последовательности
            if (strcmp(j->word, min->word) < 0)         // сравнение текущего слова j с минимальным
                min = j;
        if (min != i)              // если минимум не равен текущему i
            swap_elements(i, min); // переставляем
    }
}
/* Без заглавного элемента и без двойного указателя*/
list *insert2(char *value, list *head)
{
    list *res = (list *)calloc(1, sizeof(list));
    int len = strlen(value);     // определяем размер строки
    res->word = malloc(len + 1); // выделяем память под строку
    strcpy(res->word, value);    // копируем строку в память
    res->next = head;            // смещаем указатель на следующий
    return res;
}
void swap_elements(list *e1, list *e2)
{
    char *tmp = e1->word; // просто меняем указатели на строки
    e1->word = e2->word;
    e2->word = tmp;
}
void print_list(list *head)
{
    for (list *i = head; i != NULL; i = i->next)
        printf("%s ", i->word);
    putchar('\n');
}
