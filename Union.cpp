#include "Union.h"
#include "memory"
#include "stdexcept"


///////////Getters///////////////////
int Union::findSquadOfHunter(const int hunterId) {
	std::shared_ptr<UnionNode>* temp = huntersHash.find(hunterId);
	if ( temp == nullptr) return -1; //hunter doesnt exists.

	UnionNode* root = compress(temp->get());
	return root->squad->squadId;
}


NenAbility Union::getSquadNenabillity(const int groupId) {
	const std::shared_ptr<UnionSquad> squadPtr = groups.find(UnionSquad(groupId));
	if (squadPtr == nullptr) return NenAbility();
	return squadPtr->squadNen;
}

int Union::getSquadSize(const int groupId) {
	std::shared_ptr<UnionSquad> squadPtr = groups.find(UnionSquad(groupId));
	if (squadPtr == nullptr) return -1;
	return squadPtr->size;
}

int Union::getSquadAura(const int groupId) {
	std::shared_ptr<UnionSquad> squadPtr = groups.find(UnionSquad(groupId));
	if (squadPtr == nullptr) return -1;
	return squadPtr->squadAura;
}

int Union::getSquadExp(const int groupId) {
	std::shared_ptr<UnionSquad> squadPtr = groups.find(UnionSquad(groupId));
	if (squadPtr == nullptr) return -1;
	return squadPtr->squadExp;
}

int Union::getHunterFights(const int hunterId) {
	std::shared_ptr<UnionNode>* temp = huntersHash.find(hunterId);
	if ( temp == nullptr) return -1; //hunter doesnt exists.
	UnionNode* current = temp->get();
	UnionNode* root = compress(current);

	int fights = current->hunter.getFightsHad() - current->hunter.getJoinedGroupFights();
	fights += current->relFights;
	if (current != root) {
		fights += root->relFights;
	}

	return fights;
}


NenAbility Union::getHunterPartialNen(const int hunterId) {
	std::shared_ptr<UnionNode>* temp = huntersHash.find(hunterId);
	if ( temp == nullptr) return NenAbility::invalid(); //hunter doesnt exists.
	UnionNode* current = temp->get();
	UnionNode* root = compress(current);


	if (root->squad->groupRoot == nullptr) return NenAbility::invalid();

	NenAbility nen = current->hunter.getNenAbility() + current->relNen;
	if (current != root) {
		nen += root->relNen;
	}
	return nen;
}


//////////////////FUNctions/////////////////////

bool Union::insertSquad(const int id) {
	try {
		groups.insert(std::make_shared<UnionSquad>(id));
	} catch (const std::invalid_argument &) {
		return false;
	}
	return true;
}

//we dont delete the UnionSquad. just remove it from the tree to keep the data right for the hunter.
bool Union::removeSquad(const int id) {
	std::shared_ptr<UnionSquad> temp = groups.find(UnionSquad(id));
	if (temp == nullptr) return false;

	try {
		groups.remove(*temp);
	} catch (const std::invalid_argument &) {
		return false;
		//we should never get here. we checked it's in the tree already.
	}
	temp->groupRoot = nullptr; //cut the root access from the squad (this way we know if a group is dead)
	return true;
}


bool Union::insertHunterToGroup(const int groupId, const int hunterId, const NenAbility &nenAbility, const int aura,
                                const int fightsHad) {
	if (huntersHash.find(hunterId) != nullptr) return false; //hunter already exists.


	const std::shared_ptr<UnionSquad> squadPtr = groups.find(UnionSquad(groupId));
	if (squadPtr == nullptr) return false; //no such group.

	int squadRootRelFights = squadPtr->groupRoot == nullptr ? 0 : squadPtr->groupRoot->relFights;
	//sets rel fight according to root or not.

	Hunter newHunter(hunterId, nenAbility, aura, fightsHad, squadRootRelFights);
	std::shared_ptr<UnionNode> newNode = std::make_shared<UnionNode>(newHunter);

	if (squadPtr->groupRoot == nullptr) {
		squadPtr->groupRoot = newNode.get();
		newNode->squad = squadPtr;
		newNode->relNen = squadPtr->squadNen ;
	} else {
		newNode->parent = squadPtr->groupRoot;
		newNode->relNen = squadPtr->squadNen - newNode->parent->relNen;
	}


	huntersHash.insert(hunterId, newNode);
	squadPtr->size++;
	squadPtr->squadAura += newHunter.getAura();
	squadPtr->squadNen += newHunter.getNenAbility();
	return true;
}


bool Union::unite(const int groupToId, const int groupFromId) {
	if(groupFromId == groupToId) return true;
	const std::shared_ptr<UnionSquad> squadToPtr = groups.find(UnionSquad(groupToId));
	const std::shared_ptr<UnionSquad> squadFromPtr = groups.find(UnionSquad(groupFromId));
	if (squadToPtr == nullptr || squadFromPtr == nullptr) return false; //no such group.

	UnionNode *toRoot = squadToPtr->groupRoot;
	UnionNode *fromRoot = squadFromPtr->groupRoot;


	//From is empty. just remove it from the tree.
	if(squadFromPtr->groupRoot == nullptr) {
		try {
			groups.remove(*squadFromPtr);
		} catch (const std::invalid_argument &) {
			return false;
			//we should never get here. we checked it's in the tree already.
		}
		return true;
	}

	//To is empty.
	if(squadToPtr->groupRoot == nullptr) {
		squadToPtr->groupRoot = squadFromPtr->groupRoot;
		squadToPtr->groupRoot->squad = squadToPtr;

		squadToPtr->size = squadFromPtr->size;
		squadToPtr->squadAura = squadFromPtr->squadAura;
		squadToPtr->squadExp = squadFromPtr->squadExp;
		squadToPtr->squadNen = squadFromPtr->squadNen;

		try {
			groups.remove(*squadFromPtr);
		} catch (const std::invalid_argument &) {
			return false;
			//we should never get here. we checked it's in the tree already.
		}
		return true;
	}



	//To>=From
	if (squadToPtr->size >= squadFromPtr->size) {
		fromRoot->parent = toRoot;
		fromRoot->squad = nullptr;

		fromRoot->relFights -= toRoot->relFights;
		fromRoot->relNen += squadToPtr->squadNen - toRoot->relNen;
	} else { //To<From
		toRoot->parent = fromRoot;
		fromRoot->squad = squadToPtr;
		squadToPtr->groupRoot = fromRoot;
		toRoot->squad = nullptr;
		squadFromPtr->groupRoot = nullptr;

		toRoot->relFights -= fromRoot->relFights;

		fromRoot->relNen += squadToPtr->squadNen;
		toRoot->relNen -= fromRoot->relNen;
	}

	squadToPtr->size += squadFromPtr->size;
	squadToPtr->squadAura += squadFromPtr->squadAura;
	squadToPtr->squadExp += squadFromPtr->squadExp;
	squadToPtr->squadNen += squadFromPtr->squadNen;

	try {
		groups.remove(*squadFromPtr);
	} catch (const std::invalid_argument &) {
		return false;
		//we should never get here. we checked it's in the tree already.
	}

	return true;
}



UnionNode* Union::compress(UnionNode* node) {
	if(node == nullptr) {
		return nullptr; //safety check.
	}

	if(node->parent == nullptr) {
		return node; //this is the root.
	}

	UnionNode* root = compress(node->parent);

	if(node->parent != root) {
		node->relFights += node->parent->relFights;
		node->relNen += node->parent->relNen;
		node->parent = root;
	}

	return root;
}
