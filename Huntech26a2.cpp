// You can edit anything you want in this file.
// However, you need to implement all public Huntech functions, which are provided below as a template.

#include "Huntech26a2.h"
#include <stdexcept>

Huntech::Huntech() {
    //defualt
}

Huntech::~Huntech() {
    //default
}

StatusType Huntech::add_squad(int squadId) {
    if (squadId <= 0) return StatusType::INVALID_INPUT;
    try {
        if (!squads.insertSquad(squadId)) return StatusType::FAILURE;
    }catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    try {
        auraTree.insert(std::make_shared<AuraSquad>(squadId));
    }catch (const std::bad_alloc&) {
        squads.removeSquad(squadId); //remove will not throw since we inserted it before.
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType Huntech::remove_squad(int squadId) {
    if (squadId <= 0) return StatusType::INVALID_INPUT;

    std::shared_ptr<UnionSquad> temp = squads.findSquad(squadId);
    if (temp == nullptr) return StatusType::FAILURE;

    try {
        auraTree.remove(AuraSquad(squadId, temp->getAura()));
        squads.removeSquad(squadId);
    }catch (const std::invalid_argument&) {
        return StatusType::FAILURE; //we shouldn't get here, since we found it already.
    }
    return StatusType::SUCCESS;
}

StatusType Huntech::add_hunter(int hunterId,
                               int squadId,
                               const NenAbility &nenType,
                               int aura,
                               int fightsHad)
{
    if (hunterId <= 0 || squadId <= 0 || !nenType.isValid() || aura < 0 || fightsHad < 0) return StatusType::INVALID_INPUT;

    std::shared_ptr<UnionSquad> squad = squads.findSquad(squadId);
    if (squad == nullptr) return StatusType::FAILURE;

    bool needRankUpdate = (aura > 0);
    int oldAura = squad->getAura();


    if (needRankUpdate) {
        try {
            auraTree.insert(std::make_shared<AuraSquad>(squadId, oldAura + aura));
        }catch (const std::bad_alloc&) {
            return StatusType::ALLOCATION_ERROR;
        }
    }

    try {
        bool inserted = squads.insertHunterToGroup(squadId, hunterId, nenType, aura, fightsHad);

        if (!inserted) {
            if (needRankUpdate) {
                auraTree.remove(AuraSquad(squadId, oldAura + aura));
            }
            return StatusType::FAILURE;
        }
    } catch (const std::bad_alloc &) {
        if (needRankUpdate) {
            auraTree.remove(AuraSquad(squadId, oldAura + aura));
        }
        return StatusType::ALLOCATION_ERROR;
    }

    if (needRankUpdate) {
        try {
            auraTree.remove(AuraSquad(squadId, oldAura));  //exists because we found it before, shouldnt throw.
        }catch (const std::invalid_argument&) {
            return StatusType::FAILURE;
        }
    }
    return StatusType::SUCCESS;
}

output_t<int> Huntech::squad_duel(int squadId1, int squadId2) {

    if (squadId1 <= 0 || squadId2 <= 0 || squadId1 == squadId2) return StatusType::INVALID_INPUT;

    std::shared_ptr<UnionSquad> squad1 = squads.findSquad(squadId1);
    std::shared_ptr<UnionSquad> squad2 = squads.findSquad(squadId2);
    if (squad1 == nullptr || squad2 == nullptr) return StatusType::FAILURE;
    if (!squad1->getSize() || !squad2->getSize()) return StatusType::FAILURE;

    int effectiveAura1 = squad1->getAura() + squad1->getExp();
    int effectiveAura2 = squad2->getAura() + squad2->getExp();
    squad1->addFight();
    squad2->addFight();


    if (effectiveAura1 > effectiveAura2) {
        squad1->addExp(VICTORY_EXP);
        return 1;
    }
    if (effectiveAura2 > effectiveAura1) {
        squad2->addExp(VICTORY_EXP);
        return 3;
    }

    NenAbility Nen1 = squad1->getSquadNen();
    NenAbility Nen2 = squad2->getSquadNen();

    if (Nen1 > Nen2) {
        squad1->addExp(VICTORY_EXP);
        return 2;
    }
    if (Nen2 > Nen1) {
        squad2->addExp(VICTORY_EXP);
        return 4;
    }
    squad1->addExp(TIE_EXP);
    squad2->addExp(TIE_EXP);
    return 0;
}

output_t<int> Huntech::get_hunter_fights_number(int hunterId) {
    if (hunterId <= 0) return StatusType::INVALID_INPUT;

    int fights = squads.getHunterFights(hunterId);
    if (fights == -1) return StatusType::FAILURE;
    return fights;
}

output_t<int> Huntech::get_squad_experience(int squadId) {
    if (squadId <= 0) return StatusType::INVALID_INPUT;

    int exp = squads.getSquadExp(squadId);
    if (exp == -1) return StatusType::FAILURE;
    return exp;
}

output_t<int> Huntech::get_ith_collective_aura_squad(int i) {
    return 0;
}

output_t<NenAbility> Huntech::get_partial_nen_ability(int hunterId) {
    return NenAbility();
}

StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    return StatusType::FAILURE;
}
