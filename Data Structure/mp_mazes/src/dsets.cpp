#include "dsets.h"

// Creates n unconnected root nodes at the end of the vector.
void DisjointSets::addelements(int num) {
    for (int i = 0; i < num; i++) {
        elements_.push_back(-1);
    }
}
// This function should compress paths and works as described in lecture.
int DisjointSets::find(int elem) {
    if (elements_[elem] >= 0) {
        return find(elements_[elem]);
    }
    return elem;
}

void DisjointSets::setunion(int a, int b) {
    int x = find(a);
    int y = find(b);
    int newSize = elements_[x] + elements_[y];

    if (x == y) {
        return;
    }

    if (elements_[x] > elements_[y]) {
        elements_[x] = y;
        elements_[y] = newSize;
    } else {
        elements_[y] = x;
        elements_[x] = newSize;
    }
}

int DisjointSets::size(int elem) {
    return -1 * elements_[find(elem)];
}