#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <QDebug>
#include <QTime>
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <string>
#include <vector>

#include "SkipList.h"

using namespace std;

template <class T> class Node {
public:
  T data;
  int index = -1;
  Node<T> *next = nullptr;
  Node<T> *previous = nullptr;
};

template <class T>
bool operator>(const Node<T> &lvalue, const Node<T> &rvalue) {

  if (lvalue.data > rvalue.data)
    return true;
  else
    return false;
}

template <class T>
bool operator<(const Node<T> &lvalue, const Node<T> &rvalue) {

  if (lvalue.data < rvalue.data)
    return true;
  else
    return false;
}

/* LIST DECLARATION */
enum insertType { prepend, append, ascending, descending };

template <class T> class DoublyLinkedList {

private:
  /* mit unterstrich benennen */
  Node<T> *begin = nullptr;
  Node<T> *end = nullptr;
  Node<T> *current = nullptr;

public:
  vector<SkipList<Node<T>>> skipLists;

  /**************************************************************************/
  /* Praktikum 4 */
  /**************************************************************************/

  bool isSkiplist = false;
  unsigned long item_counter = 0;

  /* QuickSort */
  void quickSort();
  void partition(int startIndex, int endIndex, int pivotIndex);
  int getCurrentIndex();
  void stableQuickSort();
  DoublyLinkedList<T> partitionStable(DoublyLinkedList<T> list);
  void appendItem(Node<T> *value);
  int getIndex(Node<T> *value);
  void clear();

  /* HeapSort */
  void printTree();
  int calculateLevels();
  Node<T> *getNode(int index);
  int getParentIndex(int index);
  Node<T> *getParent(int index);
  int getLeftChildIndex(int index);
  Node<T> *getLeftChild(int index);
  int getRightChildIndex(int index);
  Node<T> *getRightChild(int index);
  void heapSort();
  void heapify();
  void siftDown(int start, int end);

  /* MergeSort */
  DoublyLinkedList<T> merge(DoublyLinkedList<T> &list1,
                            DoublyLinkedList<T> &list2);
  void mergeSort();
  int getSize();
  Node<T> *getBegin();
  Node<T> *getEnd();
  void swap(int lValue, int rValue);
  void swap(Node<T> *lValue, Node<T> *rValue);

  /**************************************************************************/
  /* End of Praktikum 4 */
  /**************************************************************************/

  T getItem();
  Node<T> *getCurrent();
  bool empty();
  bool isEnd();
  bool isBegin();
  void advance();
  void retreat();
  void reset();
  void setToEnd();
  void insert(const T &value, insertType type);
  void erase();
  void printList();
  void printCurrent();
};

/* FUNCTION DEFINITIONS */

template <class T> inline void DoublyLinkedList<T>::quickSort() {

  QTime time;
  srand(time.msecsSinceStartOfDay());
  int pivot = rand() % getSize();
  partition(0, getSize() - 1, pivot);
}
template <class T>
inline void DoublyLinkedList<T>::partition(int startIndex, int endIndex,
                                           int pivotIndex) {

  Node<T> *pivotNode = getNode(pivotIndex);
  Node<T> *leftIter = getNode(startIndex);
  Node<T> *rightIter = getNode(endIndex);

label_continue:

  Node<T> *itemLeft = nullptr;
  Node<T> *itemRight = nullptr;

  while (leftIter != pivotNode) {
    if (leftIter->data > pivotNode->data) {
      itemLeft = leftIter;
      break;
    }
    leftIter = leftIter->next;
  }

  while (rightIter != pivotNode) {
    if (rightIter->data < pivotNode->data) {
      itemRight = rightIter;
      break;
    }
    rightIter = rightIter->previous;
  }

  if (itemLeft and itemRight) {

    swap(getIndex(itemLeft), getIndex(itemRight));
    goto label_continue;
    /* besser als goto wäre es, die iteratoren mit in den rekursiven function
     * call aufzunehmen -> mit 2 funktionen realisieren */

    //    partition(startIndex, endIndex, pivotIndex);

  } else if (itemLeft and !itemRight) {

    swap(getIndex(itemLeft), getIndex(pivotNode));
    partition(startIndex, endIndex, pivotIndex);

  } else if (itemRight and !itemLeft) {

    swap(getIndex(itemRight), getIndex(pivotNode));
    partition(startIndex, endIndex, pivotIndex);

  } else if (!itemLeft and !itemRight) {

    if ((pivotIndex - startIndex) > 1) {
      int leftPivot = (rand() % (pivotIndex - startIndex + 1)) + startIndex;
      partition(startIndex, pivotIndex, leftPivot);
    }

    if ((endIndex - pivotIndex) > 1) {
      int rightPivot = (rand() % (endIndex - pivotIndex + 1)) + pivotIndex;
      partition(pivotIndex, endIndex, rightPivot);
    }
  }
}

