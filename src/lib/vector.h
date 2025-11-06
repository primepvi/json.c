#pragma once

#include <stdlib.h>

typedef struct {
  void *buffer;
  size_t length, elem_size;
  size_t capacity;
} vector_t;

vector_t *vector_new(size_t capacity, size_t elem_size);
void vector_destroy(vector_t *vector);

void vector_push(vector_t *vector, void *value);
void *vector_pop(vector_t *vector);
void *vector_at(vector_t *vector, size_t idx);
