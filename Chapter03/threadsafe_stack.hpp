#include <thread>
#include <exception>
#include <algorithm>
#include <stack>
#include <functional>
#include <mutex>
#include <iostream>

using namespace std;

struct empty_stack : std::exception {
  const char *what() const throw () {
    return "oops";
  }
};

template <typename T>
class threadsafe_stack {
  std::stack<T> data;
  mutable std::mutex m;
public:
  threadsafe_stack() {}
  threadsafe_stack(const threadsafe_stack &other) {
    std::lock_guard<std::mutex> lock(other.m);
    data = other.data;
  }
  threadsafe_stack &operator=(const threadsafe_stack &) = delete;

  void push(T new_value) {
    std::lock_guard<std::mutex> lock(m);
    data.push(new_value);
  }

  std::shared_ptr<T> pop() {
    std::lock_guard<std::mutex> lock(m);
    if (data.empty()) throw empty_stack(); // check for empty before trying to pop value
    const std::shared_ptr<T> res(std::make_shared<T>(data.top()));
    data.pop();
    return res;
  }
  void pop(T &value) {
    std::lock_guard<std::mutex> lock(m);
    if (data.empty()) throw empty_stack();
    value = data.top();
    data.pop();
  }
  // This is superfluous
  bool empty() const {
    std::lock_guard<std::mutex> lock(m);
    return data.empty();
  }
};
