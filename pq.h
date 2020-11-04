#ifndef PQ
#define PQ

template <typename T>
class priority_queue {
 private:

  int (*_cmp)(const T &, const T &);  // _cmp is the comparison function
  T * _data; // array of type T
  size_t _size; // the current size of the array
  size_t _capacity; // the total amount the array can hols

  // helper member function:

  // reheaps items in the array up until the all the rules of a min
  // heap are followed
  void _reheap_up(size_t index);

  // returns the parent's index
  size_t _parent(size_t index);

  // returns the left child's index
  size_t _left_child(size_t index);

  // returns the right child's index
  size_t _right_child(size_t index);

  // swaps two items in an array
  void _swap(size_t x, size_t y);

  // reheaps items in the array down until the all the rules of a min
  // heap are followed
  void _reheap_down(size_t index);

  // grows min heap if capacity is reached
  void _grow();

 public:

  // min heap contstructor
  priority_queue(int (*cmp)(const T &, const T &));

  // adds and item to the end of the array, and then reheaps up until
  // the array is a min heap
  void add(const T & item);

  // removes the root of the tree, promotes the last thing in the array
  // to be the new root, and then reheaps down until the array is a
  // min heap
  void remove();

  size_t size() const; // returns the size of the array
  T front() const;     // retursn the first item in the mean heap
};

#include "pq.cc"

#endif
