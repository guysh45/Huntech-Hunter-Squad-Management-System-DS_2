#pragma once

#include <memory>

#include "wet2util.h"
#include "Hunter.h"
#include "UnionSquad.h"

class UnionNode {
    static const int INITIAL_FIGHTS = 0;

    int relFights = INITIAL_FIGHTS;
    NenAbility relNen; //default constructor 0.

    UnionNode *parent = nullptr;
    Hunter hunter;
    std::shared_ptr<UnionSquad> squad = nullptr;

    friend class Union;

public:

    explicit UnionNode(const Hunter& hunter) : hunter(hunter), relNen(NenAbility::zero()) {};

    ~UnionNode() = default;
    UnionNode(const UnionNode&) = delete;
    UnionNode& operator=(const UnionNode&) = delete;
    UnionNode(UnionNode &&) = delete;
    UnionNode &operator=(UnionNode &&) = delete;

    /**
     * @brief When called on the root of a Group, symbolizes adding a fights to all the members of the group.
     * Practically adds 1 to relFights.
     */
    void addFight() {
        relFights++;
    }
};