#include <iostream>
#include <stdlib.h>
#include <vector>

void *operator new(size_t sz) {
  std::cout << __PRETTY_FUNCTION__ << " :" << sz << " bytes\n";
  return malloc(sz);
}

int main(void) {
  std::vector<int> vec;
  for (auto i = 0; i < 64; i++) {
    vec.push_back(i);
  }
  // std::cout << st << "\n";
  return 0;
}
