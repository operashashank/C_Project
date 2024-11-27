#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define filenames for each service provider's contact storage
#define FILE_AIRTEL "airtel.txt"
#define FILE_JIO "jio.txt"
#define FILE_VI "vi.txt"

// Structure to hold contact details
typedef struct {
    char name[50];  // Contact's name
    char number[15]; // Contact's phone number
} Contact;

// Function declarations
void addContact(const char *filename);
void viewAllContacts(const char *filename);
void searchContact(const char *filename);
void modifyContact(const char *filename);
void deactivateContact(const char *filename);
void portNumber(const char *fromFile, const char *toFile);
void chooseProvider();

int main() {
    int choice;

    // Main menu
    do {
        printf("\n=== Contact Management System ===\n");
        printf("1. Airtel\n");
        printf("2. JIO\n");
        printf("3. VI\n");
        printf("4. Port Number\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:  // Airtel menu
            case 2:  // JIO menu
            case 3:  // VI menu
                chooseProvider(choice);
                break;
            case 4: {  // Port number between providers
                int from, to;
                printf("\nPort Number - Choose From Provider:\n");
                printf("1. Airtel\n2. JIO\n3. VI\n");
                printf("Your choice: ");
                scanf("%d", &from);

                printf("\nPort Number - Choose To Provider:\n");
                printf("1. Airtel\n2. JIO\n3. VI\n");
                printf("Your choice: ");
                scanf("%d", &to);

                if (from != to) {
                    // Map the provider choice to the respective file
                    const char *fromFile = from == 1 ? FILE_AIRTEL : (from == 2 ? FILE_JIO : FILE_VI);
                    const char *toFile = to == 1 ? FILE_AIRTEL : (to == 2 ? FILE_JIO : FILE_VI);
                    portNumber(fromFile, toFile);
                } else {
                    printf("\nCannot port to the same provider!\n");
                }
                break;
            }
            case 5:  // Exit the program
                printf("\nExiting program. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice! Try again.\n");
        }
    } while (choice != 5);

    return 0;
}

// Function to handle menu options for each provider
void chooseProvider(int choice) {
    const char *filename = choice == 1 ? FILE_AIRTEL : (choice == 2 ? FILE_JIO : FILE_VI);
    int option;

    do {
        printf("\n=== Manage %s Contacts ===\n", filename == FILE_AIRTEL ? "Airtel" : (filename == FILE_JIO ? "JIO" : "VI"));
        printf("1. Add Contact\n");
        printf("2. View All Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Modify Contact\n");
        printf("5. Deactivate Contact\n");
        printf("6. Go Back\n");
        printf("Choose an option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:  // Add a new contact
                addContact(filename);
                break;
            case 2:  // View all contacts
                viewAllContacts(filename);
                break;
            case 3:  // Search for a specific contact
                searchContact(filename);
                break;
            case 4:  // Modify a contact
                modifyContact(filename);
                break;
            case 5:  // Deactivate (delete) a contact
                deactivateContact(filename);
                break;
            case 6:  // Go back to the main menu
                printf("\nReturning to main menu.\n");
                break;
            default:
                printf("\nInvalid choice! Try again.\n");
        }
    } while (option != 6);
}

// Function to add a new contact to a file
void addContact(const char *filename) {
    Contact contact;
    FILE *file = fopen(filename, "a");  // Open the file in append mode

    if (!file) {
        perror("Error opening file");
        return;
    }

    printf("\nEnter name: ");
    scanf("%s", contact.name);
    printf("Enter number: ");
    scanf("%s", contact.number);

    // Write the contact details to the file
    fprintf(file, "%s %s\n", contact.name, contact.number);
    fclose(file);

    printf("\nContact added successfully!\n");
}

// Function to display all contacts in a file
void viewAllContacts(const char *filename) {
    FILE *file = fopen(filename, "r");
    Contact contact;

    if (!file) {
        perror("Error opening file");
        return;
    }

    printf("\nContacts in %s:\n", filename);
    while (fscanf(file, "%s %s", contact.name, contact.number) == 2) {
        printf("Name: %s, Number: %s\n", contact.name, contact.number);
    }

    fclose(file);
}

// Function to search for a contact by number
void searchContact(const char *filename) {
    FILE *file = fopen(filename, "r");
    Contact contact;
    char searchNumber[15];
    int found = 0;

    if (!file) {
        perror("Error opening file");
        return;
    }

    printf("\nEnter number to search: ");
    scanf("%s", searchNumber);

    while (fscanf(file, "%s %s", contact.name, contact.number) == 2) {
        if (strcmp(contact.number, searchNumber) == 0) {
            printf("Name: %s, Number: %s\n", contact.name, contact.number);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Contact not found!\n");
    }

    fclose(file);
}

// Function to modify a contact's details
void modifyContact(const char *filename) {
    FILE *file = fopen(filename, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    Contact contact;
    char searchNumber[15];
    int found = 0;

    if (!file || !tempFile) {
        perror("Error opening file");
        return;
    }

    printf("\nEnter number to modify: ");
    scanf("%s", searchNumber);

    // Copy contacts to a temporary file, modifying the target contact
    while (fscanf(file, "%s %s", contact.name, contact.number) == 2) {
        if (strcmp(contact.number, searchNumber) == 0) {
            printf("Enter new name: ");
            scanf("%s", contact.name);
            printf("Enter new number: ");
            scanf("%s", contact.number);
            found = 1;
        }
        fprintf(tempFile, "%s %s\n", contact.name, contact.number);
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the updated temporary file
    remove(filename);
    rename("temp.txt", filename);

    if (found) {
        printf("\nContact modified successfully!\n");
    } else {
        printf("\nContact not found!\n");
    }
}

// Function to deactivate (delete) a contact
void deactivateContact(const char *filename) {
    FILE *file = fopen(filename, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    Contact contact;
    char searchNumber[15];
    int found = 0;

    if (!file || !tempFile) {
        perror("Error opening file");
        return;
    }

    printf("\nEnter number to deactivate: ");
    scanf("%s", searchNumber);

    while (fscanf(file, "%s %s", contact.name, contact.number) == 2) {
        if (strcmp(contact.number, searchNumber) == 0) {
            found = 1;  // Skip writing the contact to the temporary file
            continue;
        }
        fprintf(tempFile, "%s %s\n", contact.name, contact.number);
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("temp.txt", filename);

    if (found) {
        printf("\nContact deactivated successfully!\n");
    } else {
        printf("\nContact not found!\n");
    }
}

// Function to port a number from one provider to another
void portNumber(const char *fromFile, const char *toFile) {
    FILE *file = fopen(fromFile, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    FILE *toFilePtr = fopen(toFile, "a");
    Contact contact;
    char searchNumber[15];
    int found = 0;

    if (!file || !tempFile || !toFilePtr) {
        perror("Error opening file");
        return;
    }

    printf("\nEnter number to port: ");
    scanf("%s", searchNumber);

    while (fscanf(file, "%s %s", contact.name, contact.number) == 2) {
        if (strcmp(contact.number, searchNumber) == 0) {
            fprintf(toFilePtr, "%s %s\n", contact.name, contact.number);
            found = 1;
            continue;
        }
        fprintf(tempFile, "%s %s\n", contact.name, contact.number);
    }

    fclose(file);
    fclose(tempFile);
    fclose(toFilePtr);

    remove(fromFile);
    rename("temp.txt", fromFile);

    if (found) {
        printf("\nNumber ported successfully!\n");
    } else {
        printf("\nContact not found!\n");
    }
}