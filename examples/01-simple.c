#include "../src/json.h"
#include <stdio.h>

int main(void) {
  const char *raw =
      "{\"key\": \"users\", \"values\": [{ \"id\": 1, \"name\": \"carlos\"}]}";
  json_value_t *root = json_parse(raw);

  json_value_t *key = json_get(root, "key");
  json_value_t *id = json_get(root, "values.[0].id");

  printf("%s %lf", key->as.string, id->as.number);

  json_free(root);

  return 0;
}
