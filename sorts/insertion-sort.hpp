#include <algorithm>
#include <iterator>
#include <type_traits>
namespace DevUt {
template <typename It, typename = typename std::enable_if<
                           std::is_base_of<std::bidirectional_iterator_tag,
                                           typename std::iterator_traits<
                                               It>::iterator_category>::value,
                           bool>::type>
void insertion_sort(It first, It last) {
  using std::advance;
  for (It key = std::next(first); key != last; advance(key, 1)) {
    It elem_ptr = prev(key);
    auto val = *key;
    while (val < *elem_ptr) {
      std::swap(*next(elem_ptr), *elem_ptr);
      if (elem_ptr == first)
        break;
      else
        advance(elem_ptr, -1);
    }
  }
}

}  // namespace DevUt