#include "pqclient.h"
#include "pqarray.h"
#include "pqheap.h"
#include "vector.h"
#include "strlib.h"
#include "stack.h"
#include <sstream>
#include "SimpleTest.h"
using namespace std;

/*
 * Using the functionality of the priority queue, this function
 * sorts the input vector, v, in increasing order, in place.
 */
void pqSort(Vector<DataPoint>& v) {
//    PQArray pq;
    PQHeap pq;

    /* Using the Priority Queue data structure as a tool to sort, neat! */

    /* Add all the elements to the priority queue. */
    for (int i = 0; i < v.size(); i++) {
        pq.enqueue(v[i]);
    }

    /* Extract all the elements from the priority queue. Due
     * to the priority queue property, we know that we will get
     * these elements in sorted order, in order of increasing priority
     * value. Store elements back into vector, now in sorted order.
     */
    for (int i = 0; !pq.isEmpty(); i++) {
        v[i] = pq.dequeue();
    }
}

/*
 * The function reads DataPoint structs from a stream and saves
 * the k largest values from the stream in a vector of DataPoints,
 * which is returned.
 * The function utilizes functionality from the priority queue
 * to handle sorting.
 */
Vector<DataPoint> topK(istream& stream, int k) {
    //    PQArray pq;
    PQHeap pq;

    DataPoint cur;
    while (stream >> cur) {
        // Adding the first k items regardless of size
        if (pq.size() < k) {
            pq.enqueue(cur);
            continue;
        }

        // If cur is bigger than smallest item in top K, replace
        if (cur.priority > pq.peek().priority) {
            pq.dequeue();
            pq.enqueue(cur);
        }
    }

    // Handling empty stream
    if (pq.isEmpty()) {
        return {};
    }

    // The size of the priority queue will be, at most, k as passed in
    k = pq.size();
    Vector<DataPoint> result(k);

    // Transferring from pq to result vector
    while (!pq.isEmpty()) {
        result[k - 1] = pq.dequeue();
        k--;
    }

    return result;
}



/* * * * * * Test Cases Below This Point * * * * * */

/* Helper function that, given a list of data points, produces a stream from them. */
stringstream asStream(const Vector<DataPoint>& dataPoints) {
    stringstream result;
    for (const DataPoint& pt: dataPoints) {
        result << pt;
    }
    return result;
}

/* Helper function that, given range start and stop, produces a stream of values. */
stringstream asStream(int start, int stop) {
    stringstream result;
    for (int i = start; i <= stop; i++) {
        DataPoint pt = {"", double(i) };
        result << pt;
    }
    return result;
}

/* Helper function to fill vector with n random DataPoints. */
void fillVector(Vector<DataPoint>& vec, int n) {
    vec.clear();
    for (int i = 0; i < n; i++) {
        DataPoint pt = {"", randomReal(0, 100)};
        vec.add(pt);
    }
}

/* * * * * Student Tests Below This Point * * * * */

//STUDENT_TEST("Time trial on pqSort (PQArray)") {
//    int n = 5000;
//    int iterations = 4;

//    for (int i = 1; i <= iterations; i++) {
//        Vector<DataPoint> v;
//        fillVector(v, n);
//        TIME_OPERATION(n, pqSort(v));

//        n *= 2;
//    }
//}

STUDENT_TEST("Time trial on pqSort (PQHeap)") {
    int n = 1000000;
    int iterations = 4;

    for (int i = 1; i <= iterations; i++) {
        Vector<DataPoint> v;
        fillVector(v, n);
        TIME_OPERATION(n, pqSort(v));

        n *= 2;
    }
}


STUDENT_TEST("topK time trial with constant k, varying n") {
    int startSize = 200000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input, result;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(n, result = topK(stream, k));
        EXPECT_EQUAL(result.size(), k);
    }
}

//STUDENT_TEST("topK time trial with constant n, varying k (PQArray)") {
//    int startSize = 2000;
//    int n = 20000;
//    for (int k = startSize; k < 10*startSize; k *= 2) {
//        Vector<DataPoint> input, result;
//        fillVector(input, n);
//        stringstream stream = asStream(input);
//        TIME_OPERATION(k, result = topK(stream, k));
//        EXPECT_EQUAL(result.size(), k);
//    }
//}

STUDENT_TEST("topK time trial with constant n, varying k (PQHeap)") {
    int startSize = 5000;
    int n = 100000;
    for (int k = startSize; k < 10*startSize; k *= 2) {
        Vector<DataPoint> input, result;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(k, result = topK(stream, k));
        EXPECT_EQUAL(result.size(), k);
    }
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("pqSort: vector of random elements") {
    setRandomSeed(137); // why might it be a good idea to set seed here?

    Vector<DataPoint> input;
    Vector<double> expected;
    fillVector(input, 100);
    for (DataPoint dp : input) {
        expected.add(dp.priority);
    }
    expected.sort();

    pqSort(input);
    for (int i = 0; i < input.size(); i++) {
        EXPECT_EQUAL(input[i].priority, expected[i]);
    }
}

PROVIDED_TEST("pqSort: time trial") {
    int startSize = 1000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        fillVector(v, n);
        TIME_OPERATION(n, pqSort(v));
    }
}


/* Constant used for sizing the tests below this point. */
const int kMany = 100000;

PROVIDED_TEST("topK: stream 0 elements, ask for top 1") {
    stringstream emptyStream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(emptyStream, 1), expected);
}

PROVIDED_TEST("topK: stream 1 element, ask for top 1") {
    stringstream stream = asStream({ {"" , 1} });
    Vector<DataPoint> expected = { {"" , 1} };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: small fixed input") {
    Vector<DataPoint> input = { {"A", 1}, {"B", 2}, {"C", 3}, {"D", 4} };

    stringstream stream = asStream(input);
    Vector<DataPoint> expected = { {"D", 4}, {"C", 3} };
    EXPECT_EQUAL(topK(stream, 2), expected);

    stream = asStream(input);
    expected = { {"D", 4}, {"C", 3}, {"B", 2}, {"A", 1} };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 1") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> expected = { {"" , kMany} };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 5") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> expected = {
        {"" , kMany    },
        {"" , kMany - 1},
        {"" , kMany - 2},
        {"" , kMany - 3},
        {"" , kMany - 4},
    };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("topK: stress test, many elements, random values") {
    Vector<double> expected;
    Vector<DataPoint> points;
    fillVector(points, 10000);
    for (DataPoint dp : points) {
        expected.add(dp.priority);
    }
    sort(expected.begin(), expected.end(), greater<double>());

    stringstream stream = asStream(points);
    int k = 10;
    Vector<DataPoint> result = topK(stream, k);
    EXPECT_EQUAL(result.size(), k);
    for (int i = 0; i < k; i++) {
        EXPECT_EQUAL(result[i].priority, expected[i]);
    }
}

PROVIDED_TEST("topK: stress test - many elements, ask for top half") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> result = topK(stream, kMany/2);
    EXPECT_EQUAL(result.size(), kMany/2);
    EXPECT_EQUAL(result[0].priority, kMany);
    EXPECT_EQUAL(result[result.size()-1].priority, kMany - result.size() + 1);
}

PROVIDED_TEST("topK: time trial") {
    int startSize = 200000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input, result;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(n, result = topK(stream, k));
        EXPECT_EQUAL(result.size(), k);
    }
}
