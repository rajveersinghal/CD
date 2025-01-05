#include <stdio.h>
#include <string.h>
#include <ctype.h>

char input[100];
int pos = 0, length;

void E();
void EPrime();
void T();
void TPrime();
void F();

void error() {
    printf("Error: Invalid string\n");
    exit(1);
}

void match(char expected) {
    if (input[pos] == expected) {
        pos++;
    } else {
        error();
    }
}

void E() {
    T();
    EPrime();
}

void EPrime() {
    if (input[pos] == '+') {
        match('+');
        T();
        EPrime();
    }
    // ε case: Do nothing
}

void T() {
    F();
    TPrime();
}

void TPrime() {
    if (input[pos] == '*') {
        match('*');
        F();
        TPrime();
    }
}

void F() {
    if (input[pos] == '(') {
        match('(');
        E();
        match(')');
    } else if (isalnum(input[pos])) { // Match id (alphanumeric)
        pos++;
    } else {
        error();
    }
}

int main() {
    printf("Enter the input string (terminated by $): ");
    scanf("%s", input);
    length = strlen(input);

    if (input[length - 1] != '$') {
        printf("Error: Input must end with $\n");
        return 1;
    }

    E();
    if (pos == length - 1) {
        printf("String successfully parsed\n");
    } else {
        error();
    }

    return 0;
}
