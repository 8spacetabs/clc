#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "lexer.h"
#include "parser.h"

enum {
        SUCCESS,
        ERR_LEX_FAILED,
        ERR_MALLOC,
};

int main(int argc, char **argv) {
        Token *token_stream = NULL;
        char *repl_input = NULL;

        if (argc > 1) {
                token_stream = lex(argv[1]);
                if (token_stream == NULL) {
                        fputs("failed to allocate token stream", stderr);
                        return ERR_MALLOC;
                } else if (token_stream == (Token*)ERR_INVALID_CHAR) {
                        return ERR_LEX_FAILED;                        
                }

                printf("\n%Lf\n", parse(token_stream));
                free(token_stream);
        } else {
                while (repl_input = readline("clc> ")) {
                        token_stream = lex(repl_input);
                        if (token_stream == NULL) {
                                fputs("failed to allocate token stream", stderr);
                                free(repl_input);
                                return ERR_MALLOC;
                        } else if (token_stream == ERR_INVALID_CHAR) {
                                continue;
                        }

                        printf("\n%Lf\n", parse(token_stream));
                        add_history(repl_input);

                        free(token_stream);
                        free(repl_input);
                }
                putchar('\n');
        }

        return SUCCESS;
}

