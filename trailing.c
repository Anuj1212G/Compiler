//Trailing code
//Anuj Gupta 
//Sap I'D:-500107370
//Batch:-FSAI B1(H)

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX 26

char productions[MAX][MAX];
char trailing[MAX][MAX];
int productionCount;

// Check if a character is non-terminal (A-Z)
bool isNonTerminal(char c) {
    return c >= 'A' && c <= 'Z';
}

// Add character to trailing set if not already present
void addToSet(char *set, char c) {
    if (!strchr(set, c)) {
        int len = strlen(set);
        set[len] = c;
        set[len + 1] = '\0';
    }
}

// Split productions with '|' into individual rules
void splitProductions() {
    char temp[MAX][MAX];
    int newCount = 0;

    for (int i = 0; i < productionCount; i++) {
        char lhs = productions[i][0];
        char *rhs = productions[i] + 3;
        char *token = strtok(rhs, "|");

        while (token) {
            sprintf(temp[newCount], "%c->%s", lhs, token);
            newCount++;
            token = strtok(NULL, "|");
        }
    }

    for (int i = 0; i < newCount; i++) {
        strcpy(productions[i], temp[i]);
    }

    productionCount = newCount;
}

// Recursively find trailing of a symbol
void findTrailing(char symbol) {
    if (!isNonTerminal(symbol)) {
        addToSet(trailing[symbol - 'A'], symbol);
        return;
    }

    for (int i = 0; i < productionCount; i++) {
        if (productions[i][0] == symbol) {
            int len = strlen(productions[i]);

            for (int j = len - 1; j >= 3; j--) {
                char curr = productions[i][j];

                if (!isNonTerminal(curr)) {
                    addToSet(trailing[symbol - 'A'], curr);
                    break; // stop at first terminal from right
                } else {
                    findTrailing(curr);
                    for (int k = 0; trailing[curr - 'A'][k] != '\0'; k++) {
                        addToSet(trailing[symbol - 'A'], trailing[curr - 'A'][k]);
                    }

                    // check if there is terminal before non-terminal
                    if (j - 1 >= 3 && !isNonTerminal(productions[i][j - 1])) {
                        addToSet(trailing[symbol - 'A'], productions[i][j - 1]);
                    }

                    break; // only first non-terminal matters
                }
            }
        }
    }
}

int main() {
    printf("Enter the number of productions: ");
    scanf("%d", &productionCount);
    getchar();

    printf("Enter the productions (Example: A->aB|b):\n");
    for (int i = 0; i < productionCount; i++) {
        fgets(productions[i], MAX, stdin);
        productions[i][strcspn(productions[i], "\n")] = '\0';
    }

    splitProductions();

    for (int i = 0; i < MAX; i++) {
        trailing[i][0] = '\0';
    }

    // Compute TRAILING sets
    for (int i = 0; i < productionCount; i++) {
        findTrailing(productions[i][0]);
    }

    printf("\nTRAILING Sets:\n");
    for (int i = 0; i < MAX; i++) {
        if (trailing[i][0] != '\0') {
            printf("TRAILING(%c) = { ", 'A' + i);
            for (int j = 0; trailing[i][j] != '\0'; j++) {
                if (j > 0) printf(", ");
                printf("%c", trailing[i][j]);
            }
            printf(" }\n");
        }
    }

    return 0;
}