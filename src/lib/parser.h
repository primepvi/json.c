#pragma once

#include "lexer.h"
#include "value.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct json_parser {
  size_t cursor;
  vector_t *tokens;
} json_parser_t;

json_parser_t *jsonparser_new(vector_t *tokens);
void jsonparser_destroy(json_parser_t *parser);

json_value_t *jsonparser_parse(json_parser_t *parser);
json_value_t *jsonparser_parse_value(json_parser_t *parser);
json_value_t *jsonparser_parse_array(json_parser_t *parser);
json_value_t *jsonparser_parse_object(json_parser_t *parser);
json_value_t *jsonparser_parse_number(json_parser_t *parser);
json_value_t *jsonparser_parse_string(json_parser_t *parser);
json_value_t *jsonparser_parse_bool(json_parser_t *parser);
json_value_t *jsonparser_parse_null(json_parser_t *parser);

json_token_t *jsonparser_peek(json_parser_t *parser);
json_token_t *jsonparser_consume(json_parser_t *parser);
json_token_t *jsonparser_expect(json_parser_t *parser, json_token_kind_t kind);
