#pragma once

/**
 * A Dynamic Hash table.
 * Using chained Hashing.
 *
 * Maps between Int(id) and T(data)
 *
 * Assumptions on T:
 * - a default constructor.
 * - operator=(const& T)
 */


template<class T>
class Hash; //forward declaration.


template<class T>
class HashNode {
	HashNode *next = nullptr;
	int id = -1;
	T data;

	friend class Hash<T>;

public:
	HashNode() = default;

	~HashNode() = default;

	HashNode(const int id, const T &data) : id(id), data(data) {
	}
};


template<class T>
class Hash {
	static const int INITIAL_SIZE = 8;
	static const int GROWING_FACTOR = 2;
	static const int INITIAL_COUNT = 0;
	static constexpr double MAX_LOAD_FACTOR = 1;


	int size = INITIAL_SIZE;
	int itemCount = INITIAL_COUNT;
	HashNode<T> *array;


	/**
	*@brief The Hash function the Hash-Table will use.
	*@param The id to calculates for
	*@return The index the Id is mapped to.
	*/
	int hashFunction(int id) const {
		return id % size;
	}

	/**
	 * Expands the array by the growingFactor and rehash the items to the new table.
	 */
	void expandArray();

public:
	Hash() {
		array = new HashNode<T> [INITIAL_SIZE]; // if throws then ok noting was allocated yet.
	}

	~Hash();

	Hash(const Hash &) = delete;

	Hash &operator=(const Hash &) = delete;

	Hash(const Hash &&) = delete;

	Hash &operator=(const Hash &&) = delete;

	int getSize() const { return size; }

	/**
	 *@breif insert the requested data with the provided id.
	 *
	 * @param id the id of the Data to insert
	 * @param data the data to insert
	 * @return false if the item already exists in which case nothing happens. true if inserted successfully.
	 */
	bool insert(int id, const T &data);

	/**
	 *
	 * @param id the id to search for.
	 * @return a pointer to the data of the id.
	 */
	T* find(int const id);
};


template<class T>
void Hash<T>::expandArray() {
	int prevSize = size;
	int newSize = size * GROWING_FACTOR;
	HashNode<T> *newArray = new HashNode<T> [newSize];
	size = newSize;

	for (int i = 0; i < prevSize; i++) {
		HashNode<T> *curr = array[i].next;

		while (curr != nullptr) {
			HashNode<T> *nextInOldChain = curr->next;
			int newIndex = hashFunction(curr->id);

			curr->next = newArray[newIndex].next;
			newArray[newIndex].next = curr;

			curr = nextInOldChain;
		}
	}

	delete[] array;
	array = newArray;
}


template<class T>
bool Hash<T>::insert(int id, const T &data) {
	HashNode<T> *current = &array[hashFunction(id)];

	while (current->next != nullptr) {
		if (current->next->id == id) {
			return false;
		}
		current = current->next;
	}

	current->next = new HashNode<T>(id, data);
	itemCount++;

	if (static_cast<double>(itemCount) / size > MAX_LOAD_FACTOR) {
		expandArray();
	}
	return true;
}


template<class T>
Hash<T>::~Hash() {
	for (int i = 0; i < size; i++) {
		HashNode<T> *currentNode = array[i].next;
		while (currentNode != nullptr) {
			HashNode<T> *toDelete = currentNode;
			currentNode = currentNode->next;
			delete toDelete;
		}
	}
	delete[] array;
}


template<class T>
T *Hash<T>::find(int const id) {
	HashNode<T> *currentNode = array[hashFunction(id)].next;
	while (currentNode != nullptr) {
		if (currentNode->id == id) {
			return &currentNode->data;
		}
		currentNode = currentNode->next;
	}
	return nullptr;
}
