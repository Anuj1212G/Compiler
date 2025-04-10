//LL(1) parser
//Anuj Gupta 
//Sap I'D:-500107370
//Batch:-FSAI B1(H)

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX 20
#define SYMBOLS 26  // A-Z

char productions[MAX][MAX];
char first[SYMBOLS][MAX];
char follow[SYMBOLS][MAX];
char table[SYMBOLS][128][MAX];  // [NonTerminal][Terminal ASCII][Production]
int productionCount;
char startSymbol;

// Check if a character is a non-terminal (A-Z)
bool isNonTerminal(char c) {
    return c >= 'A' && c <= 'Z';
}

// Add a character to a set (if not already present)
void addToSet(char* set, char c) {
    if (!strchr(set, c)) {
        int len = strlen(set);
        set[len] = c;
        set[len + 1] = '\0';
    }
}

// Merge one set into another (excluding epsilon if specified)
void mergeSet(char* dest, char* src, bool excludeEpsilon) {
    for (int i = 0; src[i]; i++) {
        if (excludeEpsilon && src[i] == '#') continue;
        addToSet(dest, src[i]);
    }
}

// Split productions with multiple RHS (A->a|b becomes A->a and A->b)
void splitProductions() {
    char temp[MAX][MAX];
    int newCount = 0;

    for (int i = 0; i < productionCount; i++) {
        char lhs = productions[i][0];
        char* rhs = productions[i] + 3;
        char* token = strtok(rhs, "|");

        while (token) {
            sprintf(temp[newCount++], "%c->%s", lhs, token);
            token = strtok(NULL, "|");
        }
    }

    for (int i = 0; i < newCount; i++) {
        strcpy(productions[i], temp[i]);
    }
    productionCount = newCount;
}

// Recursive function to find FIRST of a symbol
void findFirst(char symbol) {
    if (!isNonTerminal(symbol)) {
        addToSet(first[symbol - 'A'], symbol);
        return;
    }

    for (int i = 0; i < productionCount; i++) {
        if (productions[i][0] == symbol) {
            bool addEpsilon = true;
            for (int j = 3; productions[i][j]; j++) {
                char next = productions[i][j];

                if (!isNonTerminal(next)) {
                    addToSet(first[symbol - 'A'], next);
                    addEpsilon = false;
                    break;
                }

                findFirst(next);
                mergeSet(first[symbol - 'A'], first[next - 'A'], true);

                if (!strchr(first[next - 'A'], '#')) {
                    addEpsilon = false;
                    break;
                }
            }
            if (addEpsilon)
                addToSet(first[symbol - 'A'], '#');
        }
    }
}

// Recursive function to find FOLLOW of a non-terminal
void findFollow(char symbol) {
    if (symbol == startSymbol)
        addToSet(follow[symbol - 'A'], '$');

    for (int i = 0; i < productionCount; i++) {
        char lhs = productions[i][0];
        for (int j = 3; productions[i][j]; j++) {
            if (productions[i][j] == symbol) {
                int k = j + 1;
                bool foundEpsilon = false;

                while (productions[i][k]) {
                    char next = productions[i][k];

                    if (!isNonTerminal(next)) {
                        addToSet(follow[symbol - 'A'], next);
                        break;
                    }

                    mergeSet(follow[symbol - 'A'], first[next - 'A'], true);

                    if (strchr(first[next - 'A'], '#'))
                        foundEpsilon = true;
                    else {
                        foundEpsilon = false;
                        break;
                    }
                    k++;
                }

                if (!productions[i][k] || foundEpsilon) {
                    if (lhs != symbol)
                        mergeSet(follow[symbol - 'A'], follow[lhs - 'A'], false);
                }
            }
        }
    }
}

// Construct the LL(1) Parsing Table
void buildParsingTable() {
    for (int i = 0; i < productionCount; i++) {
        char lhs = productions[i][0];
        char* rhs = productions[i] + 3;

        char tempFirst[MAX] = "";
        if (rhs[0] == '#') {
            strcpy(tempFirst, "#");
        } else {
            for (int j = 0; rhs[j]; j++) {
                char sym = rhs[j];
                if (!isNonTerminal(sym)) {
                    addToSet(tempFirst, sym);
                    break;
                }
                mergeSet(tempFirst, first[sym - 'A'], true);
                if (!strchr(first[sym - 'A'], '#'))
                    break;
                if (!rhs[j + 1]) addToSet(tempFirst, '#');
            }
        }

        for (int j = 0; tempFirst[j]; j++) {
            if (tempFirst[j] != '#') {
                strcpy(table[lhs - 'A'][(int)tempFirst[j]], productions[i]);
            }
        }

        if (strchr(tempFirst, '#')) {
            for (int j = 0; follow[lhs - 'A'][j]; j++) {
                strcpy(table[lhs - 'A'][(int)follow[lhs - 'A'][j]], productions[i]);
            }
        }
    }
}

// Display FIRST sets
void printFirstSets() {
    printf("\nFIRST Sets:\n");
    for (int i = 0; i < SYMBOLS; i++) {
        if (first[i][0]) {
            printf("FIRST(%c) = { ", 'A' + i);
            for (int j = 0; first[i][j]; j++) {
                if (j) printf(", ");
                printf("%c", first[i][j]);
            }
            printf(" }\n");
        }
    }
}

// Display FOLLOW sets
void printFollowSets() {
    printf("\nFOLLOW Sets:\n");
    for (int i = 0; i < SYMBOLS; i++) {
        if (follow[i][0]) {
            printf("FOLLOW(%c) = { ", 'A' + i);
            for (int j = 0; follow[i][j]; j++) {
                if (j) printf(", ");
                printf("%c", follow[i][j]);
            }
            printf(" }\n");
        }
    }
}

// Display the LL(1) Parsing Table
void printParsingTable() {
    printf("\nLL(1) Parsing Table:\n\n     ");
    for (char t = 'a'; t <= 'z'; t++) printf("%6c", t);
    printf("%6c\n", '$');

    for (int i = 0; i < 26; i++) {
        bool rowHasData = false;
        for (int j = 'a'; j <= 'z'; j++) {
            if (strlen(table[i][j])) {
                rowHasData = true;
                break;
            }
        }
        if (strlen(table[i]['$'])) rowHasData = true;

        if (!rowHasData) continue;

        printf("%c  ", 'A' + i);
        for (char t = 'a'; t <= 'z'; t++) {
            if (strlen(table[i][(int)t]))
                printf("%6s", table[i][(int)t]);
            else
                printf("      ");
        }

        if (strlen(table[i]['$']))
            printf("%6s", table[i]['$']);
        else
            printf("      ");
        printf("\n");
    }
}

// MAIN FUNCTION
int main() {
    printf("Enter the number of productions: ");
    scanf("%d", &productionCount);
    getchar();  // Consume newline

    printf("Enter the productions (Example: A->aB|b):\n");
    for (int i = 0; i < productionCount; i++) {
        fgets(productions[i], MAX, stdin);
        productions[i][strcspn(productions[i], "\n")] = '\0';  // Remove newline
    }

    startSymbol = productions[0][0];
    splitProductions();

    for (int i = 0; i < SYMBOLS; i++) {
        first[i][0] = follow[i][0] = '\0';
    }

    // Compute FIRST sets
    for (int i = 0; i < productionCount; i++) {
        findFirst(productions[i][0]);
    }

    // Compute FOLLOW sets
    for (int i = 0; i < productionCount; i++) {
        findFollow(productions[i][0]);
    }

    // Build and print the parsing table
    buildParsingTable();

    printFirstSets();
    printFollowSets();
    printParsingTable();

    return 0;
}
