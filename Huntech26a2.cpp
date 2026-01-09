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
    return StatusType::FAILURE;
}

output_t<int> Huntech::squad_duel(int squadId1, int squadId2) {
    return 0;
}

output_t<int> Huntech::get_hunter_fights_number(int hunterId) {
    return 0;
}

output_t<int> Huntech::get_squad_experience(int squadId) {
    return 0;
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
