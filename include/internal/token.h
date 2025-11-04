#pragma once

#include "strview.h"
#include <stdio.h>

typedef enum json_token_kind {
  JTOK_NULL = 0, // null
  JTOK_TRUE,     // true
  JTOK_FALSE,    // false
  JTOK_NUMBER,
  JTOK_STRING,

  JTOK_OPEN_BRACE,    // {
  JTOK_CLOSE_BRACE,   // }
  JTOK_OPEN_BRACKET,  // [
  JTOK_CLOSE_BRACKET, // ]

  JTOK_COMMA, // ,
  JTOK_COLON, // :

  JTOK_EOF
} json_token_kind_t;

typedef struct json_token {
  json_token_kind_t kind;
  string_view_t lexeme;
} json_token_t;

static const char *JSON_TOKEN_NAMES[16];
void jsontoken_print(json_token_t token);
