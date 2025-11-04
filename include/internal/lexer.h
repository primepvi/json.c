#pragma once

#include "strview.h"
#include "token.h"
#include "vector.h"

#include <stddef.h>

typedef struct json_lexer {
  size_t col, row, cursor;
  string_view_t source;
} json_lexer_t;

json_lexer_t *jsonlexer_new(string_view_t source);
void jsonlexer_destroy(json_lexer_t *lexer);

vector_t *jsonlexer_lex(json_lexer_t *lexer);
json_token_t jsonlexer_tokenize(json_lexer_t *lexer);
json_token_t jsonlexer_tokenize_number(json_lexer_t *lexer);
