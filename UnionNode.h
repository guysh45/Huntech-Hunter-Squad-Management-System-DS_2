#pragma once

#include "wet2util.h"
#include "Hunter.h"
#include "AvlNode.h"
#include "UnionSquad.h"

class UnionNode {
    static const int INITAIL_FIGHTS = 0;

    int relFights = INITAIL_FIGHTS;
    NenAbility relNen; //default constructor 0.

    UnionNode *parent = nullptr;
    Hunter hunter;
    std::shared_ptr<UnionSquad> squad = nullptr;



    friend class Union;

public:

    explicit UnionNode(const Hunter& hunter) :hunter(hunter){};

    ~UnionNode() = default;
    UnionNode(const UnionNode&) = delete;
    UnionNode& operator=(const UnionNode&) = delete;
    UnionNode(UnionNode &&) = delete;
    UnionNode &operator=(UnionNode &&) = delete;
};