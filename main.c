#include <stdio.h>
#include <ctype.h>

#define NUMTOKS 6
typedef enum Tok {
    ERR,
    VAR,
    AND,
    OR,
    LPAREN,
    RPAREN,
} Tok;

typedef struct Token {
    int index;
    char lex;
    Tok tok;
} Token;

void nextnonws(char *input, int *cur) {
    while (input[*cur] != '\0' && isspace(input[*cur]))
        (*cur)++;
}

void getnexttoken(char *input, int *cur, Token *ret) {
    nextnonws(input, cur);
    ret->index = *cur;
    char c = input[*cur];
    ret->lex = tolower(c);

    if (c == '\0') { // end of string
        ret->tok = ERR;
        return;
    }

    if (isalpha(c)) { // variable
        ret->tok = VAR;
        (*cur)++; // move carriage forward
        return;
    }

    char chtotok[NUMTOKS] = {'\0', '\0', '&', '|', '(', ')'};
    for (int i = 0; i < NUMTOKS; i++) { // single-character tokens
        if (chtotok[i] == c) {
            ret->tok = i;
            (*cur)++; // move carriage forward
            return;
        }
    }

    // multi-character tokens
    // *cur += etc; // move carriage forward

    // not found
    ret->tok = ERR;
}

void printtokens(char *input) {
    int cg = 0;
    Token t;
    getnexttoken(input, &cg, &t);
    char *tokstrings[NUMTOKS] = {"ERR", "VAR", "AND", "OR", "LPAREN", "RPAREN"};
    while (t.tok != ERR) {
        printf("%5s  %c  %i\n", tokstrings[t.tok], t.lex, t.index);
        getnexttoken(input, &cg, &t);
    }
}

// evaulate the expression using truth values in varvalues (same format as presentvars)
int evaluate(Token *tokens, int numtokens, int varvalues) {
    
}

int main(int argc, char **argv) {

    char *input = "a & b";
    //printtokens(in);

    int cg = 0;
    Token tokens[512]; // tokens in the expression
    int numtokens = 0;
    Token t;
    getnexttoken(input, &cg, &t);
    unsigned int presentvars = 0; // Stores which letters (vars) are present in expression 
                                  // bit format:  ......zyxwvutsrqponmlkjihgfedcba
    while (t.tok != ERR) {
        if (t.tok == VAR)
            presentvars |= (1 << (t.lex - 'a'));

        tokens[numtokens++] = t;
        getnexttoken(input, &cg, &t);
    }

    for (int i = 0; i < 26; i++) {
        if ((presentvars >> i) & 1)
            printf("%c ", i + 'a');
    }
    printf("\n");

}