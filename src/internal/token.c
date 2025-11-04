#include "../../include/internal/token.h"

static const char *JSON_TOKEN_NAMES[16] = {
    [JTOK_NULL] = "JTOK_NULL",
    [JTOK_TRUE] = "JTOK_TRUE",
    [JTOK_FALSE] = "JTOK_FALSE",
    [JTOK_NUMBER] = "JTOK_NUMBER",
    [JTOK_STRING] = "JTOK_STRING",
    [JTOK_OPEN_BRACE] = "JTOK_OPEN_BRACE",
    [JTOK_CLOSE_BRACE] = "JTOK_CLOSE_BRACE",
    [JTOK_OPEN_BRACKET] = "JTOK_OPEN_BRACKET",
    [JTOK_CLOSE_BRACKET] = "JTOK_CLOSE_BRACKET",
    [JTOK_COMMA] = "JTOK_COMMA",
    [JTOK_COLON] = "JTOK_COLON",
    [JTOK_EOF] = "JTOK_EOF"};

void jsontoken_print(json_token_t token) {
  printf("token -> [%s] = " SV_FMT "\n", JSON_TOKEN_NAMES[token.kind],
         SV_ARG(token.lexeme));
};

const char *jsontoken_kindname(json_token_kind_t kind) {
  return JSON_TOKEN_NAMES[kind];
}
