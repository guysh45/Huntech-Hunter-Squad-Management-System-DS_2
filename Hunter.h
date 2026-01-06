#pragma once

#include "wet2util.h"

class hunter {

    int hunterId;
    int fightsHad;
    int joinedGroupFights;
    int aura;
    NenAbility Nen;

public:

    hunter(int hunterId, const NenAbility &nenAbility, int aura, int fightsHad, int joinedGroupFights){
        if (hunterId <= 0 || aura <= 0 || fightsHad < 0 || joinedGroupFights < 0 || nenAbility.isValid() == false) {
            throw std::invalid_argument("invalid parameters");
        }

        this->hunterId = hunterId;
        this->fightsHad = fightsHad;
        this->joinedGroupFights = joinedGroupFights;
        this->aura = aura;
        this->Nen = nenAbility;
    };
    ~hunter() = default;
    hunter(const hunter&) = delete;
    hunter& operator=(const hunter&) = delete;
    hunter(hunter &&) = delete;
    hunter &operator=(hunter &&) = delete;

    int getHunterId() const {return this->hunterId;};
    int getFightsHad() const {return this->fightsHad;};
    int getJoinedGroupFights() const {return this->joinedGroupFights;};
    int getAura() const {return this->aura;};
    const NenAbility& getNenAbility() const {return this->Nen;};
};