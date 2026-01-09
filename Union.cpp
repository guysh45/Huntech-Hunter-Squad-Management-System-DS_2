#include "Union.h"
#include "memory"
#include "stdexcept"


///////////Getters///////////////////
int Union::findSquadOfHunter(const int hunterId) {

    if(huntersHash.find(hunterId) == nullptr) return -1; //hunter doesnt exists.
    UnionNode* current = *huntersHash.find(hunterId);
    while(current->parent != nullptr) {
        current = current->parent;
    }
    return current->squad->squadId;
}


NenAbility Union::getSquadNenabillity(const int groupId) {
    std::shared_ptr<UnionSquad> squadPtr  = groups.find(UnionSquad(groupId));
    if(squadPtr == nullptr) return NenAbility();
    return  squadPtr->squadNen;
}

int Union::getSquadSize(const int groupId) {
    std::shared_ptr<UnionSquad> squadPtr  = groups.find(UnionSquad(groupId));
    if(squadPtr == nullptr) return -1;
    return  squadPtr->size;
}

int Union::getSquadAura(const int groupId) {
    std::shared_ptr<UnionSquad> squadPtr  = groups.find(UnionSquad(groupId));
    if(squadPtr == nullptr) return -1;
    return  squadPtr->squadAura;
}

int Union::getSquadExp(const int groupId) {
    std::shared_ptr<UnionSquad> squadPtr  = groups.find(UnionSquad(groupId));
    if(squadPtr == nullptr) return -1;
    return  squadPtr->squadExp;
}

int Union::getHunterFights(const int hunterId) {
    UnionNode** temp = huntersHash.find(hunterId);
    if(temp == nullptr) return -1;
    UnionNode* current = *temp;

    int fights = current->hunter.getFightsHad() - current->hunter.getJoinedGroupFights();
    while(current != nullptr) {
        fights += current->relFights;
        current = current->parent;
    }
    return fights;
}


NenAbility Union::getHunterPartialNen(const int hunterId) {
    UnionNode** temp = huntersHash.find(hunterId);
    if(temp == nullptr) throw std::invalid_argument("no such Hunter"); //what should we do?
    UnionNode* current = *temp;

    NenAbility nen = current->hunter.getNenAbility();
    while(current != nullptr) {
        nen += current->relNen;
        current = current->parent;
    }
    return nen;
}





//////////////////FUNctions/////////////////////

bool Union::insertSquad(const int id) {
    try {
        groups.insert(std::make_shared<UnionSquad>(id));
    }catch (const std::invalid_argument&) {
        return false;
    }
    return true;
}

//we dont delete the UnionSquad. just remove it from the tree to keep the data right for the hunter.
bool Union::removeSquad(const int id) {
    std::shared_ptr<UnionSquad> temp = groups.find(UnionSquad(id));
    if (temp == nullptr) return false;

    try {
        groups.remove(*temp);
    }catch (const std::invalid_argument&) {
        return false;
        //we should never get here. we checked it's in the tree already.
    }
    return true;
}


bool Union::insertHunterToGroup(const int groupId, const int hunterId, const NenAbility& nenAbility, const int aura, const int fightsHad) {

    if(huntersHash.find(hunterId) != nullptr) return false; //hunter already exists.


   std::shared_ptr<UnionSquad> squadPtr  = groups.find(UnionSquad(groupId));
    if(squadPtr == nullptr) return false; //no such group.

    int squadRootRelFights = squadPtr->groupRoot == nullptr ? 0 : squadPtr->groupRoot->relFights;  //sets rel fight according to root or not.

    Hunter newHunter(hunterId,nenAbility,aura,fightsHad,squadRootRelFights);
    UnionNode* newNode = new UnionNode(newHunter);

    if(squadPtr->groupRoot == nullptr) {
        squadPtr->groupRoot = newNode;
        newNode->squad = squadPtr;
    } else {
        newNode->parent = squadPtr->groupRoot;
    }

    newNode->relNen = squadPtr->squadNen;

    huntersHash.insert(hunterId,newNode);
    squadPtr->size ++;
    squadPtr->squadAura += newHunter.getAura();
    squadPtr->squadNen += newHunter.getNenAbility();
    return true;
}

