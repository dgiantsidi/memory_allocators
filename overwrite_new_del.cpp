#include <iostream>
#include <memory>
#include <stdlib.h>
#include <vector>

static uint64_t allocs_size = {0};
static uint64_t deallocs_size = {0};

void *operator new(std::size_t n) {
  allocs_size += n;
  std::cout << __PRETTY_FUNCTION__ << " " << allocs_size << " " << n << "\n";
  return malloc(n);
}

void operator delete(void *p) {
  std::cout << __PRETTY_FUNCTION__ << "\n";
  free(p);
}

int main(void) {
  std::vector<int> vec;
  for (auto i = 0; i < 16; i++)
    vec.push_back(i);
  return 0;
}
