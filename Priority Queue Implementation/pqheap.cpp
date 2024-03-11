#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;    // program-wide constant
const int NONE = -1;                // used as sentinel index

/*
 * The constructor of the PQHeap initializes the capacity, sets
 * the size and allocates memory for the internal array of the PQ.
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated]();
    _numFilled = 0;
}

/*
 * The destructor of the PQHeap deletes the internal array of the PQ.
 */
PQHeap::~PQHeap() {
    delete[] _elements;
}

/*
 * The enqueue function handles the functionality for inserting
 * an element into the priority queue, with the help of helper
 * functions. The function checks if resizing the array is necessary,
 * otherwise inserting the element into the queue, incrementing the size,
 * and calling a function to handle the reordering of the heap.
 */
void PQHeap::enqueue(DataPoint elem) {
    // Resizing array if needed
    if (size() == _numAllocated) {
        // Helper function to resize array
        resizeArray();
    }

    // Inserting element
    _elements[size()] = elem;
    _numFilled++;

    // Helper function to reorder tree
    percolateUp(elem);
}

/*
 * The function returns the element at the front of the PQ, at
 * the root, calling an error if the PQ is empty.
 */
DataPoint PQHeap::peek() const {
   if (isEmpty()) {
       error("Cannot access front element of empty pqueue!");
   }
   return _elements[0];
}

/*
 * The function handles the functionality for dequeueing the
 * smallest element (the element at the front) of the PQ,
 * with the help of other functions.
 * The function retrieves the front element from the peek function,
 * thus also checking for an empty PQ. It then calls a helper function
 * to handle the reordering of the PQ following the removal of the root,
 * decrementing the size, and returning the element at the root.
 */
DataPoint PQHeap::dequeue() {
   DataPoint front = peek();

   // Calling helper function to reorder tree
   percolateDown();

   _numFilled--;

   return front;
}

/*
 * The function returns true if the size of the PQ is 0, false if not.
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/*
 * The function returns the variable tracking the number of
 * filled indices in the PQHeap, indicating size.
 */
int PQHeap::size() const {
    /* TODO: Implement this function. */
    return _numFilled;
}

/*
 * The function sets the number of filled indices in the PQHeap equal to zero.
 */
void PQHeap::clear() {
    _numFilled = 0;
}

/*
 * The function acts as a helper function for the enqueue
 * function, responsible for the operations pertaining
 * to growing the array when full.
 */
void PQHeap::resizeArray() {
    // Creating new array of double size, copying contents over
    DataPoint *newArray = new DataPoint[_numAllocated * 2 + 1];

    for (int i = 0; i < _numFilled; i++) {
       newArray[i] = _elements[i];
    }

    // Deleting old array
    delete[] _elements;

    // Reassigning variables
    _elements = newArray;
    _numAllocated = (_numAllocated * 2) + 1;
}

/*
 * The function handles the functionality for reordering the binary
 * heap as part of an enqueue operation. With the new
 * element having been placed at the bottom index, the function
 * swaps positions of the new element and its parent for as long
 * as the new element has a lower priority than its parent and is
 * not at the root.
 */
void PQHeap::percolateUp(DataPoint elem) {
    // Figure out incrementation of _numElements as it pertains to here
    int index = size() - 1;
    int parent = getParentIndex(index);

    // Percolate until found position or at root
    while ((elem.priority < _elements[parent].priority) && index > 0) {
       swapElements(index, parent);
       index = parent;
       parent = getParentIndex(index);
    }
}

/*
 * The function handles the functionality for reordering the binary
 * heap as part of a dequeue operation. With the previous bottom element
 * having been placed at the root of the heap, the function swaps
 * positions of the element and its smallest child, until the priority
 * of the element is no longer greater than that of it's smallest child,
 * or is at the bottom of the tree.
 */
void PQHeap::percolateDown() {
    int newRoot = 0;
    _elements[newRoot] = _elements[size() - 1];

    int smallest = smallestChild(newRoot);

    // Percolate until found position or at bottom
    while (smallest != NONE &&
           _elements[newRoot].priority > _elements[smallest].priority) {
       swapElements(newRoot, smallest);
       newRoot = smallest;
       smallest = smallestChild(newRoot);
    }
}

/*
 * The function calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, the function returns
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    int parent = (child - 1) / 2;

    if (validateIndex(parent)) {
       return parent;
    }

    return NONE;
}

/*
 * The function calculates the index of the element that is the
 * left child of the specified parent index. If this parent has no
 * left child, the function returns the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    int leftChild = (2 * parent) + 1;

    if (validateIndex(leftChild)) {
       return leftChild;
    }

    return NONE;
}

/*
 * The function calculates the index of the element that is the
 * right child of the specified parent index. If this parent has no
 * right child, the function returns the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    int rightChild = (2 * parent) + 2;

    if (validateIndex(rightChild)) {
       return rightChild;
    }

    return NONE;
}

/*
 * The function returns the index of the child with the smallest priority
 * of the parent passed to the function. If the parent has no children,
 * the function returns the sentinel value NONE.
 */
int PQHeap::smallestChild(int parent) const {
    int leftChild = getLeftChildIndex(parent);
    int rightChild = getRightChildIndex(parent);

    // Returning NONE if no children
    if (leftChild == NONE && rightChild == NONE) {
       return NONE;
    }

    // Finding and returning the smallest (or only existing child)
    if (_elements[leftChild].priority < _elements[rightChild].priority ||
        rightChild == NONE) {
       return leftChild;
    } else {
       return rightChild;
    }
}

/*
 * The function is a helper that confirms that index
 * is in within range of the filled portion of the element array.
 * The function returns true if the index is valid, false if not.
 */
