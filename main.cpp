#include "Union.h"
#include <iostream>
#include <memory>



using namespace std;


int main() {

    Union U;

    for (int i = 1; i <= 10; i++) {
        if (U.findSquad(i) == nullptr) {
            cout << "id: " << i <<"    NOT FOUND" << endl;
        }else {
            cout << "id: " << i << "    FOUND" << endl;
            cout << "idm?: " << U.findSquad(i)->squadId << endl;
        }
    }

    U.insertSquad(1);
    U.insertSquad(2);
    U.insertSquad(3);
    U.insertSquad(4);

    U.insertSquad(6);
    U.insertSquad(7);
    U.insertSquad(8);
    cout << "======================================================="<< endl;
    for (int i = 1; i <= 10; i++) {
        if (U.findSquad(i) == nullptr) {
            cout << "id: " << i <<"    NOT FOUND" << endl;
        }else {
            cout << "id: " << i << "    FOUND" << endl;
            cout << "idm?: " << U.findSquad(i)->squadId << endl;
        }
    }

    cout << endl << endl;
    cout << "removing 2: " << U.removeSquad(2) << " (expected true/1)" << endl;
    cout << "removing 7: " << U.removeSquad(7) << " (expected true/1)" << endl;

    cout << "======================================================="<< endl;
    for (int i = 1; i <= 10; i++) {
        if (U.findSquad(i) == nullptr) {
            cout << "id: " << i <<"    NOT FOUND" << endl;
        }else {
            cout << "id: " << i << "    FOUND" << endl;
            cout << "idm?: " << U.findSquad(i)->squadId << endl;
        }
    }
    return 0;
}