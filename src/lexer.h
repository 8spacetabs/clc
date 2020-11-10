#ifndef LEXER_H_INCLUDED
        #define LEXER_H_INCLUDED 1

        #define ERR_INVALID_CHAR ((Token*)1)

        typedef enum {
                TT_STREAM_END = 0,
                TT_CONST      = 1,
                TT_ANSWER     = 2,

                TT_ADD = '+',
                TT_SUB = '-',
                TT_DIV = '/',
                TT_MOD = '%',
                TT_MUL = '*',
                TT_POW = '^',
                TT_SQR = '\\',

                TT_LPAREN = '(',
                TT_RPAREN = ')',
        } TokenType;

        typedef struct {
                TokenType type;
                long double val;
        } Token;

        extern Token *lex(char *current_char);
#endif
