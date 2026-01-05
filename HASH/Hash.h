#pragma once
#include <stdexcept>


template<class T>
class Hash; //forward declaration.


template<class T>
class Node {
    Node *next = nullptr;
    int id = -1;
    T data;

    friend class Hash<T>;

public:
    Node() {
    };

    Node(const int id, const T &data) : id(id), data(data) {
    }
};


template<class T>
class Hash {
    static const int initialSize = 8;
    static const int growingFactor = 2;
    static const int initialCount = 0;
    static constexpr double maxLoadFactor = 0.75;


    int size = initialSize;
    int itemCount = initialCount;
    Node<T> *array;

    int hashFunction(int id) {
        return id % size;
    }

    void expandArray();

    void reHash(Node<T> *src, int srcSize);

public:
    Hash() {
        array = new Node<T> [initialSize]; // if throws then ok noting was allocated yet.
    }

    ~Hash();

    void insert(int id, const T &data);

    int getSize() const { return size; }
};


template<class T>
void Hash<T>::reHash(Node<T> *src, int srcSize) {
    itemCount = 0;
    for (int i = 0; i < srcSize; i++) {
        Node<T> *currentNode = src[i].next;
        while (currentNode != nullptr) {
            insert(currentNode->id, currentNode->data);
            currentNode = currentNode->next;
        }
    }
}

template<class T>
void Hash<T>::expandArray() {
    Node<T> *newArray = new Node<T> [size * growingFactor]; // if throws wants to keep going up.
    int prevSize = size;
    size = size * growingFactor;
    Node<T> *temp = array;
    array = newArray;

    reHash(temp, prevSize);

    for (int i = 0; i < prevSize; i++) {
        Node<T> *currentNode = temp[i].next;
        while (currentNode != nullptr) {
            Node<T> *toDelete = currentNode;
            currentNode = currentNode->next;
            delete toDelete;
        }
    }
    delete[] temp;
}


template<class T>
void Hash<T>::insert(int id, const T &data) {
    Node<T> *current = &array[hashFunction(id)];

    while (current->next != nullptr) {
        if(current->next->id == id) {
            throw std::invalid_argument("Id already exists");
        }
        current = current->next;
    }

    current->next = new Node<T>(id, data);
    itemCount++;

    if (static_cast<double>(itemCount) / size > maxLoadFactor) {
        expandArray();
    }
}

template<class T>
Hash<T>::~Hash() {
    for (int i = 0; i < size; i++) {
        Node<T> *currentNode = array[i].next;
        while (currentNode != nullptr) {
            Node<T> *toDelete = currentNode;
            currentNode = currentNode->next;
            delete toDelete;
        }
    }
    delete[] array;
}
