#pragma once

#include "wet2util.h"

#include "UnionNode.h"
#include "UnionSquad.h"
#include "AvlRankTree.h"
#include "Hash.h"
#include "memory"


class Union {
	AvlRankTree<UnionSquad> groups;
	Hash<UnionNode *> huntersHash;

public:
	Union() = default;

	~Union() = default;

	Union(const Union &) = delete;

	Union &operator=(const Union &) = delete;

	Union(Union &&) = delete;

	Union &operator=(Union &&) = delete;

	/**
	 *
	 * @param hunterId the id of the Hunter to look for.
	 * @return if found - its SquadId.
	 *          if not found - -1.
	 */
	int findSquadOfHunter(const int hunterId);

	/**
	 *
	 * @param groupId The Id of the Squad to query.
	 * @return if found - its NenAbillity.
	 *          if not found - -1.
	 */
	NenAbility getSquadNenabillity(const int groupId);

	/**
	 *
	 * @param groupId The Id of the Squad to query.
	 * @return if found - its Size.
	 *          if not found - -1.
	 */
	int getSquadSize(const int groupId);

	/**
	 *
	 * @param groupId The Id of the Squad to query.
	 * @return if found - the squad Aura.
	 *          if not found - -1.
	 */
	int getSquadAura(const int groupId);

	/**
	 *
	 * @param groupId The Id of the Squad to query.
	 * @return if found - the squad Exp.
	 *          if not found - -1.
	 */
	int getSquadExp(const int groupId);

	/**
	 *@brief Gets the number of fights the hunter had.
	 * @param hunterId The Id of the Hunter to query.
	* @return if found - the Hunter fights.
	 *          if not found - -1.
	 */
	int getHunterFights(const int hunterId);

	/**
	 * @brief Gets the sum of NenAbility of everyone who joined before the hunter to the group.
	 * @param hunterId The Id of the Hunter to query.
	* @return if found - the requested NenAbility.
	 *          if not found - NenAbility::invalid().
	 */
	NenAbility getHunterPartialNen(const int hunterId);


	/**
	 * @param id - id of squad to be inserted
	 * @return true - if insertion is successful
	 *         false - if already exists
	 *         throws if bad alloc
	 */
	bool insertSquad(const int id);


	/**
	 *
	 * @param id - id of squad to find
	 * @return share ptr to squad if found, nullptr if not
	 */
	std::shared_ptr<UnionSquad> findSquad(const int id) {
		return groups.find(UnionSquad(id));
	}

	/**
	 *
	 * @param id - id of squad to be removed
	 * @return true if successful, false otherwise
	 */
	bool removeSquad(const int id);


	/**
	 *@brief inserts a new Hunter to a squad.
	 *
	 * @param groupId the id of the squad to insert to.
	 * @param hunterId the id of the new Hunter.
	 * @param nenAbility the nenAbility of the new Hunter.
	 * @param aura the Aura of the new Hunter.
	 * @param fightsHad the fights the new Hunter has.
	 * @return if successful true.
	 *         if not false. (already exists).
	 */
	bool insertHunterToGroup(const int groupId, const int hunterId, const NenAbility &nenAbility, const int aura,
	                         const int fightsHad);


	bool unite(const int groupToId, const int groupfromId);
};
