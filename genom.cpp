#include "genom.h"
#include "myset.h"

using namespace std;

int GenomeSimilarity(const string& first,
                     const string& second) {

    MySet pairs;

    for (int i = 0; i + 1 < (int)second.length(); i++) {
        string pair = second.substr(i, 2);
        SETADD(pairs, pair);
    }

    int count = 0;

    for (int i = 0; i + 1 < (int)first.length(); i++) {
        string pair = first.substr(i, 2);

        if (SET_AT(pairs, pair)) {
            count++;
        }
    }

    return count;
}
