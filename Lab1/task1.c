#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// adjust formatting with 255 thing or 999

typedef struct
{
    char book_id[1000];
    char title[1000];
    char author[1000];
} Book;

void write_data(Book *books, int count, const char *filename)
{
    FILE *file = fopen(filename, "a");
    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s\t%s\t%s\n", books[i].book_id, books[i].title, books[i].author);
    }

    fclose(file);
}

int load_data(const char *filename, Book **books)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file for reading.\n");
        return 0;
    }

    int count = 0;
    int capacity = 10;
    *books = (Book *)malloc(capacity * sizeof(Book));

    while (fscanf(file, "%999s\t%999[^\t]\t%999[^\n]", (*books)[count].book_id, (*books)[count].title, (*books)[count].author) == 3)
    {
        count++;
        if (count >= capacity)
        {
            capacity *= 2; // Double the capacity
            *books = (Book *)realloc(*books, capacity * sizeof(Book));
        }
    }

    fclose(file);
    return count;
}

Book *search_by_id(Book *books, int count, char *id)
{
    for (int i = 0; i < count; i++)
    {
        if (books[i].book_id == id)
        {
            return &books[i];
        }
    }
    return NULL;
}

void print_books(Book *books, int count)
{
    printf("ID\tTitle\tAuthor\n");
    for (int i = 0; i < count; i++)
    {
        printf("%s\t%s\t%s\n", books[i].book_id, books[i].title, books[i].author);
    }
}

int main()
{
    Book *books = NULL;
    int book_count = 0;
    char filename[1000] = "";
    char option;
    char id[1000] = "";

    do
    {
        printf("\nChoose an option:\n");
        printf("a) Load data\n");
        printf("b) Enter data\n");
        printf("c) Search by id\n");
        printf("d) Exit the program\n");
        scanf(" %c", &option);

        switch (option)
        {
        case 'a':
            system("clear");
            free(books);
            books = NULL;
            book_count = 0;
            printf("Enter the filename to load data from: ");
            scanf("%s", filename);

            book_count = load_data(filename, &books);
            if (book_count == 0)
            {
                printf("No data loaded.\n");
            }
            else
            {
                print_books(books, book_count);
            }

            break;
        case 'b':

            free(books);
            books = NULL;
            book_count = 0;

            printf("Enter the filename to save data to: ");
            scanf("%s", filename);

            // Dynamically allocate memory for books if not already allocated
            if (books == NULL)
            {
                books = (Book *)malloc(sizeof(Book)); // Initial allocation for 1 book
                if (books == NULL)
                {
                    fprintf(stderr, "Memory allocation failed.\n");
                    exit(1);
                }
                book_count = 0;
            }

            while (1)
            {
                // Temporarily store book information to check for the exit condition
                char temp_id[1000], temp_title[1000], temp_author[1000];

                printf("Enter book ID, title, and author, or '.' to finish:\n");
                printf("ID: ");
                scanf("%s", temp_id);
                if (strcmp(temp_id, ".") == 0)
                {
                    break; // Exit without incrementing book_count or saving the last entry
                }

                // Only proceed with storing the book data if it's not the exit condition
                printf("Title: ");
                scanf(" %[^\n]", temp_title); // Note the space before % to ensure leading whitespace is consumed
                printf("Author: ");
                scanf(" %[^\n]", temp_author);

                // Check for reallocation necessity and reallocate if needed
                Book *temp_books = realloc(books, (book_count + 1) * sizeof(Book));
                if (temp_books == NULL)
                {
                    fprintf(stderr, "Memory reallocation failed.\n");
                    break; // Exit the loop if memory allocation fails
                }
                books = temp_books;

                // Now it's safe to copy temp data into the books array
                strcpy(books[book_count].book_id, temp_id);
                strcpy(books[book_count].title, temp_title);
                strcpy(books[book_count].author, temp_author);

                book_count++;
            }

            if (book_count > 0)
            {
                write_data(books, book_count, filename);
            }

            break;

        case 'c':
            system("clear");
            printf("Enter book ID to search: ");
            scanf("%s", id);
            Book *book = search_by_id(books, book_count, id);
            if (book)
            {
                printf("Book found: ID = %s, Title = %s, Author = %s\n", book->book_id, book->title, book->author);
            }
            else
            {
                printf("No book found with ID %s\n", id);
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

    return 0;
}
