#pragma once

#include "wet2util.h"
#include "Hunter.h"
#include "AvlNode.h"

class UnionNode {

    UnionNode *parent = nullptr;

    Hunter hunter;
    AvlNode<Squad>* squad = nullptr;

    friend class Union;

public:

    UnionNode(int hunterId, const NenAbility &nenAbility, int aura, int fightsHad, int joinedGroupFights, int squadId) :

            hunter(hunterId, nenAbility, aura, fightsHad, joinedGroupFights)

    {};

    ~UnionNode() = default;
    UnionNode(const UnionNode&) = delete;
    UnionNode& operator=(const UnionNode&) = delete;
    UnionNode(UnionNode &&) = delete;
    UnionNode &operator=(UnionNode &&) = delete;
};