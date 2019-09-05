#include <thread>
#include <iostream>
#include <string>

using namespace std;

struct widget_data {
  widget_data() = default;
  widget_data(int i) : ival_(i) {}
  int ival_;
};

using widget_id = int;

void update_data_for_widget(widget_id w, widget_data &data) {
  data.ival_ = w;
}


void oops_widget(widget_id w) {
  widget_data data(1999);
  // if there is no std::ref, compile error
  std::thread t(update_data_for_widget, w, std::ref(data));
  cout << data.ival_ << endl;
  t.join();
  cout << data.ival_ << endl;
}

int main() {
  oops_widget(1);
}
