#pragma once

#include "lib/strview.h"
#include "lib/value.h"
#include "lib/vector.h"

json_value_t *json_parse(const char *source);
void json_free(json_value_t *v);
void json_print(json_value_t *v);

json_value_t *json_get(json_value_t *v, const char *path);
json_value_t *json_get_index(const json_value_t *arr, size_t index);

json_value_t *json_access(const json_value_t *v, string_view_t key);
