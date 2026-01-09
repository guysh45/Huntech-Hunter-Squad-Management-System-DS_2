#include <memory>
#include "AvlRankTree.h"
#include "RankAuraSquad.h"
#include <iostream>



using namespace std;

int main() {

	AvlRankTree<AuraSquad> tree;

	tree.insert(std::make_shared<AuraSquad>(1, 10));
	tree.insert(std::make_shared<AuraSquad>(2, 12));
	tree.insert(std::make_shared<AuraSquad>(3, 7));
	tree.insert(std::make_shared<AuraSquad>(4, 2));
	tree.insert(std::make_shared<AuraSquad>(5, 999));
	tree.insert(std::make_shared<AuraSquad>(6, 999));

	for (int i = 1; i < 6; i++) {
		cout << "rank: " << i << "    ||    id: " << tree.findRank(i)->getSquadId() << endl;
	}

	tree.insert(std::make_shared<AuraSquad>(90, 998));

	cout << "================================================" << endl;

	for (int i = 1; i < 7; i++) {
		cout << "rank: " << i << "    ||    id: " << tree.findRank(i)->getSquadId() << endl;
	}
	return 0;
}