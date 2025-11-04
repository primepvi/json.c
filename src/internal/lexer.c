#include "../../include/internal/lexer.h"
#include <ctype.h>

static bool __isalnum_predicate(const char c) { return (bool)isalnum(c); }
static bool __isdigit_predicate(const char c) { return (bool)isdigit(c); }
static bool __isstr_predicate(const char c) { return c != '"'; }
static bool __isspace_predicate(const char c) { return (bool)isspace(c); }

json_lexer_t *jsonlexer_new(string_view_t source) {
  json_lexer_t *lexer = malloc(sizeof(json_lexer_t));
  lexer->source = source;
  lexer->col = lexer->row = lexer->cursor = 0;

  return lexer;
}

void jsonlexer_destroy(json_lexer_t *lexer) { free(lexer); }

vector_t *jsonlexer_lex(json_lexer_t *lexer) {
  const size_t initial_capacity = 32;
  vector_t *tokens = vector_new(initial_capacity, sizeof(json_token_t));

  while (lexer->cursor < lexer->source.length) {
    json_token_t current = jsonlexer_tokenize(lexer);
    vector_push(tokens, &current);
  }

  return tokens;
}

json_token_t jsonlexer_tokenize(json_lexer_t *lexer) {
  string_view_t source = sv_slice(lexer->source, lexer->cursor, SV_END);
  string_view_t spaces = sv_take_while(source, __isspace_predicate);
  lexer->cursor += spaces.length;

  string_view_t block = sv_shift(source, spaces.length);
  if (block.length <= 0)
    return (json_token_t){.kind = JTOK_EOF, .lexeme = SV("\0")};

  // todo ...
}
