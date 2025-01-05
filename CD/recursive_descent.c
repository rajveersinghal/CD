#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

char *input;
int index = 0;

// Function prototypes
void E();
void T();
void F();
void match(char expected);
int is_end();
char current_char();

// Function to check if the current position is at the end of input
int is_end() {
    return input[index] == '\0';
}

// Function to get the current character at the current index
char current_char() {
    return input[index];
}

// Function to match the expected character and advance the index
void match(char expected) {
    if (current_char() == expected) {
        index++;
    } else {
        printf("Syntax error: expected '%c' but found '%c'\n", expected, current_char());
        exit(1);
    }
}

// F -> ( E ) | id
void F() {
    if (current_char() == '(') {
        match('(');
        E();
        match(')');
    } else if (isalnum(current_char())) {
        match(current_char());
    } else {
        printf("Syntax error: invalid character in F\n");
        exit(1);
    }
}

// T -> T * F | F
void T() {
    F();
    while (current_char() == '*') {
        match('*');
        F();
    }
}

// E -> E + T | T
void E() {
    T();
    while (current_char() == '+') {
        match('+');
        T();
    }
}

int main() {
    // Read the input expression
    printf("Enter an expression: ");
    input = (char *)malloc(100 * sizeof(char));
    scanf("%s", input);

    // Start parsing from the start symbol
    E();

    // If we reach the end of the string, it's a valid expression
    if (is_end()) {
        printf("The expression is valid.\n");
    } else {
        printf("Syntax error: unexpected characters at the end\n");
    }

    // Free the input memory
    free(input);
    return 0;
}
