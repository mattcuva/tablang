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
    ret->lex = c;

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

int main(int argc, char **argv) {

    char *in = "a & b";
    int cg = 0;
    Token t;
    getnexttoken(in, &cg, &t);
    char *tokstrings[NUMTOKS] = {"ERR", "VAR", "AND", "OR", "LPAREN", "RPAREN"};
    while (t.tok != ERR) {
        printf("%5s  %c  %i\n", tokstrings[t.tok], t.lex, t.index);
        getnexttoken(in, &cg, &t);
    }

}