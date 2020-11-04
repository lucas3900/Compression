/*
* Lucas Barusek
* Implementaion of Priority Queue
*
*/
template <typename T>
priority_queue<T>::priority_queue(int (*cmp)(const T &, const T &)){
  /* priority queue constructor */

  // sets the capacity to initally be 256 for all the characters
  _capacity = 256;
  _size = 0; // empty min heap
  _cmp = cmp; // defines the comparison function
  _data = new T[_capacity]; // dynamically allocated array of size capacity
}

template <typename T>
void priority_queue<T>::_reheap_up(size_t index){
  /* reheaps an item up the minheap until the properties of a min heap
  are being respected */

  // while we aren't at the root of the tree and the parent is larger than
  // the data at the current index, swap the parent and tthe current index
   while ((index > 0) and (_cmp(_data[_parent(index)], _data[index]) > 0)){
    _swap(index, _parent(index));
    index = _parent(index);
  }
}

template <typename T>
void priority_queue<T>::add(const T & item){
  /* adds an item to the end of the minheap and then reheaps up to
  maintain the array as a min heap */

  //add the item to the end of the array, reheaps that item up if need be,
  // and increments the size
  _data[_size] = item;
  _reheap_up(_size);
  _size++;

  // if we are at capacity then grow!
  if (_size >= _capacity) _grow();
}

template <typename T>
size_t priority_queue<T>::_parent(size_t index){
  /* returns index of parent */
  return (index - 1) / 2;
}

template <typename T>
size_t priority_queue<T>::_left_child(size_t index){
  /* returns the index of the left child */
  return (2 * index) + 1;
}

template <typename T>
size_t priority_queue<T>::_right_child(size_t index){
  /* returns the index of the right child */
  return (2 * index) + 2;
}

template <typename T>
void priority_queue<T>::_swap(size_t x, size_t y){
  /* swaps two items within the array */

  T swapy = _data[x];
  _data[x] = _data[y];
  _data[y] = swapy;
}

template <typename T>
void priority_queue<T>::_reheap_down(size_t index){
  /* checks if an item at an index needs to be swapped with one of its
  children, and if does swaps them until the array is restored to a min heap */

  // checks if the node has no children, if so exit function
  if (_left_child(index) >= _size) return;

  // sets the left child to be current index to swap with
  size_t child = _left_child(index);

  // if the right child exists and its data is less than the left child's,
  // assign the right child's index to be the child that may be swapped with
  if ((_right_child(index) < _size) and (_cmp(_data[_right_child(index)], _data[child]) < 0))
    child = _right_child(index);

  // the current index has data larger than it's smallest child's data then
  // swap them and recursively call _reheap_down
  if (_cmp(_data[index], _data[child]) > 0){
    _swap(index, child);
    _reheap_down(child);
  }
}

template <typename T>
void priority_queue<T>::remove(){
  /* removes the front item in the priority queue */

  // assigns the last item in the minheap to be the new root, and then
  // reheaps down until array is a min heap
  _data[0] = _data[--_size];
  _reheap_down(0);
}

template <typename T>
T priority_queue<T>::front() const{
  /* returns the first item in the minheap */
  return _data[0];
}

template <typename T>
void priority_queue<T>::_grow(){
  /* increases the size of the min heap */

  // doubles the capacity
  _capacity *= 2;

  // creates a new array with double capacity
  T * copy =  new T[_capacity];

  // copies all the data from the original to the copy
  for (size_t x = 0; x < _size; x++)
    copy[x] = _data[x];

  // delets the original array, and assigns the original array to the copy
  delete [] _data;
  _data = copy;
}

template <typename T>
size_t priority_queue<T>::size() const{
  /* returns the size of the priority_queue */
  return _size;
}
