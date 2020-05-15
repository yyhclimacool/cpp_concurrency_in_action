#include <thread>
#include <string>
#include <iostream>

#define MESSAGE_JOINABLE(threadObj) \
  do { \
    if ((threadObj).joinable()) \
      std::cout << #threadObj" is joinable" << std::endl; \
    else \
      std::cout << #threadObj" is not joinable" << std::endl; \
  } \
  while (0);

void do_some_work() {
  std::cout << "func:" << __func__ << std::endl;
}

void do_other_work() {
  std::cout << "func:" << __func__ << std::endl;
}

class scoped_thread {
public:
  explicit scoped_thread(std::thread t) : t_(std::move(t)) {
    if (!t_.joinable()) {
      throw std::logic_error("thread is not joinable");
    }
  }

  ~scoped_thread() {
    t_.join();
    std::cout << "std::thread object get joined" << std::endl;
  }
  scoped_thread(const scoped_thread &) = delete;
  scoped_thread &operator=(const scoped_thread &) = delete;
private:
  std::thread t_;
};

struct Func {
public:
  int &val;

  Func(int &ival) : val(ival) {}
  void operator()() {
    std::cout << "func:" << __func__ << " val:" << val << std::endl;
  }
};

void oops() {
  int local_val = 66;
  scoped_thread{std::thread{Func(local_val)}};
}

class joining_thread {
  std::thread t;
public:
  joining_thread() noexcept = default;
  template<typename Callable, typename ... Args>
  explicit joining_thread(Callable &&func, Args && ... args) : 
    t(std::forward<Callable>(func), std::forward<Args>(args) ...) {}

  explicit joining_thread(std::thread t_) noexcept :
    t(std::move(t_)) {}

  joining_thread(joining_thread &&other) noexcept :
    t(std::move(other.t)) {}

  joining_thread &operator=(joining_thread &&other) noexcept {
    if (joinable())
      join();
    t = std::move(other.t);
    return *this;
  }

  joining_thread &operator=(std::thread other) noexcept {
    if (joinable())
      join();
    t = std::move(other);
    return *this;
  }

  ~joining_thread() noexcept {
    if (joinable())
      join();
  }

  void swap(joining_thread &other) noexcept {
    t.swap(other.t);
  }

  std::thread::id get_id() const noexcept {
    return t.get_id();
  }

  bool joinable() const noexcept {
    return t.joinable();
  }

  void join() {
    t.join();
  }

  void detach() {
    t.detach();
  }

  std::thread &as_thread() noexcept {
    return t;
  }
  const std::thread &as_thread() const noexcept {
    return t;
  }
};

int main() {
  std::thread t1(do_some_work);
  std::thread t2(std::move(t1));
  t1 = std::thread(do_other_work);
  std::thread t3;
  t3 = std::move(t2);
  //t1 = std::move(t3); // crash immediatly !

  MESSAGE_JOINABLE(t1);
  MESSAGE_JOINABLE(t2);
  MESSAGE_JOINABLE(t3);
  t1.join();
  //t2.join();
  t3.join();

  oops();
}
