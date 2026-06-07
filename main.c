#include "cache.h"

int main() {
  Cache c = { 0 };

  char res;
  cache_read(c, 4096, &res);

  return 0;
}
