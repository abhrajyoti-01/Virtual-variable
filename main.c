#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VARIABLES 100
#define MAX_NAME_LEN 50
#define MAX_VALUE_LEN 100

// Function prototypes
int find_variable(const char* name);
float to_float(const char* value);
int to_int(const char* value);

typedef struct {
    char name[MAX_NAME_LEN];
    char data_type[10];
    char value[MAX_VALUE_LEN];
} VirtualVariable;

VirtualVariable virtual_variables[MAX_VARIABLES];
int variable_count = 0;

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int is_valid_name(const char *name) {
    if (!isalpha(name[0])) {
        return 0;
    }
    for (int i = 1; name[i] != '\0'; i++) {
        if (!isalnum(name[i])) {
            return 0;
        }
    }
    return 1;
}

void create() {
    if (variable_count >= MAX_VARIABLES) {
        printf("Maximum number of variables reached.\n");
        return;
    }

    char name[MAX_NAME_LEN];
    while (1) {
        printf("Enter the name of the virtual variable: ");
        scanf("%s", name);
        if (is_valid_name(name)) {
            break;
        } else {
            printf("Invalid name. The name must start with a letter and can only contain letters and numbers.\n");
        }
    }

    char data_type[10];
    printf("Enter the data type (int, float, str, bool): ");
    scanf("%s", data_type);

    if (strcmp(data_type, "int") != 0 && strcmp(data_type, "float") != 0 && strcmp(data_type, "str") != 0 && strcmp(data_type, "bool") != 0) {
        printf("Invalid data type. Using str as default.\n");
        strcpy(data_type, "str");
    }

    char value[MAX_VALUE_LEN];
    printf("Enter the value: ");
    scanf("%s", value);

    if (strcmp(data_type, "int") == 0) {
        for (int i = 0; value[i] != '\0'; i++) {
            if (!isdigit(value[i]) && (i != 0 || value[i] != '-')) {
                printf("Value cannot be converted to int. It will be stored as a string.\n");
                strcpy(data_type, "str");
                break;
            }
        }
    } else if (strcmp(data_type, "float") == 0) {
        int dot_count = 0;
        for (int i = 0; value[i] != '\0'; i++) {
            if (!isdigit(value[i]) && value[i] != '.' && (i != 0 || value[i] != '-')) {
                printf("Value cannot be converted to float. It will be stored as a string.\n");
                strcpy(data_type, "str");
                break;
            }
            if (value[i] == '.') {
                dot_count++;
            }
        }
        if (dot_count > 1) {
            printf("Value cannot be converted to float. It will be stored as a string.\n");
            strcpy(data_type, "str");
        }
    } else if (strcmp(data_type, "bool") == 0) {
        if (strcmp(value, "true") != 0 && strcmp(value, "false") != 0 && strcmp(value, "1") != 0 && strcmp(value, "0") != 0) {
            printf("Value cannot be converted to bool. It will be stored as a string.\n");
            strcpy(data_type, "str");
        }
    }

    strcpy(virtual_variables[variable_count].name, name);
    strcpy(virtual_variables[variable_count].data_type, data_type);
    strcpy(virtual_variables[variable_count].value, value);
    variable_count++;

    printf("Virtual variable '%s' created successfully.\n", name);
}

void search() {
    clear_screen();
    char name[MAX_NAME_LEN];
    printf("Enter the name of the virtual variable to search: ");
    scanf("%s", name);

    for (int i = 0; i < variable_count; i++) {
        if (strcmp(virtual_variables[i].name, name) == 0) {
            printf("Variable '%s' (%s): %s\n", virtual_variables[i].name, virtual_variables[i].data_type, virtual_variables[i].value);
            return;
        }
    }
    printf("Variable '%s' not found.\n", name);
}

void all() {
    clear_screen();
    if (variable_count == 0) {
        printf("No virtual variables found.\n");
        return;
    }

    printf("All virtual variables:\n");
    for (int i = 0; i < variable_count; i++) {
        printf("  '%s' (%s): %s\n", virtual_variables[i].name, virtual_variables[i].data_type, virtual_variables[i].value);
    }
}

