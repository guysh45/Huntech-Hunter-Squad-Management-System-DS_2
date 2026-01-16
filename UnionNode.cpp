#include "UnionNode.h"

void UnionSquad::addFight(){
    if (groupRoot != nullptr) {
        groupRoot->addFight();
    }
}