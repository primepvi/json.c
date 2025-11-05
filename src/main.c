#include "../include/json/json.h"
#include <stdio.h>

int main(void) {
  const char *raw =
      "{\"key\": \"users\", \"values\": [{ \"id\": 1, \"name\": \"carlos\"}]}";
  json_value_t *root = json_parse(raw);

  json_value_t *key = json_get(root, "key");
  json_value_t *first_user = json_get(root, "values.[0].id");

  printf("%s %lf", key->as.string, first_user->as.number);
  return 0;
}
