#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

class some_big_object {
  friend void swap(some_big_object &, some_big_object &);
public:
  some_big_object(const std::vector<int> &v) : vec_(v) {}
private:
  std::vector<int> vec_;
};

void swap(some_big_object &lhs, some_big_object &rhs) {
  using std::swap;
  std::cout << "Swaping the internal vector..." << std::endl;
  swap(lhs.vec_, rhs.vec_);
}

class X {
  // this version of swap would deadlock
  friend void deadlock_swap(X &, X &);

  // this version of swap won't deadlock
  friend void nodeadlock_swap(X &, X &);

  // this version use std::scoped_lock
  friend void easy_swap(X &, X &);
public:
  X(const some_big_object &obj) : object_((obj)) {}

private:
  mutable std::mutex mutex_;
  some_big_object object_;
};

void deadlock_swap(X &lhs, X &rhs) {
  if (&lhs == &rhs) return;
  std::lock_guard guard(lhs.mutex_);
  std::lock_guard guard2(rhs.mutex_);
  using std::swap;
  swap(lhs.object_, rhs.object_);
}

void nodeadlock_swap(X &lhs, X &rhs) {
  // attempting to acquire a lock on std::mutex when you already hold it is undefiened behavior
  // a mutex that does permit multiple locks by the same thread is provided in the form of std::recursive_mutex
  if (&lhs == &rhs) return;

  // C++ 11
  // a function that can lock two or more mutexes at once without risk of deadlock.
  // std::lock provides all-or-nothing semantics with regard to locking the supplied mutex.
  std::lock(lhs.mutex_, rhs.mutex_);
  // std::adopt_lock indicate to the std::lock_guard that the mutexes are already locked,
  // and they should adopt the ownership of the existing lock ont the mutex rather than attempt to lock the mutex in the constructor.
  std::lock_guard<std::mutex> guard_lhs(lhs.mutex_, std::adopt_lock);
  std::lock_guard<std::mutex> guard_rhs(rhs.mutex_, std::adopt_lock);
  using std::swap;
  swap(lhs.object_, rhs.object_);
}

void easy_swap(X &lhs, X &rhs) {
  if (&lhs == &rhs) return;
  std::scoped_lock<std::mutex, std::mutex> lock(lhs.mutex_, rhs.mutex_);
  using std::swap;
  swap(lhs.object_, rhs.object_);
}

int main() {
  std::vector<int> vec1 = {1,2,3,4,5,6,7,8};
  std::vector<int> vec2(vec1.begin(), vec1.end());
  for (int i = 10; i < 20; ++i) {
    vec2.push_back(i);
  }

  X x1{some_big_object(vec1)};
  X x2{some_big_object(vec2)};

  // call deadlock_swap in reverse order
  /*
  std::cout << "two thread calling swap maybe deadlock" << std::endl;
  std::thread t1(deadlock_swap, std::ref(x2), std::ref(x1));
  deadlock_swap(x1, x2);
  t1.join();
  std::cout << "If you see me, no deadlock." << std::endl;
  */

  std::cout << "two thread calling swap won't deadlock." << std::endl;
  std::thread t2(nodeadlock_swap, std::ref(x2), std::ref(x1));
  nodeadlock_swap(x1, x2);
  t2.join();
  std::cout << "if you see me, no deadlock." << std::endl;

  std::cout << "two thread calling swap won't deadlock." << std::endl;
  std::thread t3(easy_swap, std::ref(x2), std::ref(x1));
  easy_swap(x1, x2);
  t3.join();
  std::cout << "if you see me, no deadlock." << std::endl;
}
