#include <iostream>
#include <stdlib.h>

void *operator new(size_t sz) {
  std::cout << __PRETTY_FUNCTION__ << " :" << sz << " bytes\n";
  return malloc(sz);
}

int main(void) {
  std::string st;
  for (auto i = 0; i < 64; i++) {
    st += "a";
  }
  // std::cout << st << "\n";
  return 0;
}