template <class T> inline int DoublyLinkedList<T>::getCurrentIndex() {
  int counter = 0;
  Node<T> *pHelp = getBegin();
  while (pHelp != getCurrent()) {
    pHelp = pHelp->next;
    counter++;
  }
  return counter;
}

template <class T> inline void DoublyLinkedList<T>::stableQuickSort() {

  *this = partitionStable(*this);
}

template <class T>
inline DoublyLinkedList<T>
DoublyLinkedList<T>::partitionStable(DoublyLinkedList<T> list) {

  if (list.getSize() < 2) {

    return list;

  } else {

    QTime time;
    srand(time.msecsSinceStartOfDay());
    unsigned int pivotIndex = rand() % list.getSize();
    Node<T> *pivot = list.getNode(pivotIndex);

    DoublyLinkedList<T> smaller;
    DoublyLinkedList<T> bigger;

    unsigned int indexCounter = 0;
    auto iter = list.getBegin();
    while (iter != nullptr) {

      if (iter->data < pivot->data)
        smaller.appendItem(iter);

      else if (iter->data > pivot->data)
        bigger.appendItem(iter);

      else if (iter->data == pivot->data and iter != pivot) {

        if (indexCounter < pivotIndex)
          smaller.appendItem(iter);
        else
          bigger.appendItem(iter);
      }
      iter = iter->next;
      indexCounter++;
    }

    DoublyLinkedList<T> merge;
    DoublyLinkedList<T> left = partitionStable(smaller);
    DoublyLinkedList<T> right = partitionStable(bigger);

    iter = left.getBegin();
    while (iter != nullptr) {
      merge.appendItem(iter);
      iter = iter->next;
    }

    merge.appendItem(pivot);

    iter = right.getBegin();
    while (iter != nullptr) {
      merge.appendItem(iter);
      iter = iter->next;
    }

    return merge;
  }
}

template <class T> inline void DoublyLinkedList<T>::appendItem(Node<T> *value) {

  Node<T> *item = new Node<T>(*value);

  if (item_counter == 0) {

    item->next = nullptr;
    item->previous = nullptr;
    begin = item;
    end = item;
    current = item;

  } else {

    end->next = item;
    item->previous = end;
    item->next = nullptr;
    end = item;
  }

  item_counter++;
}

template <class T> inline int DoublyLinkedList<T>::getIndex(Node<T> *value) {

  int count = 0;
  Node<T> *pHelp = getBegin();
  while (pHelp != value) {

    pHelp = pHelp->next;
    count++;
  }

  return count;
}

template <class T> inline void DoublyLinkedList<T>::clear() {
  reset();
  while (item_counter != 0)
    erase();
}

template <class T> inline void DoublyLinkedList<T>::printTree() {

  reset();
  int levelCounter = 0;
  int levels = calculateLevels();
  while (getCurrent() != nullptr) {

    for (int i = 0; i < (pow(2, levelCounter)); i++) {

      if (getCurrent() != nullptr) {

        int distanceLeft = (pow(2, (levels - levelCounter - 1)) - 1);
        for (int j = 0; j < distanceLeft; j++) {
          cout << "   ";
        }

        cout << "[" << getItem() << "]";
        advance();

        int distanceRight = (pow(2, (levels - levelCounter)) - 1);
        distanceRight -= distanceLeft;
        for (int j = 0; j < distanceRight; j++) {
          cout << "   ";
        }
      }
    }

    ++levelCounter;
    cout << "\n";

    //        int distance = ( 0.75* (pow(2, (levels - levelCounter -1)) -1)
    //        )*3; if ( ++levelCounter < levels){

    //            for( int j = 0; j<distance; j++ ){
    //                cout << " ";
    //            }
    //            cout << "/";

    //            for( int j = 0; j<distance; j++ ){
    //                cout << " ";
    //            }
    //            cout << "\\";
    //        }

    cout << "\n";
  }
}

