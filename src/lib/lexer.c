#include "lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static bool alnum_predicate(const char c) { return (bool)isalnum(c); }
static bool digit_predicate(const char c) { return (bool)isdigit(c); }
static bool str_predicate(const char c) { return c != '"'; }
static bool space_predicate(const char c) { return (bool)isspace(c); }

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
    // jsontoken_print(current);
    vector_push(tokens, &current);
  }

  return tokens;
}

json_token_t jsonlexer_tokenize(json_lexer_t *lexer) {
  string_view_t source = sv_slice(lexer->source, lexer->cursor, SV_END);
  string_view_t spaces = sv_take_while(source, space_predicate);
  lexer->cursor += spaces.length;

  string_view_t block = sv_slice(source, spaces.length, SV_END);
  if (block.length <= 0)
    return (json_token_t){.kind = JTOK_EOF, .lexeme = SV("\0")};

  string_view_t number = sv_take_while(block, digit_predicate);
  if (number.length > 0 || sv_starts_with(block, SV("-"))) {
    return jsonlexer_tokenize_number(lexer);
  }

  string_view_t text = sv_take_while(block, alnum_predicate);
  if (text.length > 0) {
    lexer->cursor += text.length;

    if (sv_equals(text, SV("false")))
      return (json_token_t){.kind = JTOK_FALSE, .lexeme = text};
    if (sv_equals(text, SV("true")))
      return (json_token_t){.kind = JTOK_TRUE, .lexeme = text};
    if (sv_equals(text, SV("null")))
      return (json_token_t){.kind = JTOK_NULL, .lexeme = text};

    fprintf(stderr,
            "jsonlexer -> unexpected token (identifier) found " SV_FMT "\n",
            SV_ARG(text));
    exit(EXIT_FAILURE);
  }

  if (sv_starts_with(block, SV("\""))) {
    string_view_t content =
        sv_take_while(sv_slice(block, 1, SV_END), str_predicate);

    sv_shift(block, content.length); // removing content
    if (!sv_starts_with(block, SV("\""))) {
      fprintf(stderr, "jsonlexer -> expected end of string at %zu\n",
              lexer->cursor);
      exit(EXIT_FAILURE);
    }

    lexer->cursor += content.length + 2;
    return (json_token_t){.kind = JTOK_STRING, .lexeme = content};
  }

  lexer->cursor += 1;
  if (sv_starts_with(block, SV("{")))
    return (json_token_t){.kind = JTOK_OPEN_BRACE, .lexeme = SV("{")};
  if (sv_starts_with(block, SV("}")))
    return (json_token_t){.kind = JTOK_CLOSE_BRACE, .lexeme = SV("}")};
  if (sv_starts_with(block, SV("[")))
    return (json_token_t){.kind = JTOK_OPEN_BRACKET, .lexeme = SV("[")};
  if (sv_starts_with(block, SV("]")))
    return (json_token_t){.kind = JTOK_CLOSE_BRACKET, .lexeme = SV("]")};
  if (sv_starts_with(block, SV(":")))
    return (json_token_t){.kind = JTOK_COLON, .lexeme = SV(":")};
  if (sv_starts_with(block, SV(",")))
    return (json_token_t){.kind = JTOK_COMMA, .lexeme = SV(",")};

  fprintf(stderr, "jsonlexer -> unexpected token found " SV_FMT "\n",
          SV_ARG(sv_shift(block, 1)));
  exit(EXIT_FAILURE);
}

json_token_t jsonlexer_tokenize_number(json_lexer_t *lexer) {
  string_view_t source = lexer->source;
  size_t start = lexer->cursor;
  bool has_dot = false;
  bool has_exp = false;

  if (source.buffer[lexer->cursor] == '-')
    lexer->cursor++;

  while (lexer->cursor < source.length) {
    char current = source.buffer[lexer->cursor];

    if (isdigit(current)) {
      lexer->cursor++;
    } else if (current == '.' && !has_dot && !has_exp) {
      has_dot = true;
      lexer->cursor++;
    } else if (tolower(current) == 'e' && !has_exp) {
      has_exp = true;
      current = source.buffer[++lexer->cursor];

      if (lexer->cursor < source.length && (current == '-' || current == '+'))
        lexer->cursor++;
    } else {
      break;
    }
  }

  string_view_t lexeme = sv_slice(source, start, lexer->cursor);
  if (sv_starts_with(lexeme, SV("-")) && lexeme.length <= 1) {
    fprintf(stderr, "jsonlexer -> expected number after '-' at %zu\n",
            lexer->cursor);
    exit(EXIT_FAILURE);
  }

  return (json_token_t){.kind = JTOK_NUMBER, .lexeme = lexeme};
}
