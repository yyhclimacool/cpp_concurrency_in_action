#include <mutex>
#include <condition_variable>
#include <thread>
#include <string>
#include <queue>
#include <chrono>

using namespace std;

template<typename T>
class threadsafe_queue {
  mutable mutex queue_mutex;
  condition_variable queue_cond;
  queue<T> data_queue;
public:
  threadsafe_queue() = default;
  threadsafe_queue(const threadsafe_queue &other) {
    lock_guard<mutex> gurad(queue_mutex);
    data_queue = other.data_queue;
  }

  void push(const T &val) {
    {
      lock_guard<mutex> gurad(queue_mutex);
      data_queue.push(val);
    }
    queue_cond.notify_one();
  }

  void wait_and_pop(T &ret) {
    unique_lock<mutex> lk(queue_mutex);
    queue_cond.wait(lk, [this]{ !data_queue.empty(); });
    ret = data_queue.front();
    data_queue.pop();
  }

  shared_ptr<T> wait_and_pop() {
    unique_lock<mutex> lk(queue_mutex);
    queue_cond.wait(lk, [this]{ !data_queue.empty(); });
    auto ret = make_shared<T>(std::move(data_queue.front()));
    data_queue.pop();
    return ret;
  }

  bool try_pop(T &ret) {
    lock_guard<mutex> guard(queue_mutex);
    if (data_queue.empty())
      return false;
    ret = data_queue.front();
    data_queue.pop();
    return true;
  }

  shared_ptr<T> try_pop() {
    lock_guard<mutex> guard(queue_mutex);
    if (data_queue.empty())
      return nullptr;
    auto ret = make_shared<T>(data_queue.front());
    data_queue.pop();
    return ret;
  }

  size_t size() const {
    lock_guard<mutex> guard(queue_mutex);
    return data_queue.size();
  }
};

threadsafe_queue<string> sq;

void thread_a() {
  for (int i = 0; i != 100; ++i) {
    char buf[32]{};
    snprintf(buf, sizeof buf, "data%d", i);
    sq.push(string(buf));
  }
}

void thread_b() {
  string val;
  while (true) {
    if(sq.try_pop(val)) {
      printf("get data from queue:[%s]\n", val.c_str());
    }
    this_thread::sleep_for(chrono::seconds(1));
  }
}

int main() {
  thread(thread_a).detach();
  thread(thread_b).detach();
  thread(thread_b).detach();
  pthread_exit(NULL);
}
