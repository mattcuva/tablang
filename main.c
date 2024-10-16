#include <stdio.h>
#include <ctype.h>

#define NUMTOKS 7
typedef enum Tok {
    ERR,
    VAR,
    AND,
    OR,
    LPAREN,
    RPAREN,
    NOT,
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

    char chtotok[NUMTOKS] = {'\0', '\0', '&', '|', '(', ')', '-'};
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
    char *tokstrings[NUMTOKS] = {"ERR", "VAR", "AND", "OR", "LPAREN", "RPAREN", "NOT"};
    while (t.tok != ERR) {
        printf("%5s  %c  %i\n", tokstrings[t.tok], t.lex, t.index);
        getnexttoken(input, &cg, &t);
    }
}

#define varvalue(varvalues, var) (varvalues >> (var - 'a')) & 1
/*
evaulate the expression using truth values in varvalues (same format as presentvars)

    Expr := [NOT] (VAR | LPAREN Expr RPAREN) [(AND|OR) Expr]
*/
int evaluate(Token *tokens, int numtokens, int *index, unsigned int varvalues) {
    unsigned int negate = 0;
    if (tokens[*index].tok == NOT) {
        // [NOT]
        negate = 1;
        (*index)++;
    }
    
    unsigned int value;

    if (tokens[*index].tok == LPAREN) {
        // LPAREN Expr RPAREN
        int lparenindex = *index;
        (*index)++;
        int exprresult = evaluate(tokens, numtokens, index, varvalues);
        if (exprresult == -1)
            return -1;
        if (*index >= numtokens) {
            printf("Unmatched LPAREN at index %i\n", lparenindex);
            return -1;
        }
        if (tokens[*index].tok != RPAREN) {
            printf("Unexpected token %c\n", tokens[*index].lex);
            return -1;
        }
        (*index)++;
        value = exprresult & 1;
    } else {
        // VAR
        if (tokens[*index].tok == VAR) {
            value = varvalue(varvalues, tokens[*index].lex);
            (*index)++;
        } else {
            printf("Unexpected token %s\n", tokens[*index].lex);
            return -1;
        }
    }

    value = (negate ? ~value : value) & 1;

    // [(AND|OR) Expr]
    if (*index >= numtokens) {
        return value;
    } else if (tokens[*index].tok == AND) {
        (*index)++;
        int exprresult = evaluate(tokens, numtokens, index, varvalues);
        if (exprresult != -1) 
            return value & exprresult;
        else
            return -1;
    } else if (tokens[*index].tok == OR) {
        (*index)++;
        int exprresult = evaluate(tokens, numtokens, index, varvalues);
        if (exprresult != -1) 
            return value | exprresult;
        else
            return -1;
    } else {    
        return value;
    }
}

int main(int argc, char **argv) {

    char *input = "(a | b) & c";
    printtokens(input);

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

    int tokindex = 0;
    printf("%i\n", evaluate(tokens, numtokens, &tokindex, 0b00000000000000000000000000000100));

}