#include "pqarray.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;    // program-wide constant

/*
 * The constructor initializes all of the member variables needed for
 * an instance of the PQArray class. The allocated capacity
 * is initialized to a starting constant and a dynamic array of that
 * size is allocated. The count of filled slots is initially zero.
 */
PQArray::PQArray() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // new with parens will zero new memory
    _numFilled = 0;
}

/* The destructor is responsible for cleaning up any resources
 * used by this instance of the PQArray class. The array
 * memory that was allocated for the PQArray is deleted here.
 */
PQArray::~PQArray() {
    delete[] _elements;
}

/*
 * The function handles the functionality for enqueueing
 * elements into the array. The function utilizes helper functions
 * to resize the array and place the element.
 */
void PQArray::enqueue(DataPoint elem) {
    // Resizing array if needed
    if (size() == _numAllocated) {
        // Helper function to resize array
        resizeArray();
    }

    // Helper function to place element
    placeElement(elem);
}

/*
 * The function acts as a helper function for the enqueue
 * function, responsible for placing the element in the
 * correct position in the array.
 */
void PQArray::placeElement(DataPoint elem) {
    // Inserting new element at the end of the array
    _elements[size()] = elem;
    _numFilled++;

    if (size() > 1) {
        int index = size() - 2;

        // For as long as needed, swapping elem with contents of PQ
        while (elem.priority > _elements[index].priority && index >= 0) {
            swapElements(index, index + 1);
            index--;
        }
    }
}

/*
 * The function acts as a helper function for the enqueue
 * function, responsible for the operations pertaining
 * to growing the array when full.
 */
void PQArray::resizeArray() {
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
 * The count of enqueued elements is tracked in the
 * member variable _numFilled.
 */
int PQArray::size() const {
    return _numFilled;
}

/*
 * The elements in the internal array are stored in decreasing order
 * of priority value. The element at index 0 is the least urgent
 * (largest priority value) and the element in the last-filled index
 * is the most urgent (minimum priority value). peek() returns the
 * frontmost elements, which is stored at the last-filled index.
 * peek() raises an error if the queue is empty.
 */
DataPoint PQArray::peek() const {
    if (isEmpty()) {
        error("Cannot access front element of empty pqueue!");
    }
    return _elements[size() - 1];
}

/*
 * This function returns the value of the frontmost element and removes
 * it from the queue.  Because the frontmost element was at the
 * last-filled index, decrementing count is sufficient to remove it.
 */
DataPoint PQArray::dequeue() {
    DataPoint front = peek();
    _numFilled--;
    return front;
}

/*
 * Returns true if the queue contains no elements, false otherwise
 */
bool PQArray::isEmpty() const {
    return size() == 0;
}

/*
 * Updates internal state to reflect that the queue is empty, i.e. count
 * of filled slots is reset to zero. The array memory remains allocated
 * at current capacity. The contents of the internal array do not need to
 * be cleared; the contents will be overwritten when additional values
 * are enqueued.
 */
void PQArray::clear() {
    _numFilled = 0;
}

/*
 * This private member function is a helper that exchanges the element
 * at indexA with the element at indexB. In addition to being a handy
 * helper function for swapping elements, it also confirms that both
 * indexes are valid.  If you were to accidentally mishandle an index,
 * you will be glad this defensive protection is here to alert you!
 */
void PQArray::swapElements(int indexA, int indexB) {
    validateIndex(indexA);
    validateIndex(indexB);
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}

/*
 * This private member function is a helper that confirms that index
 * is in within range of the filled portion of the element array,
 * reporting an error if the index is invalid.
 */
void PQArray::validateIndex(int index) const {
    if (index < 0 || index >= _numFilled) error("Invalid index " + integerToString(index));
}

/*
 * Confirm the state of the internal array is valid for this queue.
 * For PQArray, elements in the array must be stored in decreasing
 * order of priority.  If a pair of elements is found to be out
 * of order, report an error.
 */
void PQArray::debugConfirmInternalArray() const {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    /* Loop over internal array and compare priority of neighboring elements.
     * If element at index i has larger priority than at index i-1,
     * these two elements are out of order expected for PQArray.
     */
    for (int i = 1; i < size(); i++) {
        if (_elements[i].priority > _elements[i-1].priority) {
            error("PQArray elements out of order: indexes " + integerToString(i-1) + " and " + integerToString(i));
        }
    }
}

/*
 * Return a Vector copy of the elements from the internal array
 * for use as a debugging/testing aid.
 */
Vector<DataPoint> PQArray::debugGetInternalArrayContents() const {
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
void PQArray::debugSetInternalArrayContents(const Vector<DataPoint>& v, int capacity) {
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

STUDENT_TEST("Testing enqueue on empty priority queue") {
    PQArray pq;

    pq.enqueue( {"first", 1.0 } );
    Vector<DataPoint> afterEnqueue = { {"first", 1.0 } };
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), afterEnqueue);
}

STUDENT_TEST("Testing enqueue of larger element") {
    PQArray pq;

    pq.enqueue( {"first", 1.0 } );
    pq.enqueue( {"larger", 2.0 } );
    Vector<DataPoint> afterEnqueue = { {"larger", 2.0 }, {"first", 1.0 } };
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), afterEnqueue);
}

