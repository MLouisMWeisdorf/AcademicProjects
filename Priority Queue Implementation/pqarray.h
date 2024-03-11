#pragma once
#include "MemoryUtils.h"
#include "datapoint.h"
#include "vector.h"

/**
 * Priority queue of DataPoints implemented using a sorted array.
 */
class PQArray {
public:
    /**
     * Creates a new, empty priority queue.
     */
    PQArray();

    /**
     * Cleans up all memory allocated by this priority queue.
     */
    ~PQArray();

    /**
     * Adds a new element into the queue. This operation runs in time O(N),
     * where n is the number of elements in the queue.
     *
     * @param element The element to add.
     */
    void enqueue(DataPoint element);

    /**
     * Removes and returns the element that is frontmost in this priority queue.
     * The frontmost element is the one with the most urgent priority. A priority
     * of 1 is more urgent than priority 2 which is more urgent than priority 7
     * and so on. If the priority queue contains two or more elements of equal
     * priority, the order those elements are dequeued is arbitrary, i.e. there
     * is no required tie-break handling.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation runs in time O(1).
     *
     * @return The frontmost element, which is removed from queue.
     */
    DataPoint dequeue();

    /**
     * Returns, but does not remove, the element that is frontmost.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation runs in time O(1).
     *
     * @return frontmost element
     */
    DataPoint peek() const;

    /**
     * Returns whether this priority queue is empty.
     *
     * This operation runs in time O(1).
     *
     * @return true if contains no elements, false otherwise.
     */
    bool isEmpty() const;

    /**
     * Returns the count of elements in this priority queue.
     *
     * This operation runs in time O(1).
     *
     * @return The count of elements in the priority queue.
     */
    int size() const;

    /**
     * Removes all elements from the priority queue.
     *
     * This operation runs in time O(1).
     */
    void clear();


    /*
     * These three "debug" functions are intended solely for testing.
     * They provide controlled access to the queue's private internal array.
     * debugConfirmInternalArray confirms that the internal array is
     * valid (elements in expected order).
     * debugGetInternalArrayContents returns a Vector copy of the internal array.
     * debugSetInternalArrayContents replaces the contents of the internal
     * array with elements copied from a Vector.
     * These functions can be used to modify/retrieve the contents of the private
     * internal array as part of a hand-constructed test case.
     * Such debug functions would typically be used early in development
     * and could be removed (or made private) once past the need for them.
     */

    /*
     * Confirms validity of internal array. Raises an error if a problem is found.
     */
    void debugConfirmInternalArray() const;

    /*
     * Return a Vector copy of the elements from the internal array.
     */
    Vector<DataPoint> debugGetInternalArrayContents() const;

    /*
     * Allocate the internal array to requested capacity and copy the elements
     * from vector v into the internal array. The new contents of the internal
     * array are confirmed by a call to debugConfirmInternalArray.
     */
    void debugSetInternalArrayContents(const Vector<DataPoint>& v, int capacity);

private:
    DataPoint* _elements;   // dynamic array
    int _numAllocated;      // number of slots allocated in array
    int _numFilled;         // number of slots filled in array

    void validateIndex(int index) const;
    void swapElements(int indexA, int indexB);

    // Helper function for placing element during enqueue operation
    void placeElement(DataPoint elem);

    // Helper function for growing the array during enqueue operation
    void resizeArray();

    /* Weird C++isms: C++ loves to make copies of things, which is usually a good thing but
     * for the purposes of this assignment requires some C++ knowledge we haven't yet covered.
     * This next line disables all copy functions to make sure you don't accidentally end up
     * debugging something that isn't your fault.
     *
     * Curious what this does? Take CS106L!
     */
    DISALLOW_COPYING_OF(PQArray);
};