void delete_variable() {
    clear_screen();
    char name[MAX_NAME_LEN];
    printf("Enter the name of the virtual variable to delete: ");
    scanf("%s", name);

    for (int i = 0; i < variable_count; i++) {
        if (strcmp(virtual_variables[i].name, name) == 0) {
            for (int j = i; j < variable_count - 1; j++) {
                strcpy(virtual_variables[j].name, virtual_variables[j+1].name);
                strcpy(virtual_variables[j].data_type, virtual_variables[j+1].data_type);
                strcpy(virtual_variables[j].value, virtual_variables[j+1].value);
            }
            variable_count--;
            printf("Variable '%s' deleted successfully.\n", name);
            return;
        }
    }
    printf("Variable '%s' not found.\n", name);
}

void update_variable() {
    clear_screen();
    char name[MAX_NAME_LEN];
    printf("Enter the name of the virtual variable to update: ");
    scanf("%s", name);

    for (int i = 0; i < variable_count; i++) {
        if (strcmp(virtual_variables[i].name, name) == 0) {
            char new_value[MAX_VALUE_LEN];
            printf("Current value: %s\n", virtual_variables[i].value);
            printf("Enter new value: ");
            scanf("%s", new_value);
            
            if (strcmp(virtual_variables[i].data_type, "int") == 0) {
                for (int j = 0; new_value[j] != '\0'; j++) {
                    if (!isdigit(new_value[j]) && (j != 0 || new_value[j] != '-')) {
                        printf("Value cannot be converted to int. Update canceled.\n");
                        return;
                    }
                }
            } else if (strcmp(virtual_variables[i].data_type, "float") == 0) {
                int dot_count = 0;
                for (int j = 0; new_value[j] != '\0'; j++) {
                    if (!isdigit(new_value[j]) && new_value[j] != '.' && (j != 0 || new_value[j] != '-')) {
                        printf("Value cannot be converted to float. Update canceled.\n");
                        return;
                    }
                    if (new_value[j] == '.') {
                        dot_count++;
                    }
                }
                if (dot_count > 1) {
                    printf("Value cannot be converted to float. Update canceled.\n");
                    return;
                }
            } else if (strcmp(virtual_variables[i].data_type, "bool") == 0) {
                if (strcmp(new_value, "true") != 0 && strcmp(new_value, "false") != 0 && 
                    strcmp(new_value, "1") != 0 && strcmp(new_value, "0") != 0) {
                    printf("Value cannot be converted to bool. Update canceled.\n");
                    return;
                }
            }
            
            strcpy(virtual_variables[i].value, new_value);
            printf("Variable '%s' updated successfully.\n", name);
            return;
        }
    }
    printf("Variable '%s' not found.\n", name);
}

void save_to_file() {
    char filename[100];
    printf("Enter the filename to save variables: ");
    scanf("%s", filename);
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    
    fprintf(file, "%d\n", variable_count);
    for (int i = 0; i < variable_count; i++) {
        fprintf(file, "%s\n%s\n%s\n", 
                virtual_variables[i].name,
                virtual_variables[i].data_type,
                virtual_variables[i].value);
    }
    
    fclose(file);
    printf("Variables saved to '%s' successfully.\n", filename);
}

void load_from_file() {
    char filename[100];
    printf("Enter the filename to load variables from: ");
    scanf("%s", filename);
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }
    
    int count;
    if (fscanf(file, "%d\n", &count) != 1) {
        printf("Error reading file format.\n");
        fclose(file);
        return;
    }
    
    variable_count = 0;
    
    for (int i = 0; i < count && i < MAX_VARIABLES; i++) {
        if (fscanf(file, "%49[^\n]\n%9[^\n]\n%99[^\n]\n", 
                virtual_variables[i].name,
                virtual_variables[i].data_type,
                virtual_variables[i].value) != 3) {
            printf("Error reading variable #%d from file.\n", i+1);
            break;
        }
        variable_count++;
    }
    
    fclose(file);
    printf("Loaded %d variables from '%s'.\n", variable_count, filename);
}

