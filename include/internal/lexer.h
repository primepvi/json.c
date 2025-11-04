#pragma once

#include "strview.h"
#include "vector.h"
#include <stddef.h>

typedef struct json_lexer {
  size_t col, row, cursor;
  string_view_t source;
} json_lexer_t;

typedef enum json_token_kind {
  JTOK_NULL,  // null
  JTOK_TRUE,  // true
  JTOK_FALSE, // false
  JTOK_NUMBER,
  JTOK_STRING,

  JTOK_OPEN_BRACE,    // {
  JTOK_CLOSE_BRACE,   // }
  JTOK_OPEN_BRACKET,  // [
  JTOK_CLOSE_BRACKET, // ]

  JTOK_COMMA, // ,
  JTOK_COLON, // :
  JTOK_QUOTE, // "

  JTOK_EOF
} json_token_kind_t;

typedef struct json_token {
  json_token_kind_t kind;
  string_view_t lexeme;
} json_token_t;

json_lexer_t *jsonlexer_new(string_view_t source);
void jsonlexer_destroy(json_lexer_t *lexer);

vector_t *jsonlexer_lex(json_lexer_t *lexer);
json_token_t jsonlexer_tokenize(json_lexer_t *lexer);