STUDENT_TEST("Testing enqueue of smaller element") {
    PQArray pq;

    pq.enqueue( {"first", 1.0 } );
    pq.enqueue( {"smaller", 0.9 } );
    Vector<DataPoint> afterEnqueue = { {"first", 1.0 }, {"smaller", 0.9 } };
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), afterEnqueue);
}

STUDENT_TEST("Testing enqueue with doubling capacity, debugger side") {
    PQArray pq;
    int size = 11;

    for (double i = 0.0; i <= size; i++) {
        double priority = i;
        pq.enqueue({"random", priority});
    }
}

STUDENT_TEST("Testing enqueue with doubling capacity, client side") {
    PQArray pq;
    int size = 11;

    for (double i = 0.0; i <= size; i++) {
        double priority = i;
        pq.enqueue({"random", priority});
    }

    Vector<DataPoint> afterEnqueue = { {"random", 11.0 }, {"random", 10.0 }, {"random", 9.0 }, {"random", 8.0 }, {"random", 7.0 },
                                    {"random", 6.0 }, {"random", 5.0 }, {"random", 4.0 }, {"random", 3.0 }, {"random", 2.0 },
                                    {"random", 1.0 }, {"random", 0.0 } };
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), afterEnqueue);
}

void fillQueue(PQArray& pq, int n);
void emptyQueue(PQArray& pq, int n);

STUDENT_TEST("Time trial on enqueue") {
    int n = 5000;
    int iterations = 4;

    for (int i = 1; i <= iterations; i++) {
        PQArray pq;
        TIME_OPERATION(n, fillQueue(pq, n));

        n *= 2;
    }
}

STUDENT_TEST("Time trial on dequeue") {
    int n = 10000;
    int iterations = 4;

    for (int i = 1; i <= iterations; i++) {
        PQArray pq;
        fillQueue(pq, n);
        TIME_OPERATION(n, emptyQueue(pq, n));

        n *= 2;
    }
}

/* * * * * Provided Tests Below This Point * * * * */

/*
 * The first three cases demonstrate how you can use the debug
 * functions on small inputs to directly inspect the contents
 * of the internal array to confirm validity. The debug functions
 * are used to set/get the contents of the internal array. Such a
 * test case must manually construct the array contents to be matched.
 * These kinds of simple fixed tests may be a help when testing
 * simple behaviors early in your development cycle. However, these
 * hand-constructed manual tests are tedious to use for anything
 * other than very simple cases.
 */
PROVIDED_TEST("PQArray: confirm internal array contents") {
    PQArray pq;

    Vector<DataPoint> goodContents = { {"quokka", 3}, {"dingo", 2}, {"emu", 1} };
    EXPECT_NO_ERROR(pq.debugSetInternalArrayContents(goodContents, goodContents.size()));

    Vector<DataPoint> badContents = { {"cat", 5}, {"dog", 1}, {"bird", 4} };
    EXPECT_ERROR(pq.debugSetInternalArrayContents(badContents, badContents.size()));
}

PROVIDED_TEST("PQArray: dequeue, confirm internal array contents") {
    PQArray pq;

    Vector<DataPoint> init = { {"apple", 25.9}, {"pear", 2} };  // 2 entries
    pq.debugSetInternalArrayContents(init, 10);
    EXPECT_EQUAL(pq.dequeue(), {"pear", 2} );                   // dequeue
    Vector<DataPoint> afterDequeue = { {"apple", 25.9} };       // now 1 entry
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), afterDequeue);
    EXPECT_EQUAL(pq.dequeue(), {"apple", 25.9} );               // dequeue
    afterDequeue = {};                                          // now 0 entries
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), afterDequeue);
}

