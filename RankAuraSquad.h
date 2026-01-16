#pragma once

struct AuraSquad {
private:
    static const int INITIAL_AURA = 0;

    int squadId;
    int squadAura;

public:
    AuraSquad(int squadId, int squadAura) : squadId(squadId), squadAura(squadAura) {};
    explicit AuraSquad(int squadId) : squadId(squadId), squadAura(INITIAL_AURA) {};
    ~AuraSquad() = default;
    AuraSquad(const AuraSquad&) = delete;
    AuraSquad& operator=(const AuraSquad&) = delete;
    AuraSquad(AuraSquad &&) = delete;
    AuraSquad &operator=(AuraSquad &&) = delete;

    bool operator>(const AuraSquad& other) const {
        if (squadAura > other.squadAura) {
            return true;
        }
        if (squadAura < other.squadAura) {
            return false;
        }
        if (squadId > other.squadId) return true;
        return false;
    }

    int getSquadId() {
        return squadId;
    }

};