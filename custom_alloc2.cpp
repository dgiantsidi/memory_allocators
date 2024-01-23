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

template <class T, typename my_alloc> class Allocator {
public:
  using value_type = T;
  using pointer = T *;
  using reference = T &;
  using const_pointer = T const *;
  using const_reference = T const &;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

  template <class U> struct rebind { typedef Allocator<U, my_alloc> other; };

  Allocator() { std::cout << "Constructor: " << __PRETTY_FUNCTION__ << "\n"; }
  Allocator(Allocator &&) = default;

  template <class U>
  constexpr Allocator(Allocator<U, my_alloc> const &) noexcept {}

  pointer allocate(size_type num, void const * = nullptr) {
    //    std::cout << __PRETTY_FUNCTION__ << "\n";
    return reinterpret_cast<pointer>(arena.allocate(sizeof(T) * num));
  }

  void deallocate(pointer p) {
    arena.deallocate(p);
    //  std::cout << __PRETTY_FUNCTION__ << "\n";
  }

  void deallocate(pointer p, size_type s) {
    arena.deallocate(p, s);
    // std::cout << " <" << __PRETTY_FUNCTION__ << "\n";
  }

private:
  my_alloc arena;
};

template <typename T> struct my_ptr {
  T *ptr;
  size_t size;
  auto operator*() { return ptr; }
};

using heap_vector =
    std::vector<uint64_t, Allocator<uint64_t, _my_alloc_heap<uint64_t>>>;

using stack_vector = std::vector<int, Allocator<int, _my_alloc_stack<int>>>;

int main() {
  stack_vector v;
  v.push_back(4);
  v.push_back(10);
  v.push_back(40);
  v.push_back(50);
  /*
  {
  Allocator<my_ptr<int>, _my_alloc_heap<my_ptr<int>>> alloc;
  auto my_ptr = alloc.allocate(1);
  std::cout << (uintptr_t)my_ptr << "\n";
  std::cout << typeid(*my_ptr).name() << "\n";
  std::cout << (uintptr_t)(my_ptr->ptr) << "\n";
  std::cout << (uintptr_t) * (*my_ptr) << "\n";
  alloc.deallocate(my_ptr);

}
*/
}
