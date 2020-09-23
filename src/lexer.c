#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "lexer.h"

static char *char_stream_ptr;
static char current_char;

//
// next_char - sets current_char to the next character in the character stream and increments char_stream_ptr
//
static inline char next_char(void) {
        return (current_char = *char_stream_ptr++);
}

//
// strlen_no_ws - returns number of non-whitespace characters in a string
//
static size_t strlen_no_ws(const char *string) {
        size_t len = 0;
        for (int i = 0; string[i] != '\0'; ++i)
                if (!isspace(string[i]))
                        ++len;
                
        return len;
}

//
// lex - allocates, writes to and then returns a pointer to a token stream, given a stream of characters
//
Token *lex(char *char_stream) {
        char_stream_ptr = char_stream;

        Token *token_stream, *token_stream_ptr;
        if (!(token_stream_ptr = token_stream = malloc(sizeof(Token) * strlen_no_ws(char_stream) + sizeof(Token))))
                return NULL;

        next_char();
        for (;;) {
                if (isspace(current_char)) {
                        next_char();
                } else if (isdigit(current_char)) {
                        long double val = 0;

                        for (; isdigit(current_char); next_char()) {
                                val *= 10;
                                val += current_char - '0';
                        }

                        if (current_char == '.') {
                                next_char();

                                int place_val = 10;
                                for (; isdigit(current_char); next_char()) {
                                        val += (double)(current_char - '0') / place_val;
                                        place_val *= 10;
                                }
                        }

                        *token_stream_ptr++ = (Token){
                                .val  = val,
                                .type = TT_CONST
                        };
                } else {
                        switch (current_char) {
                                case '+':
                                        // FALLTHROUGH
                                case '-':
                                        // FALLTHROUGH
                                case '*':
                                        // FALLTHROUGH
                                case '/':
                                        // FALLTHROUGH
                                case '%':
                                        // FALLTHROUGH
                                case '^':
                                        // FALLTHROUGH
                                case '\\':
                                        // FALLTHROUGH
                                case '(':
                                        // FALLTHROUGH
                                case ')':
                                        token_stream_ptr->type = current_char;
                                        break;
                                case '\0':
                                        // FALLTHROUGH
                                case '\n':
                                        token_stream_ptr->type = TT_STREAM_END;
                                        return token_stream;

                                default:
                                        free(token_stream);
                                        fprintf(stderr, "invalid char `%c`\n", current_char);
                                        return ERR_INVALID_CHAR;
                        }
                        next_char();
                        ++token_stream_ptr;
                }
        }

        return token_stream;
}