int find_variable(const char* name) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(virtual_variables[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

float to_float(const char* value) {
    return atof(value);
}

int to_int(const char* value) {
    return atoi(value);
}

void perform_operations() {
    clear_screen();
    if (variable_count < 2) {
        printf("Need at least two variables to perform operations.\n");
        return;
    }

    printf("Available operations:\n");
    printf("1. Addition (+)\n");
    printf("2. Subtraction (-)\n");
    printf("3. Multiplication (*)\n");
    printf("4. Division (/)\n");
    printf("5. String concatenation\n");
    printf("6. Length (for strings)\n");
    
    int op_choice;
    printf("Select operation: ");
    scanf("%d", &op_choice);
    
    char var1_name[MAX_NAME_LEN], var2_name[MAX_NAME_LEN];
    printf("Enter first variable name: ");
    scanf("%s", var1_name);
    
    int var1_idx = find_variable(var1_name);
    if (var1_idx == -1) {
        printf("Variable '%s' not found.\n", var1_name);
        return;
    }
    
    int var2_idx = -1;
    if (op_choice != 6) {
        printf("Enter second variable name: ");
        scanf("%s", var2_name);
        
        var2_idx = find_variable(var2_name);
        if (var2_idx == -1) {
            printf("Variable '%s' not found.\n", var2_name);
            return;
        }
    }
    
    char result_name[MAX_NAME_LEN];
    printf("Enter name for result variable: ");
    scanf("%s", result_name);
    
    int result_idx = find_variable(result_name);
    if (result_idx == -1) {
        if (variable_count >= MAX_VARIABLES) {
            printf("Maximum number of variables reached.\n");
            return;
        }
        result_idx = variable_count;
        variable_count++;
        strcpy(virtual_variables[result_idx].name, result_name);
    }
    
    switch (op_choice) {
        case 1:
            if (strcmp(virtual_variables[var1_idx].data_type, "int") == 0 && 
                strcmp(virtual_variables[var2_idx].data_type, "int") == 0) {
                
                int result = to_int(virtual_variables[var1_idx].value) + 
                             to_int(virtual_variables[var2_idx].value);
                sprintf(virtual_variables[result_idx].value, "%d", result);
                strcpy(virtual_variables[result_idx].data_type, "int");
                
            } else if ((strcmp(virtual_variables[var1_idx].data_type, "float") == 0 || 
                       strcmp(virtual_variables[var1_idx].data_type, "int") == 0) &&
                      (strcmp(virtual_variables[var2_idx].data_type, "float") == 0 ||
                       strcmp(virtual_variables[var2_idx].data_type, "int") == 0)) {
                
                float result = to_float(virtual_variables[var1_idx].value) + 
                               to_float(virtual_variables[var2_idx].value);
                sprintf(virtual_variables[result_idx].value, "%.6f", result);
                strcpy(virtual_variables[result_idx].data_type, "float");
                
            } else if (strcmp(virtual_variables[var1_idx].data_type, "str") == 0 ||
                       strcmp(virtual_variables[var2_idx].data_type, "str") == 0) {
                
                if (strlen(virtual_variables[var1_idx].value) + strlen(virtual_variables[var2_idx].value) >= MAX_VALUE_LEN) {
                    printf("Error: Result would exceed maximum string length.\n");
                    if (result_idx == variable_count - 1) {
                        variable_count--;
                    }
                    return;
                }
                
                strcpy(virtual_variables[result_idx].value, virtual_variables[var1_idx].value);
                strcat(virtual_variables[result_idx].value, virtual_variables[var2_idx].value);
                strcpy(virtual_variables[result_idx].data_type, "str");
                
            } else {
                printf("Operation not supported for these variable types.\n");
                if (result_idx == variable_count - 1) {
                    variable_count--;
                }
                return;
            }
            break;
            
        case 2:
            if (strcmp(virtual_variables[var1_idx].data_type, "int") == 0 && 
                strcmp(virtual_variables[var2_idx].data_type, "int") == 0) {
                
                int result = to_int(virtual_variables[var1_idx].value) - 
                             to_int(virtual_variables[var2_idx].value);
                sprintf(virtual_variables[result_idx].value, "%d", result);
                strcpy(virtual_variables[result_idx].data_type, "int");
                
            } else if ((strcmp(virtual_variables[var1_idx].data_type, "float") == 0 || 
                       strcmp(virtual_variables[var1_idx].data_type, "int") == 0) &&
                      (strcmp(virtual_variables[var2_idx].data_type, "float") == 0 ||
                       strcmp(virtual_variables[var2_idx].data_type, "int") == 0)) {
                
                float result = to_float(virtual_variables[var1_idx].value) - 
                               to_float(virtual_variables[var2_idx].value);
                sprintf(virtual_variables[result_idx].value, "%.6f", result);
                strcpy(virtual_variables[result_idx].data_type, "float");
                
            } else {
                printf("Operation not supported for these variable types.\n");
                if (result_idx == variable_count - 1) {
                    variable_count--;
                }
                return;
            }
            break;
            
        case 3:
            if (strcmp(virtual_variables[var1_idx].data_type, "int") == 0 && 
                strcmp(virtual_variables[var2_idx].data_type, "int") == 0) {
                
                int result = to_int(virtual_variables[var1_idx].value) * 
                             to_int(virtual_variables[var2_idx].value);
                sprintf(virtual_variables[result_idx].value, "%d", result);
                strcpy(virtual_variables[result_idx].data_type, "int");
                
            } else if ((strcmp(virtual_variables[var1_idx].data_type, "float") == 0 || 
                       strcmp(virtual_variables[var1_idx].data_type, "int") == 0) &&
                      (strcmp(virtual_variables[var2_idx].data_type, "float") == 0 ||
                       strcmp(virtual_variables[var2_idx].data_type, "int") == 0)) {
                
                float result = to_float(virtual_variables[var1_idx].value) * 
                               to_float(virtual_variables[var2_idx].value);
                sprintf(virtual_variables[result_idx].value, "%.6f", result);
                strcpy(virtual_variables[result_idx].data_type, "float");
                
            } else {
                printf("Operation not supported for these variable types.\n");
                if (result_idx == variable_count - 1) {
                    variable_count--;
                }
                return;
            }
            break;
            
        case 4:
            if ((strcmp(virtual_variables[var1_idx].data_type, "int") == 0 || 
                strcmp(virtual_variables[var1_idx].data_type, "float") == 0) &&
               (strcmp(virtual_variables[var2_idx].data_type, "int") == 0 || 
                strcmp(virtual_variables[var2_idx].data_type, "float") == 0)) {
                
                float divisor = to_float(virtual_variables[var2_idx].value);
                if (divisor == 0) {
                    printf("Error: Division by zero.\n");
                    if (result_idx == variable_count - 1) {
                        variable_count--;
                    }
                    return;
                }
                
                float result = to_float(virtual_variables[var1_idx].value) / divisor;
                sprintf(virtual_variables[result_idx].value, "%.6f", result);
                strcpy(virtual_variables[result_idx].data_type, "float");
                
            } else {
                printf("Operation not supported for these variable types.\n");
                if (result_idx == variable_count - 1) {
                    variable_count--;
                }
                return;
            }
            break;
            
        case 5:
            if (strlen(virtual_variables[var1_idx].value) + strlen(virtual_variables[var2_idx].value) >= MAX_VALUE_LEN) {
                printf("Error: Result would exceed maximum string length.\n");
                if (result_idx == variable_count - 1) {
                    variable_count--;
                }
                return;
            }
            
            strcpy(virtual_variables[result_idx].value, virtual_variables[var1_idx].value);
            strcat(virtual_variables[result_idx].value, virtual_variables[var2_idx].value);
            strcpy(virtual_variables[result_idx].data_type, "str");
            break;
            
        case 6:
            sprintf(virtual_variables[result_idx].value, "%d", (int)strlen(virtual_variables[var1_idx].value));
            strcpy(virtual_variables[result_idx].data_type, "int");
            break;
            
        default:
            printf("Invalid operation.\n");
            if (result_idx == variable_count - 1) {
                variable_count--;
            }
            return;
    }
    
    printf("Operation completed. Result stored in '%s'.\n", result_name);
}

int main() {
    while (1) {
        clear_screen();
        printf("\nChoose an action:\n");
        printf("1. Create a virtual variable\n");
        printf("2. Search for a virtual variable\n");
        printf("3. Show all virtual variables\n");
        printf("4. Delete a virtual variable\n");
        printf("5. Update a variable's value\n");
        printf("6. Save variables to file\n");
        printf("7. Load variables from file\n");
        printf("8. Perform operations on variables\n");
        printf("9. Quit\n");

        int choice;
        printf("> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                create();
                break;
            case 2:
                search();
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                break;
            case 3:
                all();
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                break;
            case 4:
                delete_variable();
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                break;
            case 5:
                update_variable();
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                break;
            case 6:
                save_to_file();
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                break;
            case 7:
                load_from_file();
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                break;
            case 8:
                perform_operations();
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                break;
            case 9:
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}
