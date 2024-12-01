#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 10

char grammar[MAX][MAX] = {
    "E->TE'",
    "E'->+TE'|Є",
    "T->FT'",
    "T'->*FT'|Є",
    "F->(E)|id"
};

char first[MAX][MAX] = {0};
char follow[MAX][MAX] = {0};
int num_productions = 5;

int is_non_terminal(char c) {
    return c >= 'A' && c <= 'Z';
}

void add_to_set(char *set, char c) {
    if (strchr(set, c) == NULL) {
        int len = strlen(set);
        set[len] = c;
        set[len + 1] = '\0';
    }
}

void compute_first(char non_terminal, char *result) {
    for (int i = 0; i < num_productions; i++) {
        if (grammar[i][0] == non_terminal) {
            char *rhs = strchr(grammar[i], '>') + 1;

            for (int j = 0; rhs[j] != '\0'; j++) {
                if (rhs[j] == '|') {
                    continue;
                }

                if (!is_non_terminal(rhs[j])) {
                    add_to_set(result, rhs[j]);
                    break;
                } else {
                    char temp[MAX] = {0};
                    compute_first(rhs[j], temp);
                    for (int k = 0; k < strlen(temp); k++) {
                        if (temp[k] != 'Є') {
                            add_to_set(result, temp[k]);
                        }
                    }
                    if (!strchr(temp, 'Є')) {
                        break;
                    }
                }
            }
        }
    }
}

void compute_follow(char non_terminal, char *result) {
    if (non_terminal == 'E') {
        add_to_set(result, '$'); 
    }

    for (int i = 0; i < num_productions; i++) {
        char *rhs = strchr(grammar[i], '>') + 1;

        for (int j = 0; rhs[j] != '\0'; j++) {
            if (rhs[j] == non_terminal) {
                char temp[MAX] = {0};

                if (rhs[j + 1] != '\0') {
                    compute_first(rhs[j + 1], temp);
                    for (int k = 0; k < strlen(temp); k++) {
                        if (temp[k] != 'Є') {
                            add_to_set(result, temp[k]);
                        }
                    }
                }

                if (rhs[j + 1] == '\0' || strchr(temp, 'Є')) {
                    char follow_head[MAX] = {0};
                    compute_follow(grammar[i][0], follow_head);
                    for (int k = 0; k < strlen(follow_head); k++) {
                        add_to_set(result, follow_head[k]);
                    }
                }
            }
        }
    }
}

int main() {
    for (int i = 0; i < num_productions; i++) {
        char non_terminal = grammar[i][0];
        compute_first(non_terminal, first[non_terminal - 'A']);
    }

    for (int i = 0; i < num_productions; i++) {
        char non_terminal = grammar[i][0];
        compute_follow(non_terminal, follow[non_terminal - 'A']);
    }

    printf("First Sets:\n");
    for (int i = 0; i < num_productions; i++) {
        char non_terminal = grammar[i][0];
        printf("First(%c) = { %s }\n", non_terminal, first[non_terminal - 'A']);
    }

    printf("\nFollow Sets:\n");
    for (int i = 0; i < num_productions; i++) {
        char non_terminal = grammar[i][0];
        printf("Follow(%c) = { %s }\n", non_terminal, follow[non_terminal - 'A']);
    }
    return 0;
}
