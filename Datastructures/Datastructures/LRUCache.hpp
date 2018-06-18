#pragma once
#include <unordered_map>
#include <memory>
#include <assert.h>

template <typename Key, typename T>
struct LRUCacheNode
{
	// The key of the node
	Key key;
	// The data stored in the node
	std::shared_ptr<T> data;
	// The previous node 
	LRUCacheNode* prev;
	// The next node
	LRUCacheNode* next;

	// creates a new node
	LRUCacheNode(const Key& _key, const std::shared_ptr<T> _data)
	{
		key		= _key;
		data	= _data;
		prev	= nullptr;
		next	= nullptr;
	}
};

template <typename Key, typename T>
class LRUCache
{
private:
	// The head of the recently used list (most recently used)
	LRUCacheNode<Key, T>* mHead;
	// The tail of the recently used list (least recently used)
	LRUCacheNode<Key, T>* mTail;
	// The max capacity of the cache
	uint32_t iMaxCapacity;

	// The map used to have direct access to cache nodes by key
	std::unordered_map<Key, LRUCacheNode<Key, T>*> mNodeMap;

	/**
	 *	Removes the least recently used node from the node map and deletes from the linked list
	 *	@return The node which was evicted, this node should be used to add new data into the cache
	 *	
	 *	@note runtime complexity O(1)
	 */
	LRUCacheNode<Key, T>* evict()
	{
		// The data at the end of the list is the least recently used item
		auto lru = mTail;

		// reconnect nodes
		if (mTail->prev != nullptr)
		{ mTail->prev->next = nullptr; }
		else
		{
			// There is only 1 item in the list, evicting this item causes an empty list
			mHead = nullptr;
		}

		// reset the tail node
		mTail = mTail->prev;

		// remove the node from the node map
		mNodeMap.erase(lru->key);

		// return the node to be reclaimed
		return lru;
	}
public:
	/**
	 *	Initializes the cache with a maxium storage capacity
	 *	@param capacity The maximum amount of data to store in the cache.
	 *		Once size > capacity, the least recently used data will be evicted on insert of new data
	 *		capacity must be greater than 0
	 */
	LRUCache(int _capacity)
	{
		assert(_capacity > 0);

		mHead = nullptr;
		mTail = nullptr;
		iMaxCapacity = _capacity;
		mNodeMap.reserve(_capacity);
	}

	/**
	 * Cleans up the memory for the cache
	 */
	~LRUCache()
	{
		// free all memory for nodes in the cache
		auto current = mHead;
		while (current != nullptr)
		{
			auto next = current->next;
			delete current;
			current = next;
		}
	}

	/**
	 *	Attempts to get the value with the given key
	 *	@param key The key of the value to find
	 *	@return The found value or nullptr
	 *
	 *	@note runtime complexity O(1)
	 */
	std::shared_ptr<T> find(Key _key)
	{
		// if there is no node, we didnt find the key
		if (mNodeMap.find(_key) == mNodeMap.end())
		{ return nullptr; }

		// attempt to grab the node from the map
		auto node = mNodeMap[_key];

		// if the node is the head, then this is already least recently used, just return
		if (node == mHead)
		{ return node->data; }

		// if the node is the tail, then replace the tail pointer
		if (node == mTail)
		{ mTail = node->prev; }
		else
		{ node->next->prev = node->prev; }

		// re-arrange the pointers for the node
		node->prev->next = node->next;

		// move the node to the front of the list
		node->prev = nullptr;
		node->next = mHead;
		mHead->prev = node;
		mHead = node;

		// return the value
		return node->data;
	}

	/**
	 *	Inserts the given item into the cache with the given key
	 *	@param key The key to associate with the data
	 *	@param data The data to insert into the cache
	 *
	 *	@note runtime complexity O(1)
	 */
	void insert(Key _key, const std::shared_ptr<T> _data)
	{
		// create a new node to insert
		LRUCacheNode<Key, T>* newNode = nullptr;

		// check if the node map is currently at capacity
		if(mNodeMap.size() >= iMaxCapacity)
		{ 
			newNode = evict(); 
			newNode->data = _data;
			newNode->key = _key;
		}
		else
		{ newNode = new LRUCacheNode<Key, T>(_key, _data); }

		// if the cache is empty, create the head and tail
		if (mHead == nullptr)
		{
			mHead = newNode;
			mTail = newNode;
		}
		else 
		{
			// if the cache is non-empty, add the node to the beginning of the cache list (most recently used)
			newNode->next	= mHead;
			mHead->prev		= newNode;
			mHead			= newNode;
			newNode->prev	= nullptr;
		}

		// add the node to the node map
		mNodeMap[_key] = newNode;
	}
};