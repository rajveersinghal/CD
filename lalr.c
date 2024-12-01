#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK 100
#define MAX_INPUT 100

// Define the stack structure
typedef struct {
    int state;   // State number
    char symbol; // Grammar symbol ('E', 'T', 'F', etc.)
} StackItem;

StackItem stack[MAX_STACK];
int top = -1;

// LALR Action table
const char *action[12][7] = {
    {"S5", "", "", "S4", "", "", ""},   // State 0
    {"", "S6", "", "", "", "ACC", ""}, // State 1
    {"", "R2", "S7", "", "R2", "R2", ""}, // State 2
    {"", "R4", "R4", "", "R4", "R4", ""}, // State 3
    {"S5", "", "", "S4", "", "", ""},  // State 4
    {"", "R6", "R6", "", "R6", "R6", ""}, // State 5
    {"S5", "", "", "S4", "", "", ""},  // State 6
    {"S5", "", "", "S4", "", "", ""},  // State 7
    {"", "S6", "", "", "", "S11", ""}, // State 8
    {"", "R1", "S7", "", "R1", "R1", ""}, // State 9
    {"", "R3", "R3", "", "R3", "R3", ""}, // State 10
    {"", "R5", "R5", "", "R5", "R5", ""}  // State 11
};

// LALR Goto table
const int goto_table[12][3] = {
    {1, 2, 3}, // State 0
    {-1, -1, -1}, // State 1
    {-1, -1, -1}, // State 2
    {-1, -1, -1}, // State 3
    {8, 2, 3}, // State 4
    {-1, -1, -1}, // State 5
    {-1, 9, 3}, // State 6
    {-1, -1, 10}, // State 7
    {-1, -1, -1}, // State 8
    {-1, -1, -1}, // State 9
    {-1, -1, -1}, // State 10
    {-1, -1, -1}  // State 11
};

// Push a state onto the stack
void push(int state, char symbol) {
    stack[++top].state = state;
    stack[top].symbol = symbol;
}

// Pop items from the stack
void pop(int count) {
    for (int i = 0; i < count; i++) {
        if (top >= 0) top--;
    }
}

// Get the top state from the stack
int get_state() {
    return stack[top].state;
}

// Get the action table index for a symbol
int get_action_index(char symbol) {
    switch (symbol) {
        case 'i': return 0; // id
        case '+': return 1;
        case '*': return 2;
        case '(': return 3;
        case ')': return 4;
        case '$': return 5;
        default: return -1;
    }
}

// Parse the input string
void parse(const char *input) {
    int index = 0;
    char symbol = input[index];
    push(0, '$'); // Initialize stack with state 0

    while (1) {
        int state = get_state();
        int action_index = get_action_index(symbol);
        if (action_index == -1) {
            printf("Error: Invalid symbol '%c'\n", symbol);
            return;
        }

        const char *action_entry = action[state][action_index];

        // Print the stack and input for debugging
        printf("Stack: ");
        for (int i = 0; i <= top; i++) {
            printf("%d%c ", stack[i].state, stack[i].symbol);
        }
        printf("\tInput: %s\n", input + index);

        if (action_entry[0] == 'S') { // Shift
            int next_state = atoi(action_entry + 1);
            push(next_state, symbol);
            symbol = input[++index];
        } else if (action_entry[0] == 'R') { // Reduce
            int rule = atoi(action_entry + 1);
            switch (rule) {
                case 1: pop(3); push(goto_table[get_state()][0], 'E'); break; // E -> E + T
                case 2: pop(1); push(goto_table[get_state()][0], 'E'); break; // E -> T
                case 3: pop(3); push(goto_table[get_state()][1], 'T'); break; // T -> T * F
                case 4: pop(1); push(goto_table[get_state()][1], 'T'); break; // T -> F
                case 5: pop(3); push(goto_table[get_state()][2], 'F'); break; // F -> (E)
                case 6: pop(1); push(goto_table[get_state()][2], 'F'); break; // F -> id
                default: printf("Error: Unknown rule\n"); return;
            }
        } else if (strcmp(action_entry, "ACC") == 0) { // Accept
            printf("Parsing successful!\n");
            return;
        } else { // Error
            printf("Error: Parsing failed\n");
            return;
        }
    }
}

int main() {
    const char *input = "i+i*i$";
    printf("Parsing input: %s\n", input);
    parse(input);
    return 0;
}
