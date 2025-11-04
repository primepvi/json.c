#include "../../include/internal/strview.h"

string_view_t sv_create(const char *buffer, size_t length) {
  return (string_view_t){.buffer = buffer, .length = length};
}

bool sv_equals(string_view_t a, string_view_t b) {
  return a.length == b.length && memcmp(a.buffer, b.buffer, a.length) == 0;
}

bool sv_starts_with(string_view_t source, string_view_t prefix) {
  if (source.length < prefix.length)
    return false;

  return memcmp(source.buffer, prefix.buffer, prefix.length) == 0;
}

string_view_t sv_shift(string_view_t source, size_t amount) {
  if (amount > source.length)
    amount = source.length;

  string_view_t out = sv_create(source.buffer, amount);
  source.buffer += amount;
  source.length -= amount;

  return out;
}

string_view_t sv_pop(string_view_t source, size_t amount) {
  if (amount > source.length)
    amount = source.length;

  size_t offset = source.length - amount;
  string_view_t out = sv_create(source.buffer + offset, amount);
  source.length -= amount;

  return out;
}

string_view_t sv_slice(string_view_t source, size_t start, size_t end) {
  if (start > source.length)
    start = source.length;
  if (end > source.length)
    end = source.length;
  if (end < start)
    end = start;
  if (end == SV_END)
    end = source.length;

  return sv_create(source.buffer + start, end - start);
}

string_view_t sv_take_while(string_view_t source, sv_predicate_t predicate) {
  size_t cursor = 0;
  while (cursor < source.length && predicate(source.buffer[cursor]))
    cursor++;

  return sv_create(source.buffer, cursor);
}

char *sv_cstr(string_view_t source) {
  char *parsed = malloc(source.length + 1);
  memcpy(parsed, source.buffer, source.length);
  parsed[source.length] = '\0';

  return parsed;
}
