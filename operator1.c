//operator precedence parser
//Anuj Gupta 
//Sap I'D:-500107370
//Batch:-FSAI B1(H)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Global variables
char *input;
int i = 0;
char lasthandle[6], stack[50];
char handles[][5] = {")E(", "E*E", "E+E", "i", "E^E"}; // (E) is reversed to )E( when matched
int top = 0, l;

// Precedence table
char prec[9][9] = {
    //         +    -    *    /    ^    i    (    )    $
    /* + */   {'>', '>', '<', '<', '<', '<', '<', '>', '>'},
    /* - */   {'>', '>', '<', '<', '<', '<', '<', '>', '>'},
    /* * */   {'>', '>', '>', '>', '<', '<', '<', '>', '>'},
    /* / */   {'>', '>', '>', '>', '<', '<', '<', '>', '>'},
    /* ^ */   {'>', '>', '>', '>', '<', '<', '<', '>', '>'},
    /* i */   {'>', '>', '>', '>', '>', 'e', 'e', '>', '>'},
    /* ( */   {'<', '<', '<', '<', '<', '<', '<', 'e', 'e'},
    /* ) */   {'>', '>', '>', '>', '>', 'e', 'e', '>', '>'},
    /* $ */   {'<', '<', '<', '<', '<', '<', '<', '<', '>'}
};

// Get precedence table index for character
int getindex(char c) {
    switch (c) {
        case '+': return 0;
        case '-': return 1;
        case '*': return 2;
        case '/': return 3;
        case '^': return 4;
        case 'i': return 5;
        case '(': return 6;
        case ')': return 7;
        case '$': return 8;
        default: return -1;
    }
}

// Function to get the nearest terminal from the stack (top to bottom)
char getTopTerminal() {
    for (int j = top; j >= 0; j--) {
        if (strchr("+-*/^i()$", stack[j]) != NULL) // Checks if it's a terminal
            return stack[j];
    }
    return '$'; // default to '$' if no terminal is found
}

// Shift operation
void shift() {
    stack[++top] = input[i++];
    stack[top + 1] = '\0';
}

// Try reducing the top of the stack
int reduce() {
    int t, len, found;
    for (int h = 0; h < 5; h++) {
        len = strlen(handles[h]);
        if (stack[top] == handles[h][0] && top + 1 >= len) {
            found = 1;
            for (t = 0; t < len; t++) {
                if (stack[top - t] != handles[h][t]) {
                    found = 0;
                    break;
                }
            }
            if (found) {
                stack[top - t + 1] = 'E';
                top = top - t + 1;
                strcpy(lasthandle, handles[h]);
                stack[top + 1] = '\0';
                return 1;
            }
        }
    }
    return 0;
}

// Display stack contents
void dispstack() {
    for (int j = 0; j <= top; j++)
        printf("%c", stack[j]);
}

// Display input buffer
void dispinput() {
    for (int j = i; j < l; j++)
        printf("%c", input[j]);
}

// Main function
int main() {
    input = (char *)malloc(50 * sizeof(char));
    printf("Enter the expression (with terminals like i for id):\n");
    scanf("%s", input);

    strcat(input, "$");
    l = strlen(input);
    strcpy(stack, "$");

    printf("\nSTACK\tINPUT\tACTION\n");

    while (i < l) {
        shift();
        printf("\n");
        dispstack();
        printf("\t");
        dispinput();
        printf("\tShift");

        while (1) {
            // Get the top terminal of the stack
            char stack_top = getTopTerminal();
            char current_input = input[i];
            char relation = prec[getindex(stack_top)][getindex(current_input)];

            if (relation == '>') {
                if (!reduce()) break;
                printf("\n");
                dispstack();
                printf("\t");
                dispinput();
                printf("\tReduced: E -> %s", lasthandle);
            } else if (relation == '<') {
                break;
            } else if (relation == '=') {
                printf("\nError: Invalid symbol or precedence conflict.");
                free(input);
                return 0;
            }
        }
    }

    if (strcmp(stack, "$E$") == 0)
        printf("\nAccepted.\n");
    else
        printf("\nNot Accepted.\n");

    free(input);
    return 0;
}