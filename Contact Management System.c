#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME 50
#define MAX_PHONE 15
#define FILENAME "contacts.dat"

typedef struct Contact {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    struct Contact* prev;
    struct Contact* next;
} Contact;

Contact* head = NULL;
Contact* tail = NULL;

// Function prototypes
void addContact();
void displayContacts();
void searchContact();
void deleteContact();
void saveToFile();
void loadFromFile();
void freeList();

int main() {
    loadFromFile();

    int choice;
    do {
        printf("\nContact Management System\n");
        printf("1. Add Contact\n");
        printf("2. Display All Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear newline

        switch(choice) {
            case 1: addContact(); break;
            case 2: displayContacts(); break;
            case 3: searchContact(); break;
            case 4: deleteContact(); break;
            case 5: saveToFile(); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 5);

    freeList();
    return 0;
}

void addContact() {
    Contact* newContact = (Contact*)malloc(sizeof(Contact));

    printf("Enter name: ");
    fgets(newContact->name, MAX_NAME, stdin);
    newContact->name[strcspn(newContact->name, "\n")] = '\0';

    printf("Enter phone: ");
    fgets(newContact->phone, MAX_PHONE, stdin);
    newContact->phone[strcspn(newContact->phone, "\n")] = '\0';

    newContact->next = NULL;

    if(head == NULL) {
        newContact->prev = NULL;
        head = tail = newContact;
    } else {
        newContact->prev = tail;
        tail->next = newContact;
        tail = newContact;
    }

    printf("Contact added successfully!\n");
}

void displayContacts() {
    if(head == NULL) {
        printf("No contacts to display!\n");
        return;
    }

    Contact* current = head;
    printf("\nContacts List:\n");
    printf("%-30s %-15s\n", "Name", "Phone");
    printf("------------------------------ ---------------\n");

    while(current != NULL) {
        printf("%-30s %-15s\n", current->name, current->phone);
        current = current->next;
    }
}

void searchContact() {
    if(head == NULL) {
        printf("No contacts to search!\n");
        return;
    }

    char query[MAX_NAME];
    printf("Enter name to search: ");
    fgets(query, MAX_NAME, stdin);
    query[strcspn(query, "\n")] = '\0';

    Contact* current = head;
    bool found = false;

    while(current != NULL) {
        if(strcmp(current->name, query) == 0) {
            printf("\nContact Found:\n");
            printf("Name: %s\n", current->name);
            printf("Phone: %s\n", current->phone);
            found = true;
            break;
        }
        current = current->next;
    }

    if(!found) {
        printf("Contact not found!\n");
    }
}

void deleteContact() {
    if(head == NULL) {
        printf("No contacts to delete!\n");
        return;
    }

    char query[MAX_NAME];
    printf("Enter name to delete: ");
    fgets(query, MAX_NAME, stdin);
    query[strcspn(query, "\n")] = '\0';

    Contact* current = head;
    bool found = false;

    while(current != NULL) {
        if(strcmp(current->name, query) == 0) {
            if(current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                head = current->next;
            }

            if(current->next != NULL) {
                current->next->prev = current->prev;
            } else {
                tail = current->prev;
            }

            free(current);
            printf("Contact deleted successfully!\n");
            found = true;
            break;
        }
        current = current->next;
    }

    if(!found) {
        printf("Contact not found!\n");
    }
}

void saveToFile() {
    FILE* file = fopen(FILENAME, "wb");
    if(file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    Contact* current = head;
    while(current != NULL) {
        fwrite(current, sizeof(Contact), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("Contacts saved to file!\n");
}

void loadFromFile() {
    FILE* file = fopen(FILENAME, "rb");
    if(file == NULL) return;

    Contact temp;
    while(fread(&temp, sizeof(Contact), 1, file) == 1) {
        Contact* newContact = (Contact*)malloc(sizeof(Contact));
        *newContact = temp;
        newContact->prev = newContact->next = NULL;

        if(head == NULL) {
            head = tail = newContact;
        } else {
            newContact->prev = tail;
            tail->next = newContact;
            tail = newContact;
        }
    }

    fclose(file);
}

void freeList() {
    Contact* current = head;
    while(current != NULL) {
        Contact* temp = current;
        current = current->next;
        free(temp);
    }
    head = tail = NULL;
}
