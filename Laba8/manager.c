#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_RECORDS 100
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50

typedef struct {
    char name[MAX_NAME_LENGTH];
    char username[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} Record;

extern void generate_password(char *password, int length);
extern void swap(unsigned char *a, unsigned char *b);
extern void rc4_init(unsigned char *key, int key_length, unsigned char *S);
extern void rc4_crypt(unsigned char *data, int data_length, unsigned char *S);

void add_record(Record *records, int *num_records) 
{
    if (*num_records >= MAX_RECORDS) 
    {
        printf("Error: Maximum number of records reached.\n");
        return;
    }
    // Add record to array of structures
    Record new_record;
    printf("Enter the name of the application or website: "); 
    fgets(new_record.name, MAX_NAME_LENGTH, stdin);

    new_record.name[strcspn(new_record.name, "\n")] = '\0';
    printf("Enter the username: ");
    fgets(new_record.username, MAX_NAME_LENGTH, stdin);

    new_record.username[strcspn(new_record.username, "\n")] = '\0';
    printf("Enter the password (or leave blank to generate one): ");

    fgets(new_record.password, MAX_PASSWORD_LENGTH, stdin);
    new_record.password[strcspn(new_record.password, "\n")] = '\0';

    if (strlen(new_record.password) == 0) 
    {
        generate_password(new_record.password, 12);
        printf("Generated password: %s\n", new_record.password);
    }
    records[*num_records] = new_record;
    (*num_records)++;
}

void delete_record(Record *records, int *num_records) 
{
    if (*num_records == 0) 
    {
        printf("Error: No records to delete.\n");
        return;
    }

    printf("Enter the name of the record to delete: ");
    char name[MAX_NAME_LENGTH];
    fgets(name, MAX_NAME_LENGTH, stdin);
    name[strcspn(name, "\n")] = '\0';
    int index = -1;
    
    for (int i = 0; i < *num_records; i++) 
    {
        if (strcmp(records[i].name, name) == 0) 
        {
            index = i;
            break;
        }
    }

    if (index == -1) 
    {
        printf("Error: Record not found.\n");
        return;
    }

    for (int i = index; i < *num_records - 1; i++) 
    {
        records[i] = records[i + 1];
    }
    (*num_records)--;
}

void edit_record(Record *records, int num_records) 
{
    if (num_records == 0) 
    {
        printf("Error: No records to edit.\n");
        return;
    }

    printf("Enter the name of the record to edit: ");
    char name[MAX_NAME_LENGTH];
    fgets(name, MAX_NAME_LENGTH, stdin);
    name[strcspn(name, "\n")] = '\0';
    int index = -1;

    for (int i = 0; i < num_records; i++) 
    {
        if (strcmp(records[i].name, name) == 0) 
        {
            index = i;
            break;
        }
    }

    if (index == -1) 
    {
        printf("Error: Record not found.\n");
        return;
    }

    Record *record = &records[index];
    printf("Enter the new name of the application or website (or leave blank to keep \"%s\"): ", record->name);
    char new_name[MAX_NAME_LENGTH];
    fgets(new_name, MAX_NAME_LENGTH, stdin);
    new_name[strcspn(new_name, "\n")] = '\0';

    if (strlen(new_name) > 0) 
    {
        strcpy(record->name, new_name);
    }

    printf("Enter the new username (or leave blank to keep \"%s\"): ", record->username);
    char new_username[MAX_NAME_LENGTH];
    fgets(new_username, MAX_NAME_LENGTH, stdin);
    new_username[strcspn(new_username, "\n")] = '\0';

    if (strlen(new_username) > 0) 
    {
        strcpy(record->username, new_username);
    }

    printf("Enter the new password (or leave blank to keep \"%s\"): ", record->password);
    char new_password[MAX_PASSWORD_LENGTH];
    fgets(new_password, MAX_PASSWORD_LENGTH, stdin);
    new_password[strcspn(new_password, "\n")] = '\0';
    new_password[strcspn(new_password, "\n")] = '\0';

    if (strlen(new_password) > 0) 
    {
        strcpy(record->password, new_password);
    }
}

void print_records(Record *records, int num_records) 
{
    char key[] = "mysecretkey";
    printf("Index\tName\tUsername\tPassword\n");

    if (num_records == 0) 
    {
        printf("No records found.\n");
        return;
    }

    for (int i = 0; i < num_records; i++) 
    {
        printf("%d\t%s\t%s\t%s\n", i, records[i].name, records[i].username, records[i].password);
    }
}

int main() 
{
    char key[] = "";
    Record records[MAX_RECORDS];
    int num_records = 0;
    unsigned char S[256];

    // Load data from file into array of structures
    FILE *file = fopen("data.txt", "r");
    if (file != NULL) 
    {
        while (num_records < MAX_RECORDS && fscanf(file, "%s %s %s", records[num_records].name, records[num_records].username, records[num_records].password) == 3) {
            unsigned char S[256];
            char key[] = "mysecretkey";

            rc4_init(key, strlen(key), S);
            rc4_crypt(records[num_records].name, strlen(records[num_records].name), S);

            rc4_init(key, strlen(key), S);
            rc4_crypt(records[num_records].username, strlen(records[num_records].username), S);

            rc4_init(key, strlen(key), S);
            rc4_crypt(records[num_records].password, strlen(records[num_records].password), S);
           
            num_records++;
        }
        fclose(file);
    }

    // Main menu loop
    int choice;
    time_t last_activity_time = time(NULL);
    while (1) 
    {
        printf("\nMain Menu\n");
        printf("------------------\n");
        printf("1. Add record\n");
        printf("2. Delete record\n");
        printf("3. Edit record\n");
        printf("4. Generate password\n");
        printf("5. Print records\n");
        printf("6. Save and exit\n");
        printf("------------------\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar(); // consume newline character
        time_t current_time = time(NULL);

        if (current_time - last_activity_time > 5) 
        { // 5 minute timeout
            printf("Timeout: Program has been inactive for 5 minutes. Exiting...\n");
            break;
        }

        switch (choice) 
        {
            case 1:
                system("clear");
                add_record(records, &num_records);
                break;
            case 2:
                system("clear");
                delete_record(records, &num_records);
                break;
            case 3:
                system("clear");
                edit_record(records, num_records);
                break;
            case 4:
                system("clear");
                printf("Enter the length of the password to generate: ");
                int length;
                scanf("%d", &length);
                getchar(); // consume newline character
                char password[MAX_PASSWORD_LENGTH];
                generate_password(password, length);
                printf("Generated password: %s\n", password);
                break;
            case 5:
                system("clear");
                print_records(records, num_records);
                break;
            case 6:
                system("clear");
                // Save data from array of structures into file
                file = fopen("data.txt", "w");
                if (file != NULL) {

                    for (int i = 0; i < num_records; i++) 
                    {
                        char key[] = "mysecretkey";
                        rc4_init(key, strlen(key), S);
                        rc4_crypt(records[i].name, strlen(records[i].name), S);

                        rc4_init(key, strlen(key), S);
                        rc4_crypt(records[i].username, strlen(records[i].username), S);

                        rc4_init(key, strlen(key), S);
                        rc4_crypt(records[i].password, strlen(records[i].password), S);
                        fprintf(file, "%s %s %s\n", records[i].name, records[i].username, records[i].password);
                    }
                    fclose(file);
                }
                printf("Exiting...\n");
                return 0;
            default:
                printf("Error: Invalid choice.\n");
                break;
        }
        last_activity_time = current_time;
    }
    return 0;
}
