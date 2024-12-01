#include <stdio.h>
#include <string.h>
#include <ctype.h>

// List of keywords
const char* keywords[] = {
    "if", "else", "while", "for", "do", "int", "float", "char", "return"
};
const int numKeywords = 9;

// Function to check if a string is a keyword
int isKeyword(const char* str) {
    for (int i = 0; i < numKeywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to check if a character is an operator
int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '>' || ch == '<');
}

// Function to check if a character is a separator
int isSeparator(char ch) {
    return (ch == ',' || ch == ';' || ch == '(' || ch == ')' || ch == '{' || ch == '}');
}

// Lexical analyzer function
void lexicalAnalyzer(const char* input) {
    int i = 0;
    int tokenCount = 0; // Initialize token counter

    while (input[i] != '\0') {
        // Skip whitespace
        if (isspace(input[i])) {
            i++;
            continue;
        }

        // Check for identifiers and keywords
        if (isalpha(input[i])) {
            char buffer[50];
            int j = 0;
            while (isalnum(input[i])) {
                buffer[j++] = input[i++];
            }
            buffer[j] = '\0';

            if (isKeyword(buffer)) {
                printf("Keyword: %s\n", buffer);
            } else {
                printf("Identifier: %s\n", buffer);
            }
            tokenCount++;
            continue;
        }

        // Check for numbers
        if (isdigit(input[i])) {
            char buffer[50];
            int j = 0;
            while (isdigit(input[i])) {
                buffer[j++] = input[i++];
            }
            buffer[j] = '\0';
            printf("Number: %s\n", buffer);
            tokenCount++;
            continue;
        }

        // Check for operators
        if (isOperator(input[i])) {
            printf("Operator: %c\n", input[i]);
            i++;
            tokenCount++;
            continue;
        }

        // Check for separators
        if (isSeparator(input[i])) {
            printf("Separator: %c\n", input[i]);
            i++;
            tokenCount++;
            continue;
        }

        // Handle unknown characters
        printf("Unknown: %c\n", input[i]);
        i++;
        tokenCount++;
    }

    // Print the total token count
    printf("\nTotal Tokens: %d\n", tokenCount);
}

// Main function
int main() {
    char input[256];
    printf("Enter the input string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline character
    printf("\nLexical Analysis Output:\n");
    lexicalAnalyzer(input);
    return 0;
}
