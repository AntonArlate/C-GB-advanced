/*
Задание 4. Удаление элементов

Напишите программу, которая удаляет из списка все элементы с нечетным порядковым номером.

Пример №1
Данные на входе: aa ss dd ff gg hh jj kk ll.
Данные на выходе: aa dd gg jj ll

Пример №2
Данные на входе: aa ss dd ff gg hh jj kk ll tt.
Данные на выходе: aa dd gg jj ll
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

void insert3(char *value, list **head);
void print_list(list *head);

void delete_every_second(list *head)
{
    while (head != NULL)
    {
        if (head->next != NULL)
        {
            list *del = head->next;
            head->next = head->next->next;
            free(del->word);
            free(del);
        } 
        head = head->next;
    }
}

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
            insert3(word, &w_list);
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

    delete_every_second(w_list);
    print_list(w_list);
    return 0;
}

void insert3(char *value, list **head){
    list *res = (list *)calloc(1, sizeof(list));
    int len = strlen(value);     // определяем размер строки
    res->word = malloc(len + 1); // выделяем память под строку
    strcpy(res->word, value);    // копируем строку в память

    if (*head == NULL)
    {
        *head = res;
        return;
    }

    list *ptr;
    for (ptr = *head; ptr->next != NULL; ptr = ptr->next)
    {
    }
    ptr->next = res;
}

void print_list(list *head)
{
    for (list *i = head; i != NULL; i = i->next)
        printf("%s ", i->word);
    putchar('\n');
}
