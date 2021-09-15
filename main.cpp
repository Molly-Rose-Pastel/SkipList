#include <QCoreApplication>
#include <QDebug>
#include <QElapsedTimer>
#include <algorithm>

#include "DoublyLinkedList.h"

using namespace std;

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  /**************************************************************************/
  /* Speed Comparison                                                       */
  /**************************************************************************/

  unsigned int maxNumbers = 10000;
  QElapsedTimer timer;
  uint64_t elapsed;
  DoublyLinkedList<int> MergeSortSpeedList;
  DoublyLinkedList<int> HeapSortSpeedList;
  DoublyLinkedList<int> QuickSortSpeedList;

  /* setting up vector with unique random numbers */
  vector<int> randomSpeedNumbers;
  for (unsigned int i = 0; i < maxNumbers; i++) {
    randomSpeedNumbers.push_back(i);
  }
  auto timestamp = chrono::duration_cast<chrono::nanoseconds>(
      chrono::system_clock::now().time_since_epoch());
  auto seed = timestamp.count();
  srand(seed);
  random_shuffle(randomSpeedNumbers.begin(), randomSpeedNumbers.end());

  /* feeding random numbers into lists */

  for (unsigned int i = 0; i < randomSpeedNumbers.size(); i++) {
    MergeSortSpeedList.insert(randomSpeedNumbers[i], append);
    HeapSortSpeedList.insert(randomSpeedNumbers[i], append);
    QuickSortSpeedList.insert(randomSpeedNumbers[i], append);
  }

  cout << "\n\nALGORITHM SPEED COMPARISON STARTED ...\n\n";

  /**************************************************************************/
  /* gauging mergeSort */
  /**************************************************************************/
  timer.start();
  MergeSortSpeedList.mergeSort();
  elapsed = timer.elapsed();
  cout << "\nMergeSort took: " << elapsed / 1000.0 << " s.\n";

  /**************************************************************************/
  /* gauging heapSort without skiplist*/
  /**************************************************************************/
  HeapSortSpeedList.isSkiplist = false;
  timer.start();
  HeapSortSpeedList.heapSort();
  elapsed = timer.elapsed();
  cout << "\nHeapSort without skiplist took: " << elapsed / 1000.0 << " s.\n";

  /**************************************************************************/
  /* gauging heapSort with skiplist*/
  /**************************************************************************/

  /* clearing and reinitializing HeapSortSpeedList */
  HeapSortSpeedList.isSkiplist = true;
  while (!HeapSortSpeedList.empty()) {
    HeapSortSpeedList.reset();
    HeapSortSpeedList.erase();
  }
  for (unsigned int i = 0; i < randomSpeedNumbers.size(); i++) {
    HeapSortSpeedList.insert(randomSpeedNumbers[i], append);
  }

  timer.start();
  HeapSortSpeedList.heapSort();
  elapsed = timer.elapsed();
  cout << "\nHeapSort with skiplist took: " << elapsed / 1000.0 << " s.\n";

  /**************************************************************************/
  /* gauging quickSort without skiplist */
  /**************************************************************************/
  QuickSortSpeedList.isSkiplist = false;
  timer.start();
  QuickSortSpeedList.quickSort();
  elapsed = timer.elapsed();
  cout << "\nQuickSort without skiplist took: " << elapsed / 1000.0 << " s.\n";

  /**************************************************************************/
  /* gauging quickSort with skiplist*/
  /**************************************************************************/

  /* creating a new QuickSortSpeedList */
  DoublyLinkedList<int> QuickSortSpeedList_2;
  QuickSortSpeedList_2.isSkiplist = true;
  for (unsigned int i = 0; i < randomSpeedNumbers.size(); i++) {
    QuickSortSpeedList_2.insert(randomSpeedNumbers[i], append);
  }

  timer.start();
  QuickSortSpeedList_2.quickSort();
  elapsed = timer.elapsed();
  cout << "\nQuickSort with skiplist took: " << elapsed / 1000.0 << " s.\n";

  /**************************************************************************/
  /* gauging stable quickSort without skiplist*/
  /**************************************************************************/

  /* creating a new QuickSortSpeedList */
  DoublyLinkedList<int> QuickSortSpeedList_Stable1;
  for (unsigned int i = 0; i < randomSpeedNumbers.size(); i++) {
    QuickSortSpeedList_Stable1.insert(randomSpeedNumbers[i], append);
  }
  /* adding duplicate numbers */
  QuickSortSpeedList_Stable1.insert(randomSpeedNumbers[20], append);
  QuickSortSpeedList_Stable1.insert(randomSpeedNumbers[25], append);

  timer.start();
  QuickSortSpeedList_Stable1.stableQuickSort();
  elapsed = timer.elapsed();
  cout << "\nstable QuickSort without skiplist took: " << elapsed / 1000.0
       << " s.\n";

  //  QuickSortSpeedList_Stable1.printList();
  return a.exec();
}