bool PQHeap::validateIndex(int index) const {
    if (index < 0 || index >= _numFilled) {
       return false;
    } return true;
}

/*
 * The function carries out the functionality of swapping the positions
 * of two elements in the PQ, used in both the percolate functions.
 * Upon checking the validity of both indices, the swap is performed.
 */
void PQHeap::swapElements(int indexA, int indexB) {
    if (validateIndex(indexA) && validateIndex(indexB)) {
       DataPoint tmp = _elements[indexA];
       _elements[indexA] = _elements[indexB];
       _elements[indexB] = tmp;
    }
}


/*
 * The function verifies the validity of the internal array of the PQ,
 * ensuring that the properties of the binary heap are fulfilled.
 */
void PQHeap::debugConfirmInternalArray() const {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    /* Loop over internal array and compare priority parent/smallest child.
     * If element at index i has larger priority than its smallest child,
     * these two elements are out of order expected for PQHeap.
     */
    for (int i = 1; i < size(); i++) {
       int smallest = smallestChild(i);

       if (smallest == NONE) {
           continue;
       }

       if (_elements[i].priority > _elements[smallest].priority) {
           error("PQArray elements out of order: indexes " + integerToString(smallest) + " and " + integerToString(i));
       }
    }
}

/*
 * Return a Vector copy of the elements from the internal array
 * for use as a debugging/testing aid.
 */
Vector<DataPoint> PQHeap::debugGetInternalArrayContents() const {
    Vector<DataPoint> v;

    for (int i = 0; i < size(); i++) {
       v.add(_elements[i]);
    }
    return v;
}

/*
 * Discard the existing internal array and replace with a newly
 * allocated array whose contents are filled with elements copied
 * from the vector. The validity of the new contents are confirmed
 * by a call to debugConfirmInternalArray.
 * This debug function is used to directly configure the internal
 * array contents to a particular state to set the stage for a
 * specific scenario to be tested.
 */
void PQHeap::debugSetInternalArrayContents(const Vector<DataPoint>& v, int capacity) {
    if (v.size() > capacity || capacity == 0) {
       error("Invalid capacity for debugSetInternalArrayContents!");
    }
    delete[] _elements;                     // discard old memory
    _elements = new DataPoint[capacity];    // allocate new memory
    _numAllocated = capacity;
    _numFilled = v.size();
    for (int i = 0; i < v.size(); i++) {    // fill contents with copy from vector
       _elements[i] = v[i];
    }
    debugConfirmInternalArray();            // confirm contents valid
}


/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("Testing auxiliary functions in PQHeap class") {
    PQHeap pq;
    Vector<DataPoint> input = {{"R", 4}, {"A", 5}, {"B", 3}};
    for (DataPoint dp : input) {
       pq.enqueue(dp);
    }

    // Size and peek, isEmpty on populated pq
    EXPECT_EQUAL(pq.size(), 3);
    EXPECT_EQUAL(pq.peek(), {"B", 3});
    EXPECT_EQUAL(pq.isEmpty(), false);

    // Size and peek, isEmpty on unpopulated pq
    pq.clear();

    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_ERROR(pq.peek());
    EXPECT_EQUAL(pq.isEmpty(), true);
}

STUDENT_TEST("PQHeap short version of example in writeup") {
    PQHeap pq;
    Vector<DataPoint> input = {{"R", 4}, {"A", 5}, {"B", 3}};
    for (DataPoint dp : input) {
       pq.enqueue(dp);
    }

    EXPECT_EQUAL(pq.dequeue(), {"B", 3});
    EXPECT_EQUAL(pq.dequeue(), {"R", 4});
    EXPECT_EQUAL(pq.dequeue(), {"A", 5});
}

STUDENT_TEST("PQHeap extended version of example in writeup, need to expand array") {
    PQHeap pq;
    Vector<DataPoint> input = {
                               {"R", 4}, {"A", 5}, {"B", 3}, {"K", 7}, {"G", 2},
                               {"V", 9}, {"T", 1}, {"O", 8}, {"S", 6}, {"X", 10},
                               {"Z", 0}, {"W", 14} };

    pq.debugConfirmInternalArray();
    for (DataPoint dp : input) {
       pq.enqueue(dp);
       pq.debugConfirmInternalArray();
    }

    EXPECT_EQUAL(pq.dequeue(), {"Z", 0});
    EXPECT_EQUAL(pq.dequeue(), {"T", 1});
    EXPECT_EQUAL(pq.dequeue(), {"G", 2});
}

STUDENT_TEST("PQHeap: sequence of mixed operations") {
    PQHeap pq;
    int size = 50;

    for (int i = 0; i < size; i++) {
       double priority = randomReal(-10, 10);
       pq.enqueue({"random", priority});
    }
    for (int i = 0; i < pq.size()/3; i++) {
       DataPoint dp = pq.dequeue();
       pq.enqueue({"inverse", -dp.priority});
    }
    EXPECT_EQUAL(pq.size(), size);
    while (pq.size() > 1) {
       double cur = pq.dequeue().priority;
       EXPECT(cur <= pq.peek().priority);
    }
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example in writeup, confirm contents of internal array") {
    PQHeap pq;
    Vector<DataPoint> input = {
        {"R", 4}, {"A", 5}, {"B", 3}, {"K", 7}, {"G", 2},
        {"V", 9}, {"T", 1}, {"O", 8}, {"S", 6} };

    pq.debugConfirmInternalArray();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.debugConfirmInternalArray();
    }
    // What is contents of pq internal array right now?
    // How can your test case verify contents are as expected?
    pq.dequeue();
    pq.dequeue();
    // What is contents of internal array after two dequeue operations?
}
