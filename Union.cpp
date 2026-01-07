#include "Union.h"
#include "memory"
#include "stdexcept"

bool Union::insertSquad(const int id) {
    try {
        groups.insert(std::make_shared<UnionSquad>(id));
    }catch (const std::invalid_argument&) {
        return false;
    }
    return true;
}


bool Union::removeSquad(const int id) {
    std::shared_ptr<UnionSquad> temp = groups.find(UnionSquad(id));
    if (temp == nullptr) return false;
    if (temp->groupRoot != nullptr) {
        temp->groupRoot->squad = nullptr; //cuts the group from the tree
    }

    try {
        groups.remove(*temp);
    }catch (const std::invalid_argument&) {
        return false;
    }
    return true;
}