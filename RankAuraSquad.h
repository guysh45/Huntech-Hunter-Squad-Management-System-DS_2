#pragma once

struct AuraSuad {
private:
    int squadId;
    int squadAura;

public:
    AuraSuad(int squadId, int squadAura) : squadId(squadId), squadAura(squadAura) {};
    ~AuraSuad() = default;
    AuraSuad(const AuraSuad&) = delete;
    AuraSuad& operator=(const AuraSuad&) = delete;
    AuraSuad(AuraSuad &&) = delete;
    AuraSuad &operator=(AuraSuad &&) = delete;

    bool operator>(const AuraSuad& other) const {
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