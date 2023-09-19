#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>  // this for strtol and strtod ^))

// token types
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
    ERROR
};

// Function to classify a token
enum TokenType classifyToken(char *token) {
    if (isdigit(token[0])) {
        // Check for different types of numbers
        char *endptr;
        strtol(token, &endptr, 0);
        if (*endptr == '\0') {
            return NUMBER;
        }
        if (*endptr == '.') {
            strtod(token, &endptr);
            if (*endptr == '\0') {
                return NUMBER;
            }
        }
        if (strlen(token) >= 3 && token[0] == '0' && (token[1] == 'x' || token[1] == 'X')) {
            for (int i = 2; i < strlen(token); i++) {
                if (!isxdigit(token[i])) {
                    return ERROR;
                }
            }
            return NUMBER;
        }
        return ERROR;
    }
    if (token[0] == '"' && token[strlen(token) - 1] == '"') {
        return STRING_CONSTANT;
    }
    if (token[0] == '\'' && strlen(token) == 3 && token[2] == '\'') {
        return CHAR_CONSTANT;
    }
    if (token[0] == '#') {
        return PREPROCESSOR_DIRECTIVE;
    }
    if (strncmp(token, "//", 2) == 0 || (strncmp(token, "/*", 2) == 0 && token[strlen(token) - 2] == '*' && token[strlen(token) - 1] == '/')) {
        return COMMENT;
    }
    char *reservedWords[] = {"int", "float", "if", "else", "for", "while", "return"};
    for (int i = 0; i < sizeof(reservedWords) / sizeof(reservedWords[0]); i++) {
        if (strcmp(token, reservedWords[i]) == 0) {
            return RESERVED_WORD;
        }
    }
    char *operators[] = {"+", "-", "*", "/", "=", "==", "!=", "<", ">", "<=", ">="};
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
        if (strcmp(token, operators[i]) == 0) {
            return OPERATOR;
        }
    }
    char *punctuation[] = {";", ",", "(", ")", "{", "}"};
    for (int i = 0; i < sizeof(punctuation) / sizeof(punctuation[0]); i++) {
        if (strcmp(token, punctuation[i]) == 0) {
            return PUNCTUATION;
        }
    }
    if (isalpha(token[0]) || token[0] == '_') {
        for (int i = 1; i < strlen(token); i++) {
            if (!isalnum(token[i]) && token[i] != '_') {
                return ERROR;
            }
        }
        return IDENTIFIER;
    }
    return ERROR;
}

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
        "ERROR"
};

int main() {
    char input[] = "int main() {\n"
                   "    int x = 10;\n"
                   "    float y = 3.14;\n"
                   "    if (x > 5) {\n"
                   "        printf(\"Hello, world!\\n\");\n"
                   "    }\n"
                   "    return 0;\n"
                   "}\n";

    // Split the input into tokens
    char *token = strtok(input, " \t\n(){}[];,");
    while (token != NULL) {
        enum TokenType type = classifyToken(token);
        printf("<%s, %s>\n", token, type == ERROR ? "ERROR" : tokenNames[type]);
        token = strtok(NULL, " \t\n(){}[];,");
    }

    return 0;
}
