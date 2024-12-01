#include <stdio.h>
#include <string.h>

#define MAX 100

char stack[MAX];  // Stack for parsing
int top = -1;     // Top of the stack
char input[MAX];  // Input buffer
int pos = 0;      // Current position in input

// Function to push a symbol onto the stack
void push(char symbol) {
    stack[++top] = symbol;
}

// Function to pop the top symbol from the stack
void pop() {
    if (top >= 0) {
        top--;
    }
}

// Function to print the current stack and input
void print_state() {
    printf("Stack: ");
    for (int i = 0; i <= top; i++) {
        printf("%c", stack[i]);
    }
    printf("\tInput: %s\n", input + pos);
}

// Function to attempt reductions
int reduce() {
    // Check for (L) -> S
    if (top >= 2 && stack[top - 2] == '(' && stack[top - 1] == 'L' && stack[top] == ')') {
        pop();  // Pop ')'
        pop();  // Pop 'L'
        stack[top] = 'S';  // Replace '(' with 'S'
        printf("Reduced: (L) -> S\n");
        return 1;  // Reduction applied
    }

    // Check for L, S -> L
    if (top >= 2 && stack[top - 2] == 'L' && stack[top - 1] == ',' && stack[top] == 'S') {
        pop();  // Pop 'S'
        pop();  // Pop ','
        stack[top] = 'L';  // Replace 'L' with 'L'
        printf("Reduced: L, S -> L\n");
        return 1;  // Reduction applied
    }

    // Check for a -> S
    if (top >= 0 && stack[top] == 'a') {
        stack[top] = 'S';  // Replace 'a' with 'S'
        printf("Reduced: a -> S\n");
        return 1;  // Reduction applied
    }

    return 0;  // No reduction applied
}

void shift() {
    printf("Shifted: %c\n", input[pos]);
    push(input[pos++]);
}

int main() {
    printf("Enter the input string (terminated by $): ");
    scanf("%s", input);

    // Ensure input ends with '$'
    if (input[strlen(input) - 1] != '$') {
        printf("Error: Input must end with $\n");
        return 1;
    }

    printf("Parsing starts...\n");
    while (1) {
        print_state();

        // Check for successful parsing
        if (stack[0] == 'S' && top == 0 && input[pos] == '$') {
            printf("Parsing successful: Input string is valid.\n");
            break;
        }

        // Try reducing first; if no reduction is possible, shift
        if (!reduce()) {
            if (input[pos] != '$') {
                shift();
            } else {
                printf("Error: Cannot reduce further, invalid input.\n");
                break;
            }
        }
    }

    return 0;
}
