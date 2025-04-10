
//First and follow code
//Anuj Gupta 
//Sap I'D:-500107370
//Batch:-FSAI B1(H)

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX 20

// global variables
char productions[MAX][MAX];
char first[MAX][MAX];
char follow[MAX][MAX];
int productionCount;
char startSymbol;

// check if a character is a non-terminal
bool isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

// add a character to a set if not already present
void addToSet(char *set, char c) {
    if (!strchr(set, c)) {
        int len = strlen(set);
        set[len] = c;
        set[len + 1] = '\0';
    }
}

// split productions based on '|'
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

// find FIRST 
void findFirst(char symbol) {
    if (!isNonTerminal(symbol)) {
        addToSet(first[symbol - 'A'], symbol);
        return;
    }
    for (int i = 0; i < productionCount; i++) {
        if (productions[i][0] == symbol) {
            bool hasEpsilon = true;
            for (int j = 3; productions[i][j] != '\0'; j++) {
                char nextSymbol = productions[i][j];
                if (!isNonTerminal(nextSymbol)) {
                    addToSet(first[symbol - 'A'], nextSymbol);
                    hasEpsilon = false;
                    break;
                }
                findFirst(nextSymbol);
                bool foundEpsilon = false;
                for (int k = 0; first[nextSymbol - 'A'][k] != '\0'; k++) {
                    if (first[nextSymbol - 'A'][k] == '#') {
                        foundEpsilon = true;
                    } else {
                        addToSet(first[symbol - 'A'], first[nextSymbol - 'A'][k]);
                    }
                }
                if (!foundEpsilon) {
                    hasEpsilon = false;
                    break;
                }
            }
            if (hasEpsilon) {
                addToSet(first[symbol - 'A'], '#');
            }
        }
    }
}

// find FOLLOW 
void findFollow(char symbol) {
    if (symbol == startSymbol) {
        addToSet(follow[symbol - 'A'], '$');
    }
    for (int i = 0; i < productionCount; i++) {
        for (int j = 3; productions[i][j] != '\0'; j++) {
            if (productions[i][j] == symbol) {
                int next = j + 1;
                while (productions[i][next] != '\0') {
                    char nextSymbol = productions[i][next];
                    if (!isNonTerminal(nextSymbol)) {
                        addToSet(follow[symbol - 'A'], nextSymbol);
                        break;
                    }
                    for (int k = 0; first[nextSymbol - 'A'][k] != '\0'; k++) {
                        if (first[nextSymbol - 'A'][k] != '#') {
                            addToSet(follow[symbol - 'A'], first[nextSymbol - 'A'][k]);
                        }
                    }
                    if (!strchr(first[nextSymbol - 'A'], '#')) {
                        break;
                    }
                    next++;
                }
                if (productions[i][next] == '\0' || strchr(first[productions[i][next] - 'A'], '#')) {
                    for (int k = 0; follow[productions[i][0] - 'A'][k] != '\0'; k++) {
                        addToSet(follow[symbol - 'A'], follow[productions[i][0] - 'A'][k]);
                    }
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

    startSymbol = productions[0][0];
    splitProductions();

    for (int i = 0; i < MAX; i++) {
        first[i][0] = '\0';
        follow[i][0] = '\0';
    }

    // Compute FIRST sets
    for (int i = 0; i < productionCount; i++) {
        findFirst(productions[i][0]);
    }

    // Compute FOLLOW sets
    for (int i = 0; i < productionCount; i++) {
        findFollow(productions[i][0]);
    }

    printf("\nFIRST Sets:\n");
    for (int i = 0; i < MAX; i++) {
        if (first[i][0] != '\0') {
            printf("FIRST(%c) = { ", 'A' + i);
            for (int j = 0; first[i][j] != '\0'; j++) {
                if (j > 0) printf(", ");
                printf("%c", first[i][j]);
            }
            printf(" }\n");
        }
    }

    printf("\nFOLLOW Sets:\n");
    for (int i = 0; i < MAX; i++) {
        if (follow[i][0] != '\0') {
            printf("FOLLOW(%c) = { ", 'A' + i);
            for (int j = 0; follow[i][j] != '\0'; j++) { 
                if (j > 0) printf(", ");
                printf("%c", follow[i][j]);
            }
            printf(" }\n");
        }
    }
    return 0;
}
