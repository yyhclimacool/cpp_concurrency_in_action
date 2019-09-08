//
// Test with this shell command:
// --$> while true; do ./a.out ;done
//
#include <thread>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <memory>
#include <functional>
#include <iostream>

using namespace std;

class some_big_object {
public:
  int *ival;
  some_big_object(int i) : ival(new int(i)) {}
  some_big_object(const some_big_object &sd) {
    ival = new int(*sd.ival);
  }
  ~some_big_object() { delete ival; }

  int *get() const { return ival; }
};

void swap(some_big_object &lhs, some_big_object &rhs) {
  std::swap(lhs.ival, rhs.ival);
}

class X {
  // Problematic race-condition: dead lock
  friend void unsafe_swap(X &lhs, X &rhs) {
    if (&lhs == &rhs)
      return;
    std::lock_guard<std::mutex> lock_a(lhs.m);
    std::lock_guard<std::mutex> lock_b(rhs.m);
    swap(lhs.some_detail, rhs.some_detail);
  }

  friend void safe_swap(X &lhs, X &rhs) {
    if (&lhs == &rhs)
      return;
    // lock two mutexes
    std::lock(lhs.m, rhs.m);
    // std::adopt_lock indicate that the mutexes are already locked
    // and std::lock_guard should just adopt the ownership of the 
    // existing lock on the mutex rather than attempt to lock the 
    // mutex in the constructor.
    std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
    swap(lhs.some_detail, rhs.some_detail);
  }

private:
  some_big_object some_detail;
  std::mutex m;
public:
  X(const some_big_object &sd) : some_detail(sd) {}

  some_big_object &get_detail() { return some_detail; }
};

void unsafe_swap(X &lhs, X &rhs);
void safe_swap(X &lhs, X &rhs);

void thread_routine_unsafeswap() {
    some_big_object o1(100), o2(200);
    X x1(o1), x2(o2);
    auto t1 = std::thread(unsafe_swap, std::ref(x1), std::ref(x2));
    auto t2 = std::thread(unsafe_swap, std::ref(x2), std::ref(x1));
    // Join them in the reverse order
    // Might be a chance to dead-lock
    t2.join();
    t1.join();

    cout << *(x1.get_detail().get()) << ", " << *(x2.get_detail().get()) << endl;
}

void thread_routine_safeswap() {
    some_big_object o1(100), o2(200);
    X x1(o1), x2(o2);
    auto t1 = std::thread(safe_swap, std::ref(x1), std::ref(x2));
    auto t2 = std::thread(safe_swap, std::ref(x2), std::ref(x1));
    // Join them in the reverse order
    // Won't be dead-lock
    t2.join();
    t1.join();

    cout << *(x1.get_detail().get()) << ", " << *(x2.get_detail().get()) << endl;
}

int main() {
  // thread_routine_unsafeswap();
  thread_routine_safeswap();
}
