#pragma once

#include <memory>

#include "wet2util.h"
#include "UnionNode.h"
#include "UnionSquad.h"
#include "AvlRankTree.h"
#include "Hash.h"


/**
 * A Union class specifically for managing Hunters in Squads.
 * Using invert trees.
 * Using Path-Compression and Union by size.
 */
class Union {
	AvlRankTree<UnionSquad> groups;
	Hash<std::shared_ptr<UnionNode>> huntersHash;


	/**
	 * @brief Recursive function to compress paths from the given node. Keeps relNen and relFights correct.
	 * @param node -  The node to start at.
	 * @return The root of the Node.
	 */
	UnionNode* compress(UnionNode* node);

public:
	Union() = default;

	~Union() = default;

	Union(const Union &) = delete;

	Union &operator=(const Union &) = delete;

	Union(Union &&) = delete;

	Union &operator=(Union &&) = delete;


	/**
	 *@brief Returns the Exp of the Squad.
	 *
	 * @param groupId The Id of the Squad to query.
	 * @return if found - the squad Exp.
	 *          if not found - -1.
	 */
	int getSquadExp(const int groupId) ;

	/**
	 *@brief Gets the number of fights the hunter had.
	 *
	 * @param hunterId The Id of the Hunter to query.
	* @return if found - the Hunter fights.
	 *          if not found - -1.
	 */
	int getHunterFights(const int hunterId) ;

	/**
	 * @brief Gets the sum of NenAbility of everyone who joined before the hunter to the group.
	 * @param hunterId The Id of the Hunter to query.
	* @return if found - the requested NenAbility.
	 *          if not found / dead - NenAbility::invalid().
	 */
	NenAbility getHunterPartialNen(const int hunterId) ;


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


	/**
	 * @brief unites the groups by Id.
	 * @param groupToId - the group id which to unite to.
	 * @param groupFromId - the group id of the group to unite to the other.
	 * @return if seccsefful - true.
	 *			if one is not find - false.
	 */
	bool unite(const int groupToId, const int groupFromId);
};
