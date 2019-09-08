#include <thread>
#include <list>
#include <algorithm>
#include <mutex>
#include <iostream>
#include <string>
#include <vector>
#include <functional>

using namespace std;

std::list<int> ilst;
std::mutex _mutex;

void add_to_list(int ival) {
  std::lock_guard<std::mutex> guard(_mutex);
  ilst.push_back(ival);
}

bool list_contains(int val_to_find) {
  std::lock_guard<std::mutex> guard(_mutex);
  return std::find(ilst.begin(), ilst.end(), val_to_find) != ilst.end();
}

int main() {
  std::vector<std::thread> threads;
  for (int i = 10; i < 100; ++i) {
    threads.push_back(std::thread(add_to_list, i * 9));
  }

  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

  for (int i = 99; i >= 10; --i) {
    if ((i % 11) == 0) {
      bool ret = list_contains(i * 9);
      if (ret) printf("OK\n");
      else printf("ERROR\n");
    }
  }

  cout << "size = " << ilst.size() << endl;

  for (const auto i : ilst)
    cout << i << ", ";
  cout << endl;
}
