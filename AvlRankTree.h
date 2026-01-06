#pragma once
#include <memory>

#include "Node.h"


/**
 *ASSUMPTIONS ON T:
 *
 *T has operator>().
 *
 * @tparam T the type of data the Tree will hold (by shared_ptr)
 */
template<class T>
class AvlRankTree {
    static const int BASE_COUNTER = 0;
    static const int BF2 = 2;
    static const int BF1 = 1;
    static const int BF0 = 0;
    static const int BFM1 = -1;
    static const int BFM2 = -2;


    Node<T> *root;
    int elementsCounter = BASE_COUNTER;

    /**
     * @brief - recursive helper function for Insert.
     * @param inData - The provided data to be inserted (by shared_ptr).
     * @param insertionPoint the node to try to insert to.
     */
    void insertionHelper(std::shared_ptr<T> inData, Node<T> *&insertionPoint);


    /**
     * @brief Determent the appropriate roll and preforms it.
     * @param problematicNode The unbalanced Node.
     */
    static void rollSelector(Node<T> *&problematicNode);

    /**
     * @brief Preforms an LL roll.
     * @param problematicNode - the unbalanced node.
     */
    static void LLRoll(Node<T> *&problematicNode);

    /**
     * @brief Preforms an RR roll.
     * @param problematicNode - the unbalanced node.
     */
    static void RRRoll(Node<T> *&problematicNode);

    /**
     * @brief Preforms an LR roll.
     * @param problematicNode - the unbalanced node.
     */
    static void LRRoll(Node<T> *&problematicNode);

    /**
     * @brief Preforms an RL roll.
     * @param problematicNode - the unbalanced node.
     */
    static void RLRoll(Node<T> *&problematicNode);


    /**
    * @brief - called by destructor and goes on a killing spree of nodes.
    */
    void destructorPostOrder(Node<T> *root);


    /**
     * @brief A recursive helper function for remove.
     * @param outData - the requested data to be removed.
     */
    void removeHelper(const T &outData, Node<T> *&current);

    /**
     *
     * @param current - The node to find the minimum node in its subtree.
     * @return the minimum Node in the subtree.
     */
    Node<T> *findMin(Node<T> *current);

    /**
     *@brief A recursive helper function for find.
     *
     * @param data The data to look for.
     * @param current The current looked upon node.
     * @return If found shared_ptr to the requested data. else nullptr.
     */
    std::shared_ptr<T> findHelper(const T &data, Node<T> *&current);


     /**
     *@brief finds a node by its rank.
     * @param i - the rank looked for
     * @return If found shared_ptr to the requested data. else nullptr.
     */
    std::shared_ptr<T> find_rank(int i);

public:
    AvlRankTree(): root(nullptr) {
    }

    ~AvlRankTree() {
        destructorPostOrder(root);
    }

    AvlRankTree(const AvlRankTree &) = delete;

    AvlRankTree &operator=(const AvlRankTree &) = delete;

    AvlRankTree(AvlRankTree &&) = delete;

    AvlRankTree &operator=(AvlRankTree &&) = delete;


    /**
     * @brief - Inserts inData to the correct place in the tree. handles balancing.
     * @param inData - The Data to be inserted to the tree (by shared_ptr).
     */
    void insert(std::shared_ptr<T> inData);


    /**
     * @brief removes the requested data from the tree.
     * @param data the data to be removed.
     */
    void remove(const T &data);

    /**
     *@brief looks for the requested data in the tree.
     *
     * @param data the data to look for.
     * @return a shared_ptr for the requested Data. if not found returns nullptr.
     */
    std::shared_ptr<T> find(const T &data) {
        return findHelper(data, root);
    }
};


template<class T>
void AvlRankTree<T>::insertionHelper(std::shared_ptr<T> inData, Node<T> *&insertionPoint) {
    if (*(insertionPoint->data) > *inData) {
        if (insertionPoint->left == nullptr) {
            insertionPoint->left = new Node<T>(inData);
        } else {
            insertionHelper(inData, insertionPoint->left);
        }
    } else if (*inData > *(insertionPoint->data)) {
        if (insertionPoint->right == nullptr) {
            insertionPoint->right = new Node<T>(inData);
        } else {
            insertionHelper(inData, insertionPoint->right);
        }
    } else {
        // inData is a duplicate
        throw std::invalid_argument("The data already exists");
    }

    insertionPoint->updateNodeStatus();
    if (insertionPoint->balanceFactor < BFM1 || insertionPoint->balanceFactor > BF1) {
        rollSelector(insertionPoint);
    }
}


template<class T>
void AvlRankTree<T>::rollSelector(Node<T> *&problematicNode) {
    if (problematicNode->balanceFactor == BF2) {
        if (problematicNode->left->balanceFactor >= BF0) {
            LLRoll(problematicNode);
        } else if (problematicNode->left->balanceFactor == BFM1) {
            LRRoll(problematicNode);
        }
    } else {
        if (problematicNode->right->balanceFactor <= BF0) {
            RRRoll(problematicNode);
        } else if (problematicNode->right->balanceFactor == BF1) {
            RLRoll(problematicNode);
        }
    }
}


