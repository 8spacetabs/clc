#include <stdio.h>
#include <math.h>

#include "parser.h"
#include "lexer.h"

static Token g_current_token;
static Token *g_token_stream_ptr;
static long double g_last_result;

static long double parse_addsub(void);

//
// next_token - set g_current_token to the next token in the token stream and increments g_token_stream_ptr
//
static inline Token next_token(void) {
        return (g_current_token = *g_token_stream_ptr++);
}

//
// parse_term - parses parenthesised expressions, unary expressions with '+', '-' and '\' and constants
//
static long double parse_term(void) {
        long double result;

        switch (g_current_token.type) {
                case TT_CONST:
                        result = g_current_token.val;
                        break;
                case TT_LPAREN:
                        next_token();
                        result = parse_addsub();
                        if (g_current_token.type != TT_RPAREN) {
                                fputs("expected a closing ')'\n", stderr);
                                return 0;
                        }
                        break;
                case TT_SUB:
                        next_token();
                        return -parse_term();
                case TT_ADD:
                        next_token();
                        return parse_term();
                case TT_SQR:
                        next_token();

                        result = sqrtl(parse_term());
                        if (isnan(result)) {
                                fputs("can't take the root of a negative value\n", stderr);
                                return 0;
                        }

                        return result;
                case TT_ANSWER:
                        return g_last_result;
                case TT_STREAM_END:
                        fputs("unexpected end of input\n", stderr);
                        return 0;

                default:
                        fputs("token is invalid in this context\n", stderr);
                        return 0;
        }

        next_token();
        return result;
}

//
// parse_pow - parses exponent expressions with '^'
//
static long double parse_pow(void) {
        long double result = parse_term();

        while (g_current_token.type == TT_POW) {
                next_token();
                result = powl(result, parse_term());
        } 

        return result;
}

//
// parse_muldivmod - parses multiplication, division and modulo expression with '*', '/' and '%'
//
static long double parse_muldivmod(void) {
        long double result = parse_pow();
        long double dividend;

        for (;;) {
                switch (g_current_token.type) {
                        case TT_MUL:
                                next_token();
                                result *= parse_pow();
                                break;
                        case TT_DIV:
                                next_token();

                                dividend = parse_pow();
                                if (dividend == 0)
                                        goto div_by_zero;

                                result /= dividend;
                                break;
                        case TT_MOD:
                                next_token();

                                dividend = parse_pow();
                                if (dividend == 0)
                                        goto div_by_zero;

                                result = fmodl(result, dividend);
                                break;

                        default:
                                return result;
                }
        }

div_by_zero:
        fputs("detected division by zero\n", stderr);
        return 0;
}

//
// parse_addsub - parses addition and subtraction expressions with '+' and '-'
//
static long double parse_addsub(void) {
        long double result = parse_muldivmod();

        for (;;) {
                if (g_current_token.type == TT_ADD) {
                        next_token();
                        result += parse_muldivmod();
                } else if (g_current_token.type == TT_SUB) {
                        next_token();
                        result -= parse_muldivmod();
                } else
                        return result;
        }
}

//
// parse - points g_token_stream_ptr at the token stream and begins parsing, returns eval of entire tokenised expression
//
long double parse(Token *token_stream) {
        g_token_stream_ptr = token_stream;
        next_token();
        
        return g_last_result = parse_addsub();
}
