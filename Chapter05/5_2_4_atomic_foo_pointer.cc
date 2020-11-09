#include <atomic>
#include <cassert>
#include <iostream>

class Foo {};

int main() {
  Foo some_array[5];
  std::atomic<Foo *> p(some_array);
  Foo *t = p.fetch_add(2);
#ifdef NDEBUG
# undef NDEBUG
  assert(t == some_array);
  assert(p.load() == &some_array[2]);
#define NDEBUG
#else
  assert(t == some_array);
  assert(p.load() == &some_array[2]);
#endif
  t = (p -= 1);
#ifdef NDEBUG
# undef NDEBUG
  assert(t == &some_array[1]);
  assert(p.load() == &some_array[1]);
#define NDEBUG
#else
  assert(t == &some_array[1]);
  assert(p.load() == &some_array[1]);
#endif
}
