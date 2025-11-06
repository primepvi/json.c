#include "parser.h"

json_parser_t *jsonparser_new(vector_t *tokens) {
  json_parser_t *parser = malloc(sizeof(json_parser_t));
  parser->cursor = 0;
  parser->tokens = tokens;

  return parser;
}

void jsonparser_destroy(json_parser_t *parser) {
  vector_destroy(parser->tokens);
  free(parser);
}

json_value_t *jsonparser_parse(json_parser_t *parser) {
  json_token_t *current = jsonparser_peek(parser);
  if (current->kind == JTOK_OPEN_BRACE)
    return jsonparser_parse_object(parser);
  if (current->kind == JTOK_OPEN_BRACKET)
    return jsonparser_parse_array(parser);

  fprintf(stderr, "jsonparser -> unexpected token (%s) as found: " SV_FMT "\n",
          jsontoken_kindname(current->kind), SV_ARG(current->lexeme));
  exit(EXIT_FAILURE);
}

json_value_t *jsonparser_parse_value(json_parser_t *parser) {
  json_token_t *current = jsonparser_peek(parser);
  switch (current->kind) {
  case JTOK_TRUE:
  case JTOK_FALSE:
    return jsonparser_parse_bool(parser);
  case JTOK_STRING:
    return jsonparser_parse_string(parser);
  case JTOK_NUMBER:
    return jsonparser_parse_number(parser);
  case JTOK_NULL:
    return jsonparser_parse_null(parser);
  default:
    return jsonparser_parse(parser);
  }
}

json_value_t *jsonparser_parse_bool(json_parser_t *parser) {
  json_token_t *current = jsonparser_consume(parser);
  json_value_t *value = malloc(sizeof(json_value_t));
  value->type = JSON_BOOL;
  value->as.boolean = sv_equals(current->lexeme, SV("true"));

  return value;
}

json_value_t *jsonparser_parse_string(json_parser_t *parser) {
  json_token_t *current = jsonparser_consume(parser);
  json_value_t *value = malloc(sizeof(json_value_t));
  value->type = JSON_STRING;
  value->as.string = sv_cstr(current->lexeme);

  return value;
}

json_value_t *jsonparser_parse_number(json_parser_t *parser) {
  json_token_t *current = jsonparser_consume(parser);
  json_value_t *value = malloc(sizeof(json_value_t));
  value->type = JSON_NUMBER;

  char *endptr = NULL;
  char *data = sv_cstr(current->lexeme);
  value->as.number = strtod(data, &endptr);

  if (endptr == data) {
    fprintf(stderr, "jsonparser -> invalid number: " SV_FMT "\n",
            SV_ARG(current->lexeme));
    exit(EXIT_FAILURE);
  }

  return value;
}

json_value_t *jsonparser_parse_null(json_parser_t *parser) {
  json_token_t *current = jsonparser_consume(parser);
  json_value_t *value = malloc(sizeof(json_value_t));
  value->type = JSON_NULL;

  return value;
}

json_value_t *jsonparser_parse_object(json_parser_t *parser) {
  jsonparser_expect(parser, JTOK_OPEN_BRACE);

  vector_t *entries = vector_new(32, sizeof(json_object_entry_t));

  json_token_t *current = jsonparser_peek(parser);
  while (current->kind != JTOK_CLOSE_BRACE) {
    json_token_t *key = jsonparser_expect(parser, JTOK_STRING);
    json_token_t *colon = jsonparser_expect(parser, JTOK_COLON);
    json_value_t *value = jsonparser_parse_value(parser);

    json_object_entry_t entry = {.key = sv_cstr(key->lexeme), .value = value};
    vector_push(entries, &entry);

    current = jsonparser_peek(parser);

    if (current->kind == JTOK_CLOSE_BRACE)
      break;

    jsonparser_expect(parser, JTOK_COMMA);
  }

  jsonparser_expect(parser, JTOK_CLOSE_BRACE);

  json_object_t object = {entries};
  json_value_t *value = malloc(sizeof(json_value_t));
  value->type = JSON_OBJECT;
  value->as.object = object;

  return value;
}

json_value_t *jsonparser_parse_array(json_parser_t *parser) {
  jsonparser_expect(parser, JTOK_OPEN_BRACKET);

  vector_t *items = vector_new(32, sizeof(json_value_t *));

  json_token_t *current = jsonparser_peek(parser);
  while (current->kind != JTOK_CLOSE_BRACKET) {
    json_value_t *value = jsonparser_parse_value(parser);
    vector_push(items, &value);

    current = jsonparser_peek(parser);
    if (current->kind == JTOK_CLOSE_BRACKET)
      break;

    jsonparser_expect(parser, JTOK_COMMA);
  }

  jsonparser_expect(parser, JTOK_CLOSE_BRACKET);

  json_array_t array = {items};
  json_value_t *value = malloc(sizeof(json_value_t));
  value->type = JSON_ARRAY;
  value->as.array = array;

  return value;
}

json_token_t *jsonparser_peek(json_parser_t *parser) {
  if (parser->cursor >= parser->tokens->length)
    return NULL;

  return (json_token_t *)vector_at(parser->tokens, parser->cursor);
}

json_token_t *jsonparser_consume(json_parser_t *parser) {
  if (parser->cursor >= parser->tokens->length)
    return NULL;

  return (json_token_t *)vector_at(parser->tokens, parser->cursor++);
}

json_token_t *jsonparser_expect(json_parser_t *parser, json_token_kind_t kind) {
  if (!jsonparser_peek(parser)) {
    fprintf(stderr, "jsonparser -> expected token %s, but received null.\n",
            jsontoken_kindname(kind));
    exit(EXIT_FAILURE);
  }

  json_token_t *token = jsonparser_consume(parser);
  if (token->kind == kind)
    return token;

  if (token->kind == JTOK_EOF) {
    fprintf(stderr, "jsonparser -> expected token %s, but received eof.\n",
            jsontoken_kindname(kind));
    exit(EXIT_FAILURE);
  }

  fprintf(stderr, "jsonparser -> expected token %s, but received: %s.\n",
          jsontoken_kindname(kind), jsontoken_kindname(token->kind));
  exit(EXIT_FAILURE);
}
