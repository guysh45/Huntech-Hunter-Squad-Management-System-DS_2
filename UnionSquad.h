#pragma once

#include "UnionNode.h"
#include "wet2util.h"
#include "stdexcept"

class UnionNode;

class UnionSquad {
    static const int INITIAL_SIZE = 0;
    static const int INITIAL_AURA = 0;
    static const int INITIAL_EXP = 0;

    int squadId;
    int size = INITIAL_SIZE;
    int squadExp = INITIAL_EXP;
    int squadAura = INITIAL_AURA;
    UnionNode* groupRoot = nullptr;
    NenAbility squadNen;


    friend class Union;

public:

    explicit UnionSquad(const int squadId) {
        if (squadId <= 0) {
            throw std::invalid_argument("squadId must be greater than 0");
        }
        this->squadId = squadId;
    }

    ~UnionSquad() = default;
    UnionSquad(const UnionSquad&) = delete;
    UnionSquad& operator=(const UnionSquad&) = delete;
    UnionSquad(UnionSquad &&) = delete;
    UnionSquad &operator=(UnionSquad &&) = delete;

    bool operator>(const UnionSquad &other) const {return squadId > other.squadId;}

    int getAura() const {return squadAura;}
};
