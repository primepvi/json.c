#include "../include/internal/lexer.h"
#include "../include/internal/parser.h"

#include <stdio.h>

int main(void) {
  json_lexer_t *lexer =
      jsonlexer_new(SV("{\"key\": \"users\", \"values\": [1, 2, 3, 4, 5]}"));

  vector_t *tokens = jsonlexer_lex(lexer);

  for (size_t i = 0; i < tokens->length; i++)
    jsontoken_print(*(json_token_t *)vector_at(tokens, i));

  json_parser_t *parser = jsonparser_new(tokens);
  json_value_t *root = jsonparser_parse(parser);

  vector_t *entries = root->as.object.entries;
  for (size_t i = 0; i < entries->length; i++) {
    json_object_entry_t *entry = vector_at(entries, i);
    printf("KEY: %s\n", entry->key);
  }

  return 0;
}
