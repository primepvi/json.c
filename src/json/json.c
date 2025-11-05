#include "../../include/json/json.h"
#include "../../include/internal/lexer.h"
#include "../../include/internal/parser.h"
#include <ctype.h>
#include <stdio.h>

static bool json_key_predicate(const char c) { return c != '.'; }
static bool json_index_predicate(const char c) { return isdigit(c); }

json_value_t *json_parse(const char *source) {
  json_lexer_t *lexer = jsonlexer_new(sv_create(source, strlen(source)));
  vector_t *tokens = jsonlexer_lex(lexer);

  json_parser_t *parser = jsonparser_new(tokens);
  json_value_t *root = jsonparser_parse(parser);

  return root;
};

void json_free(json_value_t *v) {
  switch (v->type) {
  case JSON_NUMBER:
  case JSON_BOOL:
  case JSON_NULL:
    return free(v);
  case JSON_STRING:
    free(v->as.string);
    return free(v);
  case JSON_ARRAY: {
    vector_t *items = v->as.array.items;
    for (size_t i = 0; i < items->length; i++) {
      json_value_t *value = vector_at(items, i);
      json_free(value);
    }
    vector_destroy(items);
    return free(v);
  }
  case JSON_OBJECT: {
    vector_t *entries = v->as.object.entries;
    for (size_t i = 0; i < entries->length; i++) {
      json_object_entry_t *entry = vector_at(entries, i);
      json_free(entry->value);
      free(entry->key);
    }
    vector_destroy(entries);
    return free(v);
  }
  }
};

void json_print(json_value_t *v) {};

json_value_t *json_get(json_value_t *v, const char *path) {
  string_view_t view = sv_create(path, strlen(path));
  json_value_t *current = v;

  while (view.length > 0 && current != NULL) {
    if (sv_starts_with(view, SV("["))) {
      string_view_t index =
          sv_take_while(sv_slice(view, 1, SV_END), json_index_predicate);
      view = sv_slice(view, index.length + 3, SV_END);

      char *indexptr = sv_cstr(index);
      size_t parsed = atoi(indexptr);
      free(indexptr);

      current = json_get_index(current, parsed);
    } else {
      string_view_t key = sv_take_while(view, json_key_predicate);
      current = json_access(current, key);
      view = sv_slice(view, key.length + 1, SV_END);
    }
  }

  return current;
};

json_value_t *json_get_index(const json_value_t *arr, size_t index) {
  vector_t *items = arr->as.array.items;
  if (!items)
    return NULL;

  return *(json_value_t **)vector_at(items, index);
};

json_value_t *json_access(const json_value_t *v, string_view_t key) {
  vector_t *entries = v->as.object.entries;
  if (!entries)
    return NULL;

  for (size_t i = 0; i < entries->length; i++) {
    json_object_entry_t *entry = vector_at(entries, i);
    string_view_t entry_key = sv_create(entry->key, strlen(entry->key));
    if (sv_equals(key, entry_key))
      return entry->value;
  }

  return NULL;
};
