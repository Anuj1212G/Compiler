//Leading code
//Anuj Gupta 
//Sap I'D:-500107370
//Batch:-FSAI B1(H)

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX 26 

char productions[MAX][MAX];
char leading[MAX][MAX];
int productionCount;
char startSymbol;

// non-terminal character
bool isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

// add character to set
void addToSet(char *set, char c) {
    if (!strchr(set, c)) {
        int len = strlen(set); 
        set[len] = c;
        set[len + 1] = '\0';
    }
}

// if rule contains pipe '|'
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

void findLeading(char symbol) {
    if (!isNonTerminal(symbol)) {
        addToSet(leading[symbol - 'A'], symbol);
        return;
    }

    for (int i = 0; i < productionCount; i++) {
        if (productions[i][0] == symbol) {
            for (int j = 3; productions[i][j] != '\0'; j++) {
                if (productions[i][j] == symbol)  // Ignore left recursion
                    continue;
                char nextSymbol = productions[i][j];
                
                if (!isNonTerminal(nextSymbol)) {
                    addToSet(leading[symbol - 'A'], nextSymbol);
                    break; // Terminal found, stop further propagation
                }
                findLeading(nextSymbol);

                for (int k = 0; leading[nextSymbol - 'A'][k] != '\0'; k++) {
                    addToSet(leading[symbol - 'A'], leading[nextSymbol - 'A'][k]);
                }
                break; // Only first non-terminal matters in LEADING
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
        leading[i][0] = '\0';
    }

    // Compute LEADING sets
    for (int i = 0; i < productionCount; i++) {
        findLeading(productions[i][0]);
    }

    printf("\nLEADING Sets:\n");
    for (int i = 0; i < MAX; i++) {
        if (leading[i][0] != '\0') {
            printf("LEADING(%c) = { ", 'A' + i);
            for (int j = 0; leading[i][j] != '\0'; j++) {
                if (j > 0) printf(", ");
                printf("%c", leading[i][j]);
            }
            printf(" }\n");
        }
    }
    return 0;
}