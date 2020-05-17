#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include <vector>

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
  friend void swap(X &, X &);
public:
  X(const some_big_object &obj) : object_((obj)) {}

private:
  mutable std::mutex mutex_;
  some_big_object object_;
};

void swap(X &lhs, X &rhs) {
  if (&lhs == &rhs) return;
  using std::swap;
  std::cout << "swapping class X object using std::unique_lock ..." << std::endl;
  std::unique_lock<std::mutex> lock_a(lhs.mutex_, std::defer_lock);
  std::cout << "lock_a owns lock " << lock_a.owns_lock() << std::endl;
  std::unique_lock<std::mutex> lock_b(rhs.mutex_, std::defer_lock);
  std::cout << "lock_b owns lock " << lock_b.owns_lock() << std::endl;
  std::lock(lock_a, lock_b);
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

  swap(x1, x2);
}
