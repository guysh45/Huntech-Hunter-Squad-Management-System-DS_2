#include <iostream>
#include "Hash.h"

using namespace std;

int main() {
    Hash<int> hash;
    cout << hash.getSize() <<endl;
    hash.insert(1,1);
    cout << hash.getSize() <<endl;
    hash.insert(2,2);
    cout << hash.getSize() <<endl;
    hash.insert(3,3);
    cout << hash.getSize() <<endl;
    hash.insert(4,4);
    cout << hash.getSize() <<endl;
    hash.insert(5,5);
    cout << hash.getSize() <<endl;
    hash.insert(6,6);
    cout << hash.getSize() <<endl;
    hash.insert(7,7);
    cout << hash.getSize() <<endl;
    hash.insert(8,8);
    cout << hash.getSize() <<endl;
    hash.insert(9,9);
    cout << hash.getSize() <<endl;
    hash.insert(10,10);
    cout << hash.getSize() <<endl;
    hash.insert(11,11);
    cout << hash.getSize() <<endl;
    hash.insert(12,12);
    cout << hash.getSize() <<endl;
    hash.insert(13,13);
    cout << hash.getSize() <<endl;
    hash.insert(14,14);
    cout << hash.getSize() <<endl;
    hash.insert(15,15);
    cout << hash.getSize() <<endl;
    hash.insert(15,15);

    return 0;
}
