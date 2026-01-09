#pragma once

#include <memory>


template<class T>
class AvlRankTree;

template<class T>
class AvlNode {
    static const int BASE_HEIGHT = 0;
    static const int BASE_BF = 0;
    static const int BASE_BFM1 = -1;
    static const int CHILDRENS1 = 1;
    static const int CHILDRENS0 = 0;


    AvlNode *left;
    AvlNode *right;

    int height;
    int balanceFactor;
    int childrenCount;

    std::shared_ptr<T> data;

    friend class AvlRankTree<T>;

public:
    AvlNode() = delete;

    /**
     *
     * @param inData - a shared_ptr to the data the Node will hold.
     */
    explicit AvlNode(std::shared_ptr<T> inData): left(nullptr), right(nullptr), height(BASE_HEIGHT),
                                              balanceFactor(BASE_BF),childrenCount(CHILDRENS1), data(std::move(inData)) {
    }

    ~AvlNode() = default;
    AvlNode(const AvlNode&) = delete;
    AvlNode& operator=(const AvlNode&) = delete;



    /**
     * @brief - update the balanceFactor, childernCount and heights of a Node
     */
    void updateNodeStatus() {
        int leftH = BASE_BFM1, rightH = BASE_BFM1;
        int leftC = CHILDRENS0, rightC = CHILDRENS0;
        if (left != nullptr) {
            leftH = left->height;
            leftC = left->childrenCount;
        }
        if (right != nullptr) {
            rightH = right->height;
            rightC = right->childrenCount;
        }
        height = leftH >= rightH ? leftH + 1 : rightH + 1;
        balanceFactor = leftH - rightH;
        childrenCount = leftC + rightC + 1;
    }
};
