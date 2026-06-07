#include "cache.h"

int main() {
  Cache c = { 0 };

  cache_rd(&c, 1);
  cache_wr(&c, 1);

  return 0;
}
