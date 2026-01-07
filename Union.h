#pragma once

#include "UnionNode.h"
#include "UnionSquad.h"
#include "AvlRankTree.h"
#include "Hash.h"
#include "memory"


class Union {
    AvlRankTree<UnionSquad> groups;
    Hash<UnionNode*> huntersHash;

public:

    Union() = default;
    ~Union() = default;
    Union(const Union&) = delete;
    Union& operator=(const Union&) = delete;
    Union(Union &&) = delete;
    Union &operator=(Union &&) = delete;


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





};