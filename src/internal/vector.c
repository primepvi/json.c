#include "../../include/internal/vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vector_t *vector_new(size_t capacity, size_t elem_size) {
  if (capacity == 0)
    capacity = 16;

  vector_t *vector = malloc(sizeof(vector_t));
  if (!vector)
    return NULL;

  vector->buffer = malloc(capacity * elem_size);
  if (!vector->buffer) {
    free(vector);
    return NULL;
  }

  vector->length = 0;
  vector->capacity = capacity;
  vector->elem_size = elem_size;

  return vector;
}

void vector_destroy(vector_t *vector) {
  if (!vector)
    return;
  free(vector->buffer);
  free(vector);
}

void vector_push(vector_t *vector, void *value) {
  if (!vector || !value)
    return;

  if (vector->length >= vector->capacity) {
    size_t new_capacity = (vector->capacity * 3) / 2;
    if (new_capacity == 0)
      new_capacity = 1;

    void *new_buffer =
        realloc(vector->buffer, new_capacity * vector->elem_size);
    if (!new_buffer) {
      fprintf(stderr, "[vector] realloc failed\n");
      exit(EXIT_FAILURE);
    }

    vector->buffer = new_buffer;
    vector->capacity = new_capacity;
  }

  char *base = (char *)vector->buffer;
  memcpy(base + vector->length * vector->elem_size, value, vector->elem_size);
  vector->length++;
}

void *vector_pop(vector_t *vector) {
  if (!vector || vector->length == 0)
    return NULL;
  vector->length--;
  char *base = (char *)vector->buffer;
  return base + vector->length * vector->elem_size;
}

void *vector_at(vector_t *vector, size_t idx) {
  if (!vector || idx >= vector->length)
    return NULL;
  char *base = (char *)vector->buffer;
  return base + idx * vector->elem_size;
}
