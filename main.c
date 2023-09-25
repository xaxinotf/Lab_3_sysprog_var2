#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

// Define token types
enum TokenType {
    NUMBER,
    STRING_CONSTANT,
    CHAR_CONSTANT,
    PREPROCESSOR_DIRECTIVE,
    COMMENT,
    RESERVED_WORD,
    OPERATOR,
    PUNCTUATION,
    IDENTIFIER,
    ERROR,
    FLOATING_POINT_NUMBER,
    HEXADECIMAL,
    LIBRARY
};

// Array of token names for printing
const char *tokenNames[] = {
        "NUMBER",
        "STRING_CONSTANT",
        "CHAR_CONSTANT",
        "PREPROCESSOR_DIRECTIVE",
        "COMMENT",
        "RESERVED_WORD",
        "OPERATOR",
        "PUNCTUATION",
        "IDENTIFIER",
        "ERROR",
        "FLOATING_POINT_NUMBER",
        "HEXADECIMAL",
        "LIBRARY"
};

// List of reserved words
const char *reservedWords[] = {
        "int", "return", "double", "float", "char", //... (add other reserved words)
};

const char *knownLibraries[] = {
        "stdio.h",
        "stdlib.h",
        "string.h",
        // ... add other known libraries here
};



bool isReservedWord(const char *token) {
    size_t count = sizeof(reservedWords) / sizeof(reservedWords[0]);
    for (size_t i = 0; i < count; ++i) {
        if (strcmp(token, reservedWords[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool isOperator(char ch) {
    return strchr("+-*/=<>!&|^%", ch) != NULL;
}

bool isPunctuation(char ch) {
    return strchr(",.;:(){}[]", ch) != NULL;
}

enum TokenType classifyToken(char *token) {
    if (token == NULL || token[0] == '\0') {
        return ERROR;
    }

    if (token[0] == '<' && token[strlen(token) - 1] == '>') {
        return LIBRARY;
    }
    // Check for a custom string constant of the form <<...>>
    if (token[0] == '<' && token[1] == '<' && token[strlen(token)-2] == '>' && token[strlen(token)-1] == '>') {
        return STRING_CONSTANT;
    }
    // Check for a preprocessor directive
    if (token[0] == '#') {
        return PREPROCESSOR_DIRECTIVE;
    }

    // Check for a comment
    if (token[0] == '/' && (token[1] == '/' || token[1] == '*')) {
        return COMMENT;
    }

    // Check for a reserved word
    if (isReservedWord(token)) {
        return RESERVED_WORD;
    }

    // Check for an operator
    if (isOperator(token[0]) && strlen(token) == 1) {
        return OPERATOR;
    }

    // Check for a punctuation symbol
    if (isPunctuation(token[0]) && strlen(token) == 1) {
        return PUNCTUATION;
    }

    // Check for an identifier (assume any sequence of letters and/or digits)
    if (isalpha(token[0]) || token[0] == '_') {
        return IDENTIFIER;
    }

    // Check for a number (integer, floating-point, or hexadecimal)
    if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
        char *endptr;
        strtol(token, &endptr, 0);
        if (*endptr == '\0') {
            return NUMBER;
        }
        if (*endptr == '.') {
            strtod(token, &endptr);
            if (*endptr == '\0') {
                return FLOATING_POINT_NUMBER;
            }
        }
        if (strstr(token, "0x") == token || strstr(token, "0X") == token) {
            return HEXADECIMAL;
        }
    }

    // Check for string constant
    if (token[0] == '"' && token[strlen(token)-1] == '"') {
        return STRING_CONSTANT;
    }

    // Check for character constant
    if (token[0] == '\'' && token[strlen(token)-1] == '\'') {
        return CHAR_CONSTANT;
    }

    return ERROR;
}

int main() {
    FILE *inputFile = fopen("regex.txt", "r");
    if (!inputFile) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    FILE *outputFile = fopen("out.txt", "w");
    if (!outputFile) {
        perror("Error opening output file");
        fclose(inputFile);
        return EXIT_FAILURE;
    }

    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        char *token = strtok(line, " \t\n(){}[];,");
        while (token != NULL) {
            enum TokenType type = classifyToken(token);
            fprintf(outputFile, "<%s> - <%s>\n", token, tokenNames[type]);
            token = strtok(NULL, " \t\n(){}[];,");
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    return EXIT_SUCCESS;
}
