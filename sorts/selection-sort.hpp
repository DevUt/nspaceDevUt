#include <algorithm>
#include <iterator>
#include <type_traits>

namespace DevUt {
template <
    typename It,
    typename = std::enable_if_t<
        std::is_base_of_v<std::bidirectional_iterator_tag,
                          typename std::iterator_traits<It>::iterator_category>,
        bool>>
void selection_sort(It first, It last) {
  using std::advance;
  for (It placement_ptr = first; placement_ptr != last;
       advance(placement_ptr, 1)) {
    It min_elem_ptr = placement_ptr;
    for (It elem_ptr = placement_ptr; elem_ptr != last; advance(elem_ptr, 1)) {
      if (*min_elem_ptr > *elem_ptr) {
        min_elem_ptr = elem_ptr;
      }
    }
    std::swap(*min_elem_ptr, *placement_ptr);
  }
}
}  // namespace DevUt