template <class T> inline int DoublyLinkedList<T>::calculateLevels() {

  reset();
  int counter = 0;
  while (getCurrent() != nullptr) {

    counter++;
    advance();
  }
  reset();

  int retval = int(log2(counter)) + 1;

  return retval;
}

template <class T> inline Node<T> *DoublyLinkedList<T>::getNode(int index) {

  if (isSkiplist) {

    int digits = 0;
    int number = index;
    while (number > 0) {

      number /= 10;
      digits++;
    }

    if (index > 9) {

      int skipListIndex = digits - 2;
      auto *pIter = skipLists[skipListIndex]._begin;
      while (skipListIndex >= 0) {

        number = index;
        /* so the iterationAmount is wrong
         * iterationAmount = digitValue of the corresponding digit */
        /* number = 221, digits = 3, but we cant used digits because
         * we need a certain digit depending on the listIndex */

        /* certain_digit is the digit in the number counting from right,
         * starting
         * at 0 so in 110 -> 0 would be 0, 1 would be 1, and 1 would be 2 */
        /* certain_digit = skipListIndex+1 */
        string numberString = to_string(number);
        /* need to reverse, string = '145' -> string[0] = 1, string[1] = 4,
         * string[2] = 5 */
        int certain_digit = (digits - 1) - (skipListIndex + 1);
        numberString = numberString.at(certain_digit);
        int iterationAmount = stoi(numberString);

        if (skipListIndex < (digits - 2))
          iterationAmount += 1;

        /* here is the next error, the first list is initialized,
         * but when we go to the next list and we need to iterate 1
         * with this loop we wont */

        int iterationCounter = 1;
        while (iterationCounter < iterationAmount) {
          pIter = pIter->next;
          iterationCounter++;
        }

        if (skipListIndex > 0) {
          pIter = pIter->toSmaller;
          skipListIndex--;
          continue;
        } else {
          break;
        }

      } // while

      auto pNode = pIter->data;
      int iterationAmount = (index % 10) + 1;
      int counter = 0;
      while (counter < iterationAmount) {

        pNode = pNode->next;
        counter++;
      }

      return pNode;

    } else {

      int counter = 0;
      reset();
      while (counter < index) {
        advance();
        counter++;
      }
      return getCurrent();
    }

  } else {

    int counter = 0;
    reset();
    while (counter < index) {
      advance();
      counter++;
    }
    return getCurrent();
  }
}

template <class T> inline int DoublyLinkedList<T>::getParentIndex(int index) {

  int parentIndex;
  if (index % 2 == 0)
    parentIndex = (index / 2) - 1;
  else
    parentIndex = int(index / 2);

  return parentIndex;
}

template <class T> inline Node<T> *DoublyLinkedList<T>::getParent(int index) {

  return getNode(getParentIndex(index));
}

template <class T>
inline int DoublyLinkedList<T>::getLeftChildIndex(int index) {

  int leftChildIndex = (index * 2) + 1;
  return leftChildIndex;
}

template <class T>
inline Node<T> *DoublyLinkedList<T>::getLeftChild(int index) {

  return getNode(getLeftChildIndex(index));
}

template <class T>
inline int DoublyLinkedList<T>::getRightChildIndex(int index) {

  int rightChildIndex = (index + 1) * 2;
  return rightChildIndex;
}

template <class T>
inline Node<T> *DoublyLinkedList<T>::getRightChild(int index) {
  return getNode(getRightChildIndex(index));
}

