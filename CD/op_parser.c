#include <stdio.h>
#include <string.h>
#include <ctype.h> // For checking identifiers (e.g., i)

// Maximum size for stack and input
#define MAX 100

// Operator Precedence Table
char precedence[MAX][MAX] = {
    //    +    *    i    $ 
    /* + */ {'>', '<', '<', '>'},
    /* * */ {'>', '>', '<', '>'},
    /* i */ {'>', '>', 'e', '>'},
    /* $ */ {'<', '<', '<', 'a'}
};

// Operators in the grammar
char operators[] = {'+', '*', 'i', '$'};

// Function to find the index of a symbol in the precedence table
int get_index(char symbol) {
    if (symbol == '+' || symbol == '*') return symbol == '+' ? 0 : 1;
    if (symbol == 'i') return 2;
    if (symbol == '$') return 3;
    if (symbol == 'E') return 2; // Treat 'E' as an identifier
    return -1; // Invalid symbol
}

// Check if a symbol is valid
int is_terminal(char symbol) {
    return (symbol == '+' || symbol == '*' || symbol == 'i' || symbol == '$' || symbol == 'E');
}

// Perform reduction based on the grammar
int reduce(char stack[], int *top) {
    if (*top >= 1 && stack[*top] == 'i') {
        // Replace 'i' with 'E'
        stack[*top] = 'E';
        return 1;
    } else if (*top >= 2 && stack[*top - 2] == 'E' &&
               (stack[*top - 1] == '+' || stack[*top - 1] == '*') &&
               stack[*top] == 'E') {
        // Replace "E op E" with "E"
        stack[*top - 2] = 'E';
        *top -= 2;
        return 1;
    }
    return 0; // No reduction possible
}

int main() {
    char input[MAX], stack[MAX];
    int top = -1, i = 0;

    // Input string
    printf("Enter the input string (terminate with $): ");
    scanf("%s", input);

    // Ensure the input ends with '$'
    int n = strlen(input);
    if (input[n - 1] != '$') {
        printf("Error: Input must terminate with '$'.\n");
        return 1;
    }

    // Initialize stack with '$'
    stack[++top] = '$';

    printf("\nParsing steps:\n");
    printf("%-20s %-20s %-20s\n", "Stack", "Input", "Action");

    while (1) {
        // Display stack and input
        for (int j = 0; j <= top; j++) {
            printf("%c", stack[j]);
        }
        printf("%-20s ", "");
        printf("%s%-20s ", &input[i], "");

        char stack_top = stack[top];
        char current_input = input[i];

        // Convert current input to 'i' for identifiers
        if (isalpha(current_input)) current_input = 'i';

        // Get precedence table indices
        int stack_idx = get_index(stack_top);
        int input_idx = get_index(current_input);

        // Error if invalid symbol
        if (stack_idx == -1 || input_idx == -1) {
            printf("Error: Invalid symbol.\n");
            break;
        }

        // Get precedence relation
        char relation = precedence[stack_idx][input_idx];

        if (relation == '<' || relation == '=') {
            // Shift: Push input onto the stack
            stack[++top] = input[i++];
            printf("Shift\n");
        } else if (relation == '>') {
            // Reduce: Perform reduction
            if (!reduce(stack, &top)) {
                printf("Error: Reduction not possible.\n");
                break;
            }
            printf("Reduce\n");
        } else if (relation == 'a') {
            // Accept: Parsing successful
            printf("Accept\n");
            break;
        } else {
            // Error: Undefined precedence relation
            printf("Error: Parsing failed.\n");
            break;
        }
    }

    return 0;
}