template<class T>
void AvlRankTree<T>::LLRoll(Node<T> *&problematicNode) {
    Node<T> *newRoot = problematicNode->left;
    Node<T> *oldRoot = problematicNode;
    Node<T> *T2 = newRoot->right;

    problematicNode = newRoot;

    newRoot->right = oldRoot;
    oldRoot->left = T2;

    oldRoot->updateNodeStatus();
    newRoot->updateNodeStatus();
}


template<class T>
void AvlRankTree<T>::RRRoll(Node<T> *&problematicNode) {
    Node<T> *newRoot = problematicNode->right;
    Node<T> *oldRoot = problematicNode;
    Node<T> *T2 = newRoot->left;

    problematicNode = newRoot;

    newRoot->left = oldRoot;
    oldRoot->right = T2;

    oldRoot->updateNodeStatus();
    newRoot->updateNodeStatus();
}


template<class T>
void AvlRankTree<T>::LRRoll(Node<T> *&problematicNode) {
    if (problematicNode == nullptr || problematicNode->left == nullptr) return;
    RRRoll(problematicNode->left);
    LLRoll(problematicNode);
}


template<class T>
void AvlRankTree<T>::RLRoll(Node<T> *&problematicNode) {
    if (problematicNode == nullptr || problematicNode->right == nullptr) return;
    LLRoll(problematicNode->right);
    RRRoll(problematicNode);
}


template<class T>
void AvlRankTree<T>::insert(std::shared_ptr<T> inData) {
    if (root == nullptr) {
        root = new Node<T>(inData);
    } else {
        insertionHelper(inData, root);
    }
    // if for any reason (bad_alloc or invalid_argument) the previous lines throw, elementsCounter will remain the same.
    elementsCounter++;
}

template<class T>
void AvlRankTree<T>::destructorPostOrder(Node<T> *root) {
    if (root == nullptr) return;
    if (root->left != nullptr) destructorPostOrder(root->left);
    if (root->right != nullptr) destructorPostOrder(root->right);
    delete root;
}


template<class T>
void AvlRankTree<T>::remove(const T &data) {
    removeHelper(data, root);
    //if removeHelper throws the exception will propagate and the elementCounter will not change.
    elementsCounter--;
}


template<class T>
void AvlRankTree<T>::removeHelper(const T &outData, Node<T> *&current) {
    if (current == nullptr) {
        throw std::invalid_argument("The data to be removed does not exist");
    }

    if (*(current->data) > outData) {
        removeHelper(outData, current->left);
    } else if (outData > (*current->data)) {
        removeHelper(outData, current->right);
    } else {
        //at most one son
        if (current->right == nullptr || current->left == nullptr) {
            Node<T> *temp = current;

            if (current->right == nullptr) {
                current = current->left;
            } else {
                current = current->right;
            }

            delete temp;
        } else {
            //two sons

            //finding the node to replace the deleted node with and swapping only the data.
            Node<T> *replaceNode = findMin(current->right);
            std::shared_ptr<T> tempData = current->data;
            current->data = replaceNode->data;
            replaceNode->data = tempData;
            removeHelper(outData, current->right); //deletes the node the data is now in. has at most one son.
        }
    }
    //re-balancing
    if (current == nullptr) return;

    current->updateNodeStatus();

    if (current->balanceFactor > BF1 || current->balanceFactor < BFM1) {
        rollSelector(current);
    }
}


template<class T>
Node<T> *AvlRankTree<T>::findMin(Node<T> *current) {
    if (current == nullptr) return nullptr;
    while (current != nullptr && current->left != nullptr) {
        current = current->left;
    }
    return current;
}


template<class T>
std::shared_ptr<T> AvlRankTree<T>::findHelper(const T &data, Node<T> *&current) {
    if (current == nullptr) return nullptr;

    if (*(current->data) > data) {
        return findHelper(data, current->left);
    }
    if (data > *(current->data)) {
        return findHelper(data, current->right);
    }
    return current->data;
}

template<class T>
std::shared_ptr<T> AvlRankTree<T>::find_rank(int i) {
    if (i <= 0 || i > elementsCounter) {
        return nullptr;
    }

    Node<T> *current = root;

    while (current != nullptr) {
        int sizeLeft = 1;
        if (current->left != nullptr) {
            sizeLeft = current->left->childrenCount + 1;
        }

        if (sizeLeft == i) return current->data;    //found a node that has the right amount of left childrens.

        if (sizeLeft > i) {  //if left suv tree had more then i - 1 nodes. the node we are looking for is in the left size
            current = current->left;
        }else if (current->right != nullptr) { // if left sub tree doesnt have enough nodes, the one we are looking for is in the rights size
            current = current->right;
            i -= sizeLeft;
        }else {
            return nullptr;
        }
    }
}