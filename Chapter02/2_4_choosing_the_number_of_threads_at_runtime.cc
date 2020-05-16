#include <iostream>
#include <string>
#include <thread>

static const int batch_size_avg_item = 25;  // 平均N个元素形成一个batch
static const int batch_num_max_thread = 24; // 最多起M个线程,或者受到线程池大小的约束

void split_info() {
  for (int i = 1; i < 10000; ++i) {
    int batch_num = (i + batch_size_avg_item - 1)/batch_size_avg_item;
    batch_num = std::min(batch_num_max_thread, batch_num);
    int block_size = i / batch_num;
    int left = i - block_size * batch_num;
    // left < batch_num 一定成立，left可以在当前线程处理，也可以将left放到每个线程处理，这样每个线程至多只增加一个元素的负担
    std::cout << i << " -- " << "batch_num = " << batch_num << " -- block_size = " << block_size << " -- left = " << left << std::endl;
  }
}

int main() {
  split_info();
}
