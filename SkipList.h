#ifndef SKIPLIST_H
#define SKIPLIST_H

template <class T> class SkipNode {
public:
  T *data;
  SkipNode<T> *next = nullptr;
  SkipNode<T> *previous = nullptr;
  SkipNode<T> *toBigger = nullptr;
  SkipNode<T> *toSmaller = nullptr;
};

template <class T> class SkipList {
public:
  unsigned int item_counter = 0;
  SkipNode<T> *_begin = nullptr;
  SkipNode<T> *_end = nullptr;

  /* T will be a Node<T> pointer */
  void append(T *value);
  unsigned int size();
};

template <class T> inline void SkipList<T>::append(T *value) {

  SkipNode<T> *item = new SkipNode<T>;
  item->data = value;

  if (item_counter == 0) {

    _begin = item;
    _end = item;
    item->next = nullptr;
    item->previous = nullptr;

  } else {

    _end->next = item;
    item->previous = _end;
    item->next = nullptr;
    _end = item;
  }

  item_counter++;
}

template <class T> inline unsigned int SkipList<T>::size() {
  return item_counter;
}
#endif // SKIPLIST_H
