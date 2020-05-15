#include <thread>
#include <iostream>
#include <string>

using namespace std;

void f(int i, const std::string &s) {
  cout << s << ", " << i << endl;
  //s = "Settle";
}

using widget_id = int;
using widget_data = std::string;

void update_data_for_widget(widget_id w, widget_data &data) {
  cout << "id = " << w << ", data = " << data << endl;
  data += "(copied)";
}

void process_widget(widget_id w) {
  widget_data data("local_data");
  std::thread t(update_data_for_widget, w, std::ref(data));
  t.join();
  cout << data << endl;
}

int main() {
  string s = "fine";
  std::thread t(f, 3, s);
  t.join();
  std::thread t2(f, 4, "fine");
  t2.join();

  process_widget(42);
}
