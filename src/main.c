#include "../include/internal/lexer.h"

#include <stdio.h>

int main(void) {
  json_lexer_t *lexer =
      jsonlexer_new(SV("{\"key\": \"users\", \"values\": [1, 2, 3, 4, 5]}"));
  vector_t *tokens = jsonlexer_lex(lexer);

  for (size_t i = 0; i < tokens->length; i++)
    jsontoken_print(*(json_token_t *)vector_at(tokens, i));

  return 0;
}
