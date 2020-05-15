#include <thread>
#include <iostream>
#include <string>
#include <pthread.h>

void f(int i, const std::string &s) {
  std::cout << "int:" << i << " string:" << s << std::endl;
}

void oops(int val) {
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "stringfy-%d", val);
  std::thread t(f, val, std::string(buffer));
  t.detach();
}

using widget_id = int;
using widget_data = int;

void update_widget_data(widget_id w, widget_data &data) {
  (void)w;
  data = 64;
}

void oops_again() {
  widget_id w = 42;
  widget_data data;
  std::thread t(update_widget_data, w, std::ref(data));
  t.join();
}

class X {
public:
  void do_lengthy_work(int ival, std::string &msg) {
    std::cout << "Func:" << __func__ << " int:" << ival << " msg:" << msg << std::endl;
    msg = "finish";
  }
};

int main() {
  std::thread t(f, 42, "hello");
  t.join();
  oops(11);
  oops_again();

  X local_x;
  std::string str = "hello,U!";
  std::thread t1(&X::do_lengthy_work, &local_x, 42, std::ref(str));
  t1.join();
  std::cout << "str:" << str << std::endl;
  pthread_exit(NULL);
}
