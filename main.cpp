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

    cout << "======================================================="<< endl;

    U.insertSquad(11);
    U.insertSquad(12);

    U.insertHunterToGroup(11,212742779,NenAbility("Enhancer"),10,2);
    U.insertHunterToGroup(11,212081194,NenAbility("Manipulator"),50,70);
    U.insertHunterToGroup(11,2054582156,NenAbility("Transmuter"),2,1);


    U.insertHunterToGroup(12,4564964516,NenAbility("Conjurer"),2,12);


    if(U.findSquadOfHunter(212742779) == U.findSquadOfHunter(212081194) && U.findSquadOfHunter(212742779) == U.findSquadOfHunter(2054582156)) {
        cout << "Same group test  :: SUCCSESS" << endl;
    } else {
        cout << "Same group test  :: FAIL" << endl;
    }

    if(U.findSquadOfHunter(212742779) == U.findSquadOfHunter(4564964516)) {
        cout << "DIFF group test  :: FAIL" << endl;
    } else {
        cout << "DIFF group test  :: SUCCSESS" << endl;
    }
    cout << "======================================================="<< endl;
    cout << "Group 11 : " << endl;
    cout << "Squad Size :: "<< U.getSquadSize(11) << " || (Should be 3) " << endl;
    cout << "Squad Exp :: "<< U.getSquadExp(11) << " || (Should be 0) " << endl;
    cout << "Squad Aura :: "<< U.getSquadAura(11) << " || (Should be 62) " << endl;
    cout << "Squad Nenabillity :: "<< U.getSquadNenabillity(11) << " || (Should be .....) " << endl;
    cout << "Menmbers : " << endl;
    cout << "Hunter of id: 212742779 is in group ||" << U.findSquadOfHunter(212742779) << "|| And has been in ||" << U.getHunterFights(212742779) << "|| Fights || (Should be 11 and 2) " << endl;
    cout << "Hunter of id: 212081194 is in group ||" << U.findSquadOfHunter(212081194) << "|| And has been in ||" << U.getHunterFights(212081194) << "|| Fights || (Should be 11 and 70) " << endl;
    cout << "Hunter of id: 2054582156 is in group ||" << U.findSquadOfHunter(2054582156) << "|| And has been in ||" << U.getHunterFights(2054582156) << "|| Fights || (Should be 11 and 1) " << endl;
    cout << "=========================" << endl;
    cout << "Group 12 : " << endl;
    cout << "Squad Size :: "<< U.getSquadSize(12) << " || (Should be 1) " << endl;
    cout << "Squad Exp :: "<< U.getSquadExp(12) << " || (Should be 0) " << endl;
    cout << "Squad Aura :: "<< U.getSquadAura(12) << " || (Should be 2) " << endl;
    cout << "Squad Nenabillity :: "<< U.getSquadNenabillity(12) << " || (Should be one Conjurer) " << endl;
    cout << "Menmbers : " << endl;
    cout << "Hunter of id: 4564964516 is in group ||" << U.findSquadOfHunter(4564964516) << "|| And has been in ||" << U.getHunterFights(4564964516) << "|| Fights || (Should be 12 and 12) " << endl;
    cout << "======================================================="<< endl;
    cout << "Non existsing hunter : " << endl;

    cout << U.findSquadOfHunter(4) << " || (Should be -1) " << endl;




    return 0;
}