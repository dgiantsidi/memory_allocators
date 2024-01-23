#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdlib.h>
#include <vector>

template <class t> class _my_alloc {
public:
  using value_type = t;
  using pointer = t *;
  using reference = t &;
  using const_pointer = t const *;
  using const_reference = t const &;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  _my_alloc() = default;
  _my_alloc(_my_alloc &&) = default;
  t *allocate(size_t sz) { return reinterpret_cast<t *>(malloc(sz)); }

  template <class U> struct rebind { typedef _my_alloc<U> other; };
  template <class U> constexpr _my_alloc(_my_alloc<U> const &) noexcept {}

  void deallocate(pointer ptr) { free(ptr); }

  void deallocate(pointer ptr, size_t) { free(ptr); }

private:
};

template <class t, typename my_alloc> class Allocator {
public:
  using value_type = t;
  using pointer = t *;
  using reference = t &;
  using const_pointer = t const *;
  using const_reference = t const &;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

  template <class U> struct rebind { typedef Allocator<U, my_alloc> other; };

  Allocator() { std::cout << __PRETTY_FUNCTION__ << "\n"; }
  Allocator(Allocator &&) = default;

  template <class U>
  constexpr Allocator(Allocator<U, my_alloc> const &) noexcept {}

  pointer allocate(size_type num, void const * = nullptr) {

    std::cout << __PRETTY_FUNCTION__ << "\n";
    return reinterpret_cast<pointer>(arena.allocate(sizeof(t) * num));
    // return reinterpret_cast<pointer>(malloc(sizeof(T) * num));
  }

  void deallocate(pointer p) {
    arena.deallocate(p);
    std::cout << __PRETTY_FUNCTION__ << "\n";
    // free(p);
  }

  void deallocate(pointer p, size_type s) {
    arena.deallocate(p, s);
    std::cout << " <" << __PRETTY_FUNCTION__ << "\n";
    // free(p);
  }

private:
  my_alloc arena;
};

template <typename T> struct my_ptr {
  T *ptr;
  size_t size;
  auto operator*() { return ptr; }
};

using MyVector =
    std::vector<uint64_t, Allocator<uint64_t, _my_alloc<uint64_t>>>;

int main() {
  MyVector v;
  v.push_back(4);
  v.push_back(10);
  v.push_back(40);
  v.push_back(50);
  Allocator<my_ptr<int>, _my_alloc<my_ptr<int>>> alloc;
  auto my_ptr = alloc.allocate(1);
  std::cout << (uintptr_t)my_ptr << "\n";
  std::cout << typeid(*my_ptr).name() << "\n";
  std::cout << (uintptr_t)(my_ptr->ptr) << "\n";
  std::cout << (uintptr_t) * (*my_ptr) << "\n";
  alloc.deallocate(my_ptr);
}