template <class T> inline void DoublyLinkedList<T>::heapSort() {

  /* building maxHeap */
  //  printTree();
  //  cout << "\n\n";
  heapify();
  //  printTree();
  //  cout << "\n\n";
  int size = getSize() - 1;
  while (size >= 0) {
    swap(size, 0);
    size = size - 1;
    siftDown(0, size);
    //    printTree();
    //    cout << "\n\n";
  }
}

template <class T> inline void DoublyLinkedList<T>::heapify() {

  /* A max-heap is a complete binary tree in which the value
   * in each internal node is greater than or equal to the
   * values in the children of that node.
   */

  /* start is assigned the index in a(the list) of the last parent node */
  /* which means the parent of the the last element */
  int start = getSize() - 1;
  int startIndex = getParentIndex(start);

  /* and now we have to find the index of the next parent node */
  /* which is kind of complicated */
  while (startIndex >= 0) {
    siftDown(startIndex, getSize() - 1);
    /* choppy workaround which will only work on complete trees */
    start -= 2;
    startIndex = getParentIndex(start);
  }
}

template <class T>
inline void DoublyLinkedList<T>::siftDown(int start, int end) {
  int root = start;

  while (getLeftChildIndex(root) <= end) {
    int _swap = root;

    /* check if the children are bigger and then swap the bigger one */
    Node<T> *leftChild;
    Node<T> *rightChild;
    Node<T> *rootNode = getNode(root);

    leftChild = getLeftChild(root);
    if (leftChild->data > rootNode->data)
      _swap = getLeftChildIndex(root);

    /* when we have a right child we always have a left child */
    if (getRightChildIndex(root) <= end) {
      rightChild = getRightChild(root);
      if (rightChild->data > leftChild->data and
          rightChild->data > rootNode->data)
        _swap = getRightChildIndex(root);
    }

    if (_swap == root)
      return;
    else {
      swap(root, _swap);
      root = _swap;
    }
  }
}

template <class T>
inline DoublyLinkedList<T>
DoublyLinkedList<T>::merge(DoublyLinkedList<T> &list1,
                           DoublyLinkedList<T> &list2) {

  DoublyLinkedList<T> returnList;

  list1.reset();
  list2.reset();

  while (!list1.empty() or !list2.empty()) {

    if (!list1.empty()) {

      returnList.insert(list1.getItem(), ascending);
      auto pDelete = list1.getCurrent();
      list1.erase();
      delete pDelete;
    }
    if (!list2.empty()) {

      returnList.insert(list2.getItem(), ascending);
      auto pDelete = list2.getCurrent();
      list2.erase();
      delete pDelete;
    }
  }

  return returnList;
}

template <class T> inline void DoublyLinkedList<T>::mergeSort() {

  /* put each element in the list into a separate list
   * we need a list that holds all lists
   * merge 2 lists into one list, using merge(list1, list2)
   * repeat until finished */

  list<DoublyLinkedList<T>> listOfLists;

  /* put each element from this list into a separate list and remove
   * that element from this list
   * store the created lists in 'listOfLists' */
  reset();
  while (!empty()) {

    DoublyLinkedList<T> list;
    list.insert(getItem(), append);
    auto pDelete = getCurrent();
    erase();
    delete pDelete;
    listOfLists.push_back(list);
  }

  /* now take 2 lists from the front, merge them into a new one and append
   * that list at the end. Do this until listOfLists contains exactly 1
   * list */

  /*  */
  while (listOfLists.size() > 1) {

    DoublyLinkedList<T> first = listOfLists.front();
    listOfLists.pop_front();
    DoublyLinkedList<T> second = listOfLists.front();
    listOfLists.pop_front();

    DoublyLinkedList<T> result = merge(first, second);

    listOfLists.push_back(result);
  }

  /* and now add the items in the sorted list in listOfList to this */

  listOfLists.front().reset();
  while (!listOfLists.front().empty()) {

    insert(listOfLists.front().getItem(), append);
    auto pDelete = listOfLists.front().getCurrent();
    listOfLists.front().erase();
    delete pDelete;
  }

  isSkiplist = true;
}

template <class T> inline int DoublyLinkedList<T>::getSize() {

  return item_counter;
}

