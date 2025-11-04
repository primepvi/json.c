#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct string_view {
  const char *buffer;
  size_t length;
} string_view_t;

typedef bool (*sv_predicate_t)(const char c);

#define SV_LIT(__lit__)                                                        \
  (string_view_t) { .data = (__lit__), .length = strlen(__lit__) }
#define SV(__lit__) sv_create(__lit__, strlen(__lit__))
#define SV_FMT "%.*s"
#define SV_ARG(__sv__) (int)(__sv__).length, (__sv__).data

#define SV_END 0

string_view_t sv_create(const char *buffer, size_t length);
bool sv_equals(string_view_t a, string_view_t b);
bool sv_starts_with(string_view_t source, string_view_t prefix);

string_view_t sv_shift(string_view_t source, size_t amount);
string_view_t sv_pop(string_view_t source, size_t amount);
string_view_t sv_slice(string_view_t source, size_t start, size_t end);
string_view_t sv_take_while(string_view_t source, sv_predicate_t predicate);

char *sv_cstr(string_view_t source);
