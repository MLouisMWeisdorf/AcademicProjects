#pragma once
#include "MemoryUtils.h"
#include "datapoint.h"
#include "vector.h"

/**
 * Priority queue of DataPoints implemented using a binary heap.
 */
class PQHeap {
public:
    /**
     * Creates a new, empty priority queue.
     */
    PQHeap();

    /**
     * Cleans up all memory allocated by this priority queue.
     */
    ~PQHeap();

    /**
     * Adds a new element into the queue. This operation runs in time O(log n),
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
     * This operation must run in time O(log n).
     *
     * @return The frontmost element, which is removed from queue.
     */
    DataPoint dequeue();

    /**
     * Returns, but does not remove, the element that is frontmost.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation must run in time O(1).
     *
     * @return frontmost element
     */
    DataPoint peek() const;

    /**
     * Returns whether this priority queue is empty.
     *
     * This operation must run in time O(1).
     *
     * @return true if contains no elements, false otherwise.
     */
    bool isEmpty() const;

    /**
     * Returns the count of elements in this priority queue.
     *
     * This operation must run in time O(1).
     *
     * @return The count of elements in the priority queue.
     */
    int size() const;

    /**
     * Removes all elements from the priority queue.
     *
     * This operation must run in time O(1).
     */
    void clear();

    /*
     * These three "debug" functions are intended solely for testing.
     * They provide controlled access to the queue's private internal array.
     * debugConfirmInternalArray confirms that the internal array is
     * valid (elements obey expected heap structure).
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

    /* While not a strict requirement, we strongly recommend implementing the
     * helper functions declared below. They will make your code much cleaner, and
     * will aid in the debugging process.
     */
    int getParentIndex(int child) const;
    int getLeftChildIndex(int parent) const;
    int getRightChildIndex(int parent) const;
    void swapElements(int indexA, int indexB);
    bool validateIndex(int index) const;

    // Helper function for placing element during enqueue operation
    void percolateUp(DataPoint elem);

    // Helper functions for reordering tree during dequeue operation
    void percolateDown();
    int smallestChild(int parent) const;

    // Helper function for growing the array during enqueue operation
    void resizeArray();

    /* Weird C++isms: C++ loves to make copies of things, which is usually a good thing but
     * for the purposes of this assignment requires some C++ knowledge we haven't yet covered.
     * This next line disables all copy functions to make sure you don't accidentally end up
     * debugging something that isn't your fault.
     *
     * Curious what this does? Take CS106L!
     */
    DISALLOW_COPYING_OF(PQHeap);
};