template <class T> inline Node<T> *DoublyLinkedList<T>::getBegin() {
  return begin;
}

template <class T> inline Node<T> *DoublyLinkedList<T>::getEnd() { return end; }

template <class T>
inline void DoublyLinkedList<T>::swap(int lValue, int rValue) {

  int counter = 0;
  int target;
  Node<T> *rItem;
  Node<T> *lItem;

  if (rValue > lValue)
    target = rValue;
  else
    target = lValue;

  reset();
  while (counter < target + 1) {

    if (counter == rValue)
      rItem = getCurrent();
    if (counter == lValue)
      lItem = getCurrent();

    advance();
    counter++;
  }

  T buffer = rItem->data;
  rItem->data = lItem->data;
  lItem->data = buffer;
}

template <class T>
inline void DoublyLinkedList<T>::swap(Node<T> *lValue, Node<T> *rValue) {

  //    T buffer = lValue->data;
  //    lValue->data = rValue->data;
  //    rValue->data = buffer;

  Node<T> *lValuePreviousNode = lValue->previous;
  Node<T> *lValueNextNode = lValue->next;
  Node<T> *rValuePreviousNode = rValue->previous;
  Node<T> *rValueNextNode = rValue->next;

  /* edge-case : lValue == begin */

  if (lValuePreviousNode)
    lValuePreviousNode->next = rValue;
  rValue->previous = lValuePreviousNode;

  if (lValueNextNode)
    lValueNextNode->previous = rValue;
  rValue->next = lValueNextNode;

  if (rValuePreviousNode)
    rValuePreviousNode->next = lValue;
  lValue->previous = rValuePreviousNode;

  if (rValueNextNode)
    rValueNextNode->previous = lValue;
  lValue->next = rValueNextNode;

  if (begin == lValue)
    begin = rValue;

  else if (begin == rValue)
    begin = lValue;

  if (end == lValue)
    end = rValue;

  else if (end == rValue)
    end = lValue;
}

template <class T> inline T DoublyLinkedList<T>::getItem() {

  return current->data;
}

template <class T> inline Node<T> *DoublyLinkedList<T>::getCurrent() {

  return current;
}

template <class T> inline bool DoublyLinkedList<T>::empty() {

  if (begin == nullptr)
    return true;
  else
    return false;
}

template <class T> inline bool DoublyLinkedList<T>::isEnd() {

  if (current == end)
    return true;
  else
    return false;
}

template <class T> inline bool DoublyLinkedList<T>::isBegin() {

  if (current == begin)
    return true;
  else
    return false;
}

template <class T> inline void DoublyLinkedList<T>::advance() {

  current = current->next;
}

template <class T> inline void DoublyLinkedList<T>::retreat() {

  if (current != begin)
    current = current->previous;
}

template <class T> inline void DoublyLinkedList<T>::reset() { current = begin; }

template <class T> inline void DoublyLinkedList<T>::setToEnd() {
  current = end;
}

