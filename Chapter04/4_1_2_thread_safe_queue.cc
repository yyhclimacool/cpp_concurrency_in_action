#include <mutex>
#include <condition_variable>
#include <thread>
#include <string>
#include <queue>

using namespace std;

template<typename T>
class threadsafe_queue {
  mutex queue_mutex;
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
};

int main() {
}
