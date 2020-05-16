#include <iostream>
#include <string>
#include <vector>
#include <memory> // std::shared_ptr<>
#include <thread>
#include <mutex>
#include <stack>

//// An outline class definition for a thread-safe stack
//struct empty_stack : std::exception {
//  const char *what() const noexcept {
//  }
//};

template <typename T, typename Container = std::deque<T>>
class threadsafe_stack {
public:
  threadsafe_stack() = default;
  threadsafe_stack(const threadsafe_stack &other) : stack_impl_(other.stack_impl_) {}
  threadsafe_stack &operator=(const threadsafe_stack &) = delete;
  //void push(T new_value) {
  //  std::lock_guard guard(stack_mutex_);
  //  stack_impl_.push(std::move(new_value));
  //}
  void push(const T &new_value) {
    std::lock_guard guard(stack_mutex_);
    stack_impl_.push(new_value);
  }

  std::shared_ptr<T> pop() {
    std::lock_guard guard(stack_mutex_);
    // my implementation still allocate memory(exception maybe)
    auto &ptr = std::make_shared<T>(std::move(stack_impl_.top()));
    stack_impl_.pop();
    return ptr;
  }

  void pop(T &val) {
    std::lock_guard guard(stack_mutex_);
    val = std::move(stack_impl_.top());
    stack_impl_.pop();
  }

  bool empty() const {
    std::lock_guard guard(stack_mutex_);
    return stack_impl_.empty();
  }

  size_t size() const {
    std::lock_guard guard(stack_mutex_);
    return stack_impl_.size();
  }

private:
  // "mutable" is required, or compile error
  mutable std::mutex stack_mutex_;
  std::stack<T, Container> stack_impl_;
};

// manipuldate on this global_stack will core dump...
//std::stack<std::string> global_stack;

threadsafe_stack<std::string> global_stack;

void unsafe_stack_manip() {
  for (int i = 0; i < 100; ++i) {
    char str[64]{};
    snprintf(str, sizeof str, "elem-%d", i);
    global_stack.push(std::string(str));
  }
}

int main() {
  // main thread and other thread manipulate global_stack simultanously
  std::thread t1(unsafe_stack_manip);
  for (int i = 0; i < 20; ++i) {
    char str[64]{};
    snprintf(str, sizeof str, "main-thread-%d", i);
    global_stack.push(std::string(str));
  }

  t1.join();
  // print 120
  std::cout << "global_stack.size() = " << global_stack.size() << std::endl;
}
