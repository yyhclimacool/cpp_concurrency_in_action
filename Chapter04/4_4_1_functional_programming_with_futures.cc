#include <iostream>
#include <list>
#include <algorithm>
#include <future>

#define PRINTALL(x) \
  do { \
    for (const auto &elem : (x)) \
      std::cout << elem << " "; \
    std::cout << std::endl; \
  } while (false)

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input) {
  if (input.empty())
    return input;
  std::list<T> result;
  result.splice(result.begin(), input, input.begin());
  const T &pivot = *result.begin();
  auto divide_point = std::partition(input.begin(), input.end(), [&](const T &t) { return t < pivot; });
  std::list<T> lower_part;
  lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
  auto new_lower_part(sequential_quick_sort(std::move(lower_part)));
  auto new_higher_part(sequential_quick_sort(std::move(input)));
  result.splice(result.end(), new_higher_part);
  result.splice(result.begin(), new_lower_part);
  return result;
}

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
  if (input.empty())
    return input;
  std::list<T> result;
  result.splice(result.begin(), input, input.begin());
  const T &pivot = *result.begin();
  auto part_point = std::partition(input.begin(), input.end(), [&](const T &t) { return t < pivot; });
  std::list<T> low_part;
  low_part.splice(low_part.begin(), input, input.begin(), part_point);
  std::future<std::list<T>> new_low_part(std::async(parallel_quick_sort<T>, std::move(low_part)));
  auto high_part(parallel_quick_sort(std::move(input)));
  result.splice(result.end(), high_part);
  result.splice(result.begin(), new_low_part.get());
  return result;
}

int main() {
  std::list<int> ilist{5, 7, 3, 4, 1, 9, 2, 8, 10, 6};
  auto new_ilist{ilist};
  PRINTALL(ilist);
  auto res = sequential_quick_sort(ilist);
  PRINTALL(res);

  res = parallel_quick_sort(new_ilist);
  PRINTALL(res);
}