template <class T>
inline void DoublyLinkedList<T>::insert(const T &value, insertType type) {

  Node<T> *pHelp = new Node<T>;
  pHelp->data = value;
  item_counter++;

  /* setting up the skiplists */
  if (item_counter % 10 == 0 and isSkiplist) {

    unsigned int number = item_counter;
    int digits = 0;
    while (number > 0) {
      number /= 10;
      digits++;
    }

    while (skipLists.size() < (unsigned int)digits - 1) {
      SkipList<Node<T>> skip;
      skipLists.push_back(skip);
    }

    number = item_counter;
    auto listIter = skipLists.begin();

    // 120
    while (number > 1) {
      /* check if the list already contains as many numbers as the corresponding
       * digit would suggest -> 123 means for digit '2' we would need 2 elements
       * in the list with step size of 10
       * and 1 element in the list with step size of 100. */
      number /= 10;
      if (listIter->size() < number) {

        listIter->append(pHelp);
        /* so now for all lists with step size > 10 we need to set the left
         * and right pointers of the SkipNode which then need to point
         * to the corresponding SkipNode in the other skip lists
         * so the skipnode in a list with step size 10 has its left pointer
         * pointing at the corresponding element in the list with step size 100
         * (if it exists) and the list with step size 100 has its right pointer
         * to the list with step size 10 and the left pointer to the list
         * with step size 1000 (if it exists)
         * so we need to check for these conditions and then set these pointers.
         * And the nodes should be the last element in each list ! */
      }

      listIter++;
    }

    /* now we simply have to connect the last nodes in each Skiplist
     * not that simple
     * we have to connect the last nodes of each list that
     * whose last node item count is equal
     * lets say we have 1230 items
     * list10 has 123 items
     * lsit 100 has 12 items
     * in that case the links would have been set
     *
     * lets say at this point we have 1200 items
     * we would have to link the last item of
     * list10 (=120 items) and list 100(=12 items)
     *
     * for_each list whose item_count * (10^n) == total_items
     *
     * total_items = 1200
     * list10 -> item_count = 120, n=1 -> 1200
     * list100 -> item_count = 12, n=2 -> 1200
     * list1000-> item_count = 1, n=3 -> 1000
     * n = index in skiplist + 1 ( because the first item has index 0 )
     */

    vector<unsigned int> listsToConnect;
    for (unsigned int index = 0; index < skipLists.size(); index++) {

      unsigned int list_value = skipLists[index].size() * pow(10, index + 1);

      if (list_value == item_counter) {
        listsToConnect.push_back(index);
      }
    }

    if (listsToConnect.size() > 1) {

      /* list[0] only to bigger
       * list[size()-1] only to smaller
       * list[index if not the above] to bigger and smaller */
      for (auto index : listsToConnect) {

        if (index == 0)
          skipLists[index]._end->toBigger = skipLists[index + 1]._end;

        else if (index > 0 and index != listsToConnect.size() - 1) {
          skipLists[index]._end->toBigger = skipLists[index + 1]._end;
          skipLists[index]._end->toSmaller = skipLists[index - 1]._end;

        } else if (index == listsToConnect.size() - 1)
          skipLists[index]._end->toSmaller = skipLists[index - 1]._end;
      }
    }
  }

  if (type == prepend) {

    if (begin == nullptr) {

      pHelp->previous = nullptr;
      pHelp->next = nullptr;
      begin = pHelp;
      current = pHelp;
      end = pHelp;

    } else if ((current == begin) and begin != nullptr) {

      pHelp->previous = nullptr;
      pHelp->next = begin;

      begin->previous = pHelp;
      begin = pHelp;
      current = begin;

    } else {

      pHelp->previous = current->previous;
      pHelp->next = current;

      current->previous->next = pHelp;
      current->previous = pHelp;
    }
  }

  else if (type == append) {

    if (end == nullptr) {

      pHelp->previous = nullptr;
      pHelp->next = nullptr;
      begin = pHelp;
      current = pHelp;
      end = pHelp;

    } else if ((current == end) and end != nullptr) {

      pHelp->previous = current;
      pHelp->next = nullptr;

      end->next = pHelp;
      end = pHelp;
      current = end;

    } else {

      pHelp->previous = current;
      pHelp->next = current->next;

      current->next->previous = pHelp;
      current->next = pHelp;
    }
  }

  else if (type == ascending) {

    if (begin == nullptr) {

      pHelp->previous = nullptr;
      pHelp->next = nullptr;
      begin = pHelp;
      current = pHelp;
      end = pHelp;

    } else {

      Node<T> *index = begin;

      while (*index < *pHelp and index != end) {

        index = index->next;
      }

      /* at this point
       *  index could be equal
       *  index could be end
       *  index could not be end
       * index could be smaller, we just reached the end
       * index could be bigger and we are at begin
       * index could be bigger and we are not at begin */

      if (!(*index < *pHelp) and !(*index > *pHelp) and index != end) {

        current = index;

        current->next->previous = pHelp;
        pHelp->next = current->next;
        pHelp->previous = current;
        current->next = pHelp;

      } else if (!(*index < *pHelp) and !(*index > *pHelp) and index == end) {

        current = index;

        pHelp->next = nullptr;
        pHelp->previous = current;
        current->next = pHelp;
        end = pHelp;

      } else if (*index < *pHelp and index == end) {

        /* if index is smaller and we are at the end
         * we have to append to index and make pHelp the new end*/

        current = index;

        current->next = pHelp;
        pHelp->next = nullptr;
        pHelp->previous = current;

        end = pHelp;
        end->next = nullptr;
        end->previous = current;

      } else if (*index > *pHelp and index == begin) {

        /* if index is bigger and we are still at the begin
         * of the list we need to prepend to index and make
         * pHelp the new begin */

        current = index;

        current->previous = pHelp;
        pHelp->next = current;
        pHelp->previous = nullptr;

        begin = pHelp;
        begin->next = current;
        begin->previous = nullptr;

      } else if (*index > *pHelp and index != begin) {

        /* at this point we know index is bigger and we are
         * not at the begin so we prepend to index */

        current = index;

        current->previous->next = pHelp;
        pHelp->previous = current->previous;
        pHelp->next = current;
        current->previous = pHelp;
      }
    }
  }

  else if (type == descending) {

    if (begin == nullptr) {

      pHelp->previous = nullptr;
      pHelp->next = nullptr;
      begin = pHelp;
      current = pHelp;
      end = pHelp;

    } else {

      Node<T> *index = begin;

      while (*index > *pHelp and index != end) {

        index = index->next;
      }

      /* at this point
       * index could be equal
       *  index could be begin
       *  index could not be begin
       * index could be bigger, we just reached the end
       * index could be smaller and we are at begin
       * index could be smaller and we are not at begin */

      if (!(*index < *pHelp) and !(*index > *pHelp) and index != begin) {

        current = index;

        current->previous->next = pHelp;
        pHelp->previous = current->previous;
        pHelp->next = current;
        current->previous = pHelp;

      } else if (!(*index < *pHelp) and !(*index > *pHelp) and index == begin) {

        current = index;

        current->previous = pHelp;
        pHelp->previous = nullptr;
        pHelp->next = current;

        begin = pHelp;

      } else if (*index > *pHelp and index == end) {

        /* if index is bigger and we are still at the end
         * we have to append to index and make pHelp the new end*/

        current = index;

        current->next = pHelp;
        pHelp->next = nullptr;
        pHelp->previous = current;

        end = pHelp;
        end->next = nullptr;
        end->previous = current;

      } else if (*index < *pHelp and index == begin) {

        /* if index is smaller and we are still at the begin
         * of the list we need to prepend to index and make
         * pHelp the new begin */

        current = index;

        current->previous = pHelp;
        pHelp->next = current;
        pHelp->previous = nullptr;

        begin = pHelp;
        begin->next = current;
        begin->previous = nullptr;

      } else if (*index < *pHelp and index != begin) {

        /* at this point we know index is smaller and we are
         * not at the begin so we prepend to index */

        current = index;

        current->previous->next = pHelp;
        pHelp->previous = current->previous;
        pHelp->next = current;
        current->previous = pHelp;
      }
    }
  }
}

template <class T> inline void DoublyLinkedList<T>::erase() {

  if (empty()) {

    throw std::runtime_error("Nothing to delete!\n");

  } else {

    --item_counter;

    if (begin == end) {
      // deleting begin==end

      begin = nullptr;
      end = begin;
      current = begin;

    } else if (current == begin) {
      // deleting begin and begin != end

      begin = begin->next;
      begin->previous = nullptr;
      current = begin;

    } else if (current == end) {
      // deleting end and end != begin

      end = end->previous;
      end->next = nullptr;
      current = end;

    } else {
      // deleting anything in between

      Node<T> *pHelp = current->next;

      current->previous->next = current->next;
      current->next->previous = current->previous;
      current = pHelp;
    }
  }
}

template <class T> inline void DoublyLinkedList<T>::printList() {
  auto pHelp = begin;
  while (pHelp != nullptr) {

    std::cout << "index-> " << pHelp->index << " : " << pHelp->data << "\n";
    pHelp = pHelp->next;
  }
}

template <class T> inline void DoublyLinkedList<T>::printCurrent() {

  std::cout << current->data << "\n";
}

#endif // DOUBLYLINKEDLIST_H
