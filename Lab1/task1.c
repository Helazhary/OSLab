#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book {
    char book_id[1000];
    char title[1000];
    char author[1000];
    struct Book *next;
} Book;

void write_data(Book *head, const char *filename) {
    FILE *file = fopen(filename, "a"); //Im assuming that if the file is already created then the user can add more books to it
    Book *current = head;
    while (current != NULL) {
        fprintf(file, "%s\t%s\t%s\n", current->book_id, current->title, current->author);
        current = current->next;
    }
    fclose(file);
}

int load_data(const char *filename, Book **head) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return 0;
    }

    *head = NULL;
    Book *last = NULL;
    int count = 0;
    while (1) {
        Book *new_book = (Book *)malloc(sizeof(Book));
        if (fscanf(file, "%s\t%[^\t]\t%[^\n]", new_book->book_id, new_book->title, new_book->author) != 3) {
            free(new_book);
            break;
        }
        new_book->next = NULL;

        if (*head == NULL) {
            *head = new_book;
        } else {
            last->next = new_book;
        }
        last = new_book;
        count++;
    }

    fclose(file);
    return count;
}

Book *search_by_id(Book *head, char *id) {
    Book *current = head;
    while (current != NULL) {
        if (strcmp(current->book_id, id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void print_books(Book *head) {
    printf("ID\tTitle\tAuthor\n");
    Book *current = head;
    while (current != NULL) {
        printf("%s\t%s\t%s\n", current->book_id, current->title, current->author);
        current = current->next;
    }
}

void free_books(Book **head) {
    Book *current = *head;
    while (current != NULL) {
        Book *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

int main() {
    Book *head = NULL;
    int book_count = 0;
    char filename[1000] = "";
    char option;
    char id[1000] = "";

    do {
        printf("\nChoose an option:\n");
        printf("a) Load data\n");
        printf("b) Enter data\n");
        printf("c) Search by id\n");
        printf("d) Exit the program\n");
        scanf(" %c", &option);

        switch (option) {
        case 'a':
            system("clear");
            free_books(&head);
            printf("Enter the filename to load data from: ");
            scanf("%s", filename);

            book_count = load_data(filename, &head);
            if (book_count == 0) {
                printf("No data loaded.\n");
            } else {
                print_books(head);
            }
            break;
        case 'b':
            printf("Enter the filename to save data to: ");
            scanf("%s", filename);

            while (1) {
                char temp_id[1000], temp_title[1000], temp_author[1000];
                printf("Enter book ID, title, and author, or '.' to finish:\n");
                printf("ID: ");
                scanf("%s", temp_id);
                if (strcmp(temp_id, ".") == 0) {
                    break; 
                }

                printf("Title: ");
                scanf(" %[^\n]", temp_title);
                printf("Author: ");
                scanf(" %[^\n]", temp_author); 

                Book *new_book = (Book *)malloc(sizeof(Book));
                if (new_book == NULL) {
                    fprintf(stderr, "Erorr 4.\n");
                    exit(4); 
                }
                strcpy(new_book->book_id, temp_id);
                strcpy(new_book->title, temp_title);
                strcpy(new_book->author, temp_author);
                new_book->next = NULL;

                if (head == NULL) {
                    head = new_book; 
                } else {
                    Book *current = head;
                    while (current->next != NULL) {
                        current = current->next;
                    }
                    current->next = new_book; 
                }
                book_count++;
            }

            if (book_count > 0) {
                write_data(head, filename);
            }

            break;
                case 'c':
            system("clear");
            free_books(&head);
            printf("Enter the filename to load data from: ");
            scanf("%s", filename);

            book_count = load_data(filename, &head);
            if (book_count == 0) {
                printf("No data loaded.\n");
            } else {
                print_books(head); 
                printf("Enter book ID to search: ");
                scanf("%s", id);
                Book *book = search_by_id(head, id);
                if (book) {
                    system("clear");
                    printf("Book found: ID = %s, Title = %s, Author = %s\n", book->book_id, book->title, book->author);
                } else {
                    system("clear");
                    printf("No book found with ID %s\n", id);
                }
            }
            break;
        case 'd':
            system("clear");
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid option.\n");
            break;
        }
    } while (option != 'd');

    free_books(&head);
    return 0;
}
