#pragma once

#include "vector.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum {
  JSON_NULL,
  JSON_BOOL,
  JSON_NUMBER,
  JSON_STRING,
  JSON_ARRAY,
  JSON_OBJECT
} json_type_t;

typedef struct json_value json_value_t;

typedef struct json_array {
  vector_t *items;
} json_array_t;

typedef struct json_object_entry {
  char *key;
  json_value_t *value;
} json_object_entry_t;

typedef struct json_object {
  vector_t *entries;
} json_object_t;

typedef struct json_value {
  json_type_t type;
  union {
    char *string;
    double number;
    bool boolean;
    json_array_t array;
    json_object_t object;
  } as;
} json_value_t;
