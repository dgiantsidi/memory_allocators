#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <vector>

template <class T> class _my_alloc_stack {
public:
  static constexpr int kStackSz = 1024;
  using pointer = T *;

  _my_alloc_stack() : cur_ptr(reinterpret_cast<uintptr_t>(stack_mem)) {
    std::cout << __PRETTY_FUNCTION__ << " " << (void *)stack_mem << " "
              << (void *)cur_ptr << " "
              << "\n";
  };

  _my_alloc_stack(_my_alloc_stack &&) = default;

  pointer allocate(size_t sz) {
    if (sz < free_mem()) {
      allocated_mem += sz + sizeof(int);
      auto *ret_ptr = reinterpret_cast<pointer>(cur_ptr + sizeof(int));
      std::cout << __PRETTY_FUNCTION__ << " " << sz << "bytes @ " << ret_ptr
                << " set metadata to ptr=" << reinterpret_cast<int *>(cur_ptr)
                << "\n";
      ::memcpy(reinterpret_cast<int *>(cur_ptr), &sz, sizeof(int));
      cur_ptr += sz + sizeof(int);
      return ret_ptr;
    } else
      exit(128);
    return nullptr;
  }

  template <class U> struct rebind { typedef _my_alloc_stack<U> other; };
  template <class U>
  constexpr _my_alloc_stack(_my_alloc_stack<U> const &) noexcept {}

  void deallocate(pointer ptr) {
    int sz = 0;
    //           ::memcpy(&sz, reinterpret_cast<int*>(ptr-sizeof(int)),
    //           sizeof(int));
    std::cout << "freed " << sz << " bytes\n";
  }

  void deallocate(pointer ptr, size_t a) {
    std::cout << __PRETTY_FUNCTION__ << " " << a * sizeof(T) << "  @ " << ptr
              << "\n";
    int sz = 0;
    if (ptr != reinterpret_cast<pointer>(stack_mem)) {
      /*              std::cout << ((void*)(ptr)-sizeof(int)) << "\n";
                    std::cout << ((void*)(ptr)) << "\n";
                    std::cout << ((ptr)) << "\n";
                    std::cout << (reinterpret_cast<int*>(ptr)) << "\n";
                    */
      //              ::memcpy(&sz, (reinterpret_cast<int*>(ptr)- 1),
      //              sizeof(int));
      ::memcpy(&sz, (reinterpret_cast<uint8_t *>(ptr) - sizeof(int)),
               sizeof(int));
      //           ::memcpy(&sz, reinterpret_cast<int*>(ptr-sizeof(int)),
      //           sizeof(int));
      // std::cout << "freed " << sz << " bytes\n";
      allocated_mem -= sz + sizeof(int);
    }
  }

private:
  size_t free_mem() { return (kStackSz - allocated_mem); }
  uint8_t stack_mem[kStackSz];
  uint64_t allocated_mem = 0;
  uintptr_t cur_ptr;
};
