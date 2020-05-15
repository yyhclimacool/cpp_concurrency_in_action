#include <iostream>
#include <string>
#include <thread>

void split_info() {
  int batch_size_min_item = 25;

  for (int i = 1; i < 1000; ++i) {
    int batch_num = (i + batch_size_min_item - 1)/batch_size_min_item;
    int block_size = i / batch_num;
    std::cout << i << " -- " << "batch_num = " << batch_num << " -- block_size = " << block_size << std::endl;
  }
}

int main() {
  split_info();
}
