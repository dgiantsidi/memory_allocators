#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <vector>

template <class T> class _my_alloc_heap {
public:
  using pointer = T *;

  _my_alloc_heap() = default;
  _my_alloc_heap(_my_alloc_heap &&) = default;
  pointer allocate(size_t sz) { return reinterpret_cast<pointer>(malloc(sz)); }

  template <class U> struct rebind { typedef _my_alloc_heap<U> other; };
  template <class U>
  constexpr _my_alloc_heap(_my_alloc_heap<U> const &) noexcept {}

  void deallocate(pointer ptr) { free(ptr); }

  void deallocate(pointer ptr, size_t) { free(ptr); }

private:
};