PROVIDED_TEST("PQArray: enqueue, confirm internal array contents") {
    PQArray pq;                                                 // empty

    pq.enqueue( {"red", 1.4 } );                                // enqueue
    Vector<DataPoint> afterEnqueue = { {"red", 1.4 } };         // now 1 entry
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), afterEnqueue);

    pq.enqueue( {"green", 1.5} );                               // enqueue
    afterEnqueue = { {"green", 1.5}, {"red", 1.4 } };           // now 2 entries
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), afterEnqueue);

    pq.enqueue( {"blue", 1.3} );                                // enqueue
    afterEnqueue = { {"green", 1.5}, {"red", 1.4 }, {"blue", 1.3} }; // now 3
    EXPECT_EQUAL(pq.debugGetInternalArrayContents(), afterEnqueue);
}

/*
 * Internal tests like those above are good when in very early development,
 * but quickly get tedious and don't scale to larger inputs. Below are some
 * client-side test cases that work by observing the external behavior
 * of the PQArray. Client-side tests are appropriate for larger tests with more
 * comprehensive coverage.
 */

PROVIDED_TEST("PQArray: example from writeup") {
    PQArray pq;

    pq.enqueue( {"Zoe", -3 } );
    pq.enqueue( {"Elmo", 10 } );
    pq.enqueue( {"Bert", 6 } );
    EXPECT_EQUAL(pq.size(), 3);
    pq.debugConfirmInternalArray();

    pq.enqueue( {"Kermit", 5 } );
    EXPECT_EQUAL(pq.size(), 4);
    pq.debugConfirmInternalArray();

    DataPoint removed = pq.dequeue();
    DataPoint expected = {"Zoe", -3 };
    EXPECT_EQUAL(removed, expected);
    pq.debugConfirmInternalArray();
}

PROVIDED_TEST("PQArray: operations size/isEmpty/clear") {
    PQArray pq;

    EXPECT(pq.isEmpty());
    pq.clear();
    EXPECT_EQUAL(pq.isEmpty(), pq.size() == 0);
    pq.enqueue({"A", 7 });
    EXPECT_EQUAL(pq.size(), 1);
    pq.enqueue({"B", 5 });
    EXPECT_EQUAL(pq.size(), 2);
    pq.enqueue({"C", 5 });
    EXPECT_EQUAL(pq.size(), 3);
    pq.clear();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("PQArray: dequeue or peek on empty pqueue raises error") {
    PQArray pq;
    DataPoint point = {"Programming Abstractions", 106 };

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.dequeue();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.clear();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("PQArray: sequence of mixed operations") {
    setRandomSeed(106); // make test behavior deterministic
    PQArray pq;
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

PROVIDED_TEST("PQArray: stress test, cycle random elements in and out") {
    setRandomSeed(42); // make test behavior deterministic
    PQArray pq;
    DataPoint mostUrgent;
    int nOperations = 1000;
    double sumEnqueued = 0, sumDequeued = 0;

    for (int i = 0; i < nOperations; i++) {
        if (randomChance(0.9) || pq.isEmpty()) {
            DataPoint elem = {"", randomReal(-1, 1)};
            if (elem.priority < mostUrgent.priority) {
                mostUrgent = elem;
            }
            sumEnqueued += elem.priority;
            pq.enqueue(elem);
        } else {
            DataPoint elem = pq.dequeue();
            sumDequeued += elem.priority;
            EXPECT_EQUAL(elem, mostUrgent);
            mostUrgent = pq.peek();
        }
    }
    while (!pq.isEmpty()) {
        sumDequeued += pq.dequeue().priority;
    }
    EXPECT_EQUAL(sumEnqueued, sumDequeued);
}

void fillQueue(PQArray& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({"", randomReal(0, 10)});
    }
}

void emptyQueue(PQArray& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

PROVIDED_TEST("PQArray: time fillQueue and emptyQueue") {
    PQArray pq;
    int n = 20000;

    TIME_OPERATION(n, fillQueue(pq, n));
    EXPECT_EQUAL(pq.size(), n);
    TIME_OPERATION(n, emptyQueue(pq, n));
}
